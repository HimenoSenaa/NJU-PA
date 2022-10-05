#include "include/fixedptc.h"
#include <stdio.h>
#include <math.h>



int main() {
    fixedpt a = fixedpt_rconst(1.2);
    fixedpt b = fixedpt_muli(a, 10);
    fixedpt c = fixedpt_rconst(2.6);
    fixedpt d = fixedpt_rconst(-9.999);
    printf("muli(1.2, 10) = %f\n", fixedpt_tofloat(b));
    printf("mul(1.2, 2.6) = %f\n", fixedpt_tofloat(fixedpt_mul(a, c)));
    printf("d = %x\n", d);
    printf("abs(d) = %x\n", fixedpt_abs(d));
    printf("abs(-9.999) = %f\n", fixedpt_tofloat(fixedpt_abs(d)));
    for(int i = 0; i <= 10; ++i) {
        fixedpt e = fixedpt_rconst(-1 + i * 0.2);
        printf("e = %x, %f, floor(e) = %f\n", e, fixedpt_tofloat(e), fixedpt_tofloat(fixedpt_ceil(e))); 
    }
}