#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  csr.reg[REG_MCAUSE]._32 = NO;
  csr.reg[REG_MEPC]._32 = epc;
  return  csr.reg[REG_MTVEC]._32;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
