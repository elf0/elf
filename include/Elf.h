#ifndef ELF_H
#define ELF_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"

typedef struct Elf_s Elf;

typedef U64 Version;
typedef U64 ClassId;
typedef U64 ObjectId;

enum Class0{
    Digital = 0, Real = 1
};

enum DigitalType{
    Misc = 0,
    Picture,
    Video,
    Movie,
    Cartoon,
    Audio,
    Text,
    PictureText,
    VideoAudio,
    VideoAudioText,
    Book,
};

enum VideoType{
    VideoType_Misc,
    VideoType_ScienceFiction,
    VideoType_Horror,
    VideoType_Love,
    VideoType_War,
    VideoType_Documentary,
    VideoType_Music,
    VideoType_Song,
    VideoType_Pornographic
};

enum RealType{
    Misc = 0,
    StorageDevice,
    ComputingDevice,
    Sensor,
    CommunicationDevice,
    House,
    Vehicle,
//    Car,
    Picture,
    Book,
    Plant,
    Animal,
    Human,
    Female,
    Male,
};

typedef U64 Class;

//typedef struct{
//    U64 bReal: 1;
//}Class;

typedef struct{
    U64 bReal: 1;//Must be 0
    U64 bCode: 1;//Can be executed by elf
    U64 bImage: 1;//Picture, Video
    U64 bAudio: 1;
    U64 bText: 1;//UTF-8, UTF-16
    U64 bOther: 1;//Custom
    U64 nType: 58;
}DigitalClass;

//Example: picture
//bReal = 0
//bCode = 0
//bImage = 1
//bAudio = 0
//bText = 0
//bOther = 0
//nType = x

//Example: movie
//bReal = 0
//bCode = 0
//bImage = 1
//bAudio = 1
//bText = 0
//bOther = 0
//nType = x

typedef struct{
    U64 bReal: 1;//Must be 1
    U64 bLiving: 1;
    U64 bIntelligenceMade: 1;
    U64 nType: 61;
}RealClass;

//Example: Human
//bReal = 1
//bLiving = 1
//nType = x

//Example: Earth
//bReal = 1
//bLiving = 0
//nType = x

#define CLASS_DIGITAL 0x00
#define CLASS_DIGITAL_CODE 0x02
#define CLASS_DIGITAL_IMAGE 0x04
#define CLASS_DIGITAL_AUDIO 0x08
#define CLASS_DIGITAL_TEXT 0x10

#define CLASS_REAL 0x01
#define CLASS_REAL_LIVING 0x02
#define CLASS_REAL_INTELLIGENCE_MADE 0x04

#define CLASS_IS_REAL(CLASS) (CLASS.bReal)
#define DIGITALCLASS_HAS_CODE(CLASS) (CLASS.bCode)
#define DIGITALCLASS_HAS_IMAGE(CLASS) (CLASS.bImage)
#define DIGITALCLASS_HAS_AUDIO(CLASS) (CLASS.bAudio)
#define DIGITALCLASS_HAS_TEXT(CLASS) (CLASS.bText)
#define DIGITALCLASS_HAS_OTHER(CLASS) (CLASS.bOther)
#define REALCLASS_IS_LIVING(CLASS) (CLASS.bLiving)

void Elf_Command(Elf *pElf, const Char *pCommand, U16 nBytes);
void Elf_SetName(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_WakeUp(Elf *pElf);
void Elf_Sleep(Elf *pElf);
void Elf_Connect(Elf *pElf, Net *pNet);
void Elf_Disconnect(Elf *pElf, Net *pNet);
void Elf_What(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_Weather(Elf *pElf);
void Elf_Traffic(Elf *pElf);
void Elf_Game_Play(Elf *pElf, const Char *pName, U16 nBytes);

//Picture
void Elf_Picture_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Picture_Female_Search(Elf *pElf);
void Elf_Picture_Male_Search(Elf *pElf);
void Elf_Picture_Plant_Search(Elf *pElf);
void Elf_Picture_Animal_Search(Elf *pElf);
void Elf_Picture_Building_Search(Elf *pElf);
void Elf_Picture_Nature_Search(Elf *pElf);

//Video
void Elf_Video_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Video_XXX_Search(Elf *pElf);
void Elf_Video_Funny_Search(Elf *pElf);
void Elf_Video_Nature_Search(Elf *pElf);
void Elf_Video_Movie_Search(Elf *pElf);
void Elf_Video_Movie_Play(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Video_Movie_Cartoon_Search(Elf *pElf);
void Elf_Video_Movie_Cartoon_Play(Elf *pElf, const Char *pName, U16 nBytes);

//3D model
void Elf_3DModel_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_3DModel_Female_Search(Elf *pElf);
void Elf_3DModel_Male_Search(Elf *pElf);
void Elf_3DModel_Plant_Search(Elf *pElf);
void Elf_3DModel_Animal_Search(Elf *pElf);
void Elf_3DModel_Building_Search(Elf *pElf);

//Audio
void Elf_Audio_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Audio_XXX_Search(Elf *pElf);
void Elf_Audio_Story_Search(Elf *pElf);
void Elf_Audio_Music_Search(Elf *pElf);
void Elf_Audio_Music_Play(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Audio_Song_Search(Elf *pElf);
void Elf_Audio_Song_Play(Elf *pElf, const Char *pName, U16 nBytes);

//Map
void Elf_Map_Download(Elf *pElf, RectangleF32 *pRect);
void Elf_Map_City_Download(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Map_Province_Download(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Map_Country_Download(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Map_Earth_Download(Elf *pElf);

//Book
void Elf_Book_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Book_XXX_Search(Elf *pElf);
void Elf_Book_Math_Search(Elf *pElf);
void Elf_Book_Physics_Search(Elf *pElf);
void Elf_Book_Chemistry_Search(Elf *pElf);
void Elf_Book_Biology_Search(Elf *pElf);
void Elf_Book_Medicine_Search(Elf *pElf);
void Elf_Book_Cooking_Search(Elf *pElf);
void Elf_Book_Novel_Search(Elf *pElf);
void Elf_Book_Comic_Search(Elf *pElf);

//Location
void Elf_Where(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_Locate(Elf *pElf, const Location *pLocation);
void Elf_LocateNearest(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_Hotel(Elf *pElf);
void Elf_Restaurant(Elf *pElf);
void Elf_Toilet(Elf *pElf);
void Elf_Bank(Elf *pElf);
void Elf_Hospital(Elf *pElf);
void Elf_PoliceStation(Elf *pElf);

//Service
void Elf_Service_Food(Elf *pElf);
void Elf_Service_Water(Elf *pElf);
void Elf_Service_CallGirl(Elf *pElf);
void Elf_Service_Police(Elf *pElf);

//Buy
void Elf_Buy_House(Elf *pElf);
void Elf_Buy_Car(Elf *pElf);
void Elf_Buy_Electric(Elf *pElf);
void Elf_Buy_Misc(Elf *pElf);

//Sell
void Elf_Sell_House(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Sell_Car(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Sell_Electric(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Sell_Misc(Elf *pElf);

//Pay
void Elf_Pay(Elf *pElf, const Char *pTargetAccount, U16 nTargetAccountBytes, const Char *pAccount, U16 nAccountBytes, const Char *pPassword, U16 nPasswordBytes);

#endif //ELF_H

