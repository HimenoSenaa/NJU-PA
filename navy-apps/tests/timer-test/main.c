#include <stdio.h>
#include <sys/time.h>
#include <malloc.h>
#include <NDL.h>
int main() {
   NDL_Init(0);
  uint32_t top = 5000, cnt = 0;
  while(1) {
    cnt = NDL_GetTicks();
    while(cnt < top) {
        cnt = NDL_GetTicks();
    }
    printf("5s has been lost\n");
    top += 5000;
  }
   NDL_Quit();
}
