#ifndef BEECOMPILER_TYPES
#define BEECOMPILER_TYPES

#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
using namespace std;

enum InstructionSet : int
{
    Default = -1,
    Move = 0,
    Load = 1,
    Store = 2,
    Add = 3,
    AddCarry = 4,
    FusedTransfer = 5,
    Branch = 6,
    MoveReg = 7
};

typedef vector<InstructionSet> BlockMap;
typedef vector<int> ValueMap;
typedef function<void(BlockMap)> ExecFunc;
typedef int (*JumpMap)(int reg1, int reg2, int regoffs, int addroffs);
typedef function<uint8_t(uint32_t)> rbcallback;
typedef function<void(uint32_t, uint8_t)> wbcallback;


#endif // BEECOMPILER_TYPES