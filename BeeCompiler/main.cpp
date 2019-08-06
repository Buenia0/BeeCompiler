#include <stdio.h>
#include <functional>
#include "beecompiler.h"
#include "beecompiler_c.h"
#include "beecompiler_types.h"
#define BEECOMPILER_LITTLE_ENDIAN
using namespace beecompiler;
using namespace std;
using namespace std::placeholders;

// Register allocation:
//
// 6502  -  BML IR
// A        R0
// X        R1
// Y        R2
// SP       R3
// SR       R4

struct CPU6502
{
    uint8_t regaccum;
    uint8_t regx;
    uint8_t regy;
    uint8_t regsp;
    uint8_t regstat;
    uint16_t ip = 0;
};

int getexcesssize(int size)
{
    return (size - 3);
}

CPU6502 cpu;


vector<uint8_t> memory(0x10000, 0);

uint8_t readb(uint32_t addr)
{
    return memory[addr];
}
    
void writeb(uint32_t addr, uint8_t value)
{
    memory[addr] = value;
}

uint16_t risctociscbranch(uint16_t addr, int size)
{
    // cout << (addr - size - 1) << endl;
    return (addr - size - 3);
}

int totalsize = 0;

void executenextopcode(BeeCompiler &comp)
{
    uint8_t opcode = comp.readByte(cpu.ip++);
    
    switch (opcode)
    {
        case 0xA9: comp.MOV(REG_0, comp.readByte(cpu.ip++)); totalsize += getexcesssize(2); break;
        case 0xA2: comp.MOV(REG_1, comp.readByte(cpu.ip++)); totalsize += getexcesssize(2); break;
        case 0xA0: comp.MOV(REG_2, comp.readByte(cpu.ip++)); totalsize += getexcesssize(2); break;
        case 0x9D: comp.STR(REG_0, REG_1, comp.readWord(cpu.ip)); cpu.ip += 2; totalsize += getexcesssize(3); break;
        case 0xBD: comp.LDR(REG_0, REG_1, comp.readWord(cpu.ip)); cpu.ip += 2; totalsize += getexcesssize(3); break;
        case 0x69: comp.ADD(REG_0, REG_0, comp.readByte(cpu.ip++)); totalsize += getexcesssize(2); break;
        case 0x4C: comp.B(risctociscbranch(comp.readWord(cpu.ip), totalsize)); cpu.ip += 2; cpu.ip = comp.readWord(cpu.ip); totalsize = 0; break;
        case 0xAA: comp.MOVR(REG_1, REG_0); totalsize += getexcesssize(2); break;
        case 0xA8: comp.MOVR(REG_2, REG_0); totalsize += getexcesssize(2); break;
        case 0xBA: comp.MOVR(REG_1, REG_3); totalsize += getexcesssize(2); break;
        case 0x8A: comp.MOVR(REG_0, REG_1); totalsize += getexcesssize(2); break;
        case 0x9A: comp.MOVR(REG_3, REG_1); totalsize += getexcesssize(2); break;
        case 0x98: comp.MOVR(REG_0, REG_2); totalsize += getexcesssize(2); break;
        default: cout << "Unrecognized opcode at 0x" << hex << (int)(opcode) << endl; exit(1); break;
    }
}

BeeCompiler compiler;
BeeCompilerC cback(compiler);

int programcounter = 0;

void init()
{
    memory[0] = 0xA9;
    memory[1] = 0x00;
    memory[2] = 0x69;
    memory[3] = 0x01;
    memory[4] = 0x4C;
    memory[5] = 0x03;
}

/*
void init()
{
    memory[0] = 0xA9;
    memory[1] = 0x00;
    memory[2] = 0xA2;
    memory[3] = 0x1F;
    memory[4] = 0x9D;
    memory[5] = 0x00;
    memory[6] = 0x30;
    memory[7] = 0xBD;
    memory[8] = 0x00;
    memory[9] = 0x30;
    memory[10] = 0x69;
    memory[11] = 0x01;
    memory[12] = 0x9D;
    memory[13] = 0x00;
    memory[14] = 0x30;
    memory[15] = 0x4C;
    memory[16] = 0x07;
}
*/

void deinit()
{
    cpu.regaccum = (cback.getreg(0) & 0xFF);
    cpu.regx = (cback.getreg(1) & 0xFF);
    cpu.regy = (cback.getreg(2) & 0xFF);
    cpu.regsp = (cback.getreg(3) & 0xFF);
    cpu.regstat = (cback.getreg(4) & 0xFF);
    
    cout << "Reg A: " << hex << (int)(cpu.regaccum) << endl;
    cout << "Reg X: " << hex << (int)(cpu.regx) << endl;
    cout << "Reg Y: " << hex << (int)(cpu.regy) << endl;
    cout << "SP: " << hex << (int)(cpu.regsp) << endl;
    cout << "Stat: " << hex << (int)(cpu.regstat) << endl;
    cout << "[0x301F]: " << dec << (int)(compiler.readByte(0x301F)) << endl;
}

void initcallbacks()
{
    auto rb = bind(&readb, _1);
    auto wb = bind(&writeb, _1, _2);
    compiler.setmemcallback(rb, wb);
}

int main(int argc, char **argv)
{
    init();
    initcallbacks();
    compiler.setendian(LITTLE_ENDIAN);
    compiler.init();
    cback.initc();
    cpu.ip = 0;

    for (int i = 0; i < 8; i++)
    {
        executenextopcode(compiler);
    }
    
    for (int i = 0; i < 1; i++)
    {
        compiler.run();
        cout << (int)(compiler.pc) << endl;
    }

    deinit();
	return 0;
}
