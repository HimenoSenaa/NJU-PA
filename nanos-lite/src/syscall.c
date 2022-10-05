#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <sys/time.h>
extern Finfo file_table[];

void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
}

void sys_exit(Context *c) {
  halt(0);
  c->GPRx = 0;
}

void sys_write(Context *c) {
  // if(c->GPR2 == 1 || c->GPR2 == 2) {
  //   // printf("%d\n", c->GPR4);
  //   for(int i = 0; i < c->GPR4; i++) {
  //     putch(*(((char *)c->GPR3) + i));
  //   }
  //   putch('\n');
  //   c->GPRx = c->GPR4;
  // } else if(c->GPR2 >= 3 && c-> GPR2 <= 23){
    c->GPRx = fs_write(c->GPR2, (void*)c->GPR3, c->GPR4);
  // }
}

void sys_read(Context *c) {
  c->GPRx = fs_read(c->GPR2, (void*)c->GPR3, c->GPR4);
}
void sys_open(Context *c) {
  c->GPRx = fs_open((char*)c->GPR2, c->GPR3, c->GPR4);
}

void sys_close(Context* c) {
  c->GPRx = fs_close(c->GPR2);
}
void sys_brk(Context* c) {
  c->GPRx = 0;
}

void sys_lseek(Context* c) {
  c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
}

void sys_gettimeofday(Context* c) {
 struct timeval* tv = (struct timeval*)c->GPR2;
 uint64_t time = io_read(AM_TIMER_UPTIME).us;
 tv->tv_usec = time % 1000000;
 tv->tv_sec  = time / 1000000;
 c->GPRx = 0;
}

void PGPR(Context *c) {
  printf("GPR1 = %x\n", c->GPR1);
  printf("GPR2 = %x\n", c->GPR2);
  printf("GPR3 = %x\n", c->GPR3);
  printf("GPR4 = %x\n", c->GPR4);
}

void PGPRS(Context *c) {
  printf("GPR1 = %x\n", c->GPR1);
  printf("GPR2 = %s\n", c->GPR2);
  printf("GPR3 = %x\n", c->GPR3);
  printf("GPR4 = %x\n", c->GPR4);
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  // #ifdef CONFIG_STRACE
  //   if(a[0] != 2) 
  //     PGPR(c);
  //   else 
  //     PGPRS(c);
  // #endif
  switch (a[0]) {
    case SYS_yield:
      sys_yield(c);
      break;
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_write:
      sys_write(c);
      break;
    case SYS_brk:
      sys_brk(c);
      break;
    case SYS_open:
      sys_open(c);
      break;
    case SYS_close:
      sys_close(c);
      break;
    case SYS_read:
      sys_read(c);
      break;
    case SYS_lseek:
      sys_lseek(c);
      break;
    case SYS_gettimeofday:
      sys_gettimeofday(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
