#include "Connection.h"

#define BUFFER_SIZE (65536 - sizeof(StackNode))
#define DEFAULT_PORT 8888

static uv_loop_t *g_puvLoop;
static Connection *g_pClient = NULL;
DPool g_dpBuffers;
DPool g_dpConnections;
static U32 g_uConnections = 0;
static uv_tty_t g_uvhStdin;
static Buffer g_bfrStdin;

static void onuvTcpConnect(uv_connect_t *puvrConnect, int status);
//static void onuvTcpAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer);
//static void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer);
//static void onuvTcpWrite(uv_write_t *uvrWrite, int status);
//static void onuvTcpClose(uv_handle_t *puvhTcp);
//static U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes);
//static Bool onPacket(Connection *pConnection, Packet *pPacket);
//static Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes);
//static Bool Post(Connection *pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes);
static void onuvStdinAllocate(uv_handle_t *puvhStdin, size_t nSuggestedSize, uv_buf_t *pBuffer);
static void onuvStdin(uv_stream_t *puvhStdin, ssize_t nRead, const uv_buf_t *pBuffer);

int main(int argc, const char *argv[]){
    Assert(PacketTypes <= 256, 1, "PacketType");

    DPool_Initialize(&g_dpBuffers, BUFFER_SIZE);

    g_puvLoop = uv_default_loop();

    //console
    Buffer_Initialize(&g_bfrStdin, NULL, NULL);
    int r = uv_tty_init(g_puvLoop, &g_uvhStdin, 0, 1);
    Assert(r == 0, 6, "uv_tty_init");
    //    Log("uv_tty_init end");

    r = uv_read_start((uv_stream_t*)&g_uvhStdin, onuvStdinAllocate, onuvStdin);
    Assert(r == 0, 6, "uv_read_start");
    //    Log("uv_read_start end");

    //Tcp
    DPool_Initialize(&g_dpConnections, sizeof(Connection));
    g_pClient = (Connection*)DPool_Pop(&g_dpConnections);
    r = uv_tcp_init(g_puvLoop, (uv_tcp_t*)g_pClient);
    Assert(r == 0, 1, "uv_tcp_init");
    //    r = uv_tcp_keepalive(&g_server, 1, 60);
    //    CHECK(r, "tcp_keepalive");
    struct sockaddr_in address;
    r = uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &address);
    Assert(r == 0, 1, "uv_ip4_addr");
    //    r = uv_tcp_bind(&g_server, (const struct sockaddr*)&address, 0);
    //    Assert(r == 0, 2, "uv_tcp_bind");

    r = uv_tcp_connect(&g_pClient->uvrConnect, (uv_tcp_t*)g_pClient, (const struct sockaddr*)&address, onuvTcpConnect);
    Assert(r == 0, 3, "uv_tcp_connect");
    Log("Connecting remote port %u\n", DEFAULT_PORT);
    uv_run(g_puvLoop, UV_RUN_DEFAULT);
    Log("Exiting..\n");
    DPool_Finalize(&g_dpBuffers);
    DPool_Finalize(&g_dpConnections);
    return 0;
}

static void onuvTcpConnect(uv_connect_t *puvrConnect, int status){
    Assert(status == 0, 4, "onuvTcpConnect");

    Connection *pConnection = g_pClient;
    //    Connection *pConnection = (Connection*)DPool_Pop(&g_dpConnections);
    Buffer_Initialize(&pConnection->bfrInput, NULL, NULL);
    Buffer_Initialize(&pConnection->bfrOutput, NULL, NULL);
    pConnection->onInput = onTcpInput;

    int r = uv_read_start((uv_stream_t*)pConnection, onuvTcpAllocate, onuvTcpRead);
    Assert(r == 0, 6, "uv_read_start");
}

//static void onuvTcpAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer){
//    Connection* pConnection = (Connection*)puvhTcp;
//    Buffer *pbfrInput = &pConnection->bfrInput;
//    if(Buffer_Begin(pbfrInput) == NULL){
//        //        pConnection->pInput = DPool_Pop(&g_dpBuffers);
//        //        pConnection->pInputEnd = pConnection->pInput;
//        Byte *pBegin = DPool_Pop(&g_dpBuffers);
//        Buffer_Initialize(pbfrInput, pBegin, pBegin + DPool_Size(&g_dpBuffers));
//    }

//    //    Byte *pEnd = pConnection->pInput + DPool_Size(&g_dpBuffers);
//    pBuffer->base = (char*)Buffer_Tail(pbfrInput);
//    pBuffer->len = Buffer_TailSize(pbfrInput);
//}

//static void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer) {
//    //Log("onuvTcpRead(%ld)\n", nRead);

//    Connection *pConnection = (Connection*)puvhTcp;
//    Buffer *pbfrInput = &pConnection->bfrInput;
//    //    Assert(pConnection->pInput != NULL, 6, "onuvTcpRead");

//    if(nRead > 0){
//        Buffer_Push(pbfrInput, nRead);
//        U32 uDataSize = Buffer_DataSize(pbfrInput);
//        U32 nProcessed = pConnection->onInput(pConnection, Buffer_Data(pbfrInput), uDataSize);
//        //Log("nProcessed(%u)\n", nProcessed);
//        Buffer_Pop(pbfrInput, nProcessed);
//        if(nProcessed < uDataSize){
//            Buffer_ForwardData(pbfrInput);
//            return;
//        }

//        //        uv_close((uv_handle_t*)puvhTcp, onuvTcpClose);
//    }
//    else if(nRead < 0){
//        if (nRead != UV_EOF){
//            Log("Error: Read failed!\n");
//        }
//        uv_close((uv_handle_t*)puvhTcp, onuvTcpClose);
//    }
//    else{
//        Log("Read: 0\n");
//        //uv_close((uv_handle_t*)puvhTcp, onuvTcpClose);
//        return;
//    }

//    DPool_Push(&g_dpBuffers, Buffer_Begin(pbfrInput));
//    Buffer_Initialize(pbfrInput, NULL, NULL);
//}

//static void onuvTcpWrite(uv_write_t *uvrWrite, int status){
//    //Log("onuvTcpWrite(%d)\n", status);

//    Connection *pConnection = ContainerOf(uvrWrite, uvrWrite, Connection);
//    Buffer *pbfrOutput = &pConnection->bfrOutput;
//    //    Buffer_Pop(pbfrOutput, Buffer_DataSize(pbfrOutput));
//    //    if(Buffer_DataSize(pbfrOutput) == 0){
//    DPool_Push(&g_dpBuffers, Buffer_Begin(pbfrOutput));
//    Buffer_Initialize(pbfrOutput, NULL, NULL);
//    //    }
//}

//static void onuvTcpClose(uv_handle_t *puvhTcp){
//    //Log("Connections(%u): (%p)Closed!\n", --g_uConnections, puvhTcp);
//    DPool_Push(&g_dpConnections, (Byte*)puvhTcp);
//}

//static U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes){
//    //Log("Read: %ld\n", uBytes);
//    //    pData[uBytes] = 0;
//    //    Log("========\n%s\n", pData);
//    const Byte *pEnd = pData + uBytes;
//    const Byte *p = pData;
//    const Byte *pHeaderEnd;
//    const Byte *pContentEnd;
//    Packet *pPacket;
//    while((pHeaderEnd = p + sizeof(Packet)) <= pEnd){
//        pPacket = (Packet*)p;
//        pContentEnd = pHeaderEnd + pPacket->size;
//        if(pContentEnd > pEnd){
//            p -= sizeof(Packet);
//            break;
//        }

//        if(onPacket(pConnection, pPacket)){
//            uv_close((uv_handle_t*)pConnection, onuvTcpClose);
//            //Free input buffer
//            return uBytes;
//        }
//        p = pContentEnd;
//    }

//    return p - pData;
//}

//static Bool onPacket(Connection *pConnection, Packet *pPacket){
//    const Byte *pContent = (Byte*)pPacket + sizeof(Packet);
//    U16 uContent = pPacket->size;
//    switch(pPacket->type){
//    default:
//        return true;
//    case pktMessage:{
//        return onMessage(pConnection, pPacket->uSubType, pContent, uContent);
//    }break;
//    }
//    return false;
//}

Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes){
    char szMessage[65536];
    memcpy(szMessage, pContent, uBytes);
    szMessage[uBytes] = 0;
    Log("Server:\n========\n\t%s\n", szMessage);
    return false;
}

//static Bool Post(Connection* pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes){
//    Buffer *pbfrOutput = &pConnection->bfrOutput;
//    if(Buffer_Begin(pbfrOutput) == NULL){
//        Byte *pBegin = DPool_Pop(&g_dpBuffers);
//        Buffer_Initialize(pbfrOutput, pBegin, pBegin + DPool_Size(&g_dpBuffers));
//    }

//    U32 uSpace = Buffer_TailSize(pbfrOutput);
//    if(uBytes > uSpace)
//        return false;

//    Byte *p = Buffer_Tail(pbfrOutput);
//    Packet *pPacket = (Packet*)p;
//    pPacket->size = uBytes;
//    pPacket->type = type;
//    pPacket->uSubType = uSubType;
//    p += sizeof(Packet);
//    memcpy(p, pContent, uBytes);
//    p += uBytes;
//    Buffer_Push(pbfrOutput, p - (Byte*)pPacket);

//    //    Log("Write: %u\n", Buffer_DataSize(pbfrOutput));
//    uv_buf_t uvBuffer = {Buffer_Data(pbfrOutput), Buffer_DataSize(pbfrOutput)};
//    int r = uv_write(&pConnection->uvrWrite,
//                     (uv_stream_t*)pConnection,
//                     &uvBuffer,
//                     1,
//                     onuvTcpWrite);
//    Assert(r == 0, 6, "uv_write");
//    return true;
//}

static void onuvStdinAllocate(uv_handle_t *puvhStdin, size_t nSuggestedSize, uv_buf_t *pBuffer){
    Buffer *pbfrInput = &g_bfrStdin;
    if(Buffer_Begin(pbfrInput) == NULL){
        Byte *pBegin = DPool_Pop(&g_dpBuffers);
        Buffer_Initialize(pbfrInput, pBegin, pBegin + DPool_Size(&g_dpBuffers));
    }

    pBuffer->base = (char*)Buffer_Tail(pbfrInput);
    pBuffer->len = Buffer_TailSize(pbfrInput);
}

static void onuvStdin(uv_stream_t *puvhStdin, ssize_t nRead, const uv_buf_t *pBuffer){
    //    Connection *pConnection = ContainerOf(uvrStdin, uvrStdin, Connection);
    Connection *pConnection = g_pClient;
    //    Log("onuvStdin: %ld\n", );
    if(nRead > 0){
        char szInput[65536];
        memcpy(szInput, pBuffer->base, nRead);
        szInput[nRead] = 0;
        //        Log(">>%s", szInput);
        Post(pConnection, pktMessage, ptmUtf8, szInput, nRead - 1);
    }
    DPool_Push(&g_dpBuffers, Buffer_Begin(&g_bfrStdin));
    Buffer_Initialize(&g_bfrStdin, NULL, NULL);
    //#define MESSAGE "Hello, world!"
    //    Post(pConnection, pktMessage, ptmUtf8, MESSAGE, sizeof(MESSAGE) - 1);
}
