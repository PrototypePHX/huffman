#include <iostream>
#include <algorithm>
#include <cstdio>

#include "node.hpp"
#include "code.hpp"

int main(int, char **argv)
{
   std::string opt(argv[1]);
   std::transform(opt.begin(), opt.end(), opt.begin(), tolower);

   if ((opt == "-d" || opt == "--decompress")) {
      decompress de(argv[2]);
      std::cout << de.plaintext();
      return EXIT_SUCCESS;
   } else if (opt == "-c" || opt == "--compress") {
      compress en(argv[2]);
      en.write(argv[3]);
      return EXIT_SUCCESS;
   } else {
      std::cerr << "Invalid argument\n";
   }
   return EXIT_FAILURE;
}

