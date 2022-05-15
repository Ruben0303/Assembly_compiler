#include"asm.h"
#include"asm_reader.h"
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<iostream>

inst_codes codes;
std::map<std::string, int> label_address;
std::map<int, std::string> labels_to_find;


bool is_number(char c)
{
    if(c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}


std::string to_binary(int n)
{
    std::string binary_Num;
    binary_Num.resize(8);
    int i = 7;
    while (n > 0 && i >= 0) 
    {
        binary_Num[i] = '0' + n % 2 ;
        n = n / 2;
        --i;
    }
    while(i >= 0)
    {
        binary_Num[i--] = '0';
    }
    return binary_Num;
}




int ASM_Reader::get_source_val(const std::string& src, int& opcode_val, std::map<std::string, int>& reg_num, int mask)
{
    int src_val = 0;
    if(src[0] == '$')
    {

        for(int i = 1; i < src.size(); ++i)
        {
            if(!is_number(src[i]) )
            {
                return -1;            }
        }
        std::istringstream iss(src);
        char dollar_sign;
        iss >> dollar_sign >> src_val;
        opcode_val |= mask;
        //            std::cout << "OPCODE VAL IS CHANGED = " << opcode_val << std::endl;
        //            std::cout <<" immidiate src1  = " << src_val << std::endl;
    }
    else 
    {
        if(reg_num.find(src) == reg_num.end())
        {
            return -1; ///////////////////////there can be exception
        }
        src_val = reg_num[src];
        //            std::cout <<"SRC REG NUM  = " << src_val << std::endl;
    }
    return src_val;
}
int ASM_Reader::get_opcode_val(std::string & opcode)
{
    //    std::cout << "opcode IS " << opcode << "\n";
    if(std::equal(opcode.begin(), opcode.begin() + 5, "label"))
    {
        label_address[opcode] = line_number * 4; 
        is_label = true;
    } 
    else
    {
        if(opcode == "jmp")
        {
            is_jmp = true;
            std::string opcode_1;
            in >>  opcode_1;
            opcode += " " + opcode_1;
            //            std::cout << "CONDITION IS " << opcode << "\n";

        } 
        ++line_number;
        if(codes.instr_num.find(opcode) == codes.instr_num.end())
        {
            std::cerr << "UNKNOWN OPCODE IN LINE " << line_number << std::endl;
            return 0; 
        }
        return codes.instr_num[opcode];
    }
    return -1;

} 

int ASM_Reader::get_dest_val(const std::string dest)
{

    int dest_val = 0;
    if(is_jmp)
    {
        //        std::cout << "IS JUMP" << std::endl;
        if(label_address.find(dest) != label_address.end())
        {
            dest_val = label_address[dest]; 
        }
        else
        {
            int further_find_index = (line_number - 1) * 4 + 3;
            labels_to_find.emplace(further_find_index, dest);
            dest_val = 0;
        }
    }
    else if(codes.reg_num.find(dest) == codes.reg_num.end())
    {
        std::cerr << "UNKNOWN ADDRESS FOR DEST LINE " << line_number << std::endl;
        return -1;     
    }
    else
    {
        return codes.reg_num[dest];
    }
    return dest_val;
}
void ASM_Reader::push_instruction(int n)
{
    if(n > 255)
    {
        n &= 255;
    }
    num_insts.push_back(n);
}



int ASM_Reader::read_instruction()
{


    std::string  opcode;
    in >> opcode;
    if(eof())
    {
        return 3; 
    }
    int opcode_val = get_opcode_val(opcode);  
    if(!is_label)
    {
        int src1_val = 0;
        std::string src1;
        in >> src1;
        if(src1[src1.size() - 1] == ',')
        {
            src1.pop_back();
        }
        //        std::cout << "OPCODE = " << opcode << " :: opcode_val = " << opcode_val  << std::endl;
        //        std::cout << "src1 in = " << src1 << std::endl;
        src1_val = get_source_val(src1, opcode_val, codes.reg_num, mask_1imm);

        //        std::cout << "opcode val_after func call = " << opcode_val << std::endl;

        if(src1_val == -1)
        {
            std::cerr << "UNKNOWN ADDRESS FOR SRC1 " << std::endl;
            return -1;
        }       
        int  src2_val;
        if(opcode == "mov" || opcode == "not")
        {
            opcode_val |= mask_2imm; 
            src2_val = 0;
        }
        else
        {  


            std::string src2;
            in >> src2;
            //            std::cout <<"src2 read = " << src2 << std::endl;
            if(src2[src2.size() - 1] == ',')
            {
                src2.pop_back();
            }
            src2_val = get_source_val(src2, opcode_val, codes.reg_num, mask_2imm);
            if(src2_val == -1)
            {
                std::cerr << "UNKNOWN ADDRESS FOR SRC2 " << std::endl;
                return -1;
            }
        }
        std::string dest;
        in >> dest; 
        int dest_val = get_dest_val(dest);
        push_instruction(opcode_val);
        push_instruction(src1_val);
        push_instruction(src2_val);
        push_instruction(dest_val);
        is_jmp = false;
        is_label = false; 
        return 0;
    }
    else
    {
        is_jmp = false;
        is_label = false;
        return 1;
    }
}

std::vector<int> & ASM_Reader::get_num_instructions()
{
    return num_insts;
}
bool ASM_Reader::eof()
{
    return in.eof();
}
void ASM_Reader::close()
{
    in.close();
}






void ASM_runner::Print()
{
    Reader.close();
    std::ofstream hex_out;
    hex_out.open("./HEX_input.txt");
    auto num_insts = Reader.get_num_instructions();
    for(int i = 0; i < num_insts.size(); ++i)
    {
        if(i != 0)
        {
            if(i %  4 == 3)
            {

                auto need_to_fnd = labels_to_find.find(i); 
                if(need_to_fnd != labels_to_find.end())
                {
                    auto fnd_in_labels = label_address.find(need_to_fnd->second);
                    if(fnd_in_labels != label_address.end())
                    {
                        num_insts[i] = fnd_in_labels->second;
                    }
                    else    
                    {

                        std::cerr << "UNKNOWN LABEL TO JUMP IN LINE" << i / 4 + 1 << std::endl;
                        std::cerr << std::endl;
                        break;
                    }
                }  

            }
            if(i % 4 == 0)
            {
                hex_out << std::endl;
            }
        } 
        hex_out << std::hex << num_insts[i]  << " "; 
    }
    hex_out.close();
}

void ASM_runner::run ()
{
    while ( !Reader.eof() )
    {
        if(Reader.eof())
        {
            break;
        }

        int is_valid = Reader.read_instruction();
        if(is_valid == -1)
        {
            std::cerr << "NOT VALID READING" << std::endl;
            return;
        }
    }
    ASM_runner::Print();
}
