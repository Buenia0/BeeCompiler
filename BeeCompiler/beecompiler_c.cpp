#include "beecompiler_c.h"
#include <iostream>
#include <algorithm>
using namespace beecompiler;
using namespace std;

namespace beecompiler
{
    BeeCompilerC::BeeCompilerC(BeeCompiler &com) : comp(com)
    {
        
    }
    
    BeeCompilerC::~BeeCompilerC()
    {
        
    }
    
    vector<uint8_t>& BeeCompilerC::getmemory()
    {
        return memarr;
    }
    
    inline void __fastcall BeeCompilerC::cmov(int reg, int imm)
    {
        registers[reg] = imm;
        comp.pc += 1;
    }
    
    inline void __fastcall BeeCompilerC::cstr(int reg, int regaddr, int regoffs)
    {
        memarr[registers[regaddr] + regoffs] = registers[reg];
        comp.pc += 1;
    }
    
    inline void __fastcall BeeCompilerC::cldr(int reg, int regaddr, int regoffs)
    {
        registers[reg] = memarr[registers[regaddr] + regoffs];
        comp.pc += 1;
    }
    
    inline void __fastcall BeeCompilerC::cadd(int reg1, int reg2, int imm)
    {
        registers[reg1] = (registers[reg2] + imm);
        comp.pc += 1;
    }
    
    inline void __fastcall BeeCompilerC::cadc(int reg1, int reg2, int imm)
    {
        registers[reg1] = (registers[reg2] + imm);
        comp.pc += 1;
    }
    
    inline void __fastcall BeeCompilerC::cftrans(int reg, int addroffs, int regoffs)
    {
        memarr[registers[reg] + addroffs] += regoffs;
        comp.pc += 1;
    }
    
    inline void __fastcall BeeCompilerC::cb(int addr)
    {
        // cout << "Branching" << endl;
        comp.pc = addr;
    }
    
    void BeeCompilerC::initc(vector<uint8_t> &mem)
    {
        comp.setexeccallback(bind(&BeeCompilerC::executec, this, placeholders::_1));
        memarr = mem;
    }
    
    inline void __fastcall BeeCompilerC::executec(BlockMap temp)
    {
        BlockMap temparr = temp;
        ValueMap sourcearr = comp.sourceregmap;
        ValueMap destarr = comp.destregmap;
        ValueMap regoffsarr = comp.regoffsmap;
        ValueMap immarr = comp.immmap;
        
        temparr.erase(remove(temparr.begin(), temparr.end(), InstructionSet::Default), temparr.end());
        destarr.erase(remove(destarr.begin(), destarr.end(), -1), destarr.end());
        sourcearr.erase(remove(sourcearr.begin(), sourcearr.end(), -1), sourcearr.end());
        regoffsarr.erase(remove(regoffsarr.begin(), regoffsarr.end(), -1), regoffsarr.end());
        immarr.erase(remove(immarr.begin(), immarr.end(), -1), immarr.end());

        for (int i = comp.pc; i < (int)(temparr.size()); i++)
        {
            switch (temparr.at(i))
            {
                case InstructionSet::Move: cmov(destarr.at(i), immarr.at(i)); break;
                case InstructionSet::Store: cstr(destarr.at(i), sourcearr.at(i), immarr.at(i)); break;
                case InstructionSet::Load: cldr(destarr.at(i), sourcearr.at(i), immarr.at(i)); break;
                case InstructionSet::Add: cadd(destarr.at(i), sourcearr.at(i), immarr.at(i)); break;
                case InstructionSet::AddCarry: cadc(destarr.at(i), sourcearr.at(i), immarr.at(i)); break;
                case InstructionSet::Branch: cb(immarr.at(i)); break;
                case InstructionSet::FusedTransfer: cftrans(destarr.at(i), immarr.at(i), regoffsarr.at(i)); break;
                default: break;
            }
        }
    }
};