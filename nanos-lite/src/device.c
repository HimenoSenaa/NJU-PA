#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static AM_GPU_FBDRAW_T gpu_fbdraw;
static AM_GPU_CONFIG_T gpu_config;
static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

static char *__itoa(int num, char *buff)
{
  char tmp[110];
  if (num == 0)
  {
    strcat(buff, "0");
    return buff;
  }

  uint8_t i = 0;
  while (num != 0)
  {
    tmp[i] = num % 10 + '0';
    num /= 10;
    i++;
  }

  for (int j = i - 1; j >= 0; --j)
    buff[i - 1 - j] = tmp[j];
  buff[i] = '\0';

  return buff;
}

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char* str = (char*)buf;
  for(int i = 0; i < len; i++) {
    putch(str[i]);
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) return 0;
  
  char *tag = ev.keydown ? "kd " : "ku ";
  if(len < 3) panic("strncat error for len < 3");
  strncat(buf, tag, 3);
  if(len - 3 < strlen(keyname[ev.keycode]))  panic("strncat error for len - 3 < strlen(keyname[ev.keycode])");
  strncat(buf, keyname[ev.keycode], strlen(keyname[ev.keycode]));
  if(len - 3 - strlen(keyname[ev.keycode]) < 2) panic("strncat error for len - 3 - strlen(keyname[ev.keycode]) < 2");
  strncat(buf, "\n", 1);
  return strlen(buf);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  Log("dispinfo_reading\n");
  int w = io_read(AM_GPU_CONFIG).width; 
  int h = io_read(AM_GPU_CONFIG).height;
  char num_buf[50];
  strcpy(buf, "WIDTH:");
  strcat(buf, __itoa(w, num_buf));
  strcat(buf, "\nHEIGHT:");
  strcat(buf, __itoa(h, num_buf));
  strcat(buf, "\n");
  return strlen((char *)buf);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  if (len == 0)
  {
    // printf("end!\n");
    gpu_fbdraw.sync = 1;
    gpu_fbdraw.w = 0;
    gpu_fbdraw.h = 0;
    ioe_write(AM_GPU_FBDRAW, &gpu_fbdraw);
    return 0;
  }

  int width = gpu_config.width;

  gpu_fbdraw.pixels = (void *)buf;
  gpu_fbdraw.w = len;
  gpu_fbdraw.h = 1;
  gpu_fbdraw.x = offset % width;
  gpu_fbdraw.y = offset / width;
  gpu_fbdraw.sync = 0;
  ioe_write(AM_GPU_FBDRAW, &gpu_fbdraw);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();

  ioe_read(AM_GPU_CONFIG, &gpu_config);
}
