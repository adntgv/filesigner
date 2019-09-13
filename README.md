# filesigner
Signer application to geneate signature of provided file

A Console Application witten in  C++ that generates signatures for provided file. Signature is generated as follows: Input file is split onto blocks. For each block hash is generated and it is appended to ouput file.  

Command line options:
    • Input file
    • Output file
    • block_size (default 1 Mb)
    
