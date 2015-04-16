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
void Elf_Where(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_Locate(Elf *pElf, const Location *pLocation);
void Elf_LocateNearest(Elf *pElf, const Char *pKey, U16 nBytes);
void Elf_PlayMusic(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_PlayMovie(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_PlayGame(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_Weather(Elf *pElf);
void Elf_Traffic(Elf *pElf);
void Elf_Hotel(Elf *pElf);
void Elf_Restaurant(Elf *pElf);
void Elf_Toilet(Elf *pElf);
void Elf_Bank(Elf *pElf);
void Elf_PoliceStation(Elf *pElf);
void Elf_DownloadMap(Elf *pElf, RectangleF32 *pRect);
void Elf_DownloadMapOfCity(Elf *pElf, const Char *pName, U16 nBytes);
void Elf_DownloadMapOfCountry(Elf *pElf, const Char *pName, U16 nBytes);

#endif //ELF_H

