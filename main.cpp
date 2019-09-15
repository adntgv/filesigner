#include <iostream>
#include "signature.h"

#define _1M 1048576
#define _1k 1024

using namespace std;

const string info = "\
Required params:\n\
    input file\n\
    output file\n\
Optional:\n\
    block size (nb, nk, nM) Default 1M\n";

int to_byte_num(const string str){
    int mul = 0;
    int l = str.size();
    char type = str[l-1];
    switch (type)
    {
        case 'b':
            mul = 1 ;
            break;
        case 'k':
            mul = _1k;
            break;
        case 'M': 
            mul = _1M;
            break;
        default:
            throw string("Unknown size type: ") + str;
            break;
    }
    
    int num = 0;

    string num_str = str.substr(0,l-1);
    try
    {
        num = stoi(num_str); 
    }
    catch(const invalid_argument e)
    {
        string err = "Cannot convert \" " + num_str + " \" to bytes: " + str;
        throw err;
    }
    
    
    return num * mul;
}

int main(int argc, char** argv) {

    if (argc < 3) {
        cout << info << endl;
        return 1;
        }
    
    char* input_file_path = argv[1];

    char* output_file_path = argv[2];

    int block_size = _1M;

    if (argc >= 4) {
        try
        {
            block_size = to_byte_num(string(argv[3]));
        }
        catch(string e)
        {
            std::cerr << e << '\n';
            return 2;
        }
    }

    Signer s(input_file_path, output_file_path, block_size);
    try {
        s.Sign(true);
    } catch (string e) {
        std::cerr << e << '\n';
        return 3;
    }

    return 0;
}
