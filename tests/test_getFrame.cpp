#include "hypercine.h"

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;
  try {

    // 8 bit full frame

    HyperCine hc8("./images/example_8bpp.cine");
    // read a frame using a HyperFrame and the memory buffer
    int frame = 7;
    int count = 3;
    HyperCine::HyperFrame hf8(frame,count);
    // don't add any regions of interest so the whole image is read on the first iteration
    hc8.read_buffer(hf8);
    cv::Mat img(hc8.height(),hc8.width(),hc8.opencv_data_type(),hc8.data(frame));
    cv::Mat img_c(hc8.height(),hc8.width(),hc8.opencv_data_type(),hc8.data(frame+count-1));
//    img.convertTo(img, CV_8UC1);
//    cv::imwrite("gold.png",img);
    // read the same frame using the get_full_frame method
    std::vector<storage_t> data = hc8.get_frame(frame);
    cv::Mat img_(hc8.height(),hc8.width(),hc8.opencv_data_type(),&data[0]);
    //img_.convertTo(img_, CV_8UC1);
    //cv::imwrite("gold_not.png",img_);
    // diff the two
    cv::Mat img_diff;
    cv::absdiff(img,img_,img_diff);
    double min_diff, max_diff;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame " << frame << " max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);
    // test that the buffer hasn't been impacted by the call to get_full_frame
    cv::Mat img_c_(hc8.height(),hc8.width(),hc8.opencv_data_type(),hc8.data(frame+count-1));
    cv::absdiff(img_c,img_c_,img_diff);
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "post get_frame() call max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);
    ASSERT_EXPR(hc8.hyperframe()->num_frames()==count,error_count);
    ASSERT_EXPR(hc8.hyperframe()->num_windows()==1,error_count);

    // 16 bit full frame

    HyperCine hc16("./images/phantom_v7_raw_16bpp.cine",hypercine::HyperCine::TO_8_BIT);
    img = cv::imread("./images/phantom_v7_raw_16bpp_frame_238292.tiff",cv::IMREAD_GRAYSCALE);
    //cv::imwrite("gold.png",img);
    data = hc16.get_frame(238292);
    img_ = cv::Mat(hc16.height(),hc16.width(),hc16.opencv_data_type(),&data[0]);
    // truncate the values in the Mat so they are integer numbers
    for(int y=0;y<img_.rows;++y){
      for(int x=0;x<img_.cols;++x){
        img_.at<storage_t>(y,x) =  std::floor(img_.at<storage_t>(y,x));
      }
    } // then convert to 8 bit values
    img_.convertTo(img_,CV_8UC1);//,255.0/hc16.max_possible_intensity());
    //cv::imwrite("gold_not.png",img_);
    // diff the two
    cv::absdiff(img,img_,img_diff);
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "16 bit max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

    // 10 bit packed full frame

    HyperCine hc10("./images/packed_12bpp.cine",HyperCine::LINEAR_10_TO_8);
    img = cv::imread("./images/packed_12bpp_frame_60.tiff",cv::IMREAD_GRAYSCALE);
    //cv::imwrite("gold.png",img);
    data = hc10.get_frame(60);
    img_ = cv::Mat(hc10.height(),hc10.width(),hc10.opencv_data_type(),&data[0]);
    img_.convertTo(img_,CV_8UC1,255.0/hc10.max_possible_intensity());
    //cv::imwrite("gold_not.png",img_);
    // diff the two
    cv::absdiff(img,img_,img_diff);
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "10 bit max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

    // read an average of all the frames
    data = hc8.get_avg_frame(0,10);
    img_ = cv::Mat(hc8.height(),hc8.width(),hc8.opencv_data_type(),&data[0]);
    // truncate the values in the Mat so they are integer numbers
    for(int y=0;y<img_.rows;++y){
      for(int x=0;x<img_.cols;++x){
        img_.at<storage_t>(y,x) =  std::floor(img_.at<storage_t>(y,x));
      }
    } // then convert to 8 bit values
    img_.convertTo(img_, CV_8UC1);
//    cv::imwrite("avg_img.tif",img_);
    img = cv::imread("./images/avg_frame_8_bit.tif",cv::IMREAD_GRAYSCALE);
    cv::absdiff(img,img_,img_diff);
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "avg frame max diff: " << max_diff << std::endl;
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

