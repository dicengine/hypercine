#include "hypercine.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <math.h>
#include <iomanip>

namespace hypercine {

// hash table to convert 12bit integer values from the 10-bit packed format to 8 bit image intensity values
const static uint8_t Lin8UT[1024] =
 {0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,
   8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,15,15,
   15,15,16,16,16,16,17,17,17,17,18,18,18,18,19,19,19,19,20,20,20,20,21,21,21,21,22,22,22,22,
   23,23,23,23,24,24,24,24,25,25,25,25,26,26,26,26,27,27,27,27,28,28,28,28,29,29,29,29,30,30,
   30,30,31,31,31,31,32,32,32,32,33,33,33,33,34,34,34,34,35,35,35,35,36,36,36,36,37,37,37,37,
   38,38,38,38,39,39,39,39,40,40,40,40,41,41,41,41,42,42,42,42,43,43,43,43,44,44,44,44,45,45,
   45,45,46,46,46,46,47,47,47,47,48,48,48,48,49,49,49,49,50,50,50,50,51,51,51,51,52,52,52,52,
   53,53,53,53,54,54,54,54,55,55,55,55,56,56,56,56,57,57,57,57,58,58,58,58,59,59,59,59,60,60,
   60,60,61,61,61,61,62,62,62,62,63,63,63,63,64,64,64,64,65,65,65,65,66,66,66,66,67,67,67,67,
   68,68,68,68,69,69,69,69,70,70,70,70,71,71,71,71,72,72,72,72,73,73,73,73,74,74,74,74,75,75,
   75,75,76,76,76,76,77,77,77,77,78,78,78,78,79,79,79,79,80,80,80,80,81,81,81,81,82,82,82,82,
   83,83,83,83,84,84,84,84,85,85,85,85,85,86,86,86,86,87,87,87,87,88,88,88,88,89,89,89,89,90,
   90,90,90,91,91,91,91,92,92,92,92,93,93,93,93,94,94,94,94,95,95,95,95,96,96,96,96,97,97,97,
   97,98,98,98,98,99,99,99,99,100,100,100,100,101,101,101,101,102,102,102,102,103,103,103,103,104,104,104,104,105,
   105,105,105,106,106,106,106,107,107,107,107,108,108,108,108,109,109,109,109,110,110,110,110,111,111,111,111,112,112,112,
   112,113,113,113,113,114,114,114,114,115,115,115,115,116,116,116,116,117,117,117,117,118,118,118,118,119,119,119,119,120,
   120,120,120,121,121,121,121,122,122,122,122,123,123,123,123,124,124,124,124,125,125,125,125,126,126,126,126,127,127,127,
   127,128,128,128,128,129,129,129,129,130,130,130,130,131,131,131,131,132,132,132,132,133,133,133,133,134,134,134,134,135,
   135,135,135,136,136,136,136,137,137,137,137,138,138,138,138,139,139,139,139,140,140,140,140,141,141,141,141,142,142,142,
   142,143,143,143,143,144,144,144,144,145,145,145,145,146,146,146,146,147,147,147,147,148,148,148,148,149,149,149,149,150,
   150,150,150,151,151,151,151,152,152,152,152,153,153,153,153,154,154,154,154,155,155,155,155,156,156,156,156,157,157,157,
   157,158,158,158,158,159,159,159,159,160,160,160,160,161,161,161,161,162,162,162,162,163,163,163,163,164,164,164,164,165,
   165,165,165,166,166,166,166,167,167,167,167,168,168,168,168,169,169,169,169,170,170,170,170,170,171,171,171,171,172,172,
   172,172,173,173,173,173,174,174,174,174,175,175,175,175,176,176,176,176,177,177,177,177,178,178,178,178,179,179,179,179,
   180,180,180,180,181,181,181,181,182,182,182,182,183,183,183,183,184,184,184,184,185,185,185,185,186,186,186,186,187,187,
   187,187,188,188,188,188,189,189,189,189,190,190,190,190,191,191,191,191,192,192,192,192,193,193,193,193,194,194,194,194,
   195,195,195,195,196,196,196,196,197,197,197,197,198,198,198,198,199,199,199,199,200,200,200,200,201,201,201,201,202,202,
   202,202,203,203,203,203,204,204,204,204,205,205,205,205,206,206,206,206,207,207,207,207,208,208,208,208,209,209,209,209,
   210,210,210,210,211,211,211,211,212,212,212,212,213,213,213,213,214,214,214,214,215,215,215,215,216,216,216,216,217,217,
   217,217,218,218,218,218,219,219,219,219,220,220,220,220,221,221,221,221,222,222,222,222,223,223,223,223,224,224,224,224,
   225,225,225,225,226,226,226,226,227,227,227,227,228,228,228,228,229,229,229,229,230,230,230,230,231,231,231,231,232,232,
   232,232,233,233,233,233,234,234,234,234,235,235,235,235,236,236,236,236,237,237,237,237,238,238,238,238,239,239,239,239,
   240,240,240,240,241,241,241,241,242,242,242,242,243,243,243,243,244,244,244,244,245,245,245,245,246,246,246,246,247,247,
   247,247,248,248,248,248,249,249,249,249,250,250,250,250,251,251,251,251,252,252,252,252,253,253,253,253,254,254,254,254,
   255,255,255};

HyperCine::HyperCine(const char * file_name):
  file_name_(file_name){
  read_header(file_name);
}

/// check for valid frame and window id:
bool
HyperCine::valid_frame_window(const int frame, const size_t window_id){
  if(data_indices_.size()==0){
    std::cout << "error: data is not initialized yet" << std::endl;
    return false;
  }
  if(hf_.frame_ids_view()->find(frame)==hf_.frame_ids_view()->end()){
    std::cout << "error: invalid frame number: " << frame  << " not found in buffer frames" << std::endl;
    std::cout << " first frame in file " << header_.first_image_no <<
        " last frame " << header_.first_image_no + header_.image_count << std::endl;
    return false;
  }
  if(window_id>=hf_.num_windows()){
    std::cout << "error: invalid window id: " << window_id << std::endl;
    return false;
  }
  return true;
}

/// return a pointer to the raw data for a given frame and window
char *
HyperCine::data(const int frame, const size_t window_id){
  return &data_[data_indices_.find(frame)->second[window_id]];
}

void
HyperCine::read_header(const char * file_name){

  std::ifstream cine_file(file_name, std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    throw std::invalid_argument("invalid cine file " + (std::string)file_name);
  }
  cine_file.seekg(0, std::ios::end);
  long long int file_size = cine_file.tellg();
  cine_file.seekg(0, std::ios::beg);

  // CINE HEADER

  DEBUG_MSG("HyperCine::read_header(): reading the cine header info: " << file_name);
  cine_file.read(reinterpret_cast<char*>(&header_.type), sizeof(header_.type));
  DEBUG_MSG("HyperCine::read_header(): file size:            " << file_size);
  DEBUG_MSG("HyperCine::read_header(): header type:          " << header_.type);
  cine_file.read(reinterpret_cast<char*>(&header_.header_size), sizeof(header_.header_size));
  DEBUG_MSG("HyperCine::read_header(): header size:          " << header_.header_size);
  int test_size = 0;
  test_size += sizeof(header_.type);
  test_size += sizeof(header_.header_size);
  test_size += sizeof(header_.compression);
  test_size += sizeof(header_.version);
  test_size += sizeof(header_.first_movie_image);
  test_size += sizeof(header_.total_image_count);
  test_size += sizeof(header_.first_image_no);
  test_size += sizeof(header_.image_count);
  test_size += sizeof(header_.off_image_header);
  test_size += sizeof(header_.off_setup);
  test_size += sizeof(header_.off_image_offsets);
  test_size += sizeof(header_.trigger_time);
  assert(test_size==header_.header_size);
  cine_file.read(reinterpret_cast<char*>(&header_.compression), sizeof(header_.compression));
  DEBUG_MSG("HyperCine::read_header(): header compression:   " << header_.compression);
  assert(header_.compression==0);
  cine_file.read(reinterpret_cast<char*>(&header_.version), sizeof(header_.version));
  DEBUG_MSG("HyperCine::read_header(): header version:       " << header_.version);
  assert(header_.version==1);
  cine_file.read(reinterpret_cast<char*>(&header_.first_movie_image), sizeof(header_.first_movie_image));
  DEBUG_MSG("HyperCine::read_header(): header first mov img: " << header_.first_movie_image);
  cine_file.read(reinterpret_cast<char*>(&header_.total_image_count), sizeof(header_.total_image_count));
  DEBUG_MSG("HyperCine::read_header(): total image count:    " << header_.total_image_count);
  cine_file.read(reinterpret_cast<char*>(&header_.first_image_no), sizeof(header_.first_image_no));
  DEBUG_MSG("HyperCine::read_header(): first image no:       " << header_.first_image_no);
  cine_file.read(reinterpret_cast<char*>(&header_.image_count), sizeof(header_.image_count));
  DEBUG_MSG("HyperCine::read_header(): header image count:   " << header_.image_count);
  cine_file.read(reinterpret_cast<char*>(&header_.off_image_header), sizeof(header_.off_image_header));
  assert((int)header_.off_image_header==test_size);
  DEBUG_MSG("HyperCine::read_header(): offset image header:  " << header_.off_image_header);
  cine_file.read(reinterpret_cast<char*>(&header_.off_setup), sizeof(header_.off_setup));
  DEBUG_MSG("HyperCine::read_header(): offset setup:         " << header_.off_setup);
  cine_file.read(reinterpret_cast<char*>(&header_.off_image_offsets), sizeof(header_.off_image_offsets));
  DEBUG_MSG("HyperCine::read_header(): offset image offsets: " << header_.off_image_offsets);
  cine_file.read(reinterpret_cast<char*>(&header_.trigger_time), sizeof(header_.trigger_time));
  image_offsets_.resize(header_.image_count);

  // BITMAP HEADER

  DEBUG_MSG("HyperCine::read_header(): reading the cine bitmap header:");
  // seek the file posision of the image header:
  cine_file.seekg(header_.off_image_header);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.size), sizeof(bitmap_header_.size));
  DEBUG_MSG("HyperCine::read_header(): bitmap header size:      " << bitmap_header_.size);
  int header_test_size = 0;
  header_test_size += sizeof(bitmap_header_.size);
  header_test_size += sizeof(bitmap_header_.width);
  header_test_size += sizeof(bitmap_header_.height);
  header_test_size += sizeof(bitmap_header_.planes);
  header_test_size += sizeof(bitmap_header_.bit_count);
  header_test_size += sizeof(bitmap_header_.compression);
  header_test_size += sizeof(bitmap_header_.size_image);
  header_test_size += sizeof(bitmap_header_.x_pixels_per_meter);
  header_test_size += sizeof(bitmap_header_.y_pixels_per_meter);
  header_test_size += sizeof(bitmap_header_.clr_used);
  header_test_size += sizeof(bitmap_header_.clr_important);
  assert(header_test_size==(int)bitmap_header_.size);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.width), sizeof(bitmap_header_.width));
  DEBUG_MSG("HyperCine::read_header(): bitmap width:            " << bitmap_header_.width);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.height), sizeof(bitmap_header_.height));
  DEBUG_MSG("HyperCine::read_header(): bitmap height:           " << bitmap_header_.height);
  if(bitmap_header_.height < 0){
    std::cout <<"** Warning: the cine file has recorded the pixel array upside down" << std::endl;
    bitmap_header_.height *= -1;
  }
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.planes), sizeof(bitmap_header_.planes));
  DEBUG_MSG("HyperCine::read_header(): bitmap num planes:       " << bitmap_header_.planes);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.bit_count), sizeof(bitmap_header_.bit_count));
  DEBUG_MSG("HyperCine::read_header(): bitmap bit count:        " << bitmap_header_.bit_count);
  assert(bitmap_header_.bit_count==8||bitmap_header_.bit_count==16);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.compression), sizeof(bitmap_header_.compression));
  DEBUG_MSG("HyperCine::read_header(): bitmap compression:      " << bitmap_header_.compression);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.size_image), sizeof(bitmap_header_.size_image));
  DEBUG_MSG("HyperCine::read_header(): bitmap image size:       " << bitmap_header_.size_image);
  assert(bitmap_header_.size_image*header_.image_count <= file_size);
  int bit_depth = (bitmap_header_.size_image * 8) / (bitmap_header_.width * bitmap_header_.height);
  DEBUG_MSG("HyperCine::read_header(): bitmap actual bit count: " << bit_depth);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.x_pixels_per_meter), sizeof(bitmap_header_.x_pixels_per_meter));
  DEBUG_MSG("HyperCine::read_header(): bitmap x pels/meter:     " << bitmap_header_.x_pixels_per_meter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.y_pixels_per_meter), sizeof(bitmap_header_.y_pixels_per_meter));
  DEBUG_MSG("HyperCine::read_header(): bitmap y pels/meter:     " << bitmap_header_.y_pixels_per_meter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.clr_used), sizeof(bitmap_header_.clr_used));
  DEBUG_MSG("HyperCine::read_header(): bitmap colors used:      " << bitmap_header_.clr_used);
  assert(bitmap_header_.clr_used==0);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.clr_important), sizeof(bitmap_header_.clr_important));
  DEBUG_MSG("HyperCine::read_header(): important colors:        " << bitmap_header_.clr_important);

  if(bit_depth==8) bitmap_header_.bit_depth=BIT_DEPTH_8;
  else if (bit_depth==16) bitmap_header_.bit_depth=BIT_DEPTH_16;
  else if (bit_depth==10) bitmap_header_.bit_depth=BIT_DEPTH_10_PACKED;
  else{
    std::cerr << "invalid bit depth " << bit_depth << std::endl;
    throw std::invalid_argument("invalid bit depth");
  }

  // read the image offsets:
  cine_file.seekg(header_.off_image_offsets);
  assert(image_offsets_.size()==header_.image_count);
  for (size_t i=0;i<header_.image_count;++i){
    int64_t offset;
    cine_file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
    image_offsets_[i] = offset;
  }
  // close the file:
  cine_file.close();

  // compute the offset to the end of the cine file header
  const int64_t begin = image_offsets_[0];
  if(header_.image_count<=1)
    throw std::invalid_argument("Error, cine must have at least two images");
  const int64_t end = image_offsets_[1];
  long long int total_storage_for_one_frame = end - begin;
  if(total_storage_for_one_frame<=0)
    throw std::invalid_argument("Error, invalid buffer size");
  // the size of the image header is the total image storage minus the actual bitmap storage
  header_.header_offset = (total_storage_for_one_frame - bitmap_header_.size_image) / sizeof(uint8_t);
}

void
HyperCine::read_buffer(HyperFrame & hf){
  hf_ = hf; // store a copy of the
  // check that the bounds on the hyperframe are valid
  if(hf_.num_frames()<=0)
    throw std::invalid_argument("invalid HyperFrame (no frames)");
  for(std::set<int>::const_iterator set_it = hf_.frame_ids_view()->begin();set_it!=hf_.frame_ids_view()->end();++set_it){
    if(*set_it-header_.first_image_no<0||*set_it>header_.first_image_no + header_.image_count){
      std::cerr << "inavlid frame requested: " << *set_it << std::endl;
      std::cerr << "first frame no: " << header_.first_image_no << std::endl;
      std::cerr << "last frame no: " << header_.first_image_no + header_.image_count << std::endl;
      throw std::invalid_argument("invalid HyperFrame (includes inavlid frame)");
    }
  }
  // check if the HyperFrame window data is empty, if so create default bounds of the entire image
  if(hf_.num_windows()==0)
    hf_.add_window(0,bitmap_header_.width,0,bitmap_header_.height);
  for(size_t i=0;i<hf_.num_windows();++i){
    if(hf_.window_x_begin(i) < 0 || hf_.window_x_begin(i) + hf_.window_width(i) > bitmap_header_.width)
      throw std::invalid_argument("invalid HyperFrame window (window x_begin < 0 or window x_begin + window_width > image width)");
    if(hf_.window_y_begin(i) < 0 || hf_.window_y_begin(i) + hf_.window_height(i) > bitmap_header_.height)
      throw std::invalid_argument("invalid HyperFrame window (window y_begin < 0 or window y_begin + window_height > image height)");
  }
  // clear the buffer
  data_.clear();
  data_indices_.clear();

  // setup the storages
  for(std::set<int>::const_iterator set_it = hf_.frame_ids_view()->begin();set_it!=hf_.frame_ids_view()->end();++set_it){
    data_indices_.insert(std::pair<int,std::vector<size_t> >(*set_it,std::vector<size_t>(hf_.num_windows(),0)));
  }
  DEBUG_MSG("HyperCine::read_buffer(): total num pixels per frame " << hf_.num_pixels_per_frame());
  if(bitmap_header_.bit_depth==BIT_DEPTH_8){
    throw std::invalid_argument("bit depth not implemented yet");
  }
  else if (bitmap_header_.bit_depth==BIT_DEPTH_16){
    throw std::invalid_argument("bit depth not implemented yet");
  }
  else if (bitmap_header_.bit_depth==BIT_DEPTH_10_PACKED){
    // need one bytes per pixel per frame, data array is sized in one byte (char) increments
    data_.resize(hf_.num_pixels_per_frame()*hf_.num_frames());
    DEBUG_MSG("HyperCine::read_buffer(): data storage size " << data_.size());
    read_hyperframe_10_bit_packed();
  }
  else{
    throw std::invalid_argument("invalid bit depth");
  }
}

void
HyperCine::read_hyperframe_10_bit_packed(){
  DEBUG_MSG("HyperCine::read_hyperframe_10_bit_packed():");

  // chunks of memory will be read into a buffer on region of intertest and one frame at a time
  // current strategy is to read one row of one frame region, but only the width of the frame region for each read
  // and seekg between each row of the frame region to skip the rest of the row

  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cerr << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }
  // the buffer needs to be sized as big as the largest row among the frame regions in the hyperframe
  const int frame_region_row_buffer_size = ceil(((hf_.buffer_row_size()+1)*10)/8); // 10 bits per pixel divided by 8 to get bytes, +1 to oversize
  assert(((hf_.buffer_row_size()+1)*10)/8<=frame_region_row_buffer_size);
  std::vector<char> frame_region_row_buffer(frame_region_row_buffer_size);
  DEBUG_MSG("HyperCine::read_buffer(): frame_region row buffer storage size " << frame_region_row_buffer.size());
  // position to the first frame in this set:
  uint8_t * frame_region_row_buff_ptr = reinterpret_cast<uint8_t*>(&frame_region_row_buffer[0]);
  uint16_t intensity_16 = 0.0;
  uint16_t intensity_16p1 = 0.0;
  uint16_t two_byte = 0;
  size_t total_px_read = 0;
  // iterate the frames (the frame number needs to be offset with first_image_no because hf.frame is the global frame id,
  // which is not necessarily 0-based
//  const size_t frame_begin_index = hf.frame_begin - header_.first_image_no;
  for(std::set<int>::const_iterator set_it = hf_.frame_ids_view()->begin();set_it!=hf_.frame_ids_view()->end();++set_it){
    const size_t frame = *set_it - header_.first_image_no;
//  for(size_t frame=frame_begin_index;frame<frame_begin_index+hf.frame_count;++frame){
    const int64_t frame_offset = image_offsets_[frame] + header_.header_offset;
    // iterate the regions of interest
    for(size_t window=0; window<hf_.num_windows();++window){
      const size_t window_width = hf_.window_width(window);
      const size_t window_x_begin = hf_.window_x_begin(window);
      const int xbegin_mod_4 = hf_.window_x_begin(window)%4;
      data_indices_.find(*set_it)->second[window] = total_px_read;
      // iterate the frame_region rows
      for(size_t row=hf_.window_y_begin(window);row<hf_.window_y_begin(window)+hf_.window_height(window);++row){
        const size_t row_inc = row*bitmap_header_.width;
        // determine the position of the first pixel in the this row of this frame region
        const int64_t begin_frame_region_row = frame_offset + ((row_inc + window_x_begin)*10)/8; // convert the number of pixels up to this point to number of bytes to hold 10bit pixel values
        cine_file.seekg(begin_frame_region_row);
        cine_file.read(&frame_region_row_buffer[0],frame_region_row_buffer_size);
        // unpack the 10 bit image data from the array
        for(size_t px=0;px<window_width;++px){
          // and now to index into the strange beast that is 10bit packed cine files...
          // this looks strange because the ten bits are shared across bytes, the first byte has 1 10bit number, with 2 bits spilling into the
          // next byte. The second byte has 2 leftover bits from the first 10bit number, 6 bits from the current 10bit value and spills 4 into the next byte
          // the third byte has 4 bits from the third 10bit value, 4 bits of its own 10bit value and spills 6 bits into the next byte
          // the fourth byte has 6 bits from the last 10bit value, 2 bits of its own, and spills 8 into the next byte, filling that byte
          // so in total it takes 5 bytes for every 4 10bit values
          const size_t col_inc = px+window_x_begin;
          const size_t slot = (px+xbegin_mod_4)*10/8 - xbegin_mod_4;
          const size_t chunk_offset = col_inc%4; // 5 bytes per four pixels
          // create the single 16 bit combo
          intensity_16p1 = frame_region_row_buff_ptr[slot+1];
          intensity_16p1 <<= 8; // move the bits over to the beginning of the byte
          intensity_16 = frame_region_row_buff_ptr[slot];
          two_byte = intensity_16 | intensity_16p1;
          endian_swap(two_byte);
          // shift the 10 bits to the right side of the 16 bit data type;
          two_byte = two_byte >> (6 - (chunk_offset*2));
          // use a mask to zero out the left 6 bits
          two_byte = two_byte & 0x3FF; // 16 bits with only the right 10 active;
          // this next step is required because the original signal was companded from 12 bits to 10,
          // now we are compressing it back to 8 bit:
          data_[total_px_read++] = Lin8UT[two_byte];
        } // end pixel iterator
      } // end frame_region row iterator
    } // end frame_region iterator
  } // end frame iterator
  cine_file.close();
}

/// overload the ostream operator
std::ostream& operator<<(std::ostream& os,
  const HyperCine & hc){
  os << "HyperCine: num frames in data: " << hc.hf_.num_frames() << std::endl;
  os << "HyperCine: num windows per frame: " << hc.hf_.num_windows() << std::endl;
  os << "HyperCine: " << std::left << std::setw(12) << "frame"
      << std::left << std::setw(12) << "window"
      << std::left << std::setw(12) << "data_index"
      << std::left << std::setw(12) << "width"
      << std::left << std::setw(12) << "height" << std::endl;
  for(std::set<int>::const_iterator set_it = hc.hf_.frame_ids_view()->begin();set_it!=hc.hf_.frame_ids_view()->end();++set_it){
    for(size_t window=0;window<hc.hf_.num_windows();++window){
      os << "HyperCine: "  << std::left << std::setw(12) << *set_it
          << std::left << std::setw(12) << window
          << std::left << std::setw(12) << hc.data_indices_.find(*set_it)->second[window]
          << std::left << std::setw(12) << hc.hf_.window_width(window)
          << std::left << std::setw(12) << hc.hf_.window_height(window) << std::endl;
    }
  }
  return os;
}

} // end namespace hypercine
