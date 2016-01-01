#include "Connection.h"

#define BUFFER_SIZE (65536 - sizeof(StackNode))
#define DEFAULT_PORT 8888


static uv_loop_t *g_puvLoop;
static uv_tcp_t g_server;
static Connection *g_pClient = NULL;
DPool g_dpBuffers;
DPool g_dpConnections;
static U32 g_uConnections = 0;
static uv_tty_t g_uvhStdin;
static Buffer g_bfrStdin;

static void onuvConnect(uv_stream_t *puvhServer, int status);
//static void onuvTcpAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer);
//static void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer);
//static void onuvTcpWrite(uv_write_t *uvrWrite, int status);
//static void onuvTcpClose(uv_handle_t *puvhTcp);
//static U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes);
//static Bool onPacket(Connection *pConnection, Packet *pPacket);
//static Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes);
//static Bool Post(Connection* pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes);
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

    r = uv_read_start((uv_stream_t*)&g_uvhStdin, onuvStdinAllocate, onuvStdin);
    Assert(r == 0, 6, "uv_read_start");

    DPool_Initialize(&g_dpConnections, sizeof(Connection));

    r = uv_tcp_init(g_puvLoop, &g_server);
    Assert(r == 0, 1, "uv_tcp_init");
    //    r = uv_tcp_keepalive(&g_server, 1, 60);
    //    Assert(r, 1, "uv_tcp_keepalive");
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

    g_pClient = pConnection;
    Log("New connection\n");

    r = uv_read_start((uv_stream_t*)pConnection, onuvTcpAllocate, onuvTcpRead);
    Assert(r == 0, 6, "uv_read_start");
}


Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes){
    char szMessage[65536];
    memcpy(szMessage, pContent, uBytes);
    szMessage[uBytes] = 0;
    Log("Client:\n========\n\t%s\n", szMessage);

    //echo
//    Post(pConnection, pktMessage, type, pContent, uBytes);

    return false;
}


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
    Connection *pConnection = g_pClient;

    if(nRead > 0 && pConnection){
        char szInput[65536];
        memcpy(szInput, pBuffer->base, nRead);
        szInput[nRead] = 0;
        Post(pConnection, pktMessage, ptmUtf8, szInput, nRead - 1);
    }
    DPool_Push(&g_dpBuffers, Buffer_Begin(&g_bfrStdin));
    Buffer_Initialize(&g_bfrStdin, NULL, NULL);
}
