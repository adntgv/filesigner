
#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <string>

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>


class Signer {
    public:
        size_t Sign(boost::filesystem::path input_path, boost::filesystem::path output_path, uintmax_t block_size);
};
#endif