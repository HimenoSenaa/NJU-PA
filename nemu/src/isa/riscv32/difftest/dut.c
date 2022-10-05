#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"


extern const char *regs[32];

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc)
{
    bool issame = true;
    for (int i = 0; i < 32; i++) {
        if (ref_r->gpr[i]._32 != cpu.gpr[i]._32) {
            Log("\033[31m%s: ref=0x%08x\tdut=0x%08x\033[0m at 0x%08x", regs[i], ref_r->gpr[i]._32, cpu.gpr[i]._32, cpu.pc);
            issame = false;
        }
    }
    if (ref_r->pc != cpu.pc) {
        Log("\033[31mpc: ref=0x%08x\tdut=0x%08x\033[0m", ref_r->pc, cpu.pc);
        issame = false;
    }
    // HANDLE_CSRS(CHECK_CSRS)

    return issame;
}


// bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
//   for(int i = 0; i < 32; i++) {
//     if(!difftest_check_reg(reg_name(i, 0), pc, ref_r->gpr[i]._32, gpr(i))) {
//       return false;
//     }
//   }
//   return true;
// }

void isa_difftest_attach() {
}
