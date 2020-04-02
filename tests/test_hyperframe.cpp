#include "hypercine.h"

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;

  HyperCine::HyperFrame hf;
  ASSERT_EXPR(hf.frame_begin==0,error_count);
  ASSERT_EXPR(hf.frame_count==0,error_count);
  ASSERT_EXPR(!hf.is_valid(),error_count);

  hf.frame_begin = 1000;
  hf.frame_count = 25;
  hf.x_begin.push_back(45);
  hf.x_count.push_back(150);
  hf.y_begin.push_back(13);
  hf.y_count.push_back(201);
  hf.x_begin.push_back(25);
  hf.x_count.push_back(11);
  hf.y_begin.push_back(78);
  hf.y_count.push_back(111);

  ASSERT_EXPR(hf.is_valid(),error_count);
  ASSERT_EXPR(hf.num_rois()==2,error_count);

  if (error_count != 0)
    std::cout << "TEST FAILED\n";
  else
    std::cout << "TEST PASSED\n";

  return 0;

}

