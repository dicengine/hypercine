#include "hypercine.h"

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;

  HyperCine::HyperFrame hf;
  ASSERT_EXPR(hf.num_frames()==0,error_count);
  ASSERT_EXPR(hf.num_windows()==0,error_count);
  ASSERT_EXPR(hf.num_pixels_per_frame()==0,error_count);
  hf.add_frames(1000,25);
  hf.add_window(45,150,13,201);
  hf.add_window(25,11,78,111);
  ASSERT_EXPR(hf.num_windows()==2,error_count);
  ASSERT_EXPR(hf.num_frames()==25,error_count);
  ASSERT_EXPR(hf.num_pixels_per_window(5)==0,error_count);
  ASSERT_EXPR(hf.num_pixels_per_window(0)==30150,error_count);
  ASSERT_EXPR(hf.num_pixels_per_window(1)==1221,error_count);
  ASSERT_EXPR(hf.num_pixels_per_frame()==31371,error_count);
  ASSERT_EXPR(hf.window_x_begin(0)==45,error_count);
  ASSERT_EXPR(hf.window_y_begin(0)==13,error_count);
  ASSERT_EXPR(hf.window_width(0)==150,error_count);
  ASSERT_EXPR(hf.window_height(0)==201,error_count);
  ASSERT_EXPR(hf.window_x_begin(1)==25,error_count);
  ASSERT_EXPR(hf.window_y_begin(1)==78,error_count);
  ASSERT_EXPR(hf.window_width(1)==11,error_count);
  ASSERT_EXPR(hf.window_height(1)==111,error_count);
  ASSERT_EXPR(hf.buffer_row_size()==150,error_count);
  ASSERT_EXPR(*(hf.frame_ids()->begin())==1000,error_count);
  ASSERT_EXPR(*(hf.frame_ids()->rbegin())==1024,error_count);

  // test resetting the frame ids
  std::set<int> frames;
  frames.insert(200);
  frames.insert(403);
  frames.insert(21);
  hf.update_frames(frames);
  ASSERT_EXPR(hf.num_frames()==frames.size(),error_count);
  for(std::set<int>::const_iterator set_it=frames.begin();set_it!=frames.end();++set_it){
    ASSERT_EXPR(hf.has_frame(*set_it),error_count);
  }

  // test window id method
  ASSERT_EXPR(hf.window_id(12,11,78,111)==-1,error_count);
  ASSERT_EXPR(hf.window_id(25,11,78,111)==1,error_count);

  if (error_count != 0)
    std::cout << "TEST FAILED\n";
  else
    std::cout << "TEST PASSED\n";
  return 0;
}

