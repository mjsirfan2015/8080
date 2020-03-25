#include <cstdint>
uint8_t HL(Cpu*);
void setzsp(Cpu *,uint8_t);
bool parity(uint16_t);
void setfl(Cpu *,uint8_t,bool);
typedef enum flags{C,P,AC,Z,S};
typedef struct Cpu{
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    uint8_t SP;
    uint8_t PC;
    uint8_t Memory[65536];
    uint8_t cc;
    bool IntEn;
};
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