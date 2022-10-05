#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(add) f(sub) f(mul) f(mulh) f(mulhsu) f(mulhu) f(div) \
f(divu) f(rem) f(remu) f(xor) f(or) f(sll) f(sra) f(srl) f(and) f(slt) f(sltu) f(addi) \
f(srai) f(andi) f(ori) f(slli) f(xori) f(slti) f(sltiu) f(srli) f(auipc) f(jal) f(jalr) \
f(beq) f(bne) f(bge) f(bgeu) f(blt) f(bltu) f(lw) f(lwu) f(lb) f(lbu) f(lh) f(lhu) f(sw) f(sh) \
f(ecall) f(ebreak) f(csrrw) f(csrrs) f(mret)\
f(sb) f(inv) f(nemu_trap)

def_all_EXEC_ID();
