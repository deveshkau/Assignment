// @Author - Devesh Kaushik
// Roll No - CSE/16013/170
// Reg No - 170

#include<iostream>
#include<elf.h>
#include<string.h>

using namespace std;

int isELFFile(Elf64_Ehdr elfHdr) {
  if(strncmp((char *) elfHdr.e_ident, "\177ELF", 4) == 0) {
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  FILE *ElfFile = NULL;
  char *SecStrTable = NULL;
  Elf64_Ehdr elfHdr;
  Elf64_Shdr sectHdr;
  unsigned int i = 0;

  if (argc != 2) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(-1);
  }

  if ((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("Error in opening file:");
    exit(-1);
  }

  fread(&elfHdr, 1, sizeof(Elf64_Ehdr), ElfFile);

  if(!isELFFile(elfHdr)) {
    cout<<"Not an ELF file"<<endl;
    exit(-1);
  }

  
  fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(sectHdr), SEEK_SET);
  fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);

  
  fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
  
  SecStrTable = (char *) malloc(sectHdr.sh_size);
  
  fread(SecStrTable, 1, sectHdr.sh_size, ElfFile);


  
  cout << "Section names are: "<<endl;
  while(i < sectHdr.sh_size) {
    cout<<SecStrTable[i];

    if(SecStrTable[i] == '\0')
      cout<<endl;
    i++;
  }

  
  unsigned int textSectionOffset, textSectionSize;

  
  for (i = 0; i < elfHdr.e_shnum; i++) {
    const char *name = "";

    fseek(ElfFile, elfHdr.e_shoff + i * sizeof(sectHdr), SEEK_SET);
    fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);

    name = SecStrTable + sectHdr.sh_name;

    if(strcmp(name, ".text") == 0) {
      textSectionOffset = sectHdr.sh_offset;
      textSectionSize = sectHdr.sh_size;
    }
  }

  
  char textSection[textSectionSize];

  fseek(ElfFile, textSectionOffset , SEEK_SET);
  fread(&textSection, 1, textSectionSize, ElfFile);
  textSection[textSectionSize] = '\0';


  cout<<"Hex Code: \n\n";
  i = 0;
  while(i < textSectionSize) {
    cout<<hex<< (0xff & textSection[i++])<<" ";
  }

  return 0;
}