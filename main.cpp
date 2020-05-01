#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <pthread.h>
#include "src/disas.h"
#include <sys/time.h>
#include "display/draw.h"
void *UpdateDisplay(void*);
void *DrawDisplay(void*);
void *RunCpu(void*);
void *RunCpuDebug(void*);
uint8_t *mem;
int main(){
    Cpu *cpu=new Cpu;
    readfile("invaders.h",&cpu->Memory[0x000]);
    readfile("invaders.g",&cpu->Memory[0x800]);
    readfile("invaders.f",&cpu->Memory[0x1000]);
    readfile("invaders.e",&cpu->Memory[0x1800]);
    int i=0;
    cpu->SP=0xf000;
    /*FILE *f2=fopen("dbgfile","w");
    fprintf(f2," ");
    fclose(f2);
    cout<<&pixels<<endl;*/
    int dim[2]={256,224};
    pthread_t upd,draw,run;
    pixels=new uint8_t[SIZE*8 ];
    clear();
    pthread_create(&run,NULL,RunCpu,(void *)cpu);
    pthread_create(&upd,NULL,UpdateDisplay,(void *)&cpu->Memory[0x2400]);
    pthread_create(&draw,NULL,DrawDisplay,(void *)dim);
    pthread_join(run,NULL);
    pthread_join(upd,NULL);
    pthread_join(draw,NULL);
}
void *UpdateDisplay(void* args){
    while(true){
        //cout<<"Update is Running"<<endl;
        mem=(uint8_t *)args;
        setpixels(mem);
        //usleep(10000);
    }
    return NULL;
}
void *DrawDisplay(void *args){
    
    int *dim=(int *)args;
    draw(dim[0],dim[1]);
    return NULL;
}
void *RunCpu(void *args){
    int i=0;
    Cpu *cpu=(Cpu *)args;
    while(cpu->Memory[cpu->PC]!=0x76){
        excop(cpu);
        //disas(cpu,stdout);
        if(i++==100000){break;}
    }
    
   /* FILE *img=fopen("imgdump","w");
    for(i=0x2400;i<0x4000;i++){
        if(i%100==0)fprintf(img,"\n ");
        fprintf(img,"%d,",cpu->Memory[i]);
    }
    fclose(img);*/
    return NULL;
}
void *RunCpuDebug(void *args){
    Cpu *cpu=(Cpu *)args;
    int i=0;
    FILE *f2=fopen("dbgfile","w");
    fprintf(f2," ");
    fclose(f2);
    while(cpu->Memory[cpu->PC]!=0x76){
        if(cpu->PC==0x1a37){
            //cpuinfo(cpu);
        }
        FILE *f2=fopen("dbgfile","a");
        excop(cpu);
        if(i>37400&&i<50000){
            fprintf(f2,"{%d}",i);disas(cpu,f2,true);}
        if(i==37600)break;
        i++;
        fclose(f2);
    }
    FILE *img=fopen("imgdump","w");
    for(i=0x2400;i<0x4000;i++){
        if(i%20==0)fprintf(img,"\n ");
        else fprintf(img,"%d,",cpu->Memory[i]);
    }
    fclose(img);
    return NULL;
}