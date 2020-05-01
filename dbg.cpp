#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include "src/disas.h"


int main(){
    Cpu *cpu=new Cpu;
    readfile("cpudiag.bin",&cpu->Memory[0x100]);
    //Fix the first instruction to be JMP 0x100    
    cpu->PC=0;
    cpu->Memory[0]=0xc3;    
    cpu->Memory[1]=0;    
    cpu->Memory[2]=0x01;    

    //Fix the stack pointer from 0x6ad to 0x7ad    
    // this 0x06 byte 112 in the code, which is    
    // byte 112 + 0x100 = 368 in memory    
    cpu->Memory[368] = 0x7;    

    //Skip DAA test    
    cpu->Memory[0x59c] = 0xc3; //JMP    
    cpu->Memory[0x59d] = 0xc2;    
    cpu->Memory[0x59e] = 0x05;
   
    int i=0;
    FILE *f2=fopen("dbgfile","w");
    fprintf(f2," ");
    fclose(f2);
    while(cpu->Memory[cpu->PC]!=0x76){
        FILE *f2=fopen("dbgfile","a");
        excop(cpu);
        disas(cpu,f2);
        if(i==1000)break;
        i++;
        fclose(f2);
    }
    
}