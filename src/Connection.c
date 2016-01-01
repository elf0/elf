#include "Connection.h"

extern DPool g_dpBuffers;
extern DPool g_dpConnections;

void onuvTcpAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer){
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

void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer) {
    //    Log("onuvTcpRead(%ld)\n", nRead);

    Connection *pConnection = (Connection*)puvhTcp;
    Buffer *pbfrInput = &pConnection->bfrInput;
    //    Assert(pConnection->pInput != NULL, 6, "onuvTcpRead");

    if(nRead > 0){
        Buffer_Push(pbfrInput, nRead);
        U32 uDataSize = Buffer_DataSize(pbfrInput);
        U32 nProcessed = pConnection->onInput(pConnection, Buffer_Data(pbfrInput), uDataSize);
        //Log("nProcessed(%u)\n", nProcessed);
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

void onuvTcpWrite(uv_write_t *uvrWrite, int status){
    //Log("onuvTcpWrite(%d)\n", status);

    Connection *pConnection = ContainerOf(uvrWrite, uvrWrite, Connection);
    Buffer *pbfrOutput = &pConnection->bfrOutput;
    //    Buffer_Pop(pbfrOutput, Buffer_DataSize(pbfrOutput));
    //    if(Buffer_DataSize(pbfrOutput) == 0){
    DPool_Push(&g_dpBuffers, Buffer_Begin(pbfrOutput));
    Buffer_Initialize(pbfrOutput, NULL, NULL);
    //    }
}

void onuvTcpClose(uv_handle_t *puvhTcp){
    Log("Connection closed!\n");
    //Log("Connections(%u): (%p)Closed!\n", --g_uConnections, puvhTcp);
    DPool_Push(&g_dpConnections, (Byte*)puvhTcp);
}

U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes){
    //Log("Read: %ld\n", uBytes);
    //    pData[uBytes] = 0;
    //    Log("========\n%s\n", pData);
    const Byte *pEnd = pData + uBytes;
    const Byte *p = pData;
    const Byte *pHeaderEnd;
    const Byte *pContentEnd;
    Packet *pPacket;
    while((pHeaderEnd = p + sizeof(Packet)) <= pEnd){
        pPacket = (Packet*)p;
        pContentEnd = pHeaderEnd + pPacket->size;
        if(pContentEnd > pEnd){
            p -= sizeof(Packet);
            break;
        }

        if(onPacket(pConnection, pPacket)){
            uv_close((uv_handle_t*)pConnection, onuvTcpClose);
            //Free input buffer
            return uBytes;
        }
        p = pContentEnd;
    }

    return p - pData;
}

Bool onPacket(Connection *pConnection, Packet *pPacket){
    const Byte *pContent = (Byte*)pPacket + sizeof(Packet);
    U16 uContent = pPacket->size;
    switch(pPacket->type){
    default:
        return true;
    case pktMessage:{
        return onMessage(pConnection, pPacket->uSubType, pContent, uContent);
    }break;
    }
    return false;
}

Bool Post(Connection* pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes){
    Buffer *pbfrOutput = &pConnection->bfrOutput;
    if(Buffer_Begin(pbfrOutput) == NULL){
        Byte *pBegin = DPool_Pop(&g_dpBuffers);
        Buffer_Initialize(pbfrOutput, pBegin, pBegin + DPool_Size(&g_dpBuffers));
    }

    U32 uSpace = Buffer_TailSize(pbfrOutput);
    if(uBytes > uSpace)
        return false;

    Byte *p = Buffer_Tail(pbfrOutput);
    Packet *pPacket = (Packet*)p;
    pPacket->size = uBytes;
    pPacket->type = type;
    pPacket->uSubType = uSubType;
    p += sizeof(Packet);
    memcpy(p, pContent, uBytes);
    p += uBytes;
    Buffer_Push(pbfrOutput, p - (Byte*)pPacket);

    //    Log("Write: %u\n", Buffer_DataSize(pbfrOutput));
    uv_buf_t uvBuffer = {Buffer_Data(pbfrOutput), Buffer_DataSize(pbfrOutput)};
    int r = uv_write(&pConnection->uvrWrite,
                     (uv_stream_t*)pConnection,
                     &uvBuffer,
                     1,
                     onuvTcpWrite);
    Assert(r == 0, 6, "uv_write");
    return true;
}
