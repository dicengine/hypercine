#include "hypercine.h"

#include <chrono>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  if(argc!=2){
    std::cerr << "Usage: ./read_performance <cine_file>" << std::endl;
    throw std::invalid_argument("invalid input (incorrect arguments)");
  }

  std::string file_name = argv[1];
  HyperCine hc(file_name.c_str());
  const int first_frame_id = hc.file_first_frame_id();
  const int frame_count = hc.file_frame_count()/10;

  std::cout << "reading frame by frame" << std::endl;
  std::chrono::steady_clock::time_point begin_frame_by_frame = std::chrono::steady_clock::now();
  // read each frame one by one
  for(int frame=first_frame_id;frame<first_frame_id+frame_count;++frame){
//    if(frame%100==0) std::cout << "reading frame " << frame << std::endl;
    HyperCine::HyperFrame hf(frame);
    hc.read_buffer(hf);
  }
  std::chrono::steady_clock::time_point end_frame_by_frame = std::chrono::steady_clock::now();

  // read entire cine file and place in buffer all at once
  std::cout << "reading all frames at once " << std::endl;
  std::chrono::steady_clock::time_point begin_all_frames = std::chrono::steady_clock::now();
  HyperCine::HyperFrame hf_all(first_frame_id,frame_count);
  hc.read_buffer(hf_all);
  std::chrono::steady_clock::time_point end_all_frames = std::chrono::steady_clock::now();

  // read entire cine file and place in buffer all at once
  std::cout << "reading all frames at once for window " << std::endl;
  std::chrono::steady_clock::time_point begin_all_frames_w = std::chrono::steady_clock::now();
  HyperCine::HyperFrame hf_all_w(first_frame_id,frame_count);
  hf_all_w.add_window(100,100,100,200);
  hc.read_buffer(hf_all_w);
  std::chrono::steady_clock::time_point end_all_frames_w = std::chrono::steady_clock::now();

  std::cout << "Time frame by frame: " <<
      std::chrono::duration_cast<std::chrono::microseconds>(end_frame_by_frame - begin_frame_by_frame).count() <<
      " [µs]" << std::endl;
  std::cout << "Time all frames:     " <<
      std::chrono::duration_cast<std::chrono::microseconds>(end_all_frames - begin_all_frames).count() <<
      " [µs]" << std::endl;
  std::cout << "Time all frames w:   " <<
      std::chrono::duration_cast<std::chrono::microseconds>(end_all_frames_w - begin_all_frames_w).count() <<
      " [µs]" << std::endl;
}

