#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include "DPool.h"
#include "Buffer.h"
#include "Packet.h"
#include "Connection.h"

#define BUFFER_SIZE (65536 - sizeof(StackNode))
#define DEFAULT_PORT 8888

#define ContainerOf(pointer, Name, Container) \
    ((Container*)((Byte*)(pointer) - (Byte*)&((Container*)NULL)->Name))

#define Log(...) fprintf(stderr, __VA_ARGS__);

#define Assert(expression, error, message) \
    if(!(expression)){\
    fprintf(stderr, "Error: %s\n", message);\
    exit(error);\
    }

//typedef struct _Connection Connection;
//typedef U32 (*InputHandler)(Connection *pConnection, const Byte *pData, U32 uBytes);

//struct _Connection{
//    uv_tcp_t uvhTcp;
//    union{
//        uv_fs_t uvrFile;
//        uv_connect_t uvrConnect;
//        uv_write_t uvrWrite;
//    };
//    Buffer bfrInput;
//    Buffer bfrOutput;
//    InputHandler onInput;
//};

static uv_loop_t *g_puvLoop;
static uv_tcp_t g_server;
static DPool g_dpBuffers;
static DPool g_dpConnections;
static U32 g_uConnections = 0;

static void onuvConnect(uv_stream_t *puvhServer, int status);
static void onuvAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer);
static void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer);
static void onuvTcpWrite(uv_write_t *uvrWrite, int status);
static void onuvTcpClose(uv_handle_t *puvhTcp);
static U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes);
static Bool onPacket(Connection *pConnection, PacketType type, const Byte *pContent, U16 uBytes);
static Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes);
static Bool Post(Connection* pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes);

int main(int argc, const char *argv[]){
    Assert(PacketTypes <= 256, 1, "PacketType");

    DPool_Initialize(&g_dpBuffers, BUFFER_SIZE);
    DPool_Initialize(&g_dpConnections, sizeof(Connection));

    g_puvLoop = uv_default_loop();
    int r = uv_tcp_init(g_puvLoop, &g_server);
    Assert(r == 0, 1, "uv_tcp_init");
    //    r = uv_tcp_keepalive(&g_server, 1, 60);
    //    CHECK(r, "tcp_keepalive");
    struct sockaddr_in address;
    r = uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &address);
    Assert(r == 0, 1, "uv_ip4_addr");
    r = uv_tcp_bind(&g_server, (const struct sockaddr*)&address, 0);
    Assert(r == 0, 2, "uv_tcp_bind");
    r = uv_listen((uv_stream_t*)&g_server, 256, onuvConnect);
    Assert(r == 0, 3, "uv_listen");
    Log("Listening on port %u\n", DEFAULT_PORT);
    uv_run(g_puvLoop, UV_RUN_DEFAULT);
    DPool_Finalize(&g_dpBuffers);
    DPool_Finalize(&g_dpConnections);
    return 0;
}

static void onuvConnect(uv_stream_t *puvhServer, int status){
    Assert(status == 0, 4, "onuvConnect");

    Connection *pConnection = (Connection*)DPool_Pop(&g_dpConnections);
    Buffer_Initialize(&pConnection->bfrInput, NULL, NULL);
    Buffer_Initialize(&pConnection->bfrOutput, NULL, NULL);
    pConnection->onInput = onTcpInput;
    uv_tcp_init(g_puvLoop, &pConnection->uvhTcp);
    int r = uv_accept(puvhServer, (uv_stream_t*)pConnection);
    Assert(r == 0, 5, "uv_accept");

    r = uv_read_start((uv_stream_t*)pConnection, onuvAllocate, onuvTcpRead);
    Assert(r == 0, 6, "uv_read_start");
}

static void onuvAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer){
    Connection* pConnection = (Connection*)puvhTcp;
    Buffer *pbfrInput = &pConnection->bfrInput;
    if(Buffer_Begin(pbfrInput) == NULL){
        //        pConnection->pInput = DPool_Pop(&g_dpBuffers);
        //        pConnection->pInputEnd = pConnection->pInput;
        Byte *pBegin = DPool_Pop(&g_dpBuffers);
        Buffer_Initialize(pbfrInput, pBegin, pBegin + DPool_Size(&g_dpBuffers));
    }

    //    Byte *pEnd = pConnection->pInput + DPool_Size(&g_dpBuffers);
    pBuffer->base = (char*)Buffer_Tail(pbfrInput);
    pBuffer->len = Buffer_TailSize(pbfrInput);
}

