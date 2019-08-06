#include "beecompiler.h"
using namespace beecompiler;
using namespace std;

namespace beecompiler
{
    BeeCompiler::BeeCompiler()
    {
        
    }
    
    BeeCompiler::~BeeCompiler()
    {
        
    }
    
    void BeeCompiler::init()
    {
        blockarray.resize(size, InstructionSet::Default);
    }
    
    void BeeCompiler::erasefirstelement()
    {
        destregmap.erase(destregmap.begin());
        sourceregmap.erase(sourceregmap.begin());
        regoffsmap.erase(regoffsmap.begin());
        immmap.erase(immmap.begin());
    }
    
    void BeeCompiler::run()
    {
        if (blockarray.at(pc) == InstructionSet::Default)
        {
            BlockMap block = compile(pc);
            
            for (int i = 0; i < (int)block.size(); i++)
            {
                blockarray[pc + i] = block[i];
            }
        }
        
        optimize();
        
        execute(blockarray);
    }
    
    BlockMap BeeCompiler::compile(uint32_t addr)
    {
        recompile = true;
        
        while (recompile)
        {
            if (branched)
            {
                recompile = false;
            }
        }
        
        return compiled;
    }
    
    void BeeCompiler::optimize()
    {
        for (int i = 0; i < (int)(blockarray.size()); i++)
        {
            if (isftranscapable(i))
            {
                changetoftrans(i);
            }
        }
    }
    
    void BeeCompiler::eraseelement(int pos)
    {
        blockarray.erase(blockarray.begin() + pos);
        destregmap.erase(destregmap.begin() + pos);
        sourceregmap.erase(sourceregmap.begin() + pos);
        regoffsmap.erase(regoffsmap.begin() + pos);
        immmap.erase(immmap.begin() + pos);
    }
    
    void BeeCompiler::MOV(int reg, int imm)
    {
        compiled.push_back(InstructionSet::Move);
        destregmap.push_back(reg);
        sourceregmap.push_back(-2);
        regoffsmap.push_back(-2);
        immmap.push_back(imm);
    }
    
    void BeeCompiler::MOVR(int reg1, int reg2)
    {
        compiled.push_back(InstructionSet::MoveReg);
        destregmap.push_back(reg1);
        sourceregmap.push_back(reg2);
        regoffsmap.push_back(-2);
        immmap.push_back(-2);
    }
    
    void BeeCompiler::STR(int reg, int regaddr, int regoffs)
    {
        compiled.push_back(InstructionSet::Store);
        destregmap.push_back(reg);
        sourceregmap.push_back(regaddr);
        regoffsmap.push_back(-2);
        immmap.push_back(regoffs);
    }
    
    void BeeCompiler::LDR(int reg, int regaddr, int regoffs)
    {
        compiled.push_back(InstructionSet::Load);
        destregmap.push_back(reg);
        sourceregmap.push_back(regaddr);
        regoffsmap.push_back(-2);
        immmap.push_back(regoffs);
    }
    
    void BeeCompiler::ADD(int reg1, int reg2, int imm)
    {
        compiled.push_back(InstructionSet::Add);
        destregmap.push_back(reg1);
        sourceregmap.push_back(reg2);
        regoffsmap.push_back(-2);
        immmap.push_back(imm);
    }
    
    void BeeCompiler::ADC(int reg1, int reg2, int imm)
    {
        compiled.push_back(InstructionSet::AddCarry);
        destregmap.push_back(reg1);
        sourceregmap.push_back(reg2);
        regoffsmap.push_back(-2);
        immmap.push_back(imm);
    }
    
    void BeeCompiler::FTRANS(int reg, int addroffs, int regoffs)
    {
        compiled.push_back(InstructionSet::FusedTransfer);
        destregmap.push_back(reg);
        sourceregmap.push_back(-1);
        regoffsmap.push_back(regoffs);
        immmap.push_back(addroffs);
    }
    
    void BeeCompiler::B(int addr)
    {
        compiled.push_back(InstructionSet::Branch);
        destregmap.push_back(-2);
        sourceregmap.push_back(-2);
        regoffsmap.push_back(-2);
        immmap.push_back(addr);
        branched = true;
    }
};