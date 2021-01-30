#include "hypercine.h"

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;

  HyperCine hc("./images/packed_12bpp.cine",HyperCine::LINEAR_10_TO_8);

  ASSERT_EXPR(hc.header()->type==18755,error_count);
  ASSERT_EXPR(hc.header()->header_size==44,error_count);
  ASSERT_EXPR(hc.header()->compression==0,error_count);
  ASSERT_EXPR(hc.header()->version==1,error_count);
  ASSERT_EXPR(hc.header()->first_movie_image==0,error_count);
  ASSERT_EXPR(hc.header()->total_image_count==815,error_count);
  ASSERT_EXPR(hc.header()->first_image_no==60,error_count);
  ASSERT_EXPR(hc.header()->image_count==6,error_count);
  ASSERT_EXPR(hc.header()->off_image_header==44,error_count);
  ASSERT_EXPR(hc.header()->off_setup==84,error_count);
  ASSERT_EXPR(hc.header()->off_image_offsets==10612,error_count);
  ASSERT_EXPR(hc.bitmap_header()->size==40,error_count);
  ASSERT_EXPR(hc.bitmap_header()->width==256,error_count);
  ASSERT_EXPR(hc.bitmap_header()->height==128,error_count);
  ASSERT_EXPR(hc.bitmap_header()->planes==1,error_count);
  ASSERT_EXPR(hc.bitmap_header()->bit_count==8,error_count);
  ASSERT_EXPR(hc.bitmap_header()->compression==256,error_count);
  ASSERT_EXPR(hc.bitmap_header()->size_image==40960,error_count);
  ASSERT_EXPR(hc.bitmap_header()->x_pixels_per_meter==35714,error_count);
  ASSERT_EXPR(hc.bitmap_header()->y_pixels_per_meter==35714,error_count);
  ASSERT_EXPR(hc.bitmap_header()->clr_used==0,error_count);
  ASSERT_EXPR(hc.bitmap_header()->clr_important==256,error_count);
  ASSERT_EXPR(hc.conversion_factor_to_16_bit()==65535.0f/255.0f,error_count);
  ASSERT_EXPR(hc.conversion_factor_to_8_bit()==1.0f,error_count);

  HyperCine hc2("./images/packed_12bpp.cine",HyperCine::QUAD_10_TO_12);
  ASSERT_EXPR(hc2.bitmap_header()->clr_important==4096,error_count);
  ASSERT_EXPR(hc2.conversion_factor_to_16_bit()==65535.0f/4095.0f,error_count);
  ASSERT_EXPR(hc2.conversion_factor_to_8_bit()==255.0f/4095.0f,error_count);

  HyperCine hc3("./images/packed_12bpp.cine",HyperCine::QUAD_10_TO_8);
  ASSERT_EXPR(hc3.bitmap_header()->clr_important==256,error_count);
  ASSERT_EXPR(hc3.conversion_factor_to_16_bit()==65535.0f/255.0f,error_count);
  ASSERT_EXPR(hc3.conversion_factor_to_8_bit()==1.0f,error_count);

  HyperCine hc4("./images/packed_12bpp.cine",HyperCine::NO_CONVERSION);
  ASSERT_EXPR(hc4.bitmap_header()->clr_important==1024,error_count);
  ASSERT_EXPR(hc4.conversion_factor_to_16_bit()==65535.0f/1023.0f,error_count);
  ASSERT_EXPR(hc4.conversion_factor_to_8_bit()==255.0f/1023.0f,error_count);

  if (error_count != 0)
    std::cout << "TEST FAILED\n";
  else
    std::cout << "TEST PASSED\n";

  return 0;

}

