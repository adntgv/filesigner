#include <iostream>
#include "signature.h"

using namespace std;

int main(int argc, char** argv) {

    if (argc < 4) return 1;

    Signer s;
    s.Sign(
        boost::filesystem::path{argv[1]},
        boost::filesystem::path{argv[2]},
        atoi(argv[3]) 
    );
    return 0;
}