#ifndef BEECOMPILER_CACHE
#define BEECOMPILER_CACHE

#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
#include <iterator>
#include "beecompiler_types.h"
using namespace std;

#define REG_0 0
#define REG_1 1
#define REG_2 2
#define REG_3 3
#define REG_4 4
#define REG_5 5
#define REG_6 6
#define REG_7 7
#define REG_8 8
#define REG_9 9
#define REG_10 10
#define REG_11 11

namespace beecompiler
{
    class BeeCompiler
    {
        public:
            BeeCompiler();
            ~BeeCompiler();
        
            const int size = 32;
    
            BlockMap blockarray;
            ValueMap destregmap;
            ValueMap sourceregmap;
            ValueMap immmap;
            ValueMap regoffsmap;
    
            ExecFunc execute;
    
            void init();
            void run();
    
            inline void setexeccallback(ExecFunc cb)
            {
                execute = cb;
            }
    
            BlockMap compile(uint32_t addr);
    
            string currentinstr;
    
            bool branched;
    
            bool recompile;
    
            void erasefirstelement();
            void eraseelement(int pos);
            void optimize();
    
            void MOV(int reg, int imm);
            void STR(int reg, int regaddr, int regoffs);
            void LDR(int reg, int regaddr, int regoffs);
            void ADD(int reg1, int reg2, int imm);
            void ADC(int reg1, int reg2, int imm);
            void FTRANS(int reg, int addroffs, int regoffs);
            void B(int addr);
    
            BlockMap compiled;
    
            size_t pc;
    };
};



#endif // BEECOMPILER_CACHE