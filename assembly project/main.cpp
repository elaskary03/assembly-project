//  main.cpp
//  assembly project
//  Created by Abdelrahman Elaskary on 01/05/2023.

#include <iostream>
#include <fstream>
#include <string>
#include<sstream>

using namespace std;

// array of instructions {instruction,format,OPCODE,funct3,funct 7}
string instructions[][5] = {
    {"add","R","0110011","000","0000000"},
    {"addi","I","0010011","000",""},
    {"and","R","0110011","111","0000000"},
    {"andi","I","0010011","111",""},
    {"auipc","J","0010111","",""},
    {"beq","SB","1100011","000",""},
    {"bge","SB","1100011","101",""},
    {"bgeu","SB","1100011","111",""},
    {"blt","SB","1100011","100",""},
    {"bltu","SB","1100011","110",""},
    {"bne","SB","1100011","001",""},
    {"ebreak","Z","","",""},
    {"ecall","Z","","",""},
    {"fence","Z","","",""},
    {"jal","UJ","1101111","",""},
    {"jalr","I","1100111","000",""},
    {"lb","I","0000011","000",""},
    {"lbu","I","0000011","100",""},
    {"lh","I","0000011","001",""},
    {"lhu","I","0000011","101",""},
    {"li","","","",""},
    {"lui","U","0110111","",""},
    {"lw","I","0000011","010",""},
    {"mv","","","",""},
    {"neg","","","",""},
    {"or","R","0110011","110","0000000"},
    {"ori","I","0010011","110",""},
    {"sb","S","0100011","000",""},
    {"seqz","","","",""},
    {"sh","S","0100011","001",""},
    {"sll","R","0110011","001","0000000"},
    {"slli","I","0010011","001","0000000"},
    {"slt","R","0110011","010","0000000"},
    {"slti","I","0010011","010",""},
    {"sltiu","I","0010011","011",""},
    {"sltu","R","0110011","011","0000000"},
    {"sltz","","","",""},
    {"sra","R","0110011","101","0100000"},
    {"srai","I","0010011","101","0100000"},
    {"srl","R","0110011","101","0000000"},
    {"srli","I","0010011","101","0000000"},
    {"sub","R","0110011","000","0100000"},
    {"sw","S","0100011","010",""},
    {"xor","R","0110011","100","0000000"},
    {"xori","I","0010011","100",""}
};


int registers[32] = {0};
int pc = 0;
vector <string> memory;
vector <vector<string>> memory_data;
vector <vector<string>> stack;


int binarySearchString(string x, int n)
{
   int lower = 0;
   int upper = n - 1;
   while (lower <= upper)
   {
      int mid = lower + (upper - lower) / 2;
      if (x == (instructions[mid][0]))
          return mid;
      else if (x > (instructions[mid][0]))
         lower = mid + 1;
      else
         upper = mid - 1;
   }
   return -1;
}


int binToDec(string n)
{
    string num = n;
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    int base = 1;
 
    int len = num.length();
    for (int i = len - 1; i >= 0; i--)
    {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }
 
    return dec_value;
}

string twosComplement(string binary) {
    // Determine the number of bits in the binary representation
    int numBits = binary.length();

    // Find the position of the most significant bit set to 1
    int pos = -1;
    for (int i = 0; i < numBits; i++) {
        if (binary[i] == '1') {
            pos = i;
        }
    }

    // If there are no bits set to 1, the twos complement is just the original number
    if (pos == -1) {
        return binary;
    }

    // Create a string to store the ones complement
    string onesComplement = binary;

    // Flip all bits in the ones complement
    for (int i = 0; i < numBits; i++) {
        if (onesComplement[i] == '0') {
            onesComplement[i] = '1';
        }
        else {
            onesComplement[i] = '0';
        }
    }

    // Create a string to store the twos complement
    string twosComplement = onesComplement;

    // Add one to the twos complement
    for (int i = numBits - 1; i >= 0; i--) {
        if (twosComplement[i] == '0') {
            twosComplement[i] = '1';
            break;
        }
        else {
            twosComplement[i] = '0';
        }
    }

    return twosComplement;
}

string decToBinary(int decimal, int digits)
{
    bool flag=false;
    if(decimal<0)
    {
        flag=true;
        decimal*=-1;
    }
    // array to store binary number
    int binaryNum[32] = {0};
    string bin = "";
    // counter for binary array
    int i = 0;
    
   
    while (decimal > 0) {
 
        // storing remainder in binary array
        binaryNum[i] = decimal % 2;
        decimal = decimal / 2;
        i++;
    }
 
    // printing binary array in reverse order
    for (int j = digits - 1; j >= 0; j--)
        bin += to_string(binaryNum[j]);
    
    if(flag)
    {
        bin=twosComplement(bin);
    }
    return bin;
}

string decTohex(int decimal)
{
    bool flag=false;
    if(decimal<0)
    {
        flag=true;
        decimal*=-1;
    }
    // array to store binary number
    string hexNum[8] = {"0","0","0","0","0","0","0","0"};
    string hex = "0x";
    if(flag)
    {
        hex = "-0x";
    }

    int i = 0;
    while (decimal != 0)
    {
        // remainder variable to store remainder
        int rem = 0;
          
        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = decimal % 16;
  
        // check if temp < 10
        if (rem < 10) {
            ch = rem + 48;
        }
        else
        {
            ch = rem + 55;
        }
        hexNum[i] = ch;
        decimal = decimal / 16;
        i++;
        }
    // printing binary array in reverse order
    for (int j = 7; j >= 0; j--)
        hex += hexNum[j];
    return hex;
}

int hexToDec(string hex)
{
    int r, len = 8, sum = 0;
    for (int i = 2; i < 10; i++)
        {
            r = -1;
            len--;
            if(hex[i] >= '0' && hex[i] <= '9')
                r = hex[i] - 48;
            else if(hex[i] >= 'a' && hex[i] <= 'f')
                    r = hex[i] - 87;
                 else if(hex[i] >= 'A' && hex[i] <= 'F')
                        r = hex[i] - 55;
            if (r == -1)
            {
                return -1;
            }
            sum += r * pow(16,len);
        }
    return sum;
}

int get_register(string word)
{
    if (word == "zero" || word == "x0")
    {
        return 0;
    } else if (word == "ra"  || word == "x1" )
    {
        return 1;
    } else if (word == "sp" || word =="x2")
    {
        return 2;
    } else if (word == "gp" || word =="x3")
    {
        return 3;
    } else if (word == "tp" || word == "x4")
    {
        return 4;
    } else if (word == "t0"  || word == "x5")
    {
        return 5;
    } else if (word == "t1"  || word == "x6")
    {
        return 6;
    } else if (word == "t2"  || word == "x7")
    {
        return 7;
    } else if (word == "s0" || word == "x8")
    {
        return 8;
    } else if (word == "s1" || word == "x9")
    {
        return 9;
    } else if (word == "a0" || word == "x10")
    {
        return 10;
    } else if (word == "a1"  || word == "x11" )
    {
        return 11;
    } else if (word == "a2"  || word == "x12" )
    {
        return 12;
    } else if (word == "a3"  || word == "x13" )
    {
        return 13;
    } else if (word == "a4"  || word == "x14" )
    {
        return 14;
    } else if (word == "a5"  || word == "x15" )
    {
        return 15;
    } else if (word == "a6"  || word == "x16" )
    {
        return 16;
    } else if (word == "a7"  || word == "x17" )
    {
        return 17;
    } else if (word == "s2"  ||word == "x18" )
    {
        return 18;
    } else if (word == "s3"  || word == "x19" )
    {
        return 19;
    } else if (word == "s4"  || word == "x20" )
    {
        return 20;
    } else if (word == "s5"  || word == "x21")
    {
        return 21;
    } else if (word == "s6" || word == "x22")
    {
        return 22;
    } else if (word == "s7"  || word == "x23")
    {
        return 23;
    } else if (word == "s8"  || word == "x24")
    {
        return 24;
    } else if (word == "s9"  || word == "x25" )
    {
        return 25;
    } else if (word == "s10"  || word == "x26" )
    {
        return 26;
    } else if (word == "s11"  || word == "x27" )
    {
        return 27;
    } else if (word == "t3" || word == "x28" )
    {
        return 28;
    } else if (word == "t4"  || word == "x29" )
    {
        return 29;
    } else if (word == "t5"  || word == "x30" )
    {
        return 30;
    } else if (word == "t6"  || word == "x31" )
    {
        return 31;
    }
    else
    {
        return -1;
    }
}

