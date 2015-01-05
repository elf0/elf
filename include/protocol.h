#pragma once

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

/*
packet: | size(2 bytes) | type(2 bytes) | verify code(3 bytes) | data(size - 5 bytes) |
All packets are encrypted, except 'Hello' packet.
Process order:
  1. Read 2 bytes to size
  2. Read size bytes to buffer.
  3. Verify data.
  4. Decript data.
*/

#define PROTOCOL_VERSION 0

#define PACKET_BUFFER_SIZE 65536

typedef enum{
//Handshake
  pktHello, //Handshake. Protocol version, Crypto type.
  pktBye, //Can close connection or session now
//security: anonymous, encryption, authentication
  pktProxy, //Pass data to target node or site.
//chat
  pktMakeFriends,
  pktCreatChatRoom,
  pktMessage, pktMessage_DicVersion, pktMessage_Dic, pktMessage_DicCustom,
  pktImage,
  pktAudio,
  pktVideo,
  pktFile,
//broadcast: message, video, audio, advertisement
  pktBroadcast_Creat, //Create a broadcast group
  pktBroadcast_Join,
  pktBroadcast_Leave,
//file sharing
  pktFile_Get,//Download file
  pktFile_Store, //Store my file in net. File must split into many blocks, and store at many nodes, many countries. Some nodes/countries broke won't lost data.
  pktFile_Create,//Create a file in current dir
  pktFile_Delete,//Delete a file in current dir
  pktFile_Find, //Find file in current dir
  pktFile_FullTextFind, //Find file in current dir
  pktFile_Meta, //file name, size, last modified time...
  pktFile_Data,
  pktDirectory_Get, pktDirectory_Create, pktDirectory_Delete, pktDirectory_Child, pktDirectory_Parent
//Find
  pktFind_File,
  pktFind_Image,
  pktFind_Video,
  pktFind_Audio,
  pktFind_Person,
  pktFind_PersonPicture,
  pktFind_Hotel,
  pktFind_Restaurant,
  pktFind_Blood, //medical

//distributed computing
  pktCode, //Help me execute the code/function, and return result to me.

//////////////
    pktNode, //descript node
    pktLocation, //This node location(GPS). Usually for private session.
    pktSOS, //Help me! I'm in danger. Send to prefined nodes(family, friends, polices...), or every one who can help me.
    pktMessage, //Text chat
    pktDicMessage, //Dictionary-based chat
//Directory for file system
    pktDirectoryMeta, //directory name, size, last modified time...
    pktDirectoryData,
//Real-time Stream
    pktVideoMeta,
    pktVideoData,
    pktAudioMeta,
    pktAudioData
}PacketType;

typedef enum{
 ctCustom, ctAES, ctRSA
}CryptoType;


//nPostTime: packet post time
//nYear: Proxy node must drop a packet, if packet's nYear is 2022 when 2023(expired).
//nVerifyCode: accumulate packet bytes, not include 'nVerifyCode' field
//7 bytes
#define PACKET_HEADER_FIELDS \
  U16 nSize; \
  U16 nType; \
  U32 nPostTime; \
  U32 nLifeTime; \
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

