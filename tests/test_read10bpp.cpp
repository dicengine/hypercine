#include "hypercine.h"

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;
  try {
    HyperCine hc("./images/packed_12bpp.cine",HyperCine::LINEAR_10_TO_8);
    // test the bit count of this cine file
    ASSERT_EXPR(hc.bit_count()==8,error_count);
    HyperCine::HyperFrame hf(60,6); // 60, 61, 62, 63, 64, 65
    // don't add any regions of interest so the whole image is read on the first iteration
    hc.read_buffer(hf);
    std::cout << hc << std::endl;

    cv::Mat img(hc.height(),hc.width(),CV_16UC1,hc.data(60,0));
    // cv::imwrite("packed_12bpp_frame_60.tiff",img);
    // read the gold file
    cv::Mat gold_img = cv::imread("./images/packed_12bpp_frame_60.tiff",cv::IMREAD_GRAYSCALE);
    gold_img.convertTo(gold_img, CV_16UC1); // required since the gold images were saved when the buffers were still 8 bit
    cv::Mat img_diff;
    cv::absdiff(gold_img,img,img_diff);
    double min_diff, max_diff;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 60 max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

    // now try reading two small regions of interest from the same file
    hf.clear();
    hf.add_frames(60);
    hf.add_frames(61);
    hf.add_frames(62);
    hf.add_frames(63);
    hf.add_frames(64);
    hf.add_frames(65);
    hf.add_window(84,89,61,46);
    hf.add_window(169,38,17,20);
    hc.read_buffer(hf);
    std::cout << hc << std::endl;
    if(!hc.buffer_has_frame(65)){
      std::cout << "invalid frame or window" << std::endl;
      error_count ++;
    }
    if(!hc.buffer_has_frame(60)){
      std::cout << "invalid frame" << std::endl;
      error_count ++;
    }
    if(!hc.buffer_has_window(1)){
      std::cout << "invalid window" << std::endl;
      error_count ++;
    }
    if(hc.buffer_has_frame(42)){
      std::cout << "invalid frame should have been caught" << std::endl;
      error_count ++;
    }
    cv::Mat img_roi_0(hc.height(0),hc.width(0),CV_16UC1,hc.data(65,0));
    cv::Mat img_roi_1(hc.height(1),hc.width(1),CV_16UC1,hc.data(60,1));
    // cv::imwrite("packed_12bpp_frame_65_roi.tiff",img_roi);
    // read the gold files and compare
    cv::Mat gold_img_roi_0 = cv::imread("./images/packed_12bpp_frame_65_roi_0.tiff",cv::IMREAD_GRAYSCALE);
    gold_img_roi_0.convertTo(gold_img_roi_0, CV_16UC1);
    cv::Mat img_diff_roi_0;
    cv::absdiff(gold_img_roi_0,img_roi_0,img_diff_roi_0);
    cv::minMaxLoc(img_diff_roi_0,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 65 roi 0 max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

    cv::Mat gold_img_roi_1 = cv::imread("./images/packed_12bpp_frame_60_roi_1.tiff",cv::IMREAD_GRAYSCALE);
    gold_img_roi_1.convertTo(gold_img_roi_1, CV_16UC1);
    cv::Mat img_diff_roi_1;
    cv::absdiff(gold_img_roi_1,img_roi_1,img_diff_roi_1);
    cv::minMaxLoc(img_diff_roi_1,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 60 roi 1 max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

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