static void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer) {
    Connection *pConnection = (Connection*)puvhTcp;
    Buffer *pbfrInput = &pConnection->bfrInput;
    //    Assert(pConnection->pInput != NULL, 6, "onuvTcpRead");

    if(nRead > 0){
        Buffer_Push(pbfrInput, nRead);
        U32 uDataSize = Buffer_DataSize(pbfrInput);
        U32 nProcessed = pConnection->onInput(pConnection, Buffer_Data(pbfrInput), uDataSize);
        Buffer_Pop(pbfrInput, nProcessed);
        if(nProcessed < uDataSize){
            Buffer_ForwardData(pbfrInput);
            return;
        }

        //        uv_close((uv_handle_t*)puvhTcp, onuvTcpClose);
    }
    else if(nRead < 0){
        if (nRead != UV_EOF){
            Log("Error: Read failed!\n");
        }
        uv_close((uv_handle_t*)puvhTcp, onuvTcpClose);
    }
    else{
        Log("Read: 0\n");
        //uv_close((uv_handle_t*)puvhTcp, onuvTcpClose);
        return;
    }

    DPool_Push(&g_dpBuffers, Buffer_Begin(pbfrInput));
    Buffer_Initialize(pbfrInput, NULL, NULL);
}

static void onuvTcpWrite(uv_write_t *uvrWrite, int status){
    Connection *pConnection = ContainerOf(uvrWrite, uvrWrite, Connection);
    Buffer *pbfrOutput = &pConnection->bfrOutput;
    //    Buffer_Pop(pbfrOutput, Buffer_DataSize(pbfrOutput));
    //    if(Buffer_DataSize(pbfrOutput) == 0){
    DPool_Push(&g_dpBuffers, Buffer_Begin(pbfrOutput));
    Buffer_Initialize(pbfrOutput, NULL, NULL);
    //    }
}

static void onuvTcpClose(uv_handle_t *puvhTcp){
    //Log("Connections(%u): (%p)Closed!\n", --g_uConnections, puvhTcp);
    DPool_Push(&g_dpConnections, (Byte*)puvhTcp);
}

static U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes){
    //Log("Read: %ld\n", uBytes);
    //    pData[uBytes] = 0;
    //    Log("========\n%s\n", pData);
    const Byte *pEnd = pData + uBytes;
    const Byte *p = pData;
    const Byte *pSizeEnd;
    const Byte *pContentEnd;
    U32 uContent;
    PacketType type;
    while((pSizeEnd = p + 2) < pEnd){
        uContent = *(U16*)p;
        p = pSizeEnd;
        type = *p++;
        pContentEnd = p + uContent;
        if(pContentEnd > pEnd){
            p -= 3;
            break;
        }

        if(onPacket(pConnection, type, p, uContent)){
            uv_close((uv_handle_t*)pConnection, onuvTcpClose);
            //Free input buffer
            return uBytes;
        }
        p = pContentEnd;
    }

    return p - pData;
}

static Bool onPacket(Connection *pConnection, PacketType type, const Byte *pContent, U16 uBytes){
    const Byte *p = pContent;
    U8 uSubType = *p++;
    switch(type){
    default:
        return true;
    case pktMessage:{
        return onMessage(pConnection, (PacketType_Message)uSubType, p, uBytes);
    }break;
    }
    return false;
}

static Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes){
    char szMessage[65536];
    memcpy(szMessage, pContent, uBytes);
    szMessage[uBytes];
    Log("Message:\n<=======\n%s\n=======>\n", szMessage);

    //echo
    Post(pConnection, pktMessage, type, pContent, uBytes);

    return false;
}

static Bool Post(Connection* pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes){
    Buffer *pbfrOutput = &pConnection->bfrOutput;
    if(Buffer_Begin(pbfrOutput) == NULL){
        Byte *pBegin = DPool_Pop(&g_dpBuffers);
        Buffer_Initialize(pbfrOutput, pBegin, pBegin + DPool_Size(&g_dpBuffers));
    }

    U32 uSpace = Buffer_TailSize(pbfrOutput);
    if(uBytes > uSpace)
        return false;

    Byte *p = Buffer_Tail(pbfrOutput);
    Packet_Header *pHeader = (Packet_Header*)p;
    pHeader->size = uBytes;
    pHeader->type = type;
    pHeader->uSubType = uSubType;
    p += sizeof(Packet_Header);
    memcpy(p, pContent, uBytes);
    p += uBytes;
    Buffer_Push(pbfrOutput, p - (Byte*)pHeader);

    uv_buf_t uvBuffer = {Buffer_Data(pbfrOutput), Buffer_DataSize(pbfrOutput)};
    int r = uv_write(&pConnection->uvrWrite,
                     (uv_stream_t*)pConnection,
                     &uvBuffer,
                     1,
                     onuvTcpWrite);
    Assert(r == 0, 6, "uv_write");
    return true;
}
