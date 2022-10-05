def_EHelper(lw) {
  // printf("load the lw\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_lms(s, ddest, dsrc1, id_src2->imm, 4);
  // printf("load the %p = %p + %x\n", ddest, dsrc1, id_src2->imm);
  // printf("Ending def_EHelper: lw, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(lwu) {
  // printf("load the lw\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
  // printf("load the %p = %p + %x\n", ddest, dsrc1, id_src2->imm);
  // printf("Ending def_EHelper: lwu, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(lb) {
  // printf("load the lb\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_lms(s, ddest, dsrc1, id_src2->imm, 1);
  // printf("load the %p = %p + %x\n", ddest, dsrc1, id_src2->imm);
  // printf("Ending def_EHelper: lbu, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(lbu) {
  // printf("load the lbu\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 1);
  // printf("load the %p = %p + %x\n", ddest, dsrc1, id_src2->imm);
  // printf("Ending def_EHelper: lbu, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(lh) {
  // printf("load the lh\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_lms(s, ddest, dsrc1, id_src2->imm, 2);
  // printf("load the %p = %p + %x\n", ddest, dsrc1, id_src2->imm);
  // printf("Ending def_EHelper: lh, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(lhu) {
  // printf("load the lhu\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 2);
  // printf("load the %p = %p + %x\n", ddest, dsrc1, id_src2->imm);
  // printf("Ending def_EHelper: lhu, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(sw) {
  // printf("load the sw\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
  // printf("store the %p + %x : %p = %x\n", dsrc1, id_src2->imm, dsrc1 + id_src2->imm, *ddest);
  // printf("Ending def_EHelper: sw, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(sh) {
  // printf("load the sh\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 2);
  // printf("store the %p + %x : %p = %x\n", dsrc1, id_src2->imm, dsrc1 + id_src2->imm, *ddest);
  // printf("Ending def_EHelper: sh, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}

def_EHelper(sb) {
  // printf("load the sb\n");
  // rtl_sext(s, &id_src2->imm, &id_src2->imm, 2);
  // printf("%d   %x\n", id_src2->imm, id_src2->imm);
  // printf("%d   %x\n", s->pc, s->pc);
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 1);
  // printf("store the %p + %x : %p = %x\n", dsrc1, id_src2->imm, dsrc1 + id_src2->imm, *ddest);
  // printf("Ending def_EHelper: sb, pc = %x, dnpc = %x\n", s->pc, s->dnpc);
  // isa_reg_display();
}