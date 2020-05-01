#include <cstdio>
#include "cpu.h"

int main(){
    Cpu *cpu=new Cpu;
    cpu->H=0xff;
    cpu->L=0xff;
    cpu->B=0x00;
    cpu->C=0x1;
    printf("%02x%02x\n",cpu->H,cpu->L);
    dad(cpu,cpu->H,cpu->L);
    printf("%02x%02x\n",cpu->H,cpu->L);
    return 0;
}