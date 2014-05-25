#pragma once

enum PacketType{
    ptHello, //Handshake. Protocol version, Crypto type.
    ptBye, //Remote can close connection or session now
    ptDescription, //This node'description.
    ptProxy, //Pass my data to target node or site.
    ptCode, //Help me execute the code/function, and return result to me.
    ptLocation, //This node location(GPS). Usually for private session.
    ptSOS, //Help me! I'm in danger. For prefined nodes session(family, friends, polices...), or every one who can help me.
    ptMessage, //Text chat
    ptDicMessage, //Dictionary-based chat
//File
    GetFile, //Download file
    StoreFile, //Store my file in net. File must split into many blocks, and store at many nodes, many countries. Some nodes/countries broke won't lost data.
    FindFile, //Find file in net
    FileMeta, //file name, size, last modified time...
    FileData,
//Directory for file system
    DirectoryMeta, //directory name, size, last modified time...
    DirectoryData,
//Real-time Stream
    VideoMeta,
    VideoData,
    AudioMeta,
    AudioData
};

