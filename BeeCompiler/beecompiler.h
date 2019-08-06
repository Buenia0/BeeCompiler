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

#define AUTO_ENDIAN 0
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 2

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
            
            inline bool isftranscapable(int pos)
            {
                if (blockarray.at(pos) == InstructionSet::Load && blockarray.at(pos + 1) == InstructionSet::Add && blockarray.at(pos + 2) == InstructionSet::Store)
                {
                    return ((immmap.at(pos) == immmap.at(pos + 2)) ? true : false);
                }
                else
                {
                    return false;
                }
            }
            
            inline void changetoftrans(int pos)
            {
                int immval = immmap.at(pos);
                int sourcereg = sourceregmap.at(pos);
                int regoffs = immmap.at((pos + 1));
                eraseelement((pos + 1));
                eraseelement((pos + 1));
                blockarray.at(pos) = InstructionSet::FusedTransfer;
                immmap.at(pos) = immval;
                regoffsmap.at(pos) = regoffs;
                destregmap.at(pos) = sourcereg;
                sourceregmap.at(pos) = -1;
            }
    
            BlockMap compile(uint32_t addr);
    
            string currentinstr;
    
            bool branched;
    
            bool recompile;
    
            void erasefirstelement();
            void eraseelement(int pos);
            void optimize();
    
            void MOV(int reg, int imm);
            void MOVR(int reg1, int reg2);
            void STR(int reg, int regaddr, int regoffs);
            void LDR(int reg, int regaddr, int regoffs);
            void ADD(int reg1, int reg2, int imm);
            void ADC(int reg1, int reg2, int imm);
            void FTRANS(int reg, int addroffs, int regoffs);
            void B(int addr);
    
            BlockMap compiled;
    
            size_t pc;
            
            int addrendian[2] = {0};
            
            void setendian(int endian)
            {
                if (endian == BIG_ENDIAN)
                {
                    addrendian[0] = 0;
                    addrendian[1] = 1;
                }
                else
                {
                    addrendian[0] = 1;
                    addrendian[1] = 0;
                }
            }
            
            uint32_t gethiaddr(uint32_t addr, int size)
            {
                return (addr + (addrendian[0] * size));
            }
            
            uint32_t getloaddr(uint32_t addr, int size)
            {
                return (addr + (addrendian[1] * size));
            }
        
        rbcallback readByte;
        wbcallback writeByte;
        
        inline void setmemcallback(rbcallback rbcb, wbcallback wbcb)
        {
            readByte = rbcb;
            writeByte = wbcb;
        }
        
        vector<uint8_t> memory;
        
    uint16_t readWord(uint32_t addr)
    {
        return ((readByte(gethiaddr(addr, 1)) << 8) | (readByte(getloaddr(addr, 1))));
    }
    
    uint32_t readLong(uint32_t addr)
    {
        return ((readWord(gethiaddr(addr, 2)) << 16) | (readWord(getloaddr(addr, 2))));
    }

    void writeWord(uint32_t addr, uint16_t value)
    {
        writeByte(getloaddr(addr, 1), (value & 0xFF));
        writeByte(gethiaddr(addr, 1), (value >> 8));
    }
    
    void writeLong(uint32_t addr, uint32_t value)
    {
        writeWord(getloaddr(addr, 2), (value & 0xFFFF));
        writeWord(gethiaddr(addr, 2), (value >> 16));
    }
    };
    

};



#endif // BEECOMPILER_CACHE