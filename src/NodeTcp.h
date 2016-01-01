#ifndef CONNECTION_H
#define CONNECTION_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include "Type.h"
#include "Buffer.h"
#include "DPool.h"
#include "Packet.h"

#define ContainerOf(pointer, Name, Container) \
    ((Container*)((Byte*)(pointer) - (Byte*)&((Container*)NULL)->Name))

#define Log(...) fprintf(stderr, __VA_ARGS__);

#define Assert(expression, error, message) \
    if(!(expression)){\
    fprintf(stderr, "Error: %s\n", message);\
    exit(error);\
    }

typedef struct _Connection Connection;
typedef U32 (*InputHandler)(Connection *pConnection, const Byte *pData, U32 uBytes);

struct _Connection{
    uv_tcp_t uvhTcp;
    union{
        uv_fs_t uvrFile;
        uv_connect_t uvrConnect;
        uv_write_t uvrWrite;
    };
    Buffer bfrInput;
    Buffer bfrOutput;
    InputHandler onInput;
};

Bool Post(Connection *pConnection, PacketType type, U8 uSubType, const Byte *pContent, U16 uBytes);
void onuvTcpAllocate(uv_handle_t *puvhTcp, size_t nSuggestedSize, uv_buf_t *pBuffer);
void onuvTcpRead(uv_stream_t *puvhTcp, ssize_t nRead, const uv_buf_t *pBuffer);
void onuvTcpWrite(uv_write_t *uvrWrite, int status);
void onuvTcpClose(uv_handle_t *puvhTcp);
U32 onTcpInput(Connection *pConnection, const Byte *pData, U32 uBytes);
Bool onPacket(Connection *pConnection, Packet *pPacket);
Bool onMessage(Connection *pConnection, PacketType_Message type, const Byte *pContent, U16 uBytes);

#endif //CONNECTION_H
