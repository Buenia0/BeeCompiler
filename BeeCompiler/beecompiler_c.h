#ifndef BEECOMPILER_C_BACKEND
#define BEECOMPILER_C_BACKEND

#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include "beecompiler.h"
#include "beecompiler_types.h"
using namespace std;

#ifndef __fastcall
#define __fastcall __attribute__((fastcall))
#endif

namespace beecompiler
{
    class BeeCompilerC
    {
        public:
            BeeCompilerC(BeeCompiler &com);
            ~BeeCompilerC();

            BeeCompiler &comp;
        
            void initc();
            inline void __fastcall executec(BlockMap temp);
    
            inline void __fastcall cmov(int reg, int imm);
            inline void __fastcall cmovr(int reg1, int reg2);
            inline void __fastcall cstr(int reg, int regaddr, int regoffs);
            inline void __fastcall cldr(int reg, int regaddr, int regoffs);
            inline void __fastcall cadd(int reg1, int reg2, int imm);
            inline void __fastcall cadc(int reg1, int reg2, int imm);
            inline void __fastcall cftrans(int reg, int addroffs, int regoffs);
            inline void __fastcall cb(int addr);
    
            uint32_t registers[12];
            
            inline uint32_t getreg(int num)
            {
                return registers[num];
            }
    };
};

#endif // BEECOMPILER_C_BACKEND