#pragma once

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

/*
 The "elf" net is a abstract and distributed server.
 Every node is part of it.

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
  pktMakeFriends, //remember this node, give some permissions
//message dictionary
  pktMessageDictionary_Sync, //sync the newest message dictionary
//chat
  pktChat_Hello, //p2p chat: languages(i can [read, write, hear, speak])
  pktChat_DicionaryVersion,
  pktChat_ExchangeDictionary, //Exchange custom dictionary
  pktChat_CreatGroup,
  pktChat_Message, //text message
  pktChat_Dictionary, //dictionary message(id), using standard dictionary
  pktChat_Dictionary_Custom, //dictionary message(id), using custom dictionary
  pktImage,
  pktAudio,
  pktVideo,
  pktFile,
//broadcast: message, video, audio, advertisement
  pktBroadcast_Creat, //Create a broadcast group
  pktBroadcast_Join,
  pktBroadcast_Leave,
//file sharing
//file system
  pktFile_Get,//Download file
  pktFile_Store, //Store my file in net. File must split into many blocks, and store at many nodes, many countries. Some nodes/countries broke won't lost data.
  pktFile_Create,//Create a file in current dir
  pktFile_Delete,//Delete a file in current dir
  pktFile_Find, //Find file in current dir
  pktFile_FullTextFind, //Find file in current dir
  pktFile_Meta, //file name, size, last modified time...
  pktFile_Data,
  pktDirectory_Get, pktDirectory_Create, pktDirectory_Delete, pktDirectory_Child, pktDirectory_Parent
//Find everything in net
  pktFind_File,
  pktFind_Novel,
  pktFind_Picture,
  pktFind_Video,
  pktFind_Audio,
  pktFind_Person,
  pktFind_PersonPicture,
  pktFind_Hotel,
  pktFind_Restaurant,
  pktFind_Blood, //medical
//map
  pktMap_Get, //Download a rectange range
  pktMap_Add, //Upload a object
  pktMap_Delete, //Delete a object
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


//nTimeStamp: seconds from 1970-01-01 00:00:00
//nExpire: Proxy node must drop the packet, if now - nTimeStamp > nExpire.
//nVerifyCode: accumulate packet bytes, not include 'nVerifyCode' field
//7 bytes
#define PACKET_HEADER_FIELDS \
  U16 nSize; \
  U16 nType; \
  U32 nTimeStamp; \
  U32 nExpire; \
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

typedef struct{
  PACKET_HEADER_FIELDS;
  U32 nReserved: 8;
  U8 nTimes;// if not 0, select a random(can't be target) node to send, and reduce 1
}Packet_Proxy;

//8 bytes
typedef struct{
  PACKET_HEADER_FIELDS;
  U32 bSharingFile: 1;
  U32 bSharingDirectory: 1;
  U32 bSharingDisk: 1;
  U32 bSharingProcessor: 1;
}Packet_Node;

