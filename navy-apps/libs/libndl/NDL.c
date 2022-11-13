#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
static int evtdev = -1;
static int fbdev = -1;
static int dispinfo = -1;
static int screen_w = 0, screen_h = 0;

typedef struct size
{
  int w;
  int h;
} Size;
Size size;

uint32_t NDL_GetTicks() {
  struct timeval* tv = malloc(sizeof(struct timeval));
  struct timezone* tz = malloc(sizeof(struct timezone));
  gettimeofday(tv, tz);
  return tv->tv_usec / 1000 + tv->tv_sec * 1000;
}

int NDL_PollEvent(char *buf, int len) {
  buf[0] = '\0';
  assert(evtdev != -1);
  return read(evtdev, buf, len); 
}

void NDL_OpenCanvas(int *w, int *h) {
  printf("size.w = %d, size.h = %d\n", size.w, size.h);
  printf("close success\n");
  if(*w == 0 && *h == 0) {
    *w = size.w;
    *h = size.h;
  }
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  if (w == 0 && h == 0){
    w = size.w;
    h = size.h;
  }
  printf("w = %d\n", w);
  printf("h = %d\n", h);
  assert(w > 0 && w <= size.w);
  assert(h > 0 && h <= size.h);
  printf("fbdev = %d\n", fbdev);
  for (size_t row = 0; row < h; ++row){
    lseek(fbdev, x + (y + row) * size.w, SEEK_SET);
    write(fbdev, pixels + row * w,  w);
  }
  // write(fbdev, 0, 0);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  evtdev = open("/dev/events", 0, 0);
  fbdev = open("/dev/fb", O_RDWR);
  dispinfo = open("/proc/dispinfo", 0, 0);
  // FILE *fp = fopen("/proc/dispinfo", "r");
  // assert(fp);
  // fscanf(fp, "WIDTH:%d\nHEIGHT:%d\n", &size.w, &size.h);
  char buf[64];
  read(dispinfo,buf,64);
  sscanf(buf, "WIDTH:%d\nHEIGHT:%d\n",  &size.w, &size.h);
  printf("get screen width=%d, height=%d\n", size.w, size.h);
  return 0;
}

void NDL_Quit() {
  close(dispinfo);
  close(evtdev);
}