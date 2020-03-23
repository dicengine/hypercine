#include "hypercine.h"

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_flag = 0;

  HyperCine hc;

  if (error_flag != 0)
    std::cout << "End Result: TEST FAILED\n";
  else
    std::cout << "End Result: TEST PASSED\n";

  return 0;

}

