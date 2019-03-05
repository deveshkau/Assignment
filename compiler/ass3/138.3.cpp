/**
 * @Author - Ankit Tiwari
 * Roll No - CSE/16148/138
 * Reg No - 138
 */

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

  // read ELF header
  fread(&elfHdr, 1, sizeof(Elf64_Ehdr), ElfFile);

  if(!isELFFile(elfHdr)) {
    cout<<"Not an ELF file"<<endl;
    exit(-1);
  }

  // Seek to the header of section name string table
  fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(sectHdr), SEEK_SET);
  fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);

  // Seek to section name string table
  fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
  // Allocate memory for section names
  SecStrTable = (char *) malloc(sectHdr.sh_size);
  // Read the content of section name string table in SecStrTable
  fread(SecStrTable, 1, sectHdr.sh_size, ElfFile);


  // Print all the section names present in section name string table
  cout << "Section names are: "<<endl;
  while(i < sectHdr.sh_size) {
    cout<<SecStrTable[i];

    if(SecStrTable[i] == '\0')
      cout<<endl;
    i++;
  }

  // Find the offset and size for .text section
  unsigned int textSectionOffset, textSectionSize;

  // Loop through all the sections in section header table
  // and find the section whose section name matches with ".text"
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

  // Read the textSection
  char textSection[textSectionSize];

  fseek(ElfFile, textSectionOffset , SEEK_SET);
  fread(&textSection, 1, textSectionSize, ElfFile);
  textSection[textSectionSize] = '\0';


  // Print the text section
  cout<<"Hex Code: \n\n";
  i = 0;
  while(i < textSectionSize) {
    cout<<hex<< (0xff & textSection[i++])<<" ";
  }

  return 0;
}