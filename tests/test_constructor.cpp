#include "hypercine.h"

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_flag = 0;

  HyperCine meachnism_hc("/Users/dzturne/problems/dic_check_optical_flow/mechanism.cine");
  HyperCine stringlink_hc("/Users/dzturne/problems/stronglink/kdice_track/good_a/GoodA.cine");

  if (error_flag != 0)
    std::cout << "End Result: TEST FAILED\n";
  else
    std::cout << "End Result: TEST PASSED\n";

  return 0;

}

