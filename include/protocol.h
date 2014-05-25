#pragma once

typedef struct{
  U16 nSize; //packet size, not include 'nSize' field
  U16 nType; //enum PacketType
  U32 nVerifyCode; //packet verify code, not include 'nVerifyCode' field
  Byte szData[65528];
}Packet;

typedef enum{
    ptHello, //Handshake. Protocol version, Crypto type.
    ptBye, //Can close connection or session now
    ptDescription, //descript node
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
