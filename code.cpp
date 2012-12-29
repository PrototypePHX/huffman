#include "code.hpp"
#include "tree.hpp"

#include <cstdio>
#include <map>
#include <stdexcept>

// Helper functions
std::string generate_code(std::string plaintext, const tree &t);
void search(node *r, const char c, bool &found, std::string &plaincode);


compress::compress(std::string infile_): infile(infile_)
{
   if (!(infile.is_open()))
       throw std::runtime_error("Error opening file\n");
}

compress::~compress() {
   infile.close();
}

void compress::write(std::string outfile_)
{
   std::ofstream outfile(outfile_, std::ios::binary | std::ios::trunc);
   if (!(outfile.is_open()))
       throw std::runtime_error("Error opening file\n");

   //  create a histogram
   std::string plaintext;
   for (std::string line; std::getline(infile, line); ) {
      line += '\n';
      for (unsigned i = 0; i<=line.size(); histog[line[i++]]++);
      plaintext += line;
   }


   tree hcode(histog);
   std::string plaincode = generate_code(plaintext, hcode);

   //  write huffman-code to file
   uint16_t streamOffset = (sizeof(int)+sizeof(char))*histog.size()+sizeof(uint16_t);
   outfile.write(reinterpret_cast<char*>(&streamOffset), 2);

   //  write chars and their weights into the binary file
   for (auto it = histog.begin(); it != histog.end(); ++it) {
      outfile.put(it->first);
      outfile.write(reinterpret_cast<char*>(&(it->second)), sizeof(int));
   }

   //  write length of last byte
   std::string pcode = plaincode;
   outfile.put(pcode.length() %8);

   //  since only full bytes are written to file, pad last byte
   while (pcode.length() %8 != 0)
      pcode.push_back('0');

   for (; !(pcode.empty()); pcode = pcode.substr(8)) {
      unsigned char byte = 0;
      for (int j=0; j<8; ++j) {
         if (pcode.at(j) == '1')
            byte |= 0x01;
         else
            byte &= ~(0x01);
         if (j < 7) byte<<=1;
      }
      outfile.put(byte);
   }
   outfile.close();
}


std::string generate_code(std::string plaintext, const tree &t)
{
   std::string plaincode;
   while (!(plaintext.empty())) {
      node *p = t.rootnode();
      bool found = false;

      search(p, plaintext[0], found, plaincode);

      plaintext = plaintext.substr(1, plaintext.length()-1);
   }
   return plaincode;
}
void search(node *r, const char c, bool &found, std::string &plaincode)
{
   if (r->left->type() == nodetype::LEAF && dynamic_cast<leaf*>(r->left)->payload == c) {
      found = true;
      plaincode.append("0");
      return;
   }
   if (r->right->type() == nodetype::LEAF && dynamic_cast<leaf*>(r->right)->payload == c) {
      found = true;
      plaincode.append("1");
      return;
   }

   if (!found && !(r->left->type() == nodetype::LEAF)) {
      plaincode.append("0");
      search(r->left, c, found, plaincode);
      //  We went the wrong way; pop the last char successively
      if (!found)
         plaincode = plaincode.substr(0, plaincode.length()-1);
   }
   if (!found && !(r->right->type() == nodetype::LEAF)) {
      plaincode.append("1");
      search(r->right, c, found, plaincode);
      if (!found)
         plaincode = plaincode.substr(0, plaincode.length()-1);
   }
}



decompress::decompress(std::string file_): file(file_, std::ios::binary)
{
   if (!(file.is_open()))
      throw std::logic_error("Error opening file\n");

   //  first byte in the file denotes the offset to the bitstream
   file.read(reinterpret_cast<char*>(&stream_offset), 2);

   for (int i=2; i<stream_offset; i += sizeof(int)+sizeof(char)) {
      char ch = file.get();
      int weight;
      file.read(reinterpret_cast<char*>(&weight), sizeof(int));
      histog[ch] = weight;
   }

   tree hcode(histog);

   getplain(hcode);
}

decompress::~decompress()
{
   file.close();
}


std::string decompress::plaintext() const
{
   return ptext;
}


void decompress::getplain(const tree &hc)
{
   file.seekg(stream_offset, std::ios_base::beg);
   unsigned char lengthLastByte = file.get();

   node *traverser = hc.rootnode();
   bool found = false;
   unsigned char stream;
   for (stream = file.get(); file.peek() != EOF; stream = file.get()) {
      for (int j=0; j<8; ++j) {
         //  go back to the root when we have found a character
         if (found) {
            traverser = hc.rootnode();
            found = false;
         }

         if (stream & 0x80) {
            if (traverser->right->type() == nodetype::LEAF) {
               leaf *right = dynamic_cast<leaf*>(traverser->right);
               ptext.push_back(right->payload);
               found = true;
            } else {
               traverser = traverser->right;
            }
         } else {
            if (traverser->left->type() == nodetype::LEAF) {
               leaf *left = dynamic_cast<leaf*>(traverser->left);
               ptext.push_back(left->payload);
               found = true;
            } else {
               traverser = traverser->left;
            }
         }
         stream <<= 1;
      }
   }

   //  handle the last byte
   for (int i=0; i<lengthLastByte; ++i) {
      if (found) {
         traverser = hc.rootnode();
         found = false;
      }
      if (stream & 0x80) {
         if (traverser->right->type() == nodetype::LEAF) {
            leaf *right = dynamic_cast<leaf*>(traverser->right);
            ptext.push_back(right->payload);
            found = true;
         } else {
            traverser = traverser->right;
         }
      } else {
         if (traverser->left->type() == nodetype::LEAF) {
            leaf *left = dynamic_cast<leaf*>(traverser->left);
            ptext.push_back(left->payload);
            found = true;
         } else {
            traverser = traverser->left;
         }
      }
      stream <<= 1;
   }
}
