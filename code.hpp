#ifndef CODE_HPP
#define CODE_HPP

#include <map>
#include <queue>
#include <fstream>
#include <cstdint>

#include "node.hpp"
#include "tree.hpp"

class compress
{
   public:
      compress(std::string infile_);
      ~compress();

      void write(std::string outfile_);

   private:
      std::ifstream infile;
      std::map<char, int> histog;
};

class decompress
{
   public:
      decompress(std::string file_);
      ~decompress();

      std::string plaintext() const;


   private:
      std::ifstream file;
      uint16_t stream_offset;
      std::string ptext;
      std::map<char, int> histog;

      void getplain(const tree &hc);
};



#endif // CODE_HPP
