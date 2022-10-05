#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t p =*(uint32_t*)(0xa0000060); 

  kbd->keydown = (p & KEYDOWN_MASK ? true : false);
  kbd->keycode = p & ~KEYDOWN_MASK;
}
