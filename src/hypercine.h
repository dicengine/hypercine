#ifndef HYPERCINE_H
#define HYPERCINE_H
#if defined(WIN32)
#  if defined(HYPERCINE_LIB_EXPORTS_MODE)
#    define HYPERCINE_LIB_DLL_EXPORT __declspec(dllexport)
#  else
#    define HYPERCINE_LIB_DLL_EXPORT __declspec(dllimport)
#  endif
#else
#  define HYPERCINE_LIB_DLL_EXPORT
#endif
// debugging macros:
#ifdef HYPERCINE_DEBUG_MSG
#  define DEBUG_MSG(x) do { std::cout << "[DEBUG]: " << x << std::endl; } while (0)
#else
#  define DEBUG_MSG(x) do {} while (0)
#endif

#if defined(WIN32)
  #include <cstdint>
#else
  #include <stdint.h>
#endif
#include <string>
#include <vector>

namespace hypercine {

/// Fractions
typedef uint32_t FRACTIONS;
/// Pointer to fractions
typedef uint32_t *PFRACTIONS;
/// Structure to hold the trigger time
typedef struct tagTIME64
{
  /// fractions
  FRACTIONS fractions;
  /// seconds
  uint32_t seconds;
} TIME64;

/// Structure to hold the cine file header information
struct
CineFileHeader{
  /// Type
  uint16_t type;
  /// Header size
  uint16_t header_size;
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

/// Structure to hold the image information
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
  /// bitmap color important
  uint32_t clr_important;
};

/// Bit depth enumeration
enum Bit_Depth{
  BIT_DEPTH_8=0,
  BIT_DEPTH_16,
  BIT_DEPTH_10_PACKED,
  NO_SUCH_BIT_DEPTH
};

class HYPERCINE_LIB_DLL_EXPORT
HyperCine{
public:
  /// constructor
  HyperCine(const char * file_name);
  /// destructor
  ~HyperCine(){}; 

  /// method to read the cine header
  void read_header(const char * file_name);

private:
  /// struct to hold information such as the indexing into each frame of the video
  CineFileHeader header_;
  /// struct to hold information about each frame itself
  BitmapHeader bitmap_header_;
  /// pointer to the array with the offsets to each image
  std::vector<int64_t> image_offsets_;
  /// the name of the cine file
  std::string file_name_;
  /// bit depth of the file
  Bit_Depth bit_depth_;
  /// buffer for storing date read from cine file in contiguous block
  std::vector<char> data_;
};

} // end namespace hypercine

#endif
