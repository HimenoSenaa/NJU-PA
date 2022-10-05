#include <fs.h>


size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, events_read, invalid_write}, 
  {"/proc/dispinfo", 50, 0, dispinfo_read, invalid_write},
  {"/dev/fb", 0, 0, invalid_read, fb_write},
#include "files.h"
};

void init_fs() {
  AM_GPU_CONFIG_T gpu_config;
  ioe_read(AM_GPU_CONFIG, &gpu_config);
  int width = gpu_config.width, height = gpu_config.height;
  int fb_fd = fs_open("/dev/fb", 0, 0);
  file_table[fb_fd].size = width * height;
}

int fs_open(const char *pathname, int flags, int mode) {
  for(int i = 0; i < sizeof(file_table) / sizeof(Finfo); i++) {
      if(strcmp(pathname, file_table[i].name) == 0) {
      // printf("find the file : %s\n", pathname);
      return i;
    }
  }
  assert(0);
}

int fs_close(int fd) {
  file_table[fd].open_offset = 0;
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len) {
  Finfo* file = &file_table[fd];
  size_t real_len;
  if(fd > sizeof(file_table) / sizeof(Finfo)) assert(0);
  
  if(file->read != NULL) {
      real_len = file->read(buf, file->disk_offset + file->open_offset, len);
  }
  else {
      real_len = file->open_offset + len <= file->size ? len : file->size - file->open_offset;
      ramdisk_read(buf, file->disk_offset + file->open_offset, real_len);
  }
  file->open_offset += real_len;
  return real_len;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  Finfo* file = &file_table[fd];
  if(fd > sizeof(file_table) / sizeof(Finfo)) assert(0);
  // printf("open_offset = %x, len = %x, filesz = %x\n", file->open_offset, len, file->size);
  size_t ret;
  if(file->write != NULL) 
    ret = file->write(buf, file->disk_offset + file->open_offset, len);
  else {
    assert(file->open_offset + len <= file->size);
    ret = ramdisk_write(buf, file->disk_offset + file->open_offset, len);
  }
  file->open_offset += len;
  return ret;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  Finfo * file = &file_table[fd];
  switch(whence) {
    case SEEK_SET:
      assert(offset <= file->size);
      file->open_offset = offset;
      return file->open_offset;
    case SEEK_CUR:
      assert(file->open_offset + offset<= file->size);
      file->open_offset += offset;
      return file->open_offset;
    case SEEK_END:
      assert(offset <= file->size);
      file->open_offset = file->size + offset;
      return file->open_offset;
    default:
      assert(0);
  }
}


