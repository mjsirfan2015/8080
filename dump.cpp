#include <iostream>
#include "src/disas.h"
using namespace std;
int main(){
    FILE *fp=fopen("dump","w");
    Cpu *cpu=new Cpu;
    readfile("invaders.h",&cpu->Memory[0x000]);
    readfile("invaders.g",&cpu->Memory[0x800]);
    readfile("invaders.f",&cpu->Memory[0x1000]);
    readfile("invaders.e",&cpu->Memory[0x1800]);
    disas_full(cpu,fp,0x2000);
}