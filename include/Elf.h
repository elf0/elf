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
void Elf_PlayMusic(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_PlayMovie(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_PlayGame(Elf *pElf, const Char *pName, U16 nBytes);

//Picture
void Elf_Picture_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Picture_SearchGirls(Elf *pElf);

//Video
void Elf_Video_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_Video_SearchXXX(Elf *pElf);

//3D model
void Elf_3DModel_Upload(Elf *pElf, const Char *pPathName, U16 nBytes);
void Elf_3DModel_SearchPlant(Elf *pElf);
void Elf_3DModel_SearchAnimal(Elf *pElf);
void Elf_3DModel_SearchMale(Elf *pElf);
void Elf_3DModel_SearchFemale(Elf *pElf);
void Elf_3DModel_SearchBuilding(Elf *pElf);

//Map
void Elf_DownloadMap(Elf *pElf, RectangleF32 *pRect);
void Elf_DownloadMapOfCity(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_DownloadMapOfProvince(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_DownloadMapOfCountry(Elf *pElf, const Char *pName, U16 nBytes);

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

