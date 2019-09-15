#include <signature.h> 
#include <iostream>
#include <string>


using namespace boost::filesystem;

Signer::Signer(char* input_file_path, char* output_file_path, uintmax_t bs){
    input_path = boost::filesystem::path{input_file_path};
    output_path = boost::filesystem::path{output_file_path};
    block_size = bs;
    pool = std::vector<std::thread>();
    blox = std::queue<std::pair<int,std::string>>();
}

void Signer::Sign(bool multithreaded = false) {
    if (!boost::filesystem::exists(input_path))
        throw std::string("File does not exist: ") + input_path.string();
    int n_threads = std::thread::hardware_concurrency();
    if (multithreaded && n_threads > 1) {
        return MultiTreadSign(n_threads);
    }
    return SingleTreadSign();
};

void Signer::SingleTreadSign() {
    
    auto in_file_size = file_size(input_path); // size of the file in bytes
    ifstream in_strm{input_path}; 
    ofstream out_strm{output_path};
    std::string hash_string;
    
    auto num_blocks = (int)in_file_size / block_size;
    auto left = in_file_size % block_size;
    
    char in_buf[block_size];
    size_t block_hashes[num_blocks];

    boost::hash<std::string> buf_hash;
    size_t block_index = 0;

    for(; block_index < num_blocks; block_index++ ) {
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

void Signer::MultiTreadSign(int n_threads){
    auto f = [this](){
        uintmax_t block_num = 0;
        boost::hash<std::string> buf_hash;
        size_t hash = 0;
        while(! blox.empty()){
            mxq.lock();
            auto p = blox.front();
            blox.pop();
            mxq.unlock();

            block_num = p.first;
            hash = buf_hash(p.second);

            std::cout << std::this_thread::get_id() << " " << block_num << " " << hash << "\n";

            mxv.lock();
            block_hashes[block_num] = hash;
            mxv.unlock();
        }     
    };

    auto in_file_size = file_size(input_path); // size of the file in bytes
    ifstream in_strm{input_path}; 
    ofstream out_strm{output_path};
    std::string hash_string;
    
    auto num_blocks = (int)in_file_size / block_size;
    auto left = in_file_size % block_size;
    
    char in_buf[block_size];
    for (int i = 0; i < block_size; i++) in_buf[i] = 0;
    block_hashes = std::vector<size_t>( (left) ? num_blocks + 1: num_blocks);

    boost::hash<std::string> buf_hash;
    size_t block_index = 0;

    for(; block_index < num_blocks; block_index++ ) {
        in_strm.read(in_buf, block_size);
        mxq.lock();
        blox.push(std::make_pair(block_index, std::string(in_buf)));
        mxq.unlock();
    }

    if (left != 0) {
        in_strm.read(in_buf, left);
        for (int i = left; i < block_size; i++) in_buf[i] = 0;
        mxq.lock();
        blox.push(std::make_pair(block_index, std::string(in_buf)));
        mxq.unlock();
    }

    for (int i = 0; i < n_threads; i++) {
        try {pool.push_back(std::thread(f));}
        catch (const std::exception& e){ throw e.what();}
    }

    std::for_each(pool.begin(),pool.end(), std::mem_fn(&std::thread::join));

    for (auto& h: block_hashes)
    {
        hash_string.append(std::to_string(h));
    }
    
    out_strm.write(hash_string.c_str(), hash_string.size());
}

