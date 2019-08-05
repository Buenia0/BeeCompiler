#include <stdio.h>
#include "beecompiler.h"
#include "beecompiler_c.h"
using namespace beecompiler;

vector<uint8_t> memory(0x10000, 0);

BeeCompiler compiler;
BeeCompilerC cback(compiler);

int programcounter = 0;

int main(int argc, char **argv)
{
    compiler.init();
    cback.initc(memory); 

    compiler.MOV(REG_0, 0x00);
    compiler.MOV(REG_1, 0x1F);
    compiler.STR(REG_0, REG_1, 0x3000);
    compiler.LDR(REG_0, REG_1, 0x3000);
    compiler.ADD(REG_0, REG_0, 1);
    compiler.STR(REG_0, REG_1, 0x3000);
    // compiler.FTRANS(REG_1, 0x3000, 1);
    compiler.B(3);
    
    for (int i = 0; i < 1; i++)
    {
        compiler.run();
    }
    
    memory = cback.getmemory();
    cout << (int)memory.at(0x301F) << endl;
	return 0;
}