void print()
{
    for (int i = 0; i < 32; i++)
    {
        cout << "x" << i << " = " << registers[i]  << ", " << decToBinary(registers[i], 32) << ", " << decTohex(registers[i]) << endl;
    }
    cout << "Data Contents:\n";
    for (int i = 0; i < memory_data.size(); i++)
    {
        cout << memory_data[i][0] << " = " << memory_data[i][1];
        if (memory_data[i].size() == 3)
        {
            cout << " = " << memory_data[i][2];
        }
        cout << endl;
    }
    cout << "Stack content:\n";
    for (int i = 0; i < stack.size(); i++)
    {
        cout << stack[i][0] << " = " << stack[i][1] << endl;
    }
    cout << "Program counter  = " << pc << ", " << decToBinary(pc, 32) << ", " << decTohex(pc) << endl;
}

string remove_comma(string word)
{
    if (word[0] == ',')
    {
        word.erase(0,1);
    }
    if (word.ends_with(','))
    {
        word.erase(word.size()-1,1);
    }
    return word;
}
string remove_colon(string &word)
{
    if (word.ends_with(':'))
    {
        word.erase(word.size()-1,1);
    }
    return word;
}
void remove(string &word, char character)
{
    for (int i = 0; i < word.size(); i++)
    {
        if (word[i] == character)
        {
            word.erase(i, 1);
        }
    }
}
int execute(int instruction, string line)
{
    if (instructions[instruction][1] == "Z")
    {
        return -2;
    }
    istringstream iss(line);
    string word;
    iss >> word;
    int rs1,rs2,rd;
    int reg[3];
    
    
    switch (instruction)
    {
        case 0: // ADD
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            registers[rd] = registers[rs1] + registers[rs2];
            
            break;
        }
        case 1: // ADDI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            
            rd = reg[0];
            
            rs1 = reg[1];
            registers[rd] = registers[rs1] + stoi(word);
            break;
        }
        case 2: // AND
        {
            
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd  = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            int v1 = registers[rs1];
            int v2 = registers[rs2];
            string bin1 = decToBinary(v1, 32);
            string bin2 = decToBinary(v2, 32);
            string bin3 = "";
            
            for (int i = 0; i < 32; i++)
            {
                if ((bin1[i] == '1')&&(bin2[i] == '1'))
                {
                    bin3+='1';
                }
                else
                {
                    bin3+='0';
                }
            }
            registers[rd] = binToDec(bin3);
            break;
        }
        case 3: // ANDI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            int v1 = registers[rs1];
            int v2 = stoi(word);
            string bin1 = decToBinary(v1, 32);
            string bin2 = decToBinary(v2, 32);
            string bin3 = "";
            for (int i = 0; i < 32; i++)
            {
                if ((bin1[i] == '1')&&(bin2[i] == '1'))
                {
                    bin3 += '1';
                }
                else
                {
                    bin3 += '0';
                }
            }
            registers[rd] = binToDec(bin3);
            break;
        }
            
        case 4: // AUIPC
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            if ((reg[0] == -1)||(reg[0] == 0))
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rd = reg[0];
            
            registers[rd] = hexToDec(word);
            break;
        }
        case 5: // BEQ
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rs1 = reg[0];
            rs2 = reg[1];
            if (registers[rs1] == registers[rs2])
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][1] == word)
                    {
                        pc = hexToDec(memory_data[i][0])/4;
                        break;
                    }
                    else if (i == memory_data.size() - 1)
                    {
                        for (int j = 0; j < word.size();j++)
                        {
                            if (!isdigit(word[i]))
                            {
                                return -1;
                            }
                        }
                        pc += stoi(word)/4;
                    }
                }
            }
            break;
        }
        case 6: // BGE
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rs1 = reg[0];
            rs2 = reg[1];
            if (registers[rs1] >= registers[rs2])
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][1] == word)
                    {
                        pc = hexToDec(memory_data[i][0])/4;
                        break;
                    }
                    else if (i == memory_data.size() - 1)
                    {
                        for (int j = 0; j < word.size();j++)
                        {
                            if (!isdigit(word[i]))
                            {
                                return -1;
                            }
                        }
                        pc += stoi(word)/4;
                    }
                }
            }
            break;
        }
        case 7: // BGEU
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rs1 = reg[0];
            rs2 = reg[1];
            if (abs(registers[rs1]) >= abs(registers[rs2]))
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][1] == word)
                    {
                        pc = hexToDec(memory_data[i][0])/4;
                        break;
                    }
                    else if (i == memory_data.size() - 1)
                    {
                        for (int j = 0; j < word.size();j++)
                        {
                            if (!isdigit(word[i]))
                            {
                                return -1;
                            }
                        }
                        pc += stoi(word)/4;
                    }
                }
            }
            break;
        }
        case 8: // BLT
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rs1 = reg[0];
            rs2 = reg[1];
            if (registers[rs1] < registers[rs2])
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][1] == word)
                    {
                        pc = hexToDec(memory_data[i][0])/4;
                        break;
                    }
                    else if (i == memory_data.size() - 1)
                    {
                        for (int j = 0; j < word.size();j++)
                        {
                            if (!isdigit(word[i]))
                            {
                                return -1;
                            }
                        }
                        pc += stoi(word)/4;
                    }
                }
            }
            break;
        }
        case 9: // BLTU
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rs1 = reg[0];
            rs2 = reg[1];
            if (abs(registers[rs1]) < abs(registers[rs2]))
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][1] == word)
                    {
                        pc = hexToDec(memory_data[i][0])/4;
                        break;
                    }
                    else if (i == memory_data.size() - 1)
                    {
                        for (int j = 0; j < word.size();j++)
                        {
                            if (!isdigit(word[i]))
                            {
                                return -1;
                            }
                        }
                        pc += stoi(word)/4;
                    }
                }
            }
            break;
        }
        case 10: // BNE
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rs1 = reg[0];
            rs2 = reg[1];
            if (registers[rs1] != registers[rs2])
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][1] == word)
                    {
                        pc = hexToDec(memory_data[i][0])/4;
                        break;
                    }
                    else if (i == memory_data.size() - 1)
                    {
                        for (int j = 0; j < word.size();j++)
                        {
                            if (!isdigit(word[i]))
                            {
                                return -1;
                            }
                        }
                        pc += stoi(word)/4;
                    }
                }
            }
            break;
        }
        
        case 14: // JAL
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rd = reg[0];
            for (int i = 0; i < memory_data.size(); i++)
            {
                if (memory_data[i][1] == word)
                {
                    pc = hexToDec(memory_data[i][0])/4;
                    break;
                }
                else if (i == memory_data.size() - 1)
                {
                    for (int j = 0; j < word.size();j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    registers[rd] = (pc*4);
                    pc += stoi(word)/4;
                }
            }
            break;
        }
        case 15: // JALR
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rd = reg[0];
            rs1 = reg[1];
            for (int i = 0; i < memory_data.size(); i++)
            {
                if (memory_data[i][1] == word)
                {
                    registers[rd] = pc;
                    pc = ((hexToDec(memory_data[i][0]) - pc*4)/4) + registers[rs1]/4;
                    break;
                }
                else if (i == memory_data.size() - 1)
                {
                    for (int j = 0; j < word.size();j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    registers[rd] = (pc*4);
                    pc = (registers[rs1]+ stoi(word))/4;
                }
            }
            break;
        }
        case 16: // LB
        {
            string half_word;
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rd = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rs1 = get_register(regis);
            
            if(rs1==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rs1]))
                        {
                            string temp = stack[i][1];
                            
                            char temp_2= temp[0];
                            if(temp_2=='1')
                            {
                                half_word = "111111111111111111111111";
                            }
                            
                            for(int i=24; i<32; i++)
                            {
                                half_word += temp[i];
                            }
                            
                            registers[rd] = binToDec(half_word);
                            return 0;
                        }
                    }
                    registers[rd] = 0;
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rs1]))
                    {
                        string temp = memory_data[i][1];
                        
                        char temp_2= temp[0];
                        if(temp_2=='1')
                        {
                            half_word = "111111111111111111111111";
                        }
                        
                        for(int i=24; i<32; i++)
                        {
                            half_word += temp[i];
                        }
                        
                        registers[rd] = binToDec(half_word);
                        return 0;
                    }
                }
                registers[rd] = 0;
            }
            break;
        }
        
        case 17: // LBU
        {
            string half_word;
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rd = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rs1 = get_register(regis);
            
            if(rs1==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rs1]))
                        {
                            string temp = stack[i][1];
                            
                            for(int i=24; i<32; i++)
                            {
                                half_word += temp[i];
                            }
                            
                            registers[rd] = binToDec(half_word);
                            return 0;
                        }
                    }
                    registers[rd] = 0;
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rs1]))
                    {
                        string temp = memory_data[i][1];
                        
                        for(int i=24; i<32; i++)
                        {
                            half_word += temp[i];
                        }
                        
                        registers[rd] = binToDec(half_word);
                        return 0;
                    }
                }
                registers[rd] = 0;
            }
            break;
        }
        case 18: // LH
        {
            string half_word;
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rd = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rs1 = get_register(regis);
            
            if(rs1==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rs1]))
                        {
                            string temp = stack[i][1];
                            
                            char temp_2= temp[0];
                            if(temp_2=='1')
                            {
                                half_word = "1111111111111111";
                            }
                            
                            for(int i=16; i<32; i++)
                            {
                                half_word += temp[i];
                            }
                            
                            registers[rd] = binToDec(half_word);
                            return 0;
                        }
                    }
                    registers[rd] = 0;
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rs1]))
                    {
                        string temp = memory_data[i][1];
                        
                        char temp_2= temp[0];
                        if(temp_2=='1')
                        {
                            half_word = "1111111111111111";
                        }
                        
                        for(int i=16; i<32; i++)
                        {
                            half_word += temp[i];
                        }
                        
                        registers[rd] = binToDec(half_word);
                        return 0;
                    }
                }
                registers[rd] = 0;
            }
            break;
        }
    
        case 19: // LHU
        {
            string half_word;
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rd = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rs1 = get_register(regis);
            
            if(rs1==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rs1]))
                        {
                            string temp = stack[i][1];
                            
                            for(int i=16; i<32; i++)
                            {
                                half_word += temp[i];
                            }
                            
                            registers[rd] = binToDec(half_word);
                            return 0;
                        }
                    }
                    registers[rd] = 0;
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rs1]))
                    {
                        string temp = memory_data[i][1];
                        
                        for(int i=16; i<32; i++)
                        {
                            half_word += temp[i];
                        }
                        
                        registers[rd] = binToDec(half_word);
                        return 0;
                    }
                }
                registers[rd] = 0;
            }
            break;
        }
        case 20: //LI PSEUDO-INSTRUCTION
        {
            for (int i = 0; i < 1; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            
            registers[rd]=stoi(word);
            
            break;
        }
        case 21: // LUI
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rd = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            string temp = decToBinary(stoi(word), 20);
            temp += "000000000000";
            registers[rd] = binToDec(temp);
            break;
        }
            
        case 22: // LW
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rd = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rs1 = get_register(regis);
            if (rs1 == -1)
            {
                return -1;
            }
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rs1]))
                        {
                            registers[rd] = binToDec(stack[i][1]);
                            return 0;
                        }
                    }
                    registers[rd] = 0;
                }
            }
            else
            {
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rs1]))
                    {
                        registers[rd] = binToDec(memory_data[i][1]);
                        return 0;
                    }
                }
                registers[rd] = 0;
            }
            break;
        }
            
        case 23: //MV PSEUDO-INSTRUCTION
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            
            registers[rd]=registers[rs1];

            break;
        }
            
       
        case 24: //NEG PSEUDO-INSTRUCTION
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            
            registers[rd]=-1*registers[rs1];

            break;
        }
            
        case 25: // OR
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            rs2=reg[2];
            int v1 = registers[rs1];
            int v2 = registers[rs2];
            string bin1 = decToBinary(v1, 32);
            string bin2 = decToBinary(v2, 32);
            string bin3 = "";
            
            for (int i = 0; i < 32; i++)
            {
                if ((bin1[i] == '0')&&(bin2[i] == '0'))
                {
                    bin3 += '0';
                }
                else
                {
                    bin3 += '1';
                }
            }
            registers[rd] = binToDec(bin3);
            break;
        }
            
        case 26: // ORI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            int v1 = registers[rs1];
            int v2 = stoi(word);
            string bin1 = decToBinary(v1, 32);
            string bin2 = decToBinary(v2, 32);
            string bin3 = "";
            for (int i = 0; i < 32; i++)
            {
                if ((bin1[i] == '0')&&(bin2[i] == '0'))
                {
                    bin3 += '0';
                }
                else
                {
                    bin3 += '1';
                }
            }
            registers[rd] = binToDec(bin3);
            break;
        }
            
        case 27: //SB
        {
            string byte;
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rs1 = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rd = get_register(regis);
            
            if(rd==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rd]))
                        {
                            string temp = decToBinary(registers[rs1], 32);
                            
                            for(int i=24; i<32; i++)
                            {
                                byte += temp[i];
                            }
                            
                            stack[i][1] = decToBinary(binToDec(byte),32);
                            return 0;
                        }
                    }
                    string temp = decToBinary(registers[rs1], 32);
                    
                    for(int i=24; i<32; i++)
                    {
                        byte += temp[i];
                    }
                    stack.push_back({decTohex(stoi(offset) + registers[rd]),decToBinary(binToDec(byte),32)});
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rd]))
                    {
                        string temp = decToBinary(registers[rs1], 32);
                        
                        for(int i=24; i<32; i++)
                        {
                            byte += temp[i];
                        }
                        
                        memory_data[i][1] = decToBinary(binToDec(byte),32);
                        return 0;
                    }
                }
                string temp = decToBinary(registers[rs1], 32);
                
                for(int i=24; i<32; i++)
                {
                    byte += temp[i];
                }
                memory_data.push_back({decTohex(stoi(offset) + registers[rd]),decToBinary(binToDec(byte),32)});
            }
            break;
        }
            
        case 28: //SEQZ PSEUDO-INSTRUCTION
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rd=reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            int temp_1=registers[rs1];
            
            if(temp_1<0)
            {
                temp_1*=-1;
            }
            
            if (temp_1 < 1)
            {
                registers[rd]=1;
            }
            else
            {
                registers[rd]=0;
            }

            break;
        }
        case 29: // SH
        {
            string half_word;
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rs1 = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rd = get_register(regis);
            
            if(rd==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rd]))
                        {
                            string temp = decToBinary(registers[rs1], 32);
                            
                            for(int i=16; i<32; i++)
                            {
                                half_word += temp[i];
                            }
                            
                            stack[i][1] = decToBinary(binToDec(half_word),32);
                            return 0;
                        }
                    }
                    string temp = decToBinary(registers[rs1], 32);
                    
                    for(int i=16; i<32; i++)
                    {
                        half_word += temp[i];
                    }
                    stack.push_back({decTohex(stoi(offset) + registers[rd]),decToBinary(binToDec(half_word),32)});
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rd]))
                    {
                        string temp = decToBinary(registers[rs1], 32);
                        
                        for(int i=16; i<32; i++)
                        {
                            half_word += temp[i];
                        }
                        
                        memory_data[i][1] = decToBinary(binToDec(half_word),32);
                        return 0;
                    }
                }
                string temp = decToBinary(registers[rs1], 32);
                
                for(int i=16; i<32; i++)
                {
                    half_word += temp[i];
                }
                memory_data.push_back({decTohex(stoi(offset) + registers[rd]),decToBinary(binToDec(half_word),32)});
            }
            break;
        }
        case 30: // SLL
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            registers[rd] = registers[rs1] * pow(2,registers[rs2]);
            break;
        }
        case 31: // SLLI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            
            rd = reg[0];
            
            rs1 = reg[1];
            registers[rd] = registers[rs1] *pow(2,stoi(word));
            break;
        }
            
        case 32: // SLT
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            if (registers[rs1] < registers[rs2])
            {
                registers[rd]=1;
            }
            else
            {
                registers[rd]=0;
            }
            break;
        }
            
        case 33: // SLTI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rd=reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            if (registers[rs1] < stoi(word))
            {
                registers[rd]=1;
            }
            else
            {
                registers[rd]=0;
            }
            break;
        }
            
        case 34: // SLTIU
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            
            rd=reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            int temp_1=registers[rs1];
            int temp_2;
            
            if(temp_1<0)
            {
                temp_1*=-1;
            }
            
            if(stoi(word)<0)
            {
                temp_2=stoi(word)*-1;
            }
            
            if (temp_1 < temp_2)
            {
                registers[rd]=1;
            }
            else
            {
                registers[rd]=0;
            }

            break;
        }
            
        case 35: // SLTU
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            int temp_1=registers[rs1];
            int temp_2=registers[rs2];
            
            if(temp_1<0)
            {
                temp_1*=-1;
            }
            if(temp_2<0)
            {
                temp_2*=-1;
            }
            
            
            if (temp_1 < temp_2)
            {
                registers[rd]=1;
            }
            else
            {
                registers[rd]=0;
            }
            break;
        }
            
        case 36: //SLTZ PSEUDO-INSTRUCTION
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                if (word == ",")
                {
                    iss >> word;
                }
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            
            rd = reg[0];
            rs1 = reg[1];
            
            if (registers[rs1] < 0)
            {
                registers[rd]=1;
            }
            else
            {
                registers[rd]=0;
            }
            break;
        }
            
        case 37: // SRA
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            
            string x=decToBinary(registers[rs1], 32);
            
            char temp=x[0];
            
            for(int i=0; i<registers[rs2]; i++)
            {
                x.pop_back();
            }
        
            for(int i=0; i<registers[rs2]; i++)
            {
                if(temp=='1')
                {
                    x.insert(0, "1");
                }
                else
                {
                    x.insert(0,"0");
                }
            }
            registers[rd] = binToDec(x);
            break;
        }

        case 38: // SRAI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
             
            string x=decToBinary(registers[rs1], 32);
            
            char temp=x[0];
            
            for(int i=0; i<stoi(word); i++)
            {
                x.pop_back();
            }
        
            for(int i=0; i<stoi(word); i++)
            {
                if(temp=='1')
                {
                    x.insert(0, "1");
                }
                else
                {
                    x.insert(0,"0");
                }
            }
            registers[rd] = binToDec(x);

            break;
        }
        case 39: // SRL
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            registers[rd] = registers[rs1] / pow(2,registers[rs2]);

            break;
        }
        case 40: // SRLI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            
            rd = reg[0];
            
            rs1 = reg[1];
            registers[rd] = registers[rs1] / pow(2,stoi(word));

            break;
        }
            
        case 41: // SUB
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            rs2 = reg[2];
            registers[rd] = registers[rs1] - registers[rs2];

            break;
        }
           
        case 42: // SW
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = remove_comma(word);
            reg[0] = get_register(word);
            rs1 = reg[0];
            if (reg[0] == -1)
            {
                return -1;
            }
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            int n = 0;
            string offset = "";
            while (word[n] != '(')
            {
                offset += word[n];
                n++;
            }
            string regis = "";
            n++;
            while (word[n] != ')')
            {
                regis += word[n];
                n++;
            }
            rd = get_register(regis);
            
            if(rd==-1)
            {
                return -1;
            }
            
            if (regis == "sp")
            {
                if (INT_MAX-registers[2]-stoi(offset) <= 0)
                {
                    cout << "invalid stack location\n";
                    return -1;;
                }
                else
                {
                    for (int i = 0; i < stack.size(); i++)
                    {
                        if (stack[i][0] == decTohex(stoi(offset) + registers[rd]))
                        {
                            
                            stack[i][1] = decToBinary(registers[rs1],32);
                            return 0;
                        }
                    }
                    stack.push_back({decTohex(stoi(offset) + registers[rd]),decToBinary(registers[rs1],32)});
                }
            }
            
            else
            {
                
                for (int i = 0; i < memory_data.size(); i++)
                {
                    if (memory_data[i][0] == decTohex(stoi(offset) + registers[rd]))
                    {
                        memory_data[i][0] = decToBinary(registers[rs1],32);
                        return 0;
                    }
                }
                memory_data.push_back({decTohex(stoi(offset) + registers[rd]),decToBinary(registers[rs1],32)});
            }
            break;
        }
            
        case 43: // XOR
        {
            for (int i = 0; i < 3; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            rs2=reg[2];
            int v1 = registers[rs1];
            int v2 = registers[rs2];
            string bin1 = decToBinary(v1, 32);
            string bin2 = decToBinary(v2, 32);
            string bin3 = "";
            
            for (int i = 0; i < 32; i++)
            {
                if (((bin1[i] == '1')&&(bin2[i] == '0'))||((bin1[i] == '0')&&(bin2[i] == '1')))
                {
                    bin3 += '1';
                }
                else
                {
                    bin3 += '0';
                }
            }
            registers[rd] = binToDec(bin3);

            break;
        }
            
        case 44: // XORI
        {
            for (int i = 0; i < 2; i++)
            {
                iss >> word;
                word = remove_comma(word);
                reg[i] = get_register(word);
                if (reg[i] == -1)
                {
                    return -1;
                }
            }
            iss >> word;
            word = remove_comma(word);
            if (reg[0] == 0)
            {
                return -1;
            }
            rd = reg[0];
            rs1 = reg[1];
            int v1 = registers[rs1];
            int v2 = registers[stoi(word)];
            string bin1 = decToBinary(v1, 32);
            string bin2 = decToBinary(v2, 32);
            string bin3 = "";
            for (int i = 0; i < 32; i++)
            {
                if (((bin1[i] == '1')&&(bin2[i] == '0'))||((bin1[i] == '0')&&(bin2[i] == '1')))
                {
                    bin3 += '1';
                }
                else
                {
                    bin3 += '0';
                }
            }
            registers[rd] = binToDec(bin3);
            break;
        }
            
        default:
        {
            break;
        }
    }
    
    return 0;
}

