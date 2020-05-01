#include <cstdio>
#include "cpu.h"
#define MAXLEN 35
static char reg[]={'B', 'C', 'D', 'E', 'H', 'L', 'M', 'A'};
static const int instr[]={
//   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
/*0*/1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
/*1*/1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
/*2*/1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,
/*3*/1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
/*4*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*5*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*6*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*7*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*8*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*9*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*a*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*b*/1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*c*/1, 1, 3, 3, 3, 1, 1, 1, 1, 1, 3, 1, 3, 3, 2, 1,
/*d*/1, 1, 3, 2, 3, 1, 1, 1, 1, 1, 3, 2, 3, 1, 2, 1,
/*e*/1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
/*f*/1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1
};
void disas(Cpu *cpu,FILE *fp,bool info=false){
    int ind=cpu->PC;
    uint8_t *data=cpu->Memory;
    int size=ftell(fp);
    fprintf(fp,"0x%02x(%d)[%x]: ", ind, ind, data[ind]);
    switch (data[ind]) {
        case 0x00:
            fprintf(fp,"NOP");
            break;
        case 0x01:
            fprintf(fp,"LXI B,#$%02x%02x", data[ind+2], data[ind+1]);
            break;
        case 0x02:
            fprintf(fp,"STAX B");
            break;
        case 0x03:
            fprintf(fp,"INX B");
            break;
        case 0x04:
            fprintf(fp,"INR B");
            break;

        case 0x05:
            fprintf(fp,"DCR B");
            break;

        case 0x06:
            fprintf(fp,"MVI B,#$%02x", data[ind+1]);
            break;

        case 0x07:
            fprintf(fp,"RLC");
            break;

        case 0x08:
            fprintf(fp,"ERROR!");
            break;

        case 0x09:
            fprintf(fp,"DAD B");
            break;

        case 0x0a:
            fprintf(fp,"LDAX B");
            break;

        case 0x0b:
            fprintf(fp,"DCX B");
            break;

        case 0x0c:
            fprintf(fp,"INR C");
            break;

        case 0x0d:
            fprintf(fp,"DCR C");
            break;

        case 0x0e:
            fprintf(fp,"MVI C,#$%02x", data[ind+1]);
            break;

        case 0x0f:
            fprintf(fp,"RRC");
            break;

        case 0x10:
            fprintf(fp,"ERROR!");
            break;

        case 0x11:
            fprintf(fp,"LXI D,#$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x12:
            fprintf(fp,"STAX D");
            break;

        case 0x13:
            fprintf(fp,"INX D");
            break;

        case 0x014:
            fprintf(fp,"INR D");
            break;

        case 0x15:
            fprintf(fp,"DCR D");
            break;

        case 0x16:
            fprintf(fp,"MVI D,#$%02x", data[ind+1]);
            break;

        case 0x17:
            fprintf(fp,"RAL");
            break;

        case 0x18:
            fprintf(fp,"ERROR");
            break;

        case 0x19:
            fprintf(fp,"DAD d");
            break;

        case 0x1a:
            fprintf(fp,"LDAX D");
            break;

        case 0x1b:
            fprintf(fp,"DCX D");
            break;

        case 0x1c:
            fprintf(fp,"INR E");
            break;

        case 0x1d:
            fprintf(fp,"DCR E");
            break;

        case 0x1e:
            fprintf(fp,"MVI E,#$%02x", data[ind+1]);
            break;

        case 0x1f:
            fprintf(fp,"RAR");
            break;

        case 0x20:
            fprintf(fp,"ERROR");
            break;

        case 0x21:
            fprintf(fp,"LXI H,#$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x22:
            fprintf(fp,"SHLD #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x23:
            fprintf(fp,"INX H");
            break;

        case 0x24:
            fprintf(fp,"INR H");
            break;

        case 0x25:
            fprintf(fp,"DCR H");
            break;

        case 0x26:
            fprintf(fp,"MVI H,#$%02x", data[ind+1]);
            break;

        case 0x27:
            fprintf(fp,"DAA");
            break;

        case 0x28:
            fprintf(fp,"ERROR");
            break;

        case 0x29:
            fprintf(fp,"DAD H");
            break;

        case 0x2a:
            fprintf(fp,"LHLD #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x2b:
            fprintf(fp,"DCX H");
            break;

        case 0x2c:
            fprintf(fp,"INR L");
            break;

        case 0x2d:
            fprintf(fp,"DCR L");
            break;

        case 0x2e:
            fprintf(fp,"MVI L,#$%02x", data[ind+1]);
            break;

        case 0x2f:
            fprintf(fp,"CMA");
            break;

        case 0x30:
            fprintf(fp,"ERROR");
            break;

        case 0x31:
            fprintf(fp,"LXI SP,#$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x32:
            fprintf(fp,"STA adr,#$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x33:
            fprintf(fp,"INX SP");
            break;

        case 0x34:
            fprintf(fp,"INR M");
            break;

        case 0x35:
            fprintf(fp,"DCR M");
            break;

        case 0x36:
            fprintf(fp,"MVI M,#$%02x", data[ind+1]);
            break;

        case 0x37:
            fprintf(fp,"STC");
            break;

        case 0x38:
            fprintf(fp,"ERROR");
            break;

        case 0x39:
            fprintf(fp,"DAD SP");
            break;

        case 0x3a:
            fprintf(fp,"LDA #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0x3b:
            fprintf(fp,"DCX SP");
            break;

        case 0x3c:
            fprintf(fp,"INR A");
            break;

        case 0x3d:
            fprintf(fp,"DCR A");
            break;

        case 0x3e:
            fprintf(fp,"MVI A,#$%02x", data[ind+1]);
            break;

        case 0x3f:
            fprintf(fp,"CMC");
            break;

       case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
            fprintf(fp,"MOV B,%c", reg[(data[ind]-0x40)]);
            break;

        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
            fprintf(fp,"MOV C,%c", reg[(data[ind]-0x48)]);
            break;

        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
            fprintf(fp,"MOV D,%c", reg[(data[ind]-0x50)]);
            break;
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
            fprintf(fp,"MOV E,%c", reg[(data[ind]-0x58)]);
            break;

        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
            fprintf(fp,"MOV H,%c", reg[(data[ind]-0x60)]);
            break;

        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
            fprintf(fp,"MOV L,%c", reg[(data[ind]-0x68)]);
            break;

        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:
            fprintf(fp,"MOV M,%c", reg[(data[ind]-0x70)]);
            break;

        case 0x76:
            fprintf(fp,"HLT");
            break;

        case 0x77:
            fprintf(fp,"MOV M,A");
            break;

        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
            fprintf(fp,"MOV A,%c", reg[(data[ind]-0x78)]);
            break;

        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
            fprintf(fp,"ADD %c", reg[(data[ind]-0x80)]);
            break;

        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
            fprintf(fp,"ADC %c", reg[(data[ind]-0x88)]);
            break;

        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
            fprintf(fp,"SUB %c", reg[(data[ind]-0x90)]);
            break;

        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
            fprintf(fp,"SBB %c", reg[(data[ind]-0x98)]);
            break;

        case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
            fprintf(fp,"ANA %c", reg[(data[ind]-0xa0)]);
            break;

        case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
            fprintf(fp,"XRA %c", reg[(data[ind]-0xa8)]);
            break;

        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
            fprintf(fp,"ORA %c", reg[(data[ind]-0xb0)]);
            break;

        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
            fprintf(fp,"CMP %c", reg[(data[ind]-0xb8)]);
            break;

        case 0xc0:
            fprintf(fp,"RNZ");
            break;

        case 0xc1:
            fprintf(fp,"POP B");
            break;

        case 0xc2:
            fprintf(fp,"JNZ #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xc3:
            fprintf(fp,"JMP #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xc4:
            fprintf(fp,"CNZ #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xc5:
            fprintf(fp,"PUSH B");
            break;

        case 0xc6:
            fprintf(fp,"ADI #$%02x", data[ind+1]);
            break;

        case 0xc7:
            fprintf(fp,"RST");
            break;

        case 0xc8:
            fprintf(fp,"RZ");
            break;

        case 0xc9:
            fprintf(fp,"RET");
            break;

        case 0xca:
            fprintf(fp,"JZ #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xcb:
            fprintf(fp,"ERROR!");
            break;

        case 0xcc:
            fprintf(fp,"CZ #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xcd:
            fprintf(fp,"CALL #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xce:
            fprintf(fp,"ACI #$%02x", data[ind+1]);
            break;

        case 0xcf:
            fprintf(fp,"RST");
            break;

        case 0xd0:
            fprintf(fp,"RNC");
            break;

        case 0xd1:
            fprintf(fp,"POP D");
            break;

        case 0xd2:
            fprintf(fp,"JNC #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xd3:
            fprintf(fp,"OUT #$%02x", data[ind+1]);
            break;

        case 0xd4:
            fprintf(fp,"CNC #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xd5:
            fprintf(fp,"PUSH D");
            break;

        case 0xd6:
            fprintf(fp,"SUI #$%02x", data[ind+1]);
            break;

        case 0xd7:
            fprintf(fp,"RST");
            break;

        case 0xd8:
            fprintf(fp,"RC");
            break;

        case 0xd9:
            fprintf(fp,"ERROR!");
            break;

        case 0xda:
            fprintf(fp,"JC #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xdb:
            fprintf(fp,"IN #$%02x", data[ind+1]);
            break;

        case 0xdc:
            fprintf(fp,"CC #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xdd:
            fprintf(fp,"ERROR!");
            break;

        case 0xde:
            fprintf(fp,"SBI #$%02x", data[ind+1]);
            break;

        case 0xdf:
            fprintf(fp,"RST");
            break;

        case 0xe0:
            fprintf(fp,"RPO");
            break;

        case 0xe1:
            fprintf(fp,"POP H");
            break;

        case 0xe2:
            fprintf(fp,"JPO #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xe3:
            fprintf(fp,"XTHL");
            break;

        case 0xe4:
            fprintf(fp,"CPO #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xe5:
            fprintf(fp,"PUSH H");
            break;

        case 0xe6:
            fprintf(fp,"ANI #$%02x", data[ind+1]);
            break;

        case 0xe7:
            fprintf(fp,"RST");
            break;

        case 0xe8:
            fprintf(fp,"RPE");
            break;

        case 0xe9:
            fprintf(fp,"PCHL");
            break;

        case 0xea:
            fprintf(fp,"JPE #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xeb:
            fprintf(fp,"XCHG");
            break;

        case 0xec:
            fprintf(fp,"CPE #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xed:
            fprintf(fp,"ERROR!");
            break;

        case 0xee:
            fprintf(fp,"XRI #$%02x", data[ind+1]);
            break;

        case 0xef:
            fprintf(fp,"RST");
            break;

        case 0xf0:
            fprintf(fp,"RP");
            break;

        case 0xf1:
            fprintf(fp,"POP PSW");
            break;

        case 0xf2:
            fprintf(fp,"JP #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xf3:
            fprintf(fp,"DI");
            break;

        case 0xf4:
            fprintf(fp,"CP #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xf5:
            fprintf(fp,"PUSH PS");
            break;

        case 0xf6:
            fprintf(fp,"ORI #$%02x", data[ind+1]);
            break;

        case 0xf7:
            fprintf(fp,"RST");
            break;

        case 0xf8:
            fprintf(fp,"RM");
            break;

        case 0xf9:
            fprintf(fp,"SPHL");
            break;

        case 0xfa:
            fprintf(fp,"JM #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xfb:
            fprintf(fp,"EI");
            break;

        case 0xfc:
            fprintf(fp,"CM #$%02x%02x", data[ind+2], data[ind+1]);
            break;

        case 0xfd:
            fprintf(fp,"ERROR!");
            break;

        case 0xfe:
            fprintf(fp,"CPI #$%02x", data[ind+1]);
            break;

        case 0xff:
            fprintf(fp,"RST");
            break;
        default:
            break;
            printf("Unimp");
        }
        //fprintf(fp,"\n%d %d %x",ind,opbytes,data[ind])
        if(info==true){
            size=ftell(fp)-size;
            //printf("%d\n",size);;
            size=MAXLEN-size;
            
            if(size<0){printf("Space format Error!\n");exit(-1);}
            fprintf(fp,"%*s",size,"");
            cpuinfo_concise(cpu,fp);}
        else fprintf(fp,"\n");
    }
    void get(FILE *f2){
    fprintf(f2,"Hi\n");
    fprintf(f2,"Hello");
    fclose(f2);
}
void disas_full(Cpu *cpu,FILE *fp,int dsize){
    int ind=0;
    uint8_t *data=cpu->Memory;
    int size;
    while(ind<dsize){
        size=instr[data[ind]];
        fprintf(fp,"0x%02x(%d)[%x", ind, ind, data[ind]);
        if(size==2){
            fprintf(fp,"%02x]: ", data[ind+1]);
        }
        else if(size==3){
            fprintf(fp,"%02x%02x]: ", data[ind+1],data[ind+2]);
        }
        else{
            fprintf(fp,"]: ");
        }
        switch (data[ind]) {
            case 0x00:
                fprintf(fp,"NOP");
                break;
            case 0x01:
                fprintf(fp,"LXI B,#$%02x%02x", data[ind+2], data[ind+1]);
                break;
            case 0x02:
                fprintf(fp,"STAX B");
                break;
            case 0x03:
                fprintf(fp,"INX B");
                break;
            case 0x04:
                fprintf(fp,"INR B");
                break;

            case 0x05:
                fprintf(fp,"DCR B");
                break;

            case 0x06:
                fprintf(fp,"MVI B,#$%02x", data[ind+1]);
                break;

            case 0x07:
                fprintf(fp,"RLC");
                break;

            case 0x08:
                fprintf(fp,"ERROR!");
                break;

            case 0x09:
                fprintf(fp,"DAD B");
                break;

            case 0x0a:
                fprintf(fp,"LDAX B");
                break;

            case 0x0b:
                fprintf(fp,"DCX B");
                break;

            case 0x0c:
                fprintf(fp,"INR C");
                break;

            case 0x0d:
                fprintf(fp,"DCR C");
                break;

            case 0x0e:
                fprintf(fp,"MVI C,#$%02x", data[ind+1]);
                break;

            case 0x0f:
                fprintf(fp,"RRC");
                break;

            case 0x10:
                fprintf(fp,"ERROR!");
                break;

            case 0x11:
                fprintf(fp,"LXI D,#$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0x12:
                fprintf(fp,"STAX D");
                break;

            case 0x13:
                fprintf(fp,"INX D");
                break;

            case 0x014:
                fprintf(fp,"INR D");
                break;

            case 0x15:
                fprintf(fp,"DCR D");
                break;

            case 0x16:
                fprintf(fp,"MVI D,#$%02x", data[ind+1]);
                break;

            case 0x17:
                fprintf(fp,"RAL");
                break;

            case 0x18:
                fprintf(fp,"ERROR");
                break;

            case 0x19:
                fprintf(fp,"DAD d");
                break;

            case 0x1a:
                fprintf(fp,"LDAX D");
                break;

            case 0x1b:
                fprintf(fp,"DCX D");
                break;

            case 0x1c:
                fprintf(fp,"INR E");
                break;

            case 0x1d:
                fprintf(fp,"DCR E");
                break;

            case 0x1e:
                fprintf(fp,"MVI E,#$%02x", data[ind+1]);
                break;

            case 0x1f:
                fprintf(fp,"RAR");
                break;

            case 0x20:
                fprintf(fp,"ERROR");
                break;

            case 0x21:
                fprintf(fp,"LXI H,#$%02x%02x", data[ind+2], data[ind+1]);
                ind++;
                break;

            case 0x22:
                fprintf(fp,"SHLD #$%02x%02x", data[ind+2], data[ind+1]);
                ind++;
                break;

            case 0x23:
                fprintf(fp,"INX H");
                break;

            case 0x24:
                fprintf(fp,"INR H");
                break;

            case 0x25:
                fprintf(fp,"DCR H");
                break;

            case 0x26:
                fprintf(fp,"MVI H,#$%02x", data[ind+1]);
                break;

            case 0x27:
                fprintf(fp,"DAA");
                break;

            case 0x28:
                fprintf(fp,"ERROR");
                break;

            case 0x29:
                fprintf(fp,"DAD H");
                break;

            case 0x2a:
                fprintf(fp,"LHLD #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0x2b:
                fprintf(fp,"DCX H");
                break;

            case 0x2c:
                fprintf(fp,"INR L");
                break;

            case 0x2d:
                fprintf(fp,"DCR L");
                break;

            case 0x2e:
                fprintf(fp,"MVI L,#$%02x", data[ind+1]);
                break;

            case 0x2f:
                fprintf(fp,"CMA");
                break;

            case 0x30:
                fprintf(fp,"ERROR");
                break;

            case 0x31:
                fprintf(fp,"LXI SP,#$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0x32:
                fprintf(fp,"STA adr,#$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0x33:
                fprintf(fp,"INX SP");
                break;

            case 0x34:
                fprintf(fp,"INR M");
                break;

            case 0x35:
                fprintf(fp,"DCR M");
                break;

            case 0x36:
                fprintf(fp,"MVI M,#$%02x", data[ind+1]);
                break;

            case 0x37:
                fprintf(fp,"STC");
                break;

            case 0x38:
                fprintf(fp,"ERROR");
                break;

            case 0x39:
                fprintf(fp,"DAD SP");
                break;

            case 0x3a:
                fprintf(fp,"LDA #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0x3b:
                fprintf(fp,"DCX SP");
                break;

            case 0x3c:
                fprintf(fp,"INR A");
                break;

            case 0x3d:
                fprintf(fp,"DCR A");
                break;

            case 0x3e:
                fprintf(fp,"MVI A,#$%02x", data[ind+1]);
                break;

            case 0x3f:
                fprintf(fp,"CMC");
                break;

        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
                fprintf(fp,"MOV B,%c", reg[(data[ind]-0x40)]);
                break;

            case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
                fprintf(fp,"MOV C,%c", reg[(data[ind]-0x48)]);
                break;

            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
                fprintf(fp,"MOV D,%c", reg[(data[ind]-0x50)]);
                break;
            case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
                fprintf(fp,"MOV E,%c", reg[(data[ind]-0x58)]);
                break;

            case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
                fprintf(fp,"MOV H,%c", reg[(data[ind]-0x60)]);
                break;

            case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
                fprintf(fp,"MOV L,%c", reg[(data[ind]-0x68)]);
                break;

            case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:
                fprintf(fp,"MOV M,%c", reg[(data[ind]-0x70)]);
                break;

            case 0x76:
                fprintf(fp,"HLT");
                break;

            case 0x77:
                fprintf(fp,"MOV M,A");
                break;

            case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
                fprintf(fp,"MOV A,%c", reg[(data[ind]-0x78)]);
                break;

            case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
                fprintf(fp,"ADD %c", reg[(data[ind]-0x80)]);
                break;

            case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
                fprintf(fp,"ADC %c", reg[(data[ind]-0x88)]);
                break;

            case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
                fprintf(fp,"SUB %c", reg[(data[ind]-0x90)]);
                break;

            case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
                fprintf(fp,"SBB %c", reg[(data[ind]-0x98)]);
                break;

            case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
                fprintf(fp,"ANA %c", reg[(data[ind]-0xa0)]);
                break;

            case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
                fprintf(fp,"XRA %c", reg[(data[ind]-0xa8)]);
                break;

            case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
                fprintf(fp,"ORA %c", reg[(data[ind]-0xb0)]);
                break;

            case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
                fprintf(fp,"CMP %c", reg[(data[ind]-0xb8)]);
                break;

            case 0xc0:
                fprintf(fp,"RNZ");
                break;

            case 0xc1:
                fprintf(fp,"POP B");
                break;

            case 0xc2:
                fprintf(fp,"JNZ #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xc3:
                fprintf(fp,"JMP #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xc4:
                fprintf(fp,"CNZ #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xc5:
                fprintf(fp,"PUSH B");
                break;

            case 0xc6:
                fprintf(fp,"ADI #$%02x", data[ind+1]);
                break;

            case 0xc7:
                fprintf(fp,"RST");
                break;

            case 0xc8:
                fprintf(fp,"RZ");
                break;

            case 0xc9:
                fprintf(fp,"RET");
                break;

            case 0xca:
                fprintf(fp,"JZ #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xcb:
                fprintf(fp,"ERROR!");
                break;

            case 0xcc:
                fprintf(fp,"CZ #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xcd:
                fprintf(fp,"CALL #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xce:
                fprintf(fp,"ACI #$%02x", data[ind+1]);
                break;

            case 0xcf:
                fprintf(fp,"RST");
                break;

            case 0xd0:
                fprintf(fp,"RNC");
                break;

            case 0xd1:
                fprintf(fp,"POP D");
                break;

            case 0xd2:
                fprintf(fp,"JNC #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xd3:
                fprintf(fp,"OUT #$%02x", data[ind+1]);
                break;

            case 0xd4:
                fprintf(fp,"CNC #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xd5:
                fprintf(fp,"PUSH D");
                break;

            case 0xd6:
                fprintf(fp,"SUI #$%02x", data[ind+1]);
                break;

            case 0xd7:
                fprintf(fp,"RST");
                break;

            case 0xd8:
                fprintf(fp,"RC");
                break;

            case 0xd9:
                fprintf(fp,"ERROR!");
                break;

            case 0xda:
                fprintf(fp,"JC #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xdb:
                fprintf(fp,"IN #$%02x", data[ind+1]);
                break;

            case 0xdc:
                fprintf(fp,"CC #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xdd:
                fprintf(fp,"ERROR!");
                break;

            case 0xde:
                fprintf(fp,"SBI #$%02x", data[ind+1]);
                break;

            case 0xdf:
                fprintf(fp,"RST");
                break;

            case 0xe0:
                fprintf(fp,"RPO");
                break;

            case 0xe1:
                fprintf(fp,"POP H");
                break;

            case 0xe2:
                fprintf(fp,"JPO #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xe3:
                fprintf(fp,"XTHL");
                break;

            case 0xe4:
                fprintf(fp,"CPO #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xe5:
                fprintf(fp,"PUSH H");
                break;

            case 0xe6:
                fprintf(fp,"ANI #$%02x", data[ind+1]);
                break;

            case 0xe7:
                fprintf(fp,"RST");
                break;

            case 0xe8:
                fprintf(fp,"RPE");
                break;

            case 0xe9:
                fprintf(fp,"PCHL");
                break;

            case 0xea:
                fprintf(fp,"JPE #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xeb:
                fprintf(fp,"XCHG");
                break;

            case 0xec:
                fprintf(fp,"CPE #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xed:
                fprintf(fp,"ERROR!");
                break;

            case 0xee:
                fprintf(fp,"XRI #$%02x", data[ind+1]);
                break;

            case 0xef:
                fprintf(fp,"RST");
                break;

            case 0xf0:
                fprintf(fp,"RP");
                break;

            case 0xf1:
                fprintf(fp,"POP PSW");
                break;

            case 0xf2:
                fprintf(fp,"JP #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xf3:
                fprintf(fp,"DI");
                break;

            case 0xf4:
                fprintf(fp,"CP #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xf5:
                fprintf(fp,"PUSH PS");
                break;

            case 0xf6:
                fprintf(fp,"ORI #$%02x", data[ind+1]);
                break;

            case 0xf7:
                fprintf(fp,"RST");
                break;

            case 0xf8:
                fprintf(fp,"RM");
                break;

            case 0xf9:
                fprintf(fp,"SPHL");
                break;

            case 0xfa:
                fprintf(fp,"JM #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xfb:
                fprintf(fp,"EI");
                break;

            case 0xfc:
                fprintf(fp,"CM #$%02x%02x", data[ind+2], data[ind+1]);
                break;

            case 0xfd:
                fprintf(fp,"ERROR!");
                break;

            case 0xfe:
                fprintf(fp,"CPI #$%02x", data[ind+1]);
                break;

            case 0xff:
                fprintf(fp,"RST");
                break;
            default:
                break;
                printf("Unimp");
            }
            //fprintf(fp,"\n%d %d %x",ind,opbytes,data[ind])
            ind+=size;
            fprintf(fp,"\n");
        }
    }