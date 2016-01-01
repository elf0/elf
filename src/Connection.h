#ifndef CONNECTION_H
#define CONNECTION_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com
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

#endif //CONNECTION_H
