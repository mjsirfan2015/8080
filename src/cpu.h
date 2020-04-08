#include <cstdint>
#include<cstdio>
#include<stdlib.h>

enum flags{C,P,AC,Z,S};
struct Cpu{
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;
	uint16_t SP;
	uint16_t PC;
	uint8_t Memory[65536];
	uint8_t cc;
    bool IntEn;
};
void setzsp(Cpu *,uint8_t);
bool parity(uint16_t);
void setfl(Cpu *,uint8_t,bool);

void cpuinfo(Cpu *);
void unimp(uint8_t op){
    printf("Unimplimented opcode: %x\n",op);
}
uint8_t HL(Cpu *cpu){
    uint16_t h=cpu->H,l=cpu->L;
    return cpu->Memory[h<<8|l];
}
void setzsp(Cpu *cpu,uint8_t ans) {//set flags based on condition
	setfl(cpu,Z, ans == 0);
	setfl(cpu,S, (ans&0x80) == 0x80);
	setfl(cpu,P, parity((uint16_t)ans));
}
bool parity(uint16_t x){//checks parity
	uint16_t y = x ^ (x >> 1);
	y = y ^ (y >> 2);
	y = y ^ (y >> 4);
	return !(y&1 == 1);
}

void setfl(Cpu *cpu,uint8_t flag,bool b) {
    if (b == true) 
		cpu->cc |= (1 << flag);//sets flag true
    else 
		cpu->cc &= ~(1 << flag);//sets flag false
}
uint8_t getfl(Cpu *cpu,uint8_t flag){
	return (cpu->cc >> flag) & 1;
}

