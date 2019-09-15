
#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <string>
#include <vector>
#include <queue>

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <thread>
#include <mutex>


class Signer {
    public:
        Signer(char*, char*, uintmax_t);
        void Sign(bool);
        void SingleTreadSign();
        void MultiTreadSign(int);
    private:
        std::vector<std::thread> pool;
        std::queue<std::pair<int,std::string>> blox;
        std::mutex mxq;
        std::vector<size_t> block_hashes;
        std::mutex mxv;
        boost::filesystem::path input_path;
        boost::filesystem::path output_path;
        uintmax_t block_size;

};

#endif