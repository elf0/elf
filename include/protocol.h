#pragma once

//packet: | size(2 bytes) | type(2 bytes) | verify code(3 bytes) | data(size - 5 bytes) |
//All packets are encrypted, except 'Hello' packet.


#define PROTOCOL_VERSION 0

#define PACKET_BUFFER_SIZE 65536

typedef enum{
    ptHello, //Handshake. Protocol version, Crypto type.
    ptBye, //Can close connection or session now
    ptNode, //descript node
    ptProxy, //Pass data to target node or site.
    ptCode, //Help me execute the code/function, and return result to me.
    ptLocation, //This node location(GPS). Usually for private session.
    ptSOS, //Help me! I'm in danger. Send to prefined nodes(family, friends, polices...), or every one who can help me.
    ptMessage, //Text chat
    ptDicMessage, //Dictionary-based chat
//File
    ptGetFile, //Download file
    ptStoreFile, //Store my file in net. File must split into many blocks, and store at many nodes, many countries. Some nodes/countries broke won't lost data.
    ptFindFile, //Find file in net
    ptFileMeta, //file name, size, last modified time...
    ptFileData,
//Directory for file system
    ptDirectoryMeta, //directory name, size, last modified time...
    ptDirectoryData,
//Real-time Stream
    ptVideoMeta,
    ptVideoData,
    ptAudioMeta,
    ptAudioData
}PacketType;

typedef enum{
 ctCustom, ctAES, ctRSA
}CryptoType;

//nVerifyCode: accumulate packet bytes, not include 'nVerifyCode' field
//7 bytes
#define PACKET_HEADER_FIELDS \
  U16 nSize; \
  U16 nType; \
  U32 nVerifyCode: 24

//12 bytes
typedef struct{
  PACKET_HEADER_FIELDS;
  U32 nCrypto: 8;
  U32 nVersion; //protocol version
}Packet_Hello;

//8 bytes
typedef struct{
  PACKET_HEADER_FIELDS;
  U32 nReserved: 8;
}Packet_Bye;

//8 bytes
typedef struct{
  PACKET_HEADER_FIELDS;
  U32 bSharingFile: 1;
  U32 bSharingDirectory: 1;
  U32 bSharingDisk: 1;
  U32 bSharingProcessor: 1;
}Packet_Node;

