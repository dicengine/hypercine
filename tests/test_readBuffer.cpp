#include "hypercine.h"

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace hypercine;

#define IMG_WIDTH 512
#define IMG_HEIGHT 256

uint16_t function_value(const int frame, const size_t x, const size_t y){
  return std::floor(65535*std::abs(std::sin((4.0*(x+frame*50)*3.141592654)/IMG_WIDTH)*std::sin((2.0*(y+frame*10)*3.141592654)/IMG_HEIGHT)));
}

bool values_pass(cv::Mat & mat, const int frame, const size_t offset_x=0, const size_t offset_y=0){

  int diff = 0;
  for(int y=0;y<mat.rows;++y){
    for(int x=0;x<mat.cols;++x){
      diff += mat.at<uint16_t>(y,x) - function_value(frame,x+offset_x,y+offset_y);
    }
  }
  return diff==0;
}

int main(int argc, char *argv[]) {

  int error_count = 0;
  try {

    // create a cine with intensity values as a function of position for testing:
    DEBUG_MSG("*** writing test_bufferRead.cine file");
    const size_t num_gold_frames = 15;
    cv::Mat gold_img(IMG_HEIGHT,IMG_WIDTH,CV_16UC1);
    const std::string file_name = "test_readBuffer.cine";
    for(size_t frame=0;frame<num_gold_frames;++frame){
      for(int j=0;j<IMG_HEIGHT;++j){
        for(int i=0;i<IMG_WIDTH;++i){
          gold_img.at<uint16_t>(j,i) = function_value(frame,i,j);
        }
      }
      HyperCine::write_frame(file_name.c_str(),IMG_WIDTH,IMG_HEIGHT,(uint16_t*)gold_img.data,frame==0);
      //std::stringstream filename;
      //filename << "frame_" << frame << ".tif";
      //cv::imwrite(filename.str(),gold_img);
    }


    // *** buffer not yet initialized cases


    DEBUG_MSG("*** testing frame id provided for uninitialized buffer");
    cv::Mat img;
    HyperCine hc1(file_name.c_str());
    int frame = 5;
    img = cv::Mat(hc1.height(),hc1.width(),CV_16UC1,hc1.data(frame));
    ASSERT_EXPR(values_pass(img,frame),error_count);

    DEBUG_MSG("*** testing window dims and frame_id provided for uninitialized buffer");
    HyperCine hc2(file_name.c_str());
    size_t x_begin = 34;
    size_t x_count = 100;
    size_t y_begin = 28;
    size_t y_count = 200;
    frame = 12;
    img = cv::Mat(y_count,x_count,CV_16UC1,hc2.data(frame,x_begin,x_count,y_begin,y_count));
    ASSERT_EXPR(values_pass(img,frame,x_begin,y_begin),error_count);


    // *** buffer alread initialied cases


    // create a buffer with the first 10 of the 15 frames
    HyperCine::HyperFrame hf(0,10);
    hc1.read_buffer(hf);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==10,error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==1,error_count);

    // TODO create a buffer using sets of window dims for a single frame


    // frame id provided that exists in buffer (default window_id 0)
    DEBUG_MSG("*** testing in-buffer-range frame_id provided for initialized buffer");
    DEBUG_MSG("    (no calls to read buffer should be made)");
    frame = 3;
    img = cv::Mat(hc1.height(),hc1.width(),CV_16UC1,hc1.data(frame));
    ASSERT_EXPR(values_pass(img,frame),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==10,error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==1,error_count);

    // frame id provided that doesn't exist in buffer (default window id 0)
    DEBUG_MSG("*** testing out-of-buffer-range frame_id provided for initialized buffer");
    DEBUG_MSG("    (read buffer should be called)");
    frame = 14;
    img = cv::Mat(hc1.height(),hc1.width(),CV_16UC1,hc1.data(frame));
    ASSERT_EXPR(values_pass(img,frame),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==1,error_count);
    ASSERT_EXPR((*hc1.hyperframe()->get_frame_ids().begin()==frame),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==1,error_count);
//    cv::imwrite("test.tif",img);

    // frame id that doesn't exist and window dims that don't exist
    DEBUG_MSG("*** testing out-of-buffer-range frame_id and non-existing window dims provided for initialized buffer");
    DEBUG_MSG("    (read buffer should be called)");
    frame = 11;
    img = cv::Mat(y_count,x_count,CV_16UC1,hc1.data(frame,x_begin,x_count,y_begin,y_count));
    ASSERT_EXPR(values_pass(img,frame,x_begin,y_begin),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==1,error_count);
    ASSERT_EXPR((*hc1.hyperframe()->get_frame_ids().begin()==frame),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==1,error_count);
    ASSERT_EXPR(hc1.hyperframe()->window_height(0)==y_count,error_count);
    ASSERT_EXPR(hc1.hyperframe()->window_width(0)==x_count,error_count);
    ASSERT_EXPR(hc1.hyperframe()->window_id(x_begin,x_count,y_begin,y_count)==0,error_count);
    ASSERT_EXPR(hc1.hyperframe()->window_id(0,x_count,y_begin,y_count)==-1,error_count);

    HyperCine::HyperFrame hf2(0,11);
    hf2.add_window(x_begin,x_count,y_begin,y_count);
    hf2.add_window(x_begin+50,x_count,y_begin+25,y_count);
    hf2.add_window(x_begin+100,x_count+100,y_begin+25,y_count);
    hf2.add_window(x_begin+12,x_count+100,y_begin+19,y_count+3);
    hc1.read_buffer(hf2);

    // valid frame and window id
    DEBUG_MSG("*** testing in-buffer-range frame_id and valid window id provided for initialized buffer");
    DEBUG_MSG("    (no calls to read buffer should be made)");
    frame = 7;
    size_t window_id = 3;
    img = cv::Mat(y_count+3,x_count+100,CV_16UC1,hc1.data(frame,window_id));
    ASSERT_EXPR(values_pass(img,frame,x_begin+12,y_begin+19),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==11,error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==4,error_count);

    // valid frame and valid window dimensions
    DEBUG_MSG("*** testing in-buffer-range frame_id and valid window dims provided for initialized buffer");
    DEBUG_MSG("    (no calls to read buffer should be made)");
    frame = 3;
    img = cv::Mat(y_count+3,x_count+100,CV_16UC1,hc1.data(frame,x_begin+12,x_count+100,y_begin+19,y_count+3));
    ASSERT_EXPR(values_pass(img,frame,x_begin+12,y_begin+19),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==11,error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==4,error_count);

    // frame id that doesn't exist, but window dimensions that do
    DEBUG_MSG("*** testing out-of-buffer-range frame_id and valid window dims provided for initialized buffer");
    DEBUG_MSG("    (read buffer should be called, but the window dims should remain the same)");
    frame = 14;
    img = cv::Mat(y_count+3,x_count+100,CV_16UC1,hc1.data(frame,x_begin+12,x_count+100,y_begin+19,y_count+3));
    ASSERT_EXPR(values_pass(img,frame,x_begin+12,y_begin+19),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==1,error_count);
    ASSERT_EXPR((*hc1.hyperframe()->get_frame_ids().begin()==frame),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==4,error_count);

    // frame id that doesn't exist, but valid window id
    DEBUG_MSG("*** testing out-of-buffer-range frame_id and valid window id provided for initialized buffer");
    DEBUG_MSG("    (read buffer should be called, but the window dims should remain the same)");
    frame = 0;
    img = cv::Mat(y_count,x_count+100,CV_16UC1,hc1.data(frame,2));
    ASSERT_EXPR(values_pass(img,frame,x_begin+100,y_begin+25),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_frames()==1,error_count);
    ASSERT_EXPR((*hc1.hyperframe()->get_frame_ids().begin()==frame),error_count);
    ASSERT_EXPR(hc1.hyperframe()->num_windows()==4,error_count);


    // *** intentional fail cases [should throw exceptions for all these]


    // non-zero window id for uninitialied buffer
    DEBUG_MSG("*** testing non-zero window id for uninitialized buffer");
    HyperCine hc3(file_name.c_str());
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc3.data(frame,2);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

    // valid frame, initialized buffer, but invalid window id provided
    DEBUG_MSG("*** testing invalid window id for initialized buffer");
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc1.data(frame,17);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

    // frame id that doesn't exist and invalid window id
    DEBUG_MSG("*** testing invalid frame and invalid window id for initialized buffer");
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc1.data(frame+2000,17);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

    // frame id out of bounds for uninitialized buffer
    DEBUG_MSG("*** testing out of bounds frame id for uninitialized buffer");
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc3.data(frame+2000);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

    // frame id out of bounds for initialized buffer
    DEBUG_MSG("*** testing out of bounds frame id for initialized buffer");
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc1.data(frame+2000);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

    // window dims out of bounds for uninitialized buffer
    DEBUG_MSG("*** testing out of bounds window dims for uninitialized buffer");
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc3.data(frame,x_begin,x_count,y_begin,y_count+2000);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

    // window dims out of bounds for itialized buffer
    DEBUG_MSG("*** testing out of bounds window dims for initialized buffer");
    try{
      std::cout << "expecting error" << std::endl;
      uint16_t * ptr = hc1.data(frame,x_begin,x_count,y_begin,y_count+2000);
      // exception should throw before this step
      ASSERT_EXPR(false,error_count);
    } catch(std::exception &e){
    }

  } catch(std::exception& e) {
    std::cerr << "test failed due to exception thrown" << std::endl;
    std::cerr << e.what() << std::endl;
    error_count++;
  }
  if (error_count != 0)
    std::cout << "TEST FAILED\n";
  else
    std::cout << "TEST PASSED\n";

  return 0;

}