//print cpu info
void cpuinfo(Cpu *cpu) {
    printf("A= %x\n", cpu->A);
    printf("B= %x\n", cpu->B);
    printf("C= %x\n", cpu->C);
    printf("D= %x\n", cpu->D);
    printf("E= %x\n", cpu->E);
    printf("H= %x\n", cpu->H);
    printf("L= %x\n", cpu->L);
    printf("SP= %x\n", cpu->SP);
    printf("PC= %x\n", cpu->PC);
    printf("S=%d,Z=%d,C=%d,P=%d,AC=%d\n", getfl(cpu,S),
    getfl(cpu,Z), getfl(cpu,C), getfl(cpu,P), getfl(cpu,AC));
}
//execute opcodes
void  lxi(Cpu *cpu,uint8_t *r1, uint8_t *r2) {
	*r1 = cpu->Memory[cpu->PC+2];
	*r2 = cpu->Memory[cpu->PC+1];
	cpu->PC += 2;
}
void inx(uint8_t *r1, uint8_t *r2) {
	uint16_t val;
	val = (uint16_t)(*r2 + 1);
	*r1 += (uint8_t)(val >> 8);
	*r2 = (uint8_t)(val & 0xff);
}
void dcx(uint8_t *r1, uint8_t *r2) {
	uint16_t val;
	val = (uint16_t)(*r1)<<8 | (uint16_t)(*r2) - 1;
	//fmt.Printf("::%x,%x,%x\n",*r1,*r2,val>>8)
	*r1 = (uint8_t)(val >> 8);
	*r2 = (uint8_t)(val & 0xff);
}
void inr(Cpu *cpu,uint8_t *reg) {
	*reg++;
	setzsp(cpu,*reg);
	//set AC
}
void dcr(Cpu *cpu,uint8_t *reg) {
	*reg--;
	setzsp(cpu,*reg);
	//set AC
}
void mov(uint8_t *dst, uint8_t *src) {
	*dst = *src;
}
void  dad(Cpu *cpu,uint8_t r1,uint8_t r2) {
	uint32_t ans;
	uint32_t hl = (uint32_t)(cpu->H)<<8 | (uint32_t)(cpu->L);
	uint32_t r = (uint32_t)(r1)<<8 | (uint32_t)(r1);
	ans = hl + r;
	cpu->H = (uint8_t)(ans >> 8);
	cpu->L = (uint8_t)(ans & 0xff);
	//fmt.Printf("%x\n", ans)
	setfl(cpu,C, (ans > 0xffff));
}
void add(Cpu *cpu,uint16_t val, bool adc ,bool sub) {
	uint16_t c,ans;
	c = 0;
	//fmt.Printf("neg=%t ans=%t",cneg,!(ans>0xff))
	if (adc == true) {
		c = (uint16_t)(getfl(cpu,C));
	}
	if (sub == true) {
		val = ~(val + c) + 1;
	}
	ans = (uint16_t)(cpu->A) + val;
	if (sub == true) {
		setfl(cpu,C, !(ans > 0xff));
	} else {
		setfl(cpu,C, ans > 0xff);
	}
	setfl(cpu,Z, (ans&0xff) == 0);
	setfl(cpu,S, (ans&0x80) != 0);
	setfl(cpu,P, parity(ans&0xff));
	//Note:Set AC
	cpu->A = (uint8_t)(ans & 0xff);
}
void ana(Cpu *cpu,uint8_t val) {
	uint8_t ans;
	ans = cpu->A & val;

	//reset carry flag
	setfl(cpu,C, false);
	setzsp(cpu,ans);
	//Note:Set AC
	cpu->A = ans;
}
void xra(Cpu *cpu,uint8_t val) {
	uint8_t ans;
	ans = cpu->A ^ val;

	//reset carry flag
	setfl(cpu,C, false);
	setzsp(cpu,ans);
	//Note:Set AC
	cpu->A = ans;
}
void ora(Cpu *cpu,uint8_t val) {
	uint8_t ans;
	ans = cpu->A | val;

	//reset carry flag
	setfl(cpu,C, false);
	setzsp(cpu,ans);
	//Note:Set AC
	cpu->A = ans;
}
void cmp(Cpu *cpu,uint8_t val) {
	uint8_t x;
	x = cpu->A - val;
	//reset carry flag
	setfl(cpu,C, cpu->A < val);
	setzsp(cpu,x);
}
void ret(Cpu *cpu) {
	cpu->PC = (uint16_t)(cpu->Memory[cpu->SP]) | ((uint16_t)(cpu->Memory[cpu->SP+1]) << 8);
	cpu->SP += 2;
}
void pop(Cpu *cpu,uint8_t *r1, uint8_t *r2) {
	*r2 = cpu->Memory[cpu->SP];
	*r1 = cpu->Memory[cpu->SP+1];
	cpu->SP += 2;
}
void jmp(Cpu *cpu) {
	cpu->PC = (uint16_t)(cpu->Memory[cpu->PC+2])<<8 | (uint16_t)(cpu->Memory[cpu->PC+1]);
	cpu->PC--;
}
void push(Cpu *cpu, uint8_t *r1, uint8_t *r2) {
	cpu->Memory[cpu->SP-2] = *r2;
	cpu->Memory[cpu->SP-1] = *r1;
	cpu->SP -= 2;
}
void call(Cpu *cpu) {
	uint16_t ret = cpu->PC + 2;
	uint8_t m1 = (uint8_t)((ret >> 8) & 0xff),m2= (uint8_t)(ret & 0xff);
	push(cpu,&m1, &m2);
	jmp(cpu);
}
void rst(Cpu *cpu,uint16_t m) {
	uint16_t ret = cpu->PC + 2;
	uint8_t m1 = (uint8_t)((ret >> 8) & 0xff),m2= (uint8_t)(ret & 0xff);
	push(cpu,&m1, &m2);
	cpu->PC = m;
}
void excop(Cpu *cpu){
	uint8_t x,tmpD,tmpE,tmpH,tmpL;
	uint32_t adr,hl;
    uint8_t op = cpu->Memory[cpu->PC];
	bool val;
	switch (op) {
	case 0x0: //NOP
	    break;
	case 0x1: //LXI B,D16
		lxi(cpu,&cpu->B, &cpu->C);
        break;
	case 0x2: //STAX B
		cpu->Memory[(uint16_t)cpu->B<<8|(uint16_t)cpu->C] = cpu->A;
        break;
	case 0x3: //INX B
		inx(&cpu->B, &cpu->C);
        break;
	case 0x4: //INR B
		inr(cpu,&cpu->B);
        break;
	case 0x5: //DCR B
		inr(cpu,&cpu->B);
        break;
	case 0x6: //MVI B, D8
		mov(&cpu->B, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x7: //RLC
		x = cpu->A;
		setfl(cpu,C, (x&0x80) == 0x80); //set Highest bit
		cpu->A = ((x << 1) | (x&0x80)>>7);
		break;
	case 0x8:
		unimp(op); 
        break;//-
	//cpu->PC+=1
	case 0x9: //DAD B
		dad(cpu,cpu->B, cpu->C);
        break;
	case 0xa: //LDAX B
		cpu->A = cpu->Memory[(uint16_t)(cpu->B)<<8|(uint16_t)(cpu->C)];
        break;
	case 0xb: //DCX B
		dcx(&cpu->B, &cpu->C);
        break;
	case 0xc: //INR C
		inr(cpu,&cpu->C);
        break;
	case 0xd: //DCR C
		dcr(cpu,&cpu->C);
        break;
	case 0xe: //MVI C,D8
		mov(&cpu->C, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0xf: //RRC
		x = cpu->A;
		setfl(cpu,C, (x&1) == 1); //set Lowest bit
		cpu->A = ((x&1)<<7 | (x >> 1));
        break;
	case 0x10:
		unimp(op);
        break; //-
	//cpu->PC+=1
	case 0x11: //LXI D,D16
		lxi(cpu,&cpu->D, &cpu->E);
        break;
	case 0x12: //STAX D
		cpu->Memory[(uint16_t)(cpu->D)<<8|(uint16_t)(cpu->E)] = cpu->A;
        break;
	case 0x13: //INX D
		inx(&cpu->D, &cpu->E);
        break;
	case 0x14: //INR D
		inr(cpu,&cpu->D);
        break;
	case 0x15: //DCR D
		dcr(cpu,&cpu->D);
        break;
	case 0x16: //MVI D, D8
		mov(&cpu->D, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x17: //RAL
		x = cpu->A;
		cpu->A = ((x << 1) | getfl(cpu,C));
		setfl(cpu,C, (x&0x80) == 0x80); //set Lowest bit
        break;
    case 0x18:
		unimp(op);
        break; //-
	//cpu->PC+=1
	case 0x19: //DAD D
		dad(cpu,cpu->D, cpu->E);
        break;
	case 0x1a: //LDAX D
		cpu->A = cpu->Memory[(uint16_t)(cpu->D)<<8|(uint16_t)(cpu->E)];
		break;
	case 0x1b: //DCX D
		dcx(&cpu->D, &cpu->E);
        break;
	case 0x1c: //INR E
		inr(cpu,&cpu->E);
        break;
	case 0x1d: //DCR E
		dcr(cpu,&cpu->E);
        break;
	case 0x1e: //MVI E,D8
		mov(&cpu->E, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x1f: //RAR
		x = cpu->A;
		cpu->A = (getfl(cpu,C)<<7 | (x >> 1));
		setfl(cpu,C, (x&1) == 1);//set Lowest bit
        break; 
	case 0x20:
		unimp(op);
        break; //-
	//cpu->PC+=1
	case 0x21: //LXI H,D16
		lxi(cpu,&cpu->H, &cpu->L);
        break;
	case 0x22: //SHLD adr
		adr = (uint16_t)(cpu->Memory[cpu->PC+2])<<8 | (uint16_t)(cpu->Memory[cpu->PC+1]);
		cpu->Memory[adr] = cpu->L;
		cpu->Memory[adr+1] = cpu->H;
		cpu->PC += 2;
        break;
	case 0x23: //INX H
		inx(&cpu->H, &cpu->L);
        break;
	case 0x24: //INR H
		inr(cpu,&cpu->H);
        break;
	case 0x25: //DCR H
		dcr(cpu,&cpu->H);
        break;
	case 0x26: //MVI H,D8
		mov(&cpu->H, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x27:
		unimp(op); //DAA
        break;
	//cpu->PC+=1
	case 0x28:
		unimp(op);
        break; //-
	//cpu->PC+=1
	case 0x29: //DAD H
		dad(cpu,cpu->H, cpu->L);
        break;
	case 0x2a: //LHLD adr
		adr = (uint16_t)(cpu->Memory[cpu->PC+2])<<8 | (uint16_t)(cpu->Memory[cpu->PC+1]);
		cpu->L = cpu->Memory[adr];
		cpu->H = cpu->Memory[adr+1];
		cpu->PC += 2;
        break;
	case 0x2b: //DCX H
		dcx(&cpu->H, &cpu->L);
        break;
	case 0x2c: //INR L
		inr(cpu,&cpu->L);
        break;
	case 0x2d: //DCR L
		dcr(cpu,&cpu->L);
        break;
	case 0x2e: //MVI L, D8
		mov(&cpu->L, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x2f: //CMA
		cpu->A = ~cpu->A;
		break;
	case 0x30:
		unimp(op);
        break; //-
	//cpu->PC+=1
	case 0x31: //LXI SP, D16
		cpu->SP = (uint16_t)(cpu->Memory[cpu->PC+2])<<8 | (uint16_t)(cpu->Memory[cpu->PC+1]);
		cpu->PC += 2;
        break;
	case 0x32: //STA adr
		cpu->Memory[(uint16_t)(cpu->Memory[cpu->PC+2])<<8|(uint16_t)(cpu->Memory[cpu->PC+1])] = cpu->A;
		cpu->PC += 2;
        break;
	case 0x33: //INX SP
		cpu->SP++;
        break;
	case 0x34: //INR M
		inr(cpu,&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x35: //DCR M
		dcr(cpu,&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x36: //MVI M,D8
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x37: //STC
		setfl(cpu,C, true);
        break;
	case 0x38:
		unimp(op);
        break; //-
	//cpu->PC+=1
	case 0x39: //DAD SP
		uint32_t ans;
		hl = (uint32_t)(cpu->H)<<8 | (uint32_t)(cpu->L);
		ans = hl + (uint32_t)(cpu->SP);
		setfl(cpu,C, (ans > 0xffff));
        break;
	case 0x3a: //LDA adr
		cpu->A = cpu->Memory[(uint16_t)(cpu->Memory[cpu->PC+2])<<8|(uint16_t)(cpu->Memory[cpu->PC+1])];
		cpu->PC += 2;
        break;
	case 0x3b: //DCX SP
		cpu->SP--;
        break;
	case 0x3c: //INR A
		inr(cpu,&cpu->A);
        break;
	case 0x3d: //DCR A
		dcr(cpu,&cpu->A);
        break;
	case 0x3e: //MVI A,D8
		mov(&cpu->A, &cpu->Memory[cpu->PC+1]);
		cpu->PC++;
        break;
	case 0x3f: //CMC
		val = !(getfl(cpu,C) == 1);
		//fmt.Println(val)
		setfl(cpu,C, val);
        break;
		/*MOV B,B..A(0x40-47)*/
	case 0x40: //MOV B,B
		mov(&cpu->B, &cpu->B);
        break;
	case 0x41: //MOV B,C
		mov(&cpu->B, &cpu->C);
        break;
	case 0x42: //MOV B,D
		mov(&cpu->B, &cpu->D);
        break;
	case 0x43: //MOV B,E
		mov(&cpu->B, &cpu->E);
        break;
	case 0x44: //MOV B,H
		mov(&cpu->B, &cpu->H);
        break;
	case 0x45: //MOV B,L
		mov(&cpu->B, &cpu->L);
        break;
	case 0x46: //MOV B,M
		mov(&cpu->B, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x47: //MOV B,A
		mov(&cpu->B, &cpu->A);
        break;
		/*MOV C,B..A(0x48-4f)*/
	case 0x48: //MOV C,B
		mov(&cpu->C, &cpu->B);
        break;
	case 0x49: //MOV C,C
		mov(&cpu->C, &cpu->C);
        break;
	case 0x4a: //MOV C,D
		mov(&cpu->C, &cpu->D);
        break;
	case 0x4b: //MOV C,E
		mov(&cpu->C, &cpu->E);
        break;
	case 0x4c: //MOV C,H
		mov(&cpu->C, &cpu->H);
        break;
	case 0x4d: //MOV C,L
		mov(&cpu->C, &cpu->L);
        break;
    case 0x4e: //MOV C,M
		mov(&cpu->C, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x4f: //MOV C,A
		mov(&cpu->C, &cpu->A);
        break;
		/*MOV D,B..A(0x50-57)*/
	case 0x50: //MOV D,B
		mov(&cpu->D, &cpu->B);
        break;
	case 0x51: //MOV D,C
		mov(&cpu->D, &cpu->C);
        break;
	case 0x52: //MOV D,D
		mov(&cpu->D, &cpu->D);
        break;
	case 0x53: //MOV D,E
		mov(&cpu->D, &cpu->E);
        break;
	case 0x54: //MOV D,H
		mov(&cpu->D, &cpu->H);
        break;
	case 0x55: //MOV D,L
		mov(&cpu->D, &cpu->L);
        break;
	case 0x56: //MOV D,M
		mov(&cpu->D, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x57: //MOV D,A
		mov(&cpu->D, &cpu->A);
        break;
		/*MOV E,B..A(0x58-5f)*/
	case 0x58: //MOV E,B
		mov(&cpu->E, &cpu->B);
        break;
	case 0x59: //MOV E,C
		mov(&cpu->E, &cpu->C);
        break;
	case 0x5a: //MOV E,D
		mov(&cpu->E, &cpu->D);
        break;
	case 0x5b: //MOV E,E
		mov(&cpu->E, &cpu->E);
        break;
	case 0x5c: //MOV E,H
		mov(&cpu->E, &cpu->H);
        break;
	case 0x5d: //MOV E,L
		mov(&cpu->E, &cpu->L);
        break;
	case 0x5e: //MOV E,M
		mov(&cpu->E, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x5f: //MOV E,A
		mov(&cpu->E, &cpu->A);
        break;
		/*MOV H,B..A(0x60-67)*/
	case 0x60: //MOV H,B
		mov(&cpu->H, &cpu->B);
        break;
	case 0x61: //MOV H,C
		mov(&cpu->H, &cpu->C);
        break;
	case 0x62: //MOV H,D
		mov(&cpu->H, &cpu->D);
        break;
	case 0x63: //MOV H,E
		mov(&cpu->H, &cpu->E);
        break;
	case 0x64: //MOV H,H
		mov(&cpu->H, &cpu->H);
        break;
	case 0x65: //MOV H,L
		mov(&cpu->H, &cpu->L);
        break;
	case 0x66: //MOV H,M
		mov(&cpu->H, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x67: //MOV H,A
		mov(&cpu->H, &cpu->A);
        break;
		/*MOV L,B..A(0x68-6f)*/
	case 0x68: //MOV L,B
		mov(&cpu->L, &cpu->B);
        break;
	case 0x69: //MOV L,C
		mov(&cpu->L, &cpu->C);
        break;
	case 0x6a: //MOV L,D
		mov(&cpu->L, &cpu->D);
        break;
	case 0x6b: //MOV L,E
		mov(&cpu->L, &cpu->E);
        break;
	case 0x6c: //MOV L,H
		mov(&cpu->L, &cpu->H);
        break;
	case 0x6d: //MOV L,L
		mov(&cpu->L, &cpu->L);
        break;
	case 0x6e: //MOV L,M
		mov(&cpu->L, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x6f: //MOV L,A
		mov(&cpu->L, &cpu->A);
        break;
		/*MOV M,B..A(0x70-77)*/
	case 0x70: //MOV M,B
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->B);
        break;
	case 0x71: //MOV M,C
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->C);
        break;
	case 0x72: //MOV M,D
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->D);
        break;
	case 0x73: //MOV M,E
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->E);
        break;
	case 0x74: //MOV M,H
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->H);
        break;
	case 0x75: //MOV M,L
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->L);
        break;
	case 0x76: //HLT
		printf("Exiting...........");
		exit(0);
        break;
	case 0x77: //MOV M,A
		mov(&cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)], &cpu->A);
        break;
		/*MOV A,B..A(0x78-7f)*/
	case 0x78: //MOV A,B
		mov(&cpu->A, &cpu->B);
        break;
	case 0x79: //MOV A,C
		mov(&cpu->A, &cpu->C);
        break;
	case 0x7a: //MOV A,D
		mov(&cpu->A, &cpu->D);
        break;
	case 0x7b: //MOV A,E
		mov(&cpu->A, &cpu->E);
        break;
	case 0x7c: //MOV A,H
		mov(&cpu->A, &cpu->H);
        break;
	case 0x7d: //MOV A,L
		mov(&cpu->A, &cpu->L);
        break;
	case 0x7e: //MOV A,M
		mov(&cpu->A, &cpu->Memory[(uint16_t)(cpu->H)<<8|(uint16_t)(cpu->L)]);
        break;
	case 0x7f: //MOV A,A
		mov(&cpu->A, &cpu->A);
        break;

		/*ADD A..M(0x80-87)*/
	case 0x80: //ADD B
		add(cpu,(uint16_t)(cpu->B), false, false);
		break;
	case 0x81: //ADD C
		add(cpu,(uint16_t)(cpu->C), false, false);
		break;
	case 0x82: //ADD D
		add(cpu,(uint16_t)(cpu->D), false, false);
		break;
	case 0x83: //ADD E
		add(cpu,(uint16_t)(cpu->E), false, false);
		break;
	case 0x84: //ADD H
		add(cpu,(uint16_t)(cpu->H), false, false);
		break;
	case 0x85: //ADD L
		add(cpu,(uint16_t)(cpu->L), false, false);
		break;
	case 0x86: //ADD M
		add(cpu,(uint16_t)(HL(cpu)), false, false);
		break;
	case 0x87: //ADD A
		add(cpu,(uint16_t)(cpu->A), false, false);
		break;

		/*ADC A..M(0x88-8f)*/
	case 0x88: //ADC B
		add(cpu,(uint16_t)(cpu->B), true, false);
		break;
	case 0x89: //ADC C
		add(cpu,(uint16_t)(cpu->C), true, false);
		break;
	case 0x8a: //ADC D
		add(cpu,(uint16_t)(cpu->D), true, false);
		break;
	case 0x8b: //ADC E
		add(cpu,(uint16_t)(cpu->E), true, false);
		break;
	case 0x8c: //ADC H
		add(cpu,(uint16_t)(cpu->H), true, false);
		break;
	case 0x8d: //ADC L
		add(cpu,(uint16_t)(cpu->L), true, false);
		break;
	case 0x8e: //ADC M
		add(cpu,(uint16_t)(HL(cpu)), true, false);
		break;
	case 0x8f: //ADC A
		add(cpu,(uint16_t)(cpu->A), true, false);
		break;
		/*SUB A..M(0x90-97)*/
	case 0x90: //SUB B
		add(cpu,(uint16_t)(cpu->B), false, true);
		break;
	case 0x91: //SUB C
		add(cpu,(uint16_t)(cpu->C), false, true);
		break;
	case 0x92: //SUB D
		add(cpu,(uint16_t)(cpu->D), false, true);
		break;
	case 0x93: //SUB E
		add(cpu,(uint16_t)(cpu->E), false, true);
		break;
	case 0x94: //SUB H
		add(cpu,(uint16_t)(cpu->H), false, true);
		break;
	case 0x95: //SUB L
		add(cpu,(uint16_t)(cpu->L), false, true);
		break;
	case 0x96: //SUB M
		add(cpu,(uint16_t)(HL(cpu)), false, true);
		break;
	case 0x97: //SUB A
		add(cpu,(uint16_t)(cpu->A), false, true);
		break;

		/*SBB A..M(0x98-9f)*/
	case 0x98: //SBB B
		add(cpu,(uint16_t)(cpu->B), true, true);
		break;
	case 0x99: //SBB C
		add(cpu,(uint16_t)(cpu->C), true, true);
		break;
	case 0x9a: //SBB D
		add(cpu,(uint16_t)(cpu->D), true, true);
		break;
	case 0x9b: //SBB E
		add(cpu,(uint16_t)(cpu->E), true, true);
		break;
	case 0x9c: //SBB H
		add(cpu,(uint16_t)(cpu->H), true, true);
		break;
	case 0x9d: //SBB L
		add(cpu,(uint16_t)(cpu->L), true, true);
		break;
	case 0x9e: //SBB M
		add(cpu,(uint16_t)(HL(cpu)), true, true);
		break;
	case 0x9f: //SBB A
		add(cpu,(uint16_t)(cpu->A), true, true);
		break;

		/*ANA A..M(0xa0-a7)*/
	case 0xa0: //ANA B
		ana(cpu,cpu->B);
		break;
	case 0xa1: //ANA C
		ana(cpu,cpu->C);
		break;
	case 0xa2: //ANA D
		ana(cpu,cpu->D);
		break;
	case 0xa3: //ANA E
		ana(cpu,cpu->E);
		break;
	case 0xa4: //ANA H
		ana(cpu,cpu->H);
		break;
	case 0xa5: //ANA L
		ana(cpu,cpu->L);
		break;
	case 0xa6: //ANA M
		ana(cpu,HL(cpu));
		break;
	case 0xa7: //ANA A
		ana(cpu,cpu->A);
		break;
		/*XRA A..M(0xa8-af)*/
	case 0xa8: //XRA B
		xra(cpu,cpu->B);
		break;
	case 0xa9: //XRA C
		xra(cpu,cpu->C);
		break;
	case 0xaa: //XRA D
		xra(cpu,cpu->D);
		break;
	case 0xab: //XRA E
		xra(cpu,cpu->E);
		break;
	case 0xac: //XRA H
		xra(cpu,cpu->H);
		break;
	case 0xad: //XRA L
		xra(cpu,cpu->L);
		break;
	case 0xae: //XRA M
		xra(cpu,HL(cpu));
		break;
	case 0xaf: //XRA A
		xra(cpu,cpu->A);
		break;
		/*ORA A..M(0xb0-b7)*/
	case 0xb0: //ORA B
		ora(cpu,cpu->B);
		break;
	case 0xb1: //ORA C
		ora(cpu,cpu->C);
		break;
	case 0xb2: //ORA D
		ora(cpu,cpu->D);
		break;
	case 0xb3: //ORA E
		ora(cpu,cpu->E);
		break;
	case 0xb4: //ORA H
		ora(cpu,cpu->H);
		break;
	case 0xb5: //ORA L
		ora(cpu,cpu->L);
		break;
	case 0xb6: //ORA M
		ora(cpu,HL(cpu));
		break;
	case 0xb7: //ORA A
		ora(cpu,cpu->A);
		break;

		/*CMP A..M(0xb8-bf)*/
	case 0xb8: //CMP B
		cmp(cpu,cpu->B);
		break;
	case 0xb9: //CMP C
		cmp(cpu,cpu->C);
		break;
	case 0xba: //CMP D
		cmp(cpu,cpu->D);
		break;
	case 0xbb: //CMP E
		cmp(cpu,cpu->E);
		break;
	case 0xbc: //CMP H
		cmp(cpu,cpu->H);
		break;
	case 0xbd: //CMP L
		cmp(cpu,cpu->L);
		break;
	case 0xbe: //CMP M
		cmp(cpu,HL(cpu));
		break;
	case 0xbf: //CMP A
		cmp(cpu,cpu->A);
		break;
	case 0xc0: //RNZ
		if (getfl(cpu,Z) != 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xc1: //POP B
		pop(cpu,&cpu->B, &cpu->C);
		break;
	case 0xc2: //JNZ adr
		if (getfl(cpu,Z) != 1) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xc3: //JMP adr
		jmp(cpu);
		break;
	case 0xc4: //CNZ adr
		if (getfl(cpu,Z) != 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xc5: //PUSH B
		push(cpu,&cpu->B, &cpu->C);
		break;
	case 0xc6: //ADI D8
		add(cpu,(uint16_t)(cpu->Memory[cpu->PC+1]), false, false);
		cpu->PC++;
		break;
	case 0xc7:
		//RST 0
		rst(cpu,0);
		break;
	case 0xc8: //RZ
		if (getfl(cpu,Z) == 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xc9: //RET
		ret(cpu);
		break;
	case 0xca: //JZ adr
		if (getfl(cpu,Z) == 1) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xcb:
		unimp(op);
		break; //-
	//cpu->PC+=1
	case 0xcc: //CZ adr
		if (getfl(cpu,Z) == 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xcd: //CALL adr
		call(cpu);
		break;
	case 0xce: //ACI D8
		add(cpu,(uint16_t)(cpu->Memory[cpu->PC+1]), true, false);
		cpu->PC++;
		break;
	case 0xcf: //RST 1
		rst(cpu,8);
		break;
	case 0xd0: //RNC
		if (getfl(cpu,C) != 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xd1: //POP D
		pop(cpu,&cpu->D, &cpu->E);
		break;
	case 0xd2: //JNC adr
		if (getfl(cpu,C) != 1) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xd3: //OUT D8
		cpu->PC += 1;
		break;
	case 0xd4: //CNC adr
		if (getfl(cpu,C) != 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xd5: //PUSH D
		push(cpu,&cpu->D, &cpu->E);
		break;
	case 0xd6: //SUI D8
		add(cpu,(uint16_t)(cpu->Memory[cpu->PC+1]), false, true);
		cpu->PC++;
		;
		break;
	case 0xd7: //RST 2
		rst(cpu,16);
		break;
	case 0xd8: //RC
		if (getfl(cpu,C) == 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xd9:
		unimp(op);
		break; //-
	//cpu->PC+=1
	case 0xda: //JC adr
		if (getfl(cpu,C) == 1) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xdb: //IN D8
		cpu->PC += 1;
		break;
	case 0xdc: //CC adr
		if (getfl(cpu,C) == 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xdd:
		unimp(op);
		break; //-
	//cpu->PC+=1
	case 0xde: //SBI D8
		add(cpu,(uint16_t)(cpu->Memory[cpu->PC+1]), true, true);
		cpu->PC++;
		break;
	case 0xdf: //RST 3
		rst(cpu,24);
		break;
	case 0xe0: //RPO
		if (getfl(cpu,P) != 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xe1: //POP H
		pop(cpu,&cpu->H, &cpu->L);
		break;
	case 0xe2: //JPO adr
		if (getfl(cpu,P) == 0) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xe3: //XTHL
		cpu->Memory[cpu->SP], cpu->H = cpu->H, cpu->Memory[cpu->SP];
		cpu->Memory[cpu->SP+1], cpu->L = cpu->L, cpu->Memory[cpu->SP+1];
	case 0xe4: //CPO adr
		if (getfl(cpu,P) != 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xe5: //PUSH H
		push(cpu,&cpu->H, &cpu->L);
		break;
	case 0xe6: //ANI D8
		ana(cpu,cpu->Memory[cpu->PC+1]);
		cpu->PC++;
		break;
	case 0xe7: //RST 4
		rst(cpu,32);
		break;
	case 0xe8: //RPE
		if (getfl(cpu,P) == 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xe9: //PCHL
		cpu->PC = (uint16_t)(cpu->H)<<8 | (uint16_t)(cpu->L);
		break;
	case 0xea: //JPE adr
		if (getfl(cpu,P) == 1) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xeb: //XCHG
		//cpu->H, cpu->L, cpu->D, cpu->E = cpu->D, cpu->E, cpu->H, cpu->L;
		tmpH=cpu->H;tmpL=cpu->L;tmpD=cpu->D;tmpE=cpu->E;
		cpu->H=tmpD;
		cpu->L=tmpE;
		cpu->D=tmpH;
		cpu->E=tmpL;
		break;
	case 0xec: //CPE adr
		if (getfl(cpu,P) == 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xed:
		unimp(op);
		break; //-
	//cpu->PC+=1
	case 0xee: //XRI D8
		xra(cpu,cpu->Memory[cpu->PC+1]);
		cpu->PC++;
		break;
	case 0xef: //RST 5
		rst(cpu,40);
		break;
	case 0xf0: //RP
		if (getfl(cpu,S) == 0) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xf1: //POP PSW
		pop(cpu,&cpu->A, &cpu->cc);
		break;
	case 0xf2: //JP adr
		if (getfl(cpu,S) == 0) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xf3: //DI
		cpu->IntEn = false;
		break;
	case 0xf4: //CP adr
		if (getfl(cpu,S) == 0) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xf5: //PUSH PSW
		push(cpu,&cpu->A, &cpu->cc);
		break;
	case 0xf6: //ORI D8
		ora(cpu,cpu->Memory[cpu->PC+1]);
		cpu->PC++;
		break;
	case 0xf7: //RST 6
		rst(cpu,48);
		break;
	case 0xf8: //RM
		if (getfl(cpu,S) == 1) {
			ret(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xf9: //SPHL
		cpu->SP = (uint16_t)(cpu->H)<<8 | (uint16_t)(cpu->L);
		break;
	case 0xfa: //JM adr
		if (getfl(cpu,S) == 1) {
			jmp(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xfb: //EI
		cpu->IntEn = true;
		break;
	case 0xfc: //CM adr
		if (getfl(cpu,S) == 1) {
			call(cpu);
		} else {
			cpu->PC += 2;
		}
		break;
	case 0xfd:
		unimp(op);
		break; //-
	//cpu->PC+=1
	case 0xfe: //CPI D8
		cmp(cpu,cpu->Memory[cpu->PC+1]);
		cpu->PC++;
		break;
	case 0xff: //RST 7
		rst(cpu,56);
		break;
	}
	cpu->PC++;
}