void program(string filename)
{
    fill(registers, registers + 32, 0);
    memory_data.clear();
    stack.clear();
    memory.clear();
    registers[2] = INT_MAX;
    string read;
    ifstream my_file(filename);
    int x,i=0;
    while (getline(my_file, read))
    {
        memory.push_back(read);
        istringstream iss(read);
        string word;
        iss >> word;
        if (read.empty())
        {
            i++;
            continue;
        } else if (word[0] == '#')
        {
            i++;
            continue;
        }
        else if (word.ends_with(':'))
        {
            remove_colon(word);
            memory_data.push_back({decTohex(i*4),word});
            i++;
            continue;
        }else if (word == ".data")
        {
            while (word != ".text")
            {
                getline(my_file, read);
                memory.push_back(read);
                istringstream iss(read);
                string word;
                iss >> word;
                remove_colon(word);
                string name,type;
                name = word;
                iss >> type;
                if (type == ".string")
                {
                    string message = "";
                    while (iss >> word)
                    {
                        message += word;
                    }
                    remove(message, '"');
                    memory_data.push_back({decTohex(i*4),name, message});
                }
                else
                {
                    cout << "format not supported\n";
                }
                i++;

                break;
            }
        }
        i++;
    }
    my_file.close();
    for (pc; pc < memory.size(); pc++)
    {
        istringstream iss(memory[pc]);
        string word;
        iss >> word;
        if (memory[pc].empty())
        {
            continue;
        }
        if (word[0] == '#')
        {
            continue;
        } else if (word.ends_with(':'))
        {
            continue;
        }else if (word == ".data")
        {
            while (memory[pc] != ".text")
            {
                pc++;
            }
            continue;
        }
        int z = binarySearchString(word, 45);
        if (z == -1)
        {
            cout << "ERROR! Invalid instruction, program terminated.\n";
            break;
        }
        x = execute(z, memory[pc]);
        print();
        if (x == -1)
        {
            cout << "ERROR! Invalid instruction, program terminated.\n";
            break;
        }
        if (x == -2)
        {
            break;
        }
    }
}

int main()
{
    string file_name;
    cout << "please enter name of first program ex. file.txt\n";
    cin >> file_name;
    program(file_name);
    cout << "please enter name of second program ex. file.txt\n";
    cin >> file_name;
    program(file_name);
    cout << "please enter name of third program ex. file.txt\n";
    cin >> file_name;
    program(file_name);
    return 0;
}


