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
  uint16_t Type;
  /// Header size
  uint16_t Headersize;
  /// Compression
  uint16_t Compression;
  /// Version
  uint16_t Version;
  /// First movie image
  int32_t FirstMovieImage;
  /// Total image count
  uint32_t TotalImageCount;
  /// First image index
  int32_t FirstImageNo;
  /// Image count
  uint32_t ImageCount;
  /// Offset to header
  uint32_t OffImageHeader;
  /// Offset to setup
  uint32_t OffSetup;
  /// Offset to image offsets
  uint32_t OffImageOffsets;
  /// trigger time
  TIME64 TriggerTime;
};

/// Structure to hold the image information
struct BitmapHeader{
  /// bitmap size
  uint32_t biSize;
  /// bitmap width
  int32_t biWidth;
  /// bitmap height
  int32_t biHeight;
  /// bitmap planes
  uint16_t biPlanes;
  /// bitmap bit count
  uint16_t biBitCount;
  /// bitmap compression
  uint32_t biCompression;
  /// bitmap image size
  uint32_t biSizeImage;
  /// bitmap x pixels per meter
  int32_t biXPelsPerMeter;
  /// bitmap y pixels per meter
  int32_t biYPelsPerMeter;
  /// bitmap color used
  uint32_t biClrUsed;
  /// bitmap color important
  uint32_t biClrImportant;
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
  CineFileHeader header_;
  BitmapHeader bitmap_header_;
  /// pointer to the array with the offsets to each image
  std::vector<int64_t> image_offsets_;
  /// the name of the cine file
  std::string file_name_;
  /// bit depth of the file
  Bit_Depth bit_depth_;
};

} // end namespace hypercine

#endif
