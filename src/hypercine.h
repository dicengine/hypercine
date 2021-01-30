// @HEADER
// ************************************************************************
//
//               Hypercine is part of the DICe code base
//               Digital Image Correlation Engine (DICe)
//                 Copyright 2015 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY NTESS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NTESS OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact: Dan Turner (dzturne@sandia.gov)
//
// ************************************************************************
// @HEADER

// In a nutshell, the hypercine library is used to read cine files in large chunks, which can
// include several frames and does not necessarily include the entire image for each frame. The
// main purpose for this is to provide performance improvements by putting large portions of
// a cine file in local memory without having to go to disk for each frame. Another purpose is
// that many applications only require a portion of the images stored in a cine file. Having
// flexibility to read portions of the image can greatly reduce memory requirements for cines with
// lots of frames.
//
// The main object in this library is a HyperCine object. This class stores all the header information
// from the cine file (for example the offsets to all the individual images, the bit-depth, etc. This
// class is intended to be instantiated once at the beginning of reading from a cine file to avoid
// having to re-populate this information every time frame is read.
//
// Another class in this library is the HyperFrame class. This class defines the frames and windows
// within an image that the user would like to read. If no window is defined, the entire image is read
// for each frame.
//
// The actual reading of image data from a cine file is executed by a call to HyerCine.read_buffer(hf),
// which takes a HyperFrame as an argument to define which parts of the cine file to load into the
// memory buffer. ***The memory buffer for a HyperCine object is always uint16_t, regardless of the
// bit-depth.*** This is enable whatever application is using hypercine to pass the pointer around
// for the image storage without having to cast between types for 8-bit or 16-bit images. For 8-bit
// cines (or 10-bit packed cines that get converted to 8-bit values), this leads to an empty 8bits for
// each storage array value.
//
// Accessing the intensity value storage is done through a call to HyperCine.data(), which returns a
// uint16_t pointer to the storage array, which is always contiguous in memory.
//
// For the most basic file read operation, the psuedo-code would look something like this:
// HyperCine hc(image_file_name); - initialize the cine header info for the specified file
// HyperFrame hf(frame_begin,frame_count); - define the subset of the cine file that the user would like to read
// HyperCine.read_buffer(hf); - load this portion of the image data into memory
// uint16_t * data_ptr = HyperCine.data(); - obtain a pointer to the data to use in the application


#ifndef HYPERCINE_H
#define HYPERCINE_H
#if defined(WIN32)
// disable some common windows compiler warnings that we don't really care about
#  pragma warning(disable:4018)
#  pragma warning(disable:4005)
#  pragma warning(disable:4091)
#  pragma warning(disable:4244)
#  pragma warning(disable:4251)
#  pragma warning(disable:4267)
#  pragma warning(disable:4297)
#  pragma warning(disable:4305)
#  pragma warning(disable:4800)
#  pragma warning(disable:4996)
#  if defined(HYPERCINE_LIB_EXPORTS_MODE)
#    define HYPERCINE_LIB_DLL_EXPORT __declspec(dllexport)
#  else
#    define HYPERCINE_LIB_DLL_EXPORT __declspec(dllimport)
#  endif
#else
#  define HYPERCINE_LIB_DLL_EXPORT
#endif
// debugging macros:
#ifndef DEBUG_MSG
#ifdef HYPERCINE_DEBUG_MSG
#  define DEBUG_MSG(x) do { std::cout << "[DEBUG]: " << x << std::endl; } while (0)
#else
#  define DEBUG_MSG(x) do {} while (0)
#endif
#endif
// buffer output macros (for interfacing with javascript):
#define BUFFER_MSG(var,val) do { std::cout << "[--BUFFER_OUT--]: " << var <<  " " << val << std::endl; } while (0)

# define ASSERT_EXPR(test,count){if(test){}else{std::cout << "[-- failed assertion --](" << __FILE__ <<  " line "<< __LINE__ <<"): " <<  #test << std::endl;count++;}}
# define MAX_WRITE_FRAMES 20000
# define HEADER_SIZE 44  // 36 + sizeof(TIME64)
# define BITMAP_HEADER_SIZE 40
# define IMAGE_WIDTH_OFFSET 48  // 36 + sizeof(TIME64) + 4
# define IMAGE_COUNT_OFFSET 20  // 4 uint16_t and 3 uint32_t


#if defined(WIN32)
  #include <cstdint>
#else
  #include <stdint.h>
#endif
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>

namespace hypercine {

class HYPERCINE_LIB_DLL_EXPORT
HyperCine{
public:

  /// \brief function to swap the endianness of a 16bit data type
  /// \param x the variable to endian swap
  static void endian_swap(uint16_t& x){
    x = (x>>8) | (x<<8);
  }

  /// Fractions
  typedef uint32_t FRACTIONS;
  /// Pointer to fractions
  typedef uint32_t *PFRACTIONS;

  /// Struct to hold the trigger time
  typedef struct tagTIME64
  {
    /// fractions
    FRACTIONS fractions;
    /// seconds
    uint32_t seconds;
  } TIME64;

  /// Struct to hold the cine file header information
  struct
  CineFileHeader{
    /// Type
    uint16_t type;
    /// Header size
    uint16_t header_size;
    /// Header offset
    uint64_t header_offset;
    /// Compression
    uint16_t compression;
    /// Version
    uint16_t version;
    /// First movie image
    int32_t first_movie_image;
    /// Total image count
    uint32_t total_image_count;
    /// First image index
    int32_t first_image_no;
    /// Image count
    uint32_t image_count;
    /// Offset to header
    uint32_t off_image_header;
    /// Offset to setup
    uint32_t off_setup;
    /// Offset to image offsets
    uint32_t off_image_offsets;
    /// trigger time
    TIME64 trigger_time;
  };

  /// Bit depth enumeration
  enum Bit_Depth{
    BIT_DEPTH_8=0,
    BIT_DEPTH_16,
    BIT_DEPTH_10_PACKED,
    NO_SUCH_BIT_DEPTH
  };

  /// Conversion hash enum
  enum Conversion_Type_10_Bit{
    NO_CONVERSION=0,
    LINEAR_10_TO_8,
    QUAD_10_TO_8,
    QUAD_10_TO_12
  };

  /// Struct to hold the image information
  struct BitmapHeader{
    /// bitmap size
    uint32_t size;
    /// bitmap width
    int32_t width;
    /// bitmap height
    int32_t height;
    /// bitmap planes
    uint16_t planes;
    /// bitmap bit count
    uint16_t bit_count;
    /// bitmap compression
    uint32_t compression;
    /// bitmap image size
    uint32_t size_image;
    /// bitmap x pixels per meter
    int32_t x_pixels_per_meter;
    /// bitmap y pixels per meter
    int32_t y_pixels_per_meter;
    /// bitmap color used
    uint32_t clr_used;
    /// max pixel intensity value
    uint32_t clr_important;
    /// bit depth of the file
    Bit_Depth bit_depth;
  };

  /// struct to hold the image bounds requested during a file read
  struct HYPERCINE_LIB_DLL_EXPORT
  HyperFrame{
    // constructor
    HyperFrame(){};
    // constructor with frame id
    HyperFrame(const int frame, const int count=1){
      add_frames(frame,count);
    };
    // constructor with set of frame ids
    HyperFrame(const std::set<int> & frame_ids){
      update_frames(frame_ids);
    };
    // copy constructor
    HyperFrame(const HyperFrame & hf){
      frame_ids_ = hf.get_frame_ids();
      for(size_t i=0;i<hf.num_windows();++i){
        x_begin_.push_back(hf.window_x_begin(i));
        y_begin_.push_back(hf.window_y_begin(i));
        x_count_.push_back(hf.window_width(i));
        y_count_.push_back(hf.window_height(i));
      }
    }
    // reset the hyperframe parameters
    void clear(){
      frame_ids_.clear();
      x_begin_.clear();
      y_begin_.clear();
      x_count_.clear();
      y_count_.clear();
    }
    int window_id(const size_t x_begin,
      const size_t x_count,
      const size_t y_begin,
      const size_t y_count)const{
      if(num_frames()==0||num_windows()==0){
        DEBUG_MSG("HyperFrame::window_id(): no frames or windows in HyperFrame");
        return -1;
      }
      for(int window=0; window<(int)num_windows();++window){
        if(x_count == x_count_[window] &&
            y_count == y_count_[window] &&
            x_begin == x_begin_[window] &&
            y_begin == y_begin_[window]) return window;
      }
      return -1;
    }
    // add a range of frames to the hyperframe
    void add_frames(const int frame_begin, const int count=1){
      for(int i=frame_begin;i<frame_begin+count;++i)
        frame_ids_.insert(i);
    }
    // update a range of frames to the hyperframe
    void update_frames(const int frame_begin, const int count=1){
      frame_ids_.clear();
      add_frames(frame_begin,count);
    }
    // add a set of frame ids to the hyperframe
    void update_frames(const std::set<int> & frame_ids){
      frame_ids_ = frame_ids;
    }
    // return a deep copy of the frame id set
    std::set<int> get_frame_ids()const{
      return frame_ids_;
    }
    // return a pointer to the frame ids
    std::set<int> const * frame_ids()const{
      return & frame_ids_;
    }
    // returns true if the frame exists in the HyperFrame
    bool has_frame(const int frame)const{
      return frame_ids_.find(frame)!=frame_ids_.end();
    }
    // return the number of frames
    size_t num_frames()const{
      return frame_ids_.size();
    }
    // add a region of interest to the hyperframe
    void add_window(const size_t x_b, const size_t x_c, const size_t y_b, const size_t y_c){
      x_begin_.push_back(x_b);
      y_begin_.push_back(y_b);
      x_count_.push_back(x_c);
      y_count_.push_back(y_c);
    }
    // returns the number of regions of interst
    size_t num_windows()const{return x_begin_.size();}
    // return the total number of pixels per frame (including all windows in the frame)
    size_t num_pixels_per_frame()const{
      size_t num_pixels_pf = 0;
      for(size_t i=0;i<num_windows();++i)
        num_pixels_pf += x_count_[i]*y_count_[i];
      return num_pixels_pf;
    }
    // return the number of pixels in the selected window
    size_t num_pixels_per_window(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return x_count_[window_id]*y_count_[window_id];
    }
    // return the width of the selected window
    size_t window_width(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return x_count_[window_id];
    }
    // return the height of the selected window
    size_t window_height(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return y_count_[window_id];
    }
    // return the x begin for the selected window
    size_t window_x_begin(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return x_begin_[window_id];
    }
    // return the x begin for the selected window
    size_t window_y_begin(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return y_begin_[window_id];
    }
    // returns the number of pixels required to store an entire row for the widest window
    size_t buffer_row_size()const{
      size_t max_num_pixels = 0;
      for(size_t i=0;i<num_windows();++i)
        if(window_width(i)>max_num_pixels)
          max_num_pixels = window_width(i);
      return max_num_pixels;
    }
  private:
    // storage of frame ids
    std::set<int> frame_ids_;
    // vector x coordinates of upper left corners for the frame windows or regions of interest
    std::vector<size_t> x_begin_;
    // vector of widths of the frame windows or regions of interest
    std::vector<size_t> x_count_;
    // vector of y coordinats of upper left corners for the frame windows or regions of interest
    std::vector<size_t> y_begin_;
    // vector of heights of the frame windows or regions of interest
    std::vector<size_t> y_count_;
  };

  /// constructor
  /// \param file_name name of the file to read
  HyperCine(const char * file_name, Conversion_Type_10_Bit type=QUAD_10_TO_12);
  /// destructor
  ~HyperCine(){};

  /// method to read the cine file into the storage buffer
  /// \param hf the HyperFrame that defines what regions of interest to read and for what frame range
  /// This method is used to manually populate the memory buffer in large chunks for
  /// a cine file. Alternatively, the user can simply call data(frame, window_id) or data(frame, window_dims)
  /// which will automatically call read_buffer to populate the reqeusted frame information.
  void read_buffer(HyperFrame & hf);

  /// method to read the cine file into the storage buffer
  /// convenience method for reading the buffer when the member data hf_ has been
  /// modified
  /// See note above about when to manually call read_buffer() rather than have it implicitly called
  /// when data(frame,window_id) or data(frame,window_dims) is called.
  void read_buffer();

  /// return pointer to the cine file header
  CineFileHeader * header(){return &header_;}

  /// return pointer to the bitmap header
  BitmapHeader * bitmap_header(){return &bitmap_header_;}

  /// return a pointer to the image offsets
  std::vector<int64_t> * image_offsets(){return &image_offsets_;}

  /// return the filename
  std::string file_name()const{return file_name_;}

  /// return the bit depth (this is an enum)
  Bit_Depth bit_depth() const{return bitmap_header_.bit_depth;}

  /// return the bit count (either 8 or 16, BIT_DEPTH_8=8, BIT_DEPTH_!0_PACKED=8, BIT_DEPTH_16=16)
  int bit_count() const{return bitmap_header_.bit_count;}

  /// return the range of intensity values for this cine file (not the max value but the max possible value)
  int max_possible_intensity()const {return bitmap_header_.clr_important - 1;}

  /// return a copy of the current hyperframe
  HyperFrame * hyperframe(){return & hf_;}

  /// return the image width, or the width of a window if specified
  int width(const int window_id=-1)const{
     if(window_id==-1) return bitmap_header_.width;
     else return hf_.window_width(window_id);
  };

  /// return the image height, or the height of a window if specified
  int height(const int window_id=-1)const{
    if(window_id==-1) return bitmap_header_.height;
    else return hf_.window_height(window_id);
  };

  /// return the first frame id
  int file_first_frame_id()const{
    return header_.first_image_no;
  }

  /// return the first frame id
  int file_frame_count()const{
    return header_.image_count;
  }

  /// returns true if the frame has been loaded into the data buffer
  bool buffer_has_frame(const int frame) const;

  /// returns true if the window id has been loaded into the data buffer
  bool buffer_has_window(const size_t window_id) const;

  /// return a pointer to the raw data for a given frame, and window
  /// if the frame is valid, but out of range of what is loaded in the buffer
  /// this call will automatically load the requested frame into the buffer
  /// after releasing it's existing contents. If the window_id is not in the
  /// buffer an error will be thrown.
  uint16_t * data(const int frame, const size_t window_id=0);

  /// return a pointer to the raw data for a given frame, and window extents
  /// if the data for the requested frame is not loaded
  /// into the memory buffer, but the window dimensions match a window that is
  /// loaded, the requested frame will be loaded, keeping all
  /// of the original windows. The reslults in a single frame for all the windows
  /// in the memory buffer. If the requested window dimensions do no match an
  /// existing set, all existing window dimensions are replaced with a single
  /// window with the requested dimensions. If the frame is out of range of the
  /// cine file or the window dimensions not valid, an error is thrown.
  uint16_t * data(const int frame,
    const size_t x_begin,
    const size_t x_count,
    const size_t y_begin,
    const size_t y_count);

  /// convenience method to read a full frame, return it as a copied vector
  /// of intensity values and not interfere with the memory buffer data_ if
  /// it has already been populated with data
  std::vector<uint16_t> get_frame(const int frame);

  /// convenience method to read an average of full frames in a particular range,
  /// return the data as a copied vector
  /// of intensity values and not interfere with the memory buffer data_ if
  /// it has already been populated with data
  std::vector<uint16_t> get_avg_frame(const int frame_begin, const int frame_end);

  /// return the type of conversion that was used for 10bit packed
  Conversion_Type_10_Bit conversion_type()const{
    return conversion_type_;
  }

  /// return the conversion factor needed to convert the intensity values in the memory
  /// buffer to 8 bit
  float conversion_factor_to_8_bit()const{
    return 255.0f/(bitmap_header_.clr_important-1.0f);
  }

  /// return the conversion factor needed to convert the intensity values in the memory
  /// buffer to 16 bit
  float conversion_factor_to_16_bit()const{
    return 65535.0f/(bitmap_header_.clr_important-1.0f);
  }

  // write a frame to cine file
  // NOTE: only implemented for 8-bit array values currently
  static void write_frame(const char * file_name, const size_t width,
    const size_t height, uint16_t * data, const bool overwrite=false);

  // write a cine file header
  static void write_header(const char * file_name, const size_t width,
    const size_t height, uint16_t bit_count=16);

  /// send hc to ostreams like cout
  friend HYPERCINE_LIB_DLL_EXPORT std::ostream& operator<<(std::ostream& os, const HyperCine & hc);

private:
  /// method to read the cine header
  void read_header(const char * file_name);

  /// method to read a hyperframe into the buffer for a 10bit packed cine file
  void read_hyperframe_10_bit_packed();

  /// method to read a full frame into a vector
  /// This method doesn't rely on the information in the hf_ HyperFrame stored in this class.
  /// It also doesn't interfere with the memory buffer data_ of the class instance
  /// so it can be called without clearing the stored memory buffer
  /// It's used to get a copy of a single frame while the buffer may already be populated with
  /// data from what's defined in hf_
  void read_hyperframe_10_bit_packed_full(const int frame, std::vector<uint16_t> & data);

  /// method to read a hyperframe into the buffer for an 8bit cine file
  void read_hyperframe_8_bit();

  /// method to read a full frame into a vector
  /// This method doesn't rely on the information in the hf_ HyperFrame stored in this class.
  /// It also doesn't interfere with the memory buffer data_ of the class instance
  /// so it can be called without clearing the stored memory buffer
  /// It's used to get a copy of a single frame while the buffer may already be populated with
  /// data from what's defined in hf_
  void read_hyperframe_8_bit_full(const int frame, std::vector<uint16_t> & data);

  /// method to read a hyperframe into the buffer for an 8bit cine file
  void read_hyperframe_16_bit();

  /// method to read a full frame into a vector
  /// This method doesn't rely on the information in the hf_ HyperFrame stored in this class.
  /// It also doesn't interfere with the memory buffer data_ of the class instance
  /// so it can be called without clearing the stored memory buffer
  /// It's used to get a copy of a single frame while the buffer may already be populated with
  /// data from what's defined in hf_
  void read_hyperframe_16_bit_full(const int frame, std::vector<uint16_t> & data);

  /// struct to hold information such as the indexing into each frame of the video
  CineFileHeader header_;
  /// struct to hold information about each frame itself
  BitmapHeader bitmap_header_;
  /// pointer to the array with the offsets to each image
  std::vector<int64_t> image_offsets_;
  /// the name of the cine file
  std::string file_name_;
  /// buffer for storing date read from cine file in contiguous block (all frames, all windows together)
  std::vector<uint16_t> data_;
  /// storage for the index of the first pixel of a frame window for each frame, one vector of indices per frame
  /// (access is [frame][window] to get the index of the first pixel's value in the data_ storage)
  std::map<int,std::vector<size_t> > data_indices_;
  /// store the HyperFrame that defines the windows and frames
  HyperFrame hf_;
  /// type of conversion from 10 bit to 8 or 12 bit
  Conversion_Type_10_Bit conversion_type_;
  /// pointer to the hash table for converting 10 bit values to 8 or 12 bit
  const uint16_t * hash_ptr_;
};


} // end namespace hypercine

#endif
