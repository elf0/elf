#ifndef ELF_H
#define ELF_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

typedef struct Elf_s Elf;

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
void Elf_Movie_Search(Elf *pElf);
void Elf_Movie_Play(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Cartoon_Search(Elf *pElf);
void Elf_Cartoon_Play(Elf *pElf, const Char *pName, U16 nBytes);

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
void Elf_Music_Search(Elf *pElf);
void Elf_Music_Play(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Song_Search(Elf *pElf);
void Elf_Song_Play(Elf *pElf, const Char *pName, U16 nBytes);

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
void Elf_Novel_Search(Elf *pElf);
void Elf_Comic_Search(Elf *pElf);

//Location
void Elf_Where(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_Locate(Elf *pElf, const Location *pLocation);
void Elf_LocateNearest(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_Hotel(Elf *pElf);
void Elf_Restaurant(Elf *pElf);
void Elf_Toilet(Elf *pElf);
void Elf_Bank(Elf *pElf);
void Elf_PoliceStation(Elf *pElf);

//Service
void Elf_Service_Food(Elf *pElf);
void Elf_Service_Water(Elf *pElf);
void Elf_Service_CallGirl(Elf *pElf);
void Elf_Service_Police(Elf *pElf);

#endif //ELF_H

