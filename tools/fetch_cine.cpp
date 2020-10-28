#include "hypercine.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  if(argc!=2&&argc!=4){
    std::cerr << "Usage: ./fetch_cine <cine_file> [<frame_id> <out_file_name>]" << std::endl;
    throw std::invalid_argument("invalid input (incorrect arguments)");
  }

  std::string file_name = argv[1];
  HyperCine hc(file_name.c_str());
  const int first_frame_id = hc.file_first_frame_id();
  const int frame_count = hc.file_frame_count();

  if(argc==2){

    BUFFER_MSG("FIRST_FRAME_ID",first_frame_id);
    BUFFER_MSG("FRAME_COUNT",frame_count);
    return 0;

  }else{

    int frame = strtol(argv[2], NULL, 10);
    std::string out_file_name = argv[3];
    DEBUG_MSG("requested frame: " << frame);
    if(frame<first_frame_id||frame>=first_frame_id+frame_count){
      std::cerr << "invalid frame: " << frame << " first frame " << first_frame_id << " last frame " <<
          first_frame_id + frame_count - 1 << std::endl;
      throw std::invalid_argument("invalid frame requested");
    }
    HyperCine::HyperFrame hf(frame);
    hc.read_buffer(hf);
    cv::Mat img(hc.height(),hc.width(),CV_8UC1,hc.data(frame));
    cv::imwrite(out_file_name.c_str(),img);

  }
}

