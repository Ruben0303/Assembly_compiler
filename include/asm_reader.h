#pragma once
#include"asm.h"
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<iostream>

#define mask_1imm 64
#define mask_2imm 128
bool is_number(char c);
std::string to_binary(int n);

extern inst_codes codes;
extern std::map<std::string, int> label_address;
extern std::map<int, std::string> labels_to_find;


class ASM_Reader
{
private:
    std::ifstream in; 
    std::string file_name; 
    std::map<int, std::string> labels_to_find;
    std::vector<int> num_insts;
    int line_number;
    bool is_label;
    bool is_jmp;
    int get_source_val(const std::string& src, int& opcode_val, std::map<std::string, int>& reg_num, int mask);
    int get_opcode_val(std::string & opcode);
    int get_dest_val(const std::string dest);

public:
    ASM_Reader(std::string file_name) : line_number(0)
    {
        in.open(file_name);
    }
    ~ASM_Reader()
    {
        in.close();
    }

    void push_instruction(int n);
    int  read_instruction();
    std::vector<int> & get_num_instructions();
    bool eof();
    void close();
};

class ASM_runner
{
    std::map<std::string, int> label_address;
    ASM_Reader Reader;
    public:
    ASM_runner (const std::string &  file_name) : Reader(file_name){}
    void run ();
    void Print();
};

