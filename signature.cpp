#include <signature.h> 
#include <iostream>
#include <string>


using namespace boost::filesystem;

size_t Signer::Sign(path input_path, path output_path, uintmax_t block_size) {

            auto in_file_size = file_size(input_path); // size of the file in bytes
            ifstream in_strm{input_path}; 
            ofstream out_strm{output_path};
            std::string hash_string;
            
            auto num_blocks = in_file_size / block_size;
            auto left = in_file_size % block_size;
            
            char in_buf[block_size];
            size_t block_hashes[num_blocks];

            boost::hash<std::string> buf_hash;

            for(auto block_index = 0; block_index < num_blocks; block_index++ ) {
                in_strm.read(in_buf, block_size);
                block_hashes[block_index] = buf_hash(in_buf);
                
                hash_string.append(std::to_string( buf_hash(in_buf)));
            }

            if (left != 0) {
                for (int i = left; i < block_size; i++) in_buf[i] = 0;
                in_strm.read(in_buf, left);
                hash_string.append(std::to_string( buf_hash(in_buf)));
            }

            out_strm.write(hash_string.c_str(), hash_string.size());
};