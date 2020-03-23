#include "hypercine.h"

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_flag = 0;

  HyperCine hc("./images/packed_12bpp.cine");

  if (error_flag != 0)
    std::cout << "TEST FAILED\n";
  else
    std::cout << "TEST PASSED\n";

  return 0;

}

