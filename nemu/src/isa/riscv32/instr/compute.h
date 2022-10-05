#include<isa.h>
def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}
def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mul) {
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulh) {
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulhu) {
  rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulhsu) {
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(div) {
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(divu) {
  rtl_divu_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(remu) {
  rtl_divu_r(s, ddest, dsrc1, dsrc2);
}


def_EHelper(xor) {
  rtl_xor(s, ddest, dsrc1, dsrc2);
} 

def_EHelper(or) {
  // rtl_li(s, ddest, (*dsrc1) | (*dsrc2));
  rtl_or(s, ddest, dsrc1, dsrc2);
} 
def_EHelper(sll) {
 // rtl_li(s, ddest, *dsrc1 << (*dsrc2 & (0x1f)));
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  // rtl_li(s, ddest, (sword_t)*dsrc1 >> (*dsrc2 & (0x1f)));
  rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srl) {
  // rtl_li(s, ddest, *dsrc1 >> (*dsrc2 & (0x1f)));
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  // rtl_li(s, ddest, (*dsrc1) & (*dsrc2));
  rtl_and(s, ddest, dsrc1, dsrc2);
}



def_EHelper(addi) {
  // rtl_mv(s, ddest, id_src1->preg);
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  // rtl_addi(s, ddest, ddest, id_src2->imm);
  rtl_addi(s, ddest, dsrc1, c_sext(id_src2->imm, 20));
}

def_EHelper(srai) {
  // rtl_li(s, ddest, ((sword_t)*dsrc1) >> (id_src2->imm & (0x3f)));
  rtl_srai(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(andi) {
  // rtl_andi(s, ddest, dsrc1, id_src2->imm);
  rtl_andi(s, ddest, dsrc1, c_sext(id_src2->imm, 20));
}

def_EHelper(ori) {
 // rtl_ori(s, ddest, dsrc1, id_src2->imm);
 rtl_ori(s, ddest, dsrc1, c_sext(id_src2->imm, 20));
}

def_EHelper(slli) {
//  rtl_li(s, ddest, *dsrc1 << (id_src2->imm & (0x3f)));
  rtl_slli(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(xori) {
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  // rtl_li(s, ddest, (*dsrc1) ^ (id_src2->imm));
  rtl_xori(s, ddest, dsrc1, c_sext(id_src2->imm, 20));
}

def_EHelper(srli) {
//  rtl_li(s, ddest, (*dsrc1) >> (id_src2->imm & (0x3f)));
  rtl_srli(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(auipc) {
  rtl_addi(s, ddest, &s->pc, c_sext(id_src1->imm, 0));
}


def_EHelper(sltu) {
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
}

def_EHelper(slti) {
  rtl_setrelopi(s, RELOP_LT, ddest, dsrc1, c_sext(id_src2->imm, 20));
}
def_EHelper(sltiu) {
  rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, c_sext(id_src2->imm, 20));
}

def_EHelper(jal) {
  rtl_li(s, ddest, s->pc + 4);
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 4);
  // rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
  // important: need to fresh the pc register
  rtl_addi(s, s0, &(s->pc), c_sext(id_src2->imm, 11));
  rtl_j(s, *s0);
}

def_EHelper(jalr) {
  rtl_addi(s, s0, &(s->pc), 4);

  rtl_addi(s, s1, id_src1->preg, c_sext(id_src2->imm, 20));
  rtl_andi(s, s1, s1, -2); //(sword_t)-2 is (word_t)0xfffffffe
  rtl_j(s, *s1);
  rtl_mv(s, id_dest->preg, s0);
}

def_EHelper(beq) {  
  // rtl_sext(s, &id_dest->imm, &id_dest->imm, 2);
  // if(*dsrc1 == *dsrc2) {
  //   s->dnpc = s->pc + id_dest->imm;
  // } 
  rtl_setrelop(s, RELOP_EQ, s0, id_src1->preg, id_src2->preg);
  if (*s0)
  {
    rtl_addi(s, s0, &(s->pc), c_sext(id_dest->imm, 19));
    rtl_j(s, *s0);
  }
}

def_EHelper(bne) {
  // rtl_sext(s, &id_dest->imm, &id_dest->imm, 2);
  // if(*dsrc1 != *dsrc2) {
  //   s->dnpc = s->pc + id_dest->imm;
  // } 
  rtl_setrelop(s, RELOP_NE, s0, id_src1->preg, id_src2->preg);
  if (*s0)
  {
    rtl_addi(s, s0, &(s->pc), c_sext(id_dest->imm, 19));
    rtl_j(s, *s0);
  }
}

def_EHelper(bge) {
  // rtl_sext(s, &id_dest->imm, &id_dest->imm, 2);
  // if((sword_t)*dsrc1 >= (sword_t) *dsrc2) {
  //   s->dnpc = s->pc + id_dest->imm;
  // } 
  rtl_setrelop(s, RELOP_GE, s0, id_src1->preg, id_src2->preg);
  if (*s0)
  {
    rtl_addi(s, s0, &(s->pc), c_sext(id_dest->imm, 19));
    rtl_j(s, *s0);
  }
}

def_EHelper(bgeu) {
  // rtl_sext(s, &id_dest->imm, &id_dest->imm, 2);
  // if(*dsrc1 >= *dsrc2) {
  //   s->dnpc = s->pc + id_dest->imm;
  // }
  rtl_setrelop(s, RELOP_GEU, s0, id_src1->preg, id_src2->preg);
  if (*s0)
  {
    rtl_addi(s, s0, &(s->pc), c_sext(id_dest->imm, 19));
    rtl_j(s, *s0);
  }
}
def_EHelper(blt) {
  // rtl_sext(s, &id_dest->imm, &id_dest->imm, 2);
  // if((sword_t) *dsrc1 < (sword_t)*dsrc2) {
  //   s->dnpc = s->pc + id_dest->imm;
  // }
  rtl_setrelop(s, RELOP_LT, s0, id_src1->preg, id_src2->preg);
  if (*s0)
  {
    rtl_addi(s, s0, &(s->pc), c_sext(id_dest->imm, 19));
    rtl_j(s, *s0);
  }
}

def_EHelper(bltu) {
  // rtl_sext(s, &id_dest->imm, &id_dest->imm, 2);
  // if(*dsrc1 < *dsrc2) {
  //   s->dnpc = s->pc + id_dest->imm;
  // }
  rtl_setrelop(s, RELOP_LTU, s0, id_src1->preg, id_src2->preg);
  if (*s0)
  {
    rtl_addi(s, s0, &(s->pc), c_sext(id_dest->imm, 19));
    rtl_j(s, *s0);
  }
}

static inline word_t transfer(word_t x) {
  switch (x) {
  case 0x305: 
    return REG_MTVEC;
    break;
  case 0x342: 
    return REG_MCAUSE;
    break;
  case 0x300:
    return REG_MSTATUS;
    break;
  case 0x341:
    return REG_MEPC;
    break;
  default:
    Assert(0, "transfer error");
    break;
  }
  return 0;
}
def_EHelper(ecall) {
  word_t vec = isa_raise_intr(11, cpu.pc);
  IFDEF(CONFIG_ETRACE, Log("\033[31;43mj to %x", vec));
  rtl_j(s, vec);
}

def_EHelper(ebreak) {
  word_t vec = isa_raise_intr(11, cpu.pc);
  IFDEF(CONFIG_ETRACE, Log("\033[31;43mj to %x", vec));
  printf("in the ebreak\n");
  rtl_j(s, vec);
}

def_EHelper(csrrw) {
  rtl_mv(s, s0, &csr.reg[transfer(id_src2->imm)]._32);
  rtl_mv(s, &csr.reg[transfer(id_src2->imm)]._32, dsrc1);
  rtl_mv(s, ddest, s0); 
}

def_EHelper(csrrs) {
  rtl_mv(s, s0, &csr.reg[transfer(id_src2->imm)]._32);
  rtl_or(s, &csr.reg[transfer(id_src2->imm)]._32, s0, dsrc1);
  rtl_mv(s, ddest, s0);
}

def_EHelper(mret) {
  IFDEF(CONFIG_ETRACE, Log("\033[31;43mmret to %x", csr.reg[REG_MEPC]._32));
  s->dnpc = csr.reg[REG_MEPC]._32;
}

// def_EHelper(lui) {
//   rtl_li(s, ddest, id_src1->imm);
// }

// def_EHelper(addi) {
//   rtl_addi(s, ddest, dsrc1, id_src2->imm);
// }

// def_EHelper(andi) {
//   rtl_andi(s, ddest, dsrc1, id_src2->imm);
// }

// def_EHelper(xori) {
//   rtl_xori(s, ddest, dsrc1, id_src2->imm);
// }

// def_EHelper(ori) {
//   rtl_ori(s, ddest, dsrc1, id_src2->imm);
// }


// def_EHelper(sll) {
//   rtl_sll(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(add) {
//   rtl_add(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(sub) {
//   rtl_sub(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(mul) {
//   rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(mulh) {//TODO
//   rtl_muls_hi(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(mulhu) {//TODO
//   rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(sltu) {
//   rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
// }

// def_EHelper(slt) {
//   rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
// }

// def_EHelper(xor) {
//   rtl_xor(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(div) {
//   rtl_divs_q(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(divu) {
//   rtl_divu_q(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(rem) {
//   rtl_divs_r(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(remu) {
//   rtl_divu_r(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(or) {
//   rtl_or(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(and) {
//   rtl_and(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(sra) {
//   rtl_sra(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(srai) {
//   rtl_srai(s, ddest, dsrc1, id_src2->imm & 0b111111u);
// }

// def_EHelper(srl) {
//   rtl_srl(s, ddest, dsrc1, dsrc2);
// }

// def_EHelper(srli) {
//   rtl_srli(s, ddest, dsrc1, id_src2->imm & 0b111111u);
// }

// def_EHelper(slli) {
//   rtl_slli(s, ddest, dsrc1, id_src2->imm & 0b111111u);
// }

// def_EHelper(beq) {
//   if (*dsrc1 == *ddest){
//     rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
//   }
// }

// def_EHelper(bne) {
//   if (*dsrc1 != *ddest){
//     rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
//   }
// }

// def_EHelper(bge) {
//   if ((sword_t)(*dsrc1) >= (sword_t)(*ddest)){
//     rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
//   }
// }

// def_EHelper(bgeu) {
//   if ((*dsrc1) >= (*ddest)){
//     rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
//   }
// }

// def_EHelper(blt) {
//   if ((sword_t)(*dsrc1) < (sword_t)(*ddest)){
//     rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
//   }
// }

// def_EHelper(bltu) {
//   if ((*dsrc1) < (*ddest)){
//     rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
//   }
// }

// def_EHelper(sltiu) {
//   rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, id_src2->imm);
// }

// def_EHelper(slti) {
//   rtl_setrelopi(s, RELOP_LT, ddest, dsrc1, id_src2->imm);
// }

// def_EHelper(auipc) {
//   rtl_addi(s, ddest, &s->pc, id_src1->imm);
// }

// def_EHelper(jal) {
//   rtl_addi(s, ddest, &s->pc, 4);
//   rtl_addi(s, &s->dnpc, &s->pc, id_src1->imm);
// }

// def_EHelper(jalr) {
//   rtl_addi(s, s0, &s->pc, 4);
//   rtl_addi(s, &s->dnpc, dsrc1, id_src2->imm);
//   rtl_andi(s, &s->dnpc, &s->dnpc, ~1);
//   rtl_addi(s, ddest, s0, 0);
// }
