#pragma once
#include<map>
#include<string>

struct inst_codes
{
    std::map<std::string, int> instr_num;
    std::map<std::string, int> reg_num;  
    
    inst_codes()
    {
        instr_num["add"] = 0;
        instr_num["sub"] = 1;
        instr_num["and"] = 2;
        instr_num["or" ] = 3;
        instr_num["mov"] = 3;
        instr_num["not"] = 4;
        instr_num["xor"] = 5;
        instr_num["jmp eq" ] = 32;
        instr_num["jmp neq"] = 33;
        instr_num["jmp lt" ] = 34;
        instr_num["jmp leq"] = 35;
        instr_num["jmp gt" ] = 36;
        instr_num["jmp geq"] = 37;
        reg_num["%r0"] = 0;
        reg_num["%r1"] = 1;
        reg_num["%r2"] = 2;
        reg_num["%r3"] = 3;
        reg_num["%r4"] = 4;
        reg_num["%r5"] = 5;
        reg_num["out"] = 6;
        reg_num["in" ] = 6;
    }
};
