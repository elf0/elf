#ifndef PACKET_H
#define PACKET_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

/*
Packet-based protocol
================
Packet content size: 0 to 65535 bytes
Packet content fixed part(For weak node): 0 to 255 bytes
Packet type: 0 to 255

5 layers:
  1. Channel layer (radio, udp, tcp, http, ..)
  2. Verification layer (Optional)
  3. Encryption layer (Optional)
  4. Dispatch layer
  5. Data layer

TODO: Radio communication
*/

#include "Type.h"

#define ELF_PROTOCOL 0

typedef enum{
    pktMessage,
    //Not implementation
    pktFile,
    pktCommand,
    pktCode,
    pktNet,
    pktStream, //Custom stream
    pktVideo,
    pktAudio,
    pktRadio,
    pktTemperature,
    pktLocation,
    pktTime,
    pktNews,
    pktWiki,
    pktMap,
    pktMoney,
    pktPerson,
    pktFriendLoveSex,
    pktFoodDrink,
    pktHealth,
    pktTransport,
    pktSos,
    pktEvidence,
    pktOrganism,
    pktAnimal,
    pktPlant,
    PacketTypes
}PacketType;

typedef enum{
    ptmUnavailable = 0,
    ptmUtf8, ptmUtf16,
    ptmIndexedDatabase,
    ptmIndexed,
    ptmPrivateIndexed, ptmSessionIndexed
}PacketType_Message;

typedef struct{
    U16 size;
    U8   type;
    U8   uSubType;
}Packet_Header;

typedef struct{
    Char szText[1];
}Packet_Message_Text;

typedef struct{
    U8 uDatabase;
    U16 uText;
}Packet_Message_Indexed;

//typedef enum{
//    rptUnavailable, //Function unavailable
//    rptForbidden
//}ResultPacketType;

////typedef enum{
////    sptVideo,
////    sptAudio
////}StreamPacketType;
//typedef enum{
//    nptProxy, nptNodes, nptHello
//}NetPacketType;

//typedef enum{
//    fptUnavailable = 0,
//    fptFileData, fptGetFile, fptGetIndexedFile, fptPostFile,
//}FilePacketType;

//typedef enum{
//    //money
//    mptUnavailable = 0,
//    mptMoney_New, mptMoney_Delete,
//    mptMoney_Remainder, mptMoney_Add, mptMoney_Sub,
//    mptMoney_QueryHistory, mptMoney_History,
//    MoneyPacketTypes
//}MoneyPacketType;

//typedef struct{
//    U32 uIp;
//    U16 uPort;
//    U16 uTcpPort;
//}NodeAddress;

////Min 4 bytes
//typedef struct{
//    U16 uVersion; //Protocol version (default ELF_PROTOCOL)

//    //Connections
//    U16 uRadio: 1; //Have radio channels (default 0)
//    U16 uFixed: 1; //fixed address (default 0)
//    U16 uAvailable: 1; //Always available (default 0)
//    U16 uProxy: 1; //Proxy node (default 1)

//    //Hardware
//    //    U16 uSuperComputing: 1;
//    //    U16 uSuperStore: 1;
//    //    U16 uSuperWideband: 1;

//    //Services
//    //proxy and store is default
//    U16 uHome: 1; //Like a http site (default 1)
//    U16 uFile: 1; //Share files(search, list, download) (default 1)
//    U16 uStore: 1; //Share store device (default 1)
//    U16 uComputing: 1; //Share computing device (default 0)

//    U16 uPort; //Udp port. 0: Udp not used (default 1)
//    U16 uTcpPort; //0: Tcp not used (default 0)
//    Char szName[32]; //Nick name
//    Char szSoftware[32]; //Software description
//    U8 uNodes;
//    NodeAddress nodes[1]; //Share known nodes
//}Packet_Hello;

//typedef struct{
//    NodeAddress nodes[1];
//}Packet_Nodes;

//typedef struct{
//    U64 uIndex; //Index of file at peer node
//    U64 uSum; //Sum of file
//}Packet_GetIndexedFile;

//typedef struct{
//    Char szPath[1];
//}Packet_GetFile;

//typedef struct{
//    Char szDirectory[1];
//}Packet_GetDirectory;

//typedef struct{
//    Char szDirectory[1];
//}Packet_DirectoryItems;

#endif //PACKET_H
