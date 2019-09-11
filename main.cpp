#include <iostream>
#include "signature.h"

using namespace std;

int main(int argc, char** argv) {
    Signer s;
    s.Sign(
        boost::filesystem::path{"input.txt"},
        boost::filesystem::path{"input.txt"},
        1024*1024 
    );
    return 0;
}