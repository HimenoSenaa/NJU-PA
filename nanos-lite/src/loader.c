#include <proc.h>
#include <elf.h>
#include <fs.h>
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Off  Elf64_Off
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Off  Elf32_Off
# define Elf_Phdr Elf32_Phdr
#endif
// static Elf_Phdr ehdr;
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  
  Elf_Ehdr elf_header;

  fs_lseek(fd, 0, SEEK_SET);
  fs_read(fd, &elf_header, sizeof(elf_header));
  //根据小端法 0x7F E L F
  assert(*(uint32_t *)elf_header.e_ident == 0x464c457f);
  
  Elf_Off ph_offset = elf_header.e_phoff;
  uint32_t entry = elf_header.e_entry;
  int ph_num = elf_header.e_phnum;

  Log("phoffset = %d  entry = %p  phnum = %d", ph_offset, entry, ph_num);
  Elf_Phdr phdr;
  for (int i = 0; i < ph_num; ++i){
    printf("in loop %d\n", i);
    fs_lseek(fd, ph_offset + i * sizeof(phdr), SEEK_SET);
    fs_read(fd, &phdr, sizeof(phdr));
    printf("p_type == %d\n", phdr.p_type);
    if(phdr.p_type != PT_LOAD) continue;

    printf("PT_LOAD IN %d\n", i);

    void * virt_addr = (void *)phdr.p_vaddr;
    Elf_Off offset = phdr.p_offset; 
    uint32_t file_size = phdr.p_filesz;
    printf("offset = %p\n", offset);
    fs_lseek(fd, offset, SEEK_SET);
    fs_read(fd, virt_addr, file_size);
    printf("read ended in %d\n", i);
    memset(virt_addr + file_size, 0, 
      phdr.p_memsz - file_size);

    printf("%d\n", i);
  }
  return entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}
