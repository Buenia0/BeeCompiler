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
        destregmap.resize(size, -1);
        sourceregmap.resize(size, -1);
        regoffsmap.resize(size, -1);
        immmap.resize(size, -1);
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
        
        optimize();
        
        return compiled;
    }
    
    void BeeCompiler::optimize()
    {
        for (int i = 0; i < (int)(compiled.size()); i++)
        {
            if (compiled.at(i) == InstructionSet::Load && compiled.at(i + 1) == InstructionSet::Add && compiled.at(i + 2) == InstructionSet::Store)
            {
                // cout << "True" << endl;
            }
            
            cout << compiled.at(i) << endl;
        }
    }
    
    void BeeCompiler::eraseelement(int pos)
    {
        compiled.erase(compiled.begin() + pos);
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
        erasefirstelement();
    }
    
    void BeeCompiler::STR(int reg, int regaddr, int regoffs)
    {
        compiled.push_back(InstructionSet::Store);
        destregmap.push_back(reg);
        sourceregmap.push_back(regaddr);
        regoffsmap.push_back(-2);
        immmap.push_back(regoffs);
        erasefirstelement();
    }
    
    void BeeCompiler::LDR(int reg, int regaddr, int regoffs)
    {
        compiled.push_back(InstructionSet::Load);
        destregmap.push_back(reg);
        sourceregmap.push_back(regaddr);
        regoffsmap.push_back(-2);
        immmap.push_back(regoffs);
        erasefirstelement();
    }
    
    void BeeCompiler::ADD(int reg1, int reg2, int imm)
    {
        compiled.push_back(InstructionSet::Add);
        destregmap.push_back(reg1);
        sourceregmap.push_back(reg2);
        regoffsmap.push_back(-2);
        immmap.push_back(imm);
        erasefirstelement();
    }
    
    void BeeCompiler::ADC(int reg1, int reg2, int imm)
    {
        compiled.push_back(InstructionSet::AddCarry);
        destregmap.push_back(reg1);
        sourceregmap.push_back(reg2);
        regoffsmap.push_back(-2);
        immmap.push_back(imm);
        erasefirstelement();
    }
    
    void BeeCompiler::FTRANS(int reg, int addroffs, int regoffs)
    {
        compiled.push_back(InstructionSet::FusedTransfer);
        destregmap.push_back(reg);
        sourceregmap.push_back(-1);
        regoffsmap.push_back(regoffs);
        immmap.push_back(addroffs);
        erasefirstelement();
    }
    
    void BeeCompiler::B(int addr)
    {
        compiled.push_back(InstructionSet::Branch);
        destregmap.push_back(-2);
        sourceregmap.push_back(-2);
        regoffsmap.push_back(-2);
        immmap.push_back(addr);
        erasefirstelement();
        branched = true;
    }
};