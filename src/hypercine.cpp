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
#include "hypercine.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <stdexcept>

namespace hypercine {

const static uint16_t no_op[1024] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
  33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,
  74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
  112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
  176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
  208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
  240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,
  272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,
  304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,
  336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,
  368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,
  400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,
  432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,463,
  464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,
  496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,
  528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,
  560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,
  592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,
  624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,
  656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,
  688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,
  720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,
  752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,
  784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,
  816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,
  848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,
  880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,
  912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,940,941,942,943,
  944,945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,963,964,965,966,967,968,969,970,971,972,973,974,975,
  976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999,1000,1001,1002,1003,1004,1005,1006,
  1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023};

// hash table to convert 12bit integer values from the 10-bit packed format to 8 bit image intensity values
const static uint16_t lin_10bit_to_8bit[1024] =
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

// converts 10 bit packed values to 12 bit using the same quad curve as in the Vision Research hash below, but then to 8 bit values via a linear conversion
// 8 bit = vision_research_12bit(10bit_packed)*255/4095;
const static uint16_t quad_10bit_to_8bit[1024] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,
  13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,17,17,17,
  17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,22,
  22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,25,25,25,25,25,25,25,26,26,26,26,26,26,26,27,27,27,27,27,27,
  27,28,28,28,28,28,28,28,29,29,29,29,29,29,29,30,30,30,30,30,30,31,31,31,31,31,31,32,32,32,32,32,32,32,33,33,33,33,33,33,34,34,
  34,34,34,34,35,35,35,35,35,35,36,36,36,36,36,36,37,37,37,37,37,38,38,38,38,38,38,39,39,39,39,39,40,40,40,40,40,40,41,41,41,41,
  41,42,42,42,42,42,43,43,43,43,43,44,44,44,44,44,44,45,45,45,45,45,46,46,46,46,46,47,47,47,47,48,48,48,48,48,49,49,49,49,49,50,
  50,50,50,50,51,51,51,51,51,52,52,52,52,53,53,53,53,53,54,54,54,54,54,55,55,55,55,56,56,56,56,57,57,57,57,57,58,58,58,58,59,59,
  59,59,60,60,60,60,60,61,61,61,61,62,62,62,62,63,63,63,63,64,64,64,64,65,65,65,65,66,66,66,66,67,67,67,67,68,68,68,68,68,69,69,
  69,69,70,70,70,71,71,71,71,72,72,72,72,73,73,73,73,74,74,74,74,75,75,75,76,76,76,76,77,77,77,77,78,78,78,79,79,79,79,80,80,80,
  80,81,81,81,82,82,82,82,83,83,83,84,84,84,84,85,85,85,86,86,86,86,87,87,87,88,88,88,88,89,89,89,90,90,90,91,91,91,91,92,92,92,
  93,93,93,94,94,94,94,95,95,95,96,96,96,97,97,97,98,98,98,98,99,99,99,100,100,100,101,101,101,102,102,102,103,103,103,103,104,
  104,104,105,105,105,106,106,106,107,107,107,108,108,108,109,109,109,110,110,110,111,111,111,112,112,112,113,113,113,114,114,114,
  115,115,115,116,116,116,117,117,117,118,118,119,119,119,119,120,120,121,121,121,122,122,122,123,123,123,124,124,124,125,125,126,
  126,126,127,127,127,128,128,128,129,129,129,130,130,131,131,131,132,132,132,133,133,134,134,134,135,135,135,136,136,136,137,137,
  138,138,138,139,139,140,140,140,141,141,141,142,142,143,143,143,144,144,145,145,145,146,146,146,147,147,148,148,148,149,149,150,
  150,150,151,151,152,152,152,153,153,154,154,154,155,155,156,156,156,157,157,158,158,158,159,159,160,160,161,161,161,162,162,163,
  163,163,164,164,165,165,166,166,166,167,167,168,168,168,169,169,170,170,171,171,171,172,172,173,173,174,174,174,175,175,176,176,
  177,177,177,178,178,179,179,180,180,181,181,181,182,182,183,183,184,184,184,185,185,186,186,187,187,188,188,188,189,189,190,190,
  191,191,192,192,193,193,193,194,194,195,195,196,196,197,197,198,198,199,199,199,200,200,201,201,202,202,203,203,204,204,205,205,
  206,206,206,207,207,208,208,209,209,210,210,211,211,212,212,213,213,214,214,215,215,216,216,217,217,218,218,219,219,220,220,220,
  221,221,222,222,223,223,224,224,225,225,226,226,227,227,228,228,229,229,230,230,231,231,232,232,233,233,234,234,235,235,236,237,
  237,238,238,239,239,240,240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,251,251,252,252,253,255,255,255,255,255,255,255,255,255};

// look up table for converting 10bit packed pixels to 12 bit (taken from Appendix 1 of the Vision Research Phantom Cine File Format Spec with permission)
const static uint16_t quad_10bit_to_12bit[1024] =
{2, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,17,18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 33, 34, 35, 36, 37, 38, 39, 40,
 41, 42, 43, 44, 45, 46, 47, 48, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 79, 80,
 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
 121, 122, 123, 124, 125, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 156, 157, 158, 159, 160,
 161, 162, 163, 164, 165, 167, 168, 169, 170, 171, 172, 173, 175, 176, 177, 178, 179, 181, 182, 183, 184, 186, 187, 188, 189, 191, 192, 193, 194, 196, 197, 198, 200, 201, 202, 204, 205, 206, 208, 209, 210,
 212, 213, 215, 216, 217, 219, 220, 222, 223, 225, 226, 227, 229, 230, 232, 233, 235, 236, 238, 239, 241, 242, 244, 245, 247, 249, 250, 252, 253, 255, 257, 258, 260, 261, 263, 265, 266, 268, 270, 271, 273,
 275, 276, 278, 280, 281, 283, 285, 287, 288, 290, 292, 294, 295, 297, 299, 301, 302, 304, 306, 308, 310, 312, 313, 315, 317, 319, 321, 323, 325, 327, 328, 330, 332, 334, 336, 338, 340, 342, 344, 346, 348,
 350, 352, 354, 356, 358, 360, 362, 364, 366, 368, 370, 372, 374, 377, 379, 381, 383, 385, 387, 389, 391, 394, 396, 398, 400, 402, 404, 407, 409, 411, 413, 416, 418, 420, 422, 425, 427, 429, 431, 434, 436,
 438, 441, 443, 445, 448, 450, 452, 455, 457, 459, 462, 464, 467, 469, 472, 474, 476, 479, 481, 484, 486, 489, 491, 494, 496, 499, 501, 504, 506, 509, 511, 514, 517, 519, 522, 524, 527, 529, 532, 535, 537,
 540, 543, 545, 548, 551, 553, 556, 559, 561, 564, 567, 570, 572, 575, 578, 581, 583, 586, 589, 592, 594, 597, 600, 603, 606, 609, 611, 614, 617, 620, 623, 626, 629, 632, 635, 637, 640, 643, 646, 649, 652,
 655, 658, 661, 664, 667, 670, 673, 676, 679, 682, 685, 688, 691, 694, 698, 701, 704, 707, 710, 713, 716, 719, 722, 726, 729, 732, 735, 738, 742, 745, 748, 751, 754, 758, 761, 764, 767, 771, 774, 777, 781,
 784, 787, 790, 794, 797, 800, 804, 807, 811, 814, 817, 821, 824, 828, 831, 834, 838, 841, 845, 848, 852, 855, 859, 862, 866, 869, 873, 876, 880, 883, 887, 890, 894, 898, 901, 905, 908, 912, 916, 919, 923,
 927, 930, 934, 938, 941, 945, 949, 952, 956, 960, 964, 967, 971, 975, 979, 982, 986, 990, 994, 998,1001,1005,1009,1013,1017,1021,1025,1028,1032,1036,1040,1044,1048,1052,1056,1060,1064,1068,1072,1076,1080,
 1084,1088,1092,1096,1100,1104,1108,1112,1116,1120, 1124,1128,1132,1137,1141,1145,1149,1153,1157,1162,1166,1170,1174,1178,1183,1187, 1191,1195,1200,1204,1208,1212,1217,1221,1225,1230,1234,1238,1243,1247,
 1251,1256, 1260,1264,1269,1273,1278,1282,1287,1291,1295,1300,1304,1309,1313,1318,1322,1327, 1331,1336,1340,1345,1350,1354,1359,1363,1368,1372,1377,1382,1386,1391,1396,1400, 1405,1410,1414,1419,1424,1428,
 1433,1438,1443,1447,1452,1457,1462,1466,1471,1476, 1481,1486,1490,1495,1500,1505,1510,1515,1520,1524,1529,1534,1539,1544,1549,1554, 1559,1564,1569,1574,1579,1584,1589,1594,1599,1604,1609,1614,1619,1624,
 1629,1634, 1639,1644,1649,1655,1660,1665,1670,1675,1680,1685,1691,1696,1701,1706,1711,1717, 1722,1727,1732,1738,1743,1748,1753,1759,1764,1769,1775,1780,1785,1791,1796,1801, 1807,1812,1818,1823,1828,1834,
 1839,1845,1850,1856,1861,1867,1872,1878,1883,1889, 1894,1900,1905,1911,1916,1922,1927,1933,1939,1944,1950,1956,1961,1967,1972,1978, 1984,1989,1995,2001,2007,2012,2018,2024,2030,2035,2041,2047,2053,2058,
 2064,2070, 2076,2082,2087,2093,2099,2105,2111,2117,2123,2129,2135,2140,2146,2152,2158,2164, 2170,2176,2182,2188,2194,2200,2206,2212,2218,2224,2231,2237,2243,2249,2255,2261, 2267,2273,2279,2286,2292,2298,
 2304,2310,2317,2323,2329,2335,2341,2348,2354,2360, 2366,2373,2379,2385,2392,2398,2404,2411,2417,2423,2430,2436,2443,2449,2455,2462, 2468,2475,2481,2488,2494,2501,2507,2514,2520,2527,2533,2540,2546,2553,
 2559,2566, 2572,2579,2586,2592,2599,2605,2612,2619,2625,2632,2639,2645,2652,2659,2666,2672, 2679,2686,2693,2699,2706,2713,2720,2726,2733,2740,2747,2754,2761,2767,2774,2781, 2788,2795,2802,2809,2816,2823,
 2830,2837,2844,2850,2857,2864,2871,2878,2885,2893, 2900,2907,2914,2921,2928,2935,2942,2949,2956,2963,2970,2978,2985,2992,2999,3006, 3013,3021,3028,3035,3042,3049,3057,3064,3071,3078,3086,3093,3100,3108,
 3115,3122, 3130,3137,3144,3152,3159,3166,3174,3181,3189,3196,3204,3211,3218,3226,3233,3241, 3248,3256,3263,3271,3278,3286,3294,3301,3309,3316,3324,3331,3339,3347,3354,3362, 3370,3377,3385,3393,3400,3408,
 3416,3423,3431,3439,3447,3454,3462,3470,3478,3486, 3493,3501,3509,3517,3525,3533,3540,3548,3556,3564,3572,3580,3588,3596,3604,3612, 3620,3628,3636,3644,3652,3660,3668,3676,3684,3692,3700,3708,3716,3724,
 3732,3740, 3749,3757,3765,3773,3781,3789,3798,3806,3814,3822,3830,3839,3847,3855,3863,3872, 3880,3888,3897,3905,3913,3922,3930,3938,3947,3955,3963,3972,3980,3989,3997,4006, 4014,4022,4031,4039,4048,4056,
 4064,4095,4095,4095,4095,4095,4095,4095,4095,4095};

HyperCine::HyperFrame::HyperFrame(const int frame, const int count){
  add_frames(frame,count);
}

HyperCine::HyperFrame::HyperFrame(const std::set<int> & frame_ids){
  update_frames(frame_ids);
}

HyperCine::HyperFrame::HyperFrame(const HyperFrame & hf){
  frame_ids_ = hf.get_frame_ids();
  for(size_t i=0;i<hf.num_windows();++i){
    x_begin_.push_back(hf.window_x_begin(i));
    y_begin_.push_back(hf.window_y_begin(i));
    x_count_.push_back(hf.window_width(i));
    y_count_.push_back(hf.window_height(i));
  }
}

void
HyperCine::HyperFrame::clear(){
  frame_ids_.clear();
  x_begin_.clear();
  y_begin_.clear();
  x_count_.clear();
  y_count_.clear();
}

int
HyperCine::HyperFrame::window_id(const size_t x_begin,
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
void
HyperCine::HyperFrame::add_frames(const int frame_begin, const int count){
  for(int i=frame_begin;i<frame_begin+count;++i)
    frame_ids_.insert(i);
}

// update a range of frames to the hyperframe
void
HyperCine::HyperFrame::update_frames(const int frame_begin, const int count){
  frame_ids_.clear();
  add_frames(frame_begin,count);
}

// add a set of frame ids to the hyperframe
void
HyperCine::HyperFrame::update_frames(const std::set<int> & frame_ids){
  frame_ids_ = frame_ids;
}
// return a deep copy of the frame id set
std::set<int>
HyperCine::HyperFrame::get_frame_ids()const{
  return frame_ids_;
}

// return a pointer to the frame ids
std::set<int> const *
HyperCine::HyperFrame::frame_ids()const{
  return & frame_ids_;
}

// returns true if the frame exists in the HyperFrame
bool
HyperCine::HyperFrame::has_frame(const int frame)const{
  return frame_ids_.find(frame)!=frame_ids_.end();
}

// return the number of frames
size_t
HyperCine::HyperFrame::num_frames()const{
  return frame_ids_.size();
}
// add a region of interest to the hyperframe
void
HyperCine::HyperFrame::add_window(const size_t x_b, const size_t x_c, const size_t y_b, const size_t y_c){
  x_begin_.push_back(x_b);
  y_begin_.push_back(y_b);
  x_count_.push_back(x_c);
  y_count_.push_back(y_c);
}
// returns the number of regions of interst
size_t
HyperCine::HyperFrame::num_windows()const{
  return x_begin_.size();
}

// return the total number of pixels per frame (including all windows in the frame)
size_t
HyperCine::HyperFrame::num_pixels_per_frame()const{
  size_t num_pixels_pf = 0;
  for(size_t i=0;i<num_windows();++i)
    num_pixels_pf += x_count_[i]*y_count_[i];
  return num_pixels_pf;
}

// return the number of pixels in the selected window
size_t
HyperCine::HyperFrame::num_pixels_per_window(const size_t window_id)const{
  if(window_id>=num_windows()) return 0;
  return x_count_[window_id]*y_count_[window_id];
}

// return the width of the selected window
size_t
HyperCine::HyperFrame::window_width(const size_t window_id)const{
  if(window_id>=num_windows()) return 0;
  return x_count_[window_id];
}

// return the height of the selected window
size_t
HyperCine::HyperFrame::window_height(const size_t window_id)const{
  if(window_id>=num_windows()) return 0;
  return y_count_[window_id];
}

// return the x begin for the selected window
size_t
HyperCine::HyperFrame::window_x_begin(const size_t window_id)const{
  if(window_id>=num_windows()) return 0;
  return x_begin_[window_id];
}

// return the x begin for the selected window
size_t
HyperCine::HyperFrame::window_y_begin(const size_t window_id)const{
  if(window_id>=num_windows()) return 0;
  return y_begin_[window_id];
}

// returns the number of pixels required to store an entire row for the widest window
size_t
HyperCine::HyperFrame::buffer_row_size()const{
  size_t max_num_pixels = 0;
  for(size_t i=0;i<num_windows();++i)
    if(window_width(i)>max_num_pixels)
      max_num_pixels = window_width(i);
  return max_num_pixels;
}

HyperCine::HyperCine(const char * file_name, Bit_Depth_Conversion_Type type):
  file_name_(file_name),
  conversion_type_(type),
  conversion_factor_16_to_8_(1.0f){
  if(conversion_type_==NO_CONVERSION)
    hash_ptr_ = &no_op[0];
  else if(conversion_type_==LINEAR_10_TO_8)
    hash_ptr_ = &lin_10bit_to_8bit[0];
  else if(conversion_type_==QUAD_10_TO_8 || conversion_type_==TO_8_BIT)
    hash_ptr_ = &quad_10bit_to_8bit[0];
  else if(conversion_type_==QUAD_10_TO_12)
    hash_ptr_ = &quad_10bit_to_12bit[0];
  else
    throw std::invalid_argument("invalid conversion type");
  read_header(file_name);
}

/// check for valid frame (it's loaded into the buffer):
bool
HyperCine::buffer_has_frame(const int frame) const{
  if(!hf_.has_frame(frame)){
    DEBUG_MSG("HyperCine::buffer_has_frame(): frame " << frame << " not found in buffer frames");
    return false;
  }
  return true;
}

/// check for valid window id (it's loaded into the buffer):
bool
HyperCine::buffer_has_window(const size_t window_id) const{
  if(window_id<0||window_id>=hf_.num_windows()){
    DEBUG_MSG("HyperCine::buffer_has_window(): window id not found: " << window_id);
    return false;
  }
  return true;
}

/// return a pointer to the raw data for a given frame and window
storage_t *
HyperCine::data(const int frame, const size_t window_id){
  // first check if no data has been read into the buffer
  if(data_indices_.size()==0){
    // if this is the case, the window_id must be 0
    if (window_id!=0){
      std::cout << "error: HyperCine::data(), cannot specify non-zero window_id for uninitialized buffer" << std::endl;
      throw std::exception();
    }
    DEBUG_MSG("HyperCine::data(frame,window_id): reading buffer since it is uninitialized");
    HyperFrame hf(frame);
    read_buffer(hf);
  }else{ // data exists in the buffer
    // check if the window_id requested exists
    if(!buffer_has_window(window_id)){
      std::cout << "error: HyperCine::data(), invalid window id" << std::endl;
      throw std::exception();
    }
    // the frame requested does not exist
    if(!buffer_has_frame(frame)){
      DEBUG_MSG("HyperCine::data(frame,window_id): buffer is initialized, "
        "but reading buffer again since the requested frame has not been read");
      std::set<int> frames;
      frames.insert(frame);
      hf_.update_frames(frames);
      read_buffer();
    }
  }
  return &data_[data_indices_.find(frame)->second[window_id]];
}

/// return a pointer to the raw data for a given frame and window
storage_t *
HyperCine::data(const int frame,
  const size_t x_begin,
  const size_t x_count,
  const size_t y_begin,
  const size_t y_count){
  int window_id = 0;
  // first check if no data has been read into the buffer
  if(data_indices_.size()==0){
    DEBUG_MSG("HyperCine::data(frame,x_begin...): reading buffer since it is uninitialized");
    HyperFrame hf(frame);
    hf.add_window(x_begin,x_count,y_begin,y_count);
    read_buffer(hf);
  }else{ // data exists in the buffer
    // check to see if the window is not in the buffer (id == -1)
    // if the window is not in the buffer, it triggers a read_buffer
    // regardless of if the frame is in the buffer
    window_id = hf_.window_id(x_begin,x_count,y_begin,y_count);
    if(window_id<0){
      DEBUG_MSG("HyperCine::data(frame,x_begin...): buffer is initialized, but reading buffer"
        " again since the requested window is not in the buffer");
      HyperFrame hf(frame);
      hf.add_window(x_begin,x_count,y_begin,y_count);
      read_buffer(hf);
      window_id = 0;
    }else{ // the window extents are in the buffer, but this particular frame
      // might not be, if the frame does not exist, reset the frame_ids on the hf_
      // and call read_buffer again
      if(!buffer_has_frame(frame)){
        DEBUG_MSG("HyperCine::data(frame,x_begin...): buffer is initialized, but reading buffer"
          " again since the requested frame is not in the buffer");
        std::set<int> frames;
        frames.insert(frame);
        hf_.update_frames(frames);
        read_buffer();
      }
    }
  }
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
  ASSERT_OR_EXCEPTION(test_size==header_.header_size);
  cine_file.read(reinterpret_cast<char*>(&header_.compression), sizeof(header_.compression));
  DEBUG_MSG("HyperCine::read_header(): header compression:   " << header_.compression);
  ASSERT_OR_EXCEPTION(header_.compression==0);
  cine_file.read(reinterpret_cast<char*>(&header_.version), sizeof(header_.version));
  DEBUG_MSG("HyperCine::read_header(): header version:       " << header_.version);
  ASSERT_OR_EXCEPTION(header_.version==1);
  cine_file.read(reinterpret_cast<char*>(&header_.first_movie_image), sizeof(header_.first_movie_image));
  DEBUG_MSG("HyperCine::read_header(): header first mov img: " << header_.first_movie_image);
  cine_file.read(reinterpret_cast<char*>(&header_.total_image_count), sizeof(header_.total_image_count));
  DEBUG_MSG("HyperCine::read_header(): total image count:    " << header_.total_image_count);
  cine_file.read(reinterpret_cast<char*>(&header_.first_image_no), sizeof(header_.first_image_no));
  DEBUG_MSG("HyperCine::read_header(): first image no:       " << header_.first_image_no);
  cine_file.read(reinterpret_cast<char*>(&header_.image_count), sizeof(header_.image_count));
  DEBUG_MSG("HyperCine::read_header(): header image count:   " << header_.image_count);
  cine_file.read(reinterpret_cast<char*>(&header_.off_image_header), sizeof(header_.off_image_header));
  DEBUG_MSG("HyperCine::read_header(): offset image header:  " << header_.off_image_header);
  ASSERT_OR_EXCEPTION((int)header_.off_image_header==test_size);
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
  ASSERT_OR_EXCEPTION(header_test_size==(int)bitmap_header_.size);
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
  ASSERT_OR_EXCEPTION(bitmap_header_.bit_count==8||bitmap_header_.bit_count==16);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.compression), sizeof(bitmap_header_.compression));
  DEBUG_MSG("HyperCine::read_header(): bitmap compression:      " << bitmap_header_.compression);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.size_image), sizeof(bitmap_header_.size_image));
  DEBUG_MSG("HyperCine::read_header(): bitmap image size:       " << bitmap_header_.size_image);
  ASSERT_OR_EXCEPTION(bitmap_header_.size_image*header_.image_count <= file_size);
  int bit_depth = (bitmap_header_.size_image * 8) / (bitmap_header_.width * bitmap_header_.height);
  DEBUG_MSG("HyperCine::read_header(): bitmap actual bit count: " << bit_depth);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.x_pixels_per_meter), sizeof(bitmap_header_.x_pixels_per_meter));
  DEBUG_MSG("HyperCine::read_header(): bitmap x pels/meter:     " << bitmap_header_.x_pixels_per_meter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.y_pixels_per_meter), sizeof(bitmap_header_.y_pixels_per_meter));
  DEBUG_MSG("HyperCine::read_header(): bitmap y pels/meter:     " << bitmap_header_.y_pixels_per_meter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.clr_used), sizeof(bitmap_header_.clr_used));
  DEBUG_MSG("HyperCine::read_header(): bitmap colors used:      " << bitmap_header_.clr_used);
  ASSERT_OR_EXCEPTION(bitmap_header_.clr_used==0);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.clr_important), sizeof(bitmap_header_.clr_important));
  DEBUG_MSG("HyperCine::read_header(): important colors:        " << bitmap_header_.clr_important);

  if(bit_depth==8){
    bitmap_header_.bit_depth=BIT_DEPTH_8;
    if(bitmap_header_.clr_important==0)
      bitmap_header_.clr_important = 256;
  }
  else if (bit_depth==16){
    bitmap_header_.bit_depth=BIT_DEPTH_16;
    if(bitmap_header_.clr_important==0)
      bitmap_header_.clr_important = 65536;
    if(conversion_type_==TO_8_BIT){
      conversion_factor_16_to_8_ = 255.0f/(bitmap_header_.clr_important-1);
      bitmap_header_.clr_important = 256;
    }
  }
  else if (bit_depth==10){
    // if the bit_depth is 8 or 10_packed, the data is stored in single bytes so the bit_count needs to be changed to 8
    // since the values get compressed into an equivalent 8 bit value using the table at the top of this file
    bitmap_header_.bit_depth=BIT_DEPTH_10_PACKED;
    if(conversion_type_==NO_CONVERSION||conversion_type_==QUAD_10_TO_12)
      bitmap_header_.bit_count=16;
    else
      bitmap_header_.bit_count=8;
    // also update the clr important value since the intensity values get scaled to the 8 bit range
    if(conversion_type_==NO_CONVERSION)
      bitmap_header_.clr_important=1024;
    else if(conversion_type_==QUAD_10_TO_12)
      bitmap_header_.clr_important=4096;
    else
      bitmap_header_.clr_important=256;
    DEBUG_MSG("HyperCine::read_header(): reset important colors to: " << bitmap_header_.clr_important);
  }else{
    std::cout << "invalid bit depth " << bit_depth << std::endl;
    throw std::invalid_argument("invalid bit depth");
  }

  cine_file.seekg(header_.off_setup);
  uint16_t frame_rate;
  cine_file.read(reinterpret_cast<char*>(&frame_rate), sizeof(frame_rate));
  bitmap_header_.frame_rate = frame_rate;

  // read the image offsets:
  cine_file.seekg(header_.off_image_offsets);
  ASSERT_OR_EXCEPTION(image_offsets_.size()==header_.image_count);
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
  hf_ = hf; // store a copy of the window dimensions and frame ids
  read_buffer();
}

void
HyperCine::read_buffer(){
  DEBUG_MSG("HyperCine::read_buffer(): called");
  // check that the bounds on the hyperframe are valid
  if(hf_.num_frames()<=0){
    std::cout << "no frames specified in read_buffer() call" << std::endl;
    throw std::invalid_argument("invalid HyperFrame (no frames)");
  }
  for(std::set<int>::const_iterator set_it = hf_.frame_ids()->begin();set_it!=hf_.frame_ids()->end();++set_it){
    int frame = *set_it;
    int end_frame = (int)header_.first_image_no + (int)header_.image_count - 1;
    int begin_frame = header_.first_image_no;
    if(frame<begin_frame||frame>end_frame){
      std::cout << "error: invalid frame requested: " << *set_it << std::endl;
      std::cout << "first frame no: " << begin_frame << std::endl;
      std::cout << "last frame no: " << end_frame << std::endl;
      throw std::invalid_argument("invalid HyperFrame (includes invalid frame)");
    }
  }
  // check if the HyperFrame window data is empty, if so create default bounds of the entire image
  if(hf_.num_windows()==0)
    hf_.add_window(0,bitmap_header_.width,0,bitmap_header_.height);
  for(size_t i=0;i<hf_.num_windows();++i){
    if(hf_.window_x_begin(i) < 0 || hf_.window_x_begin(i) + hf_.window_width(i) > bitmap_header_.width){
      std::cout << "error: invalid HyperFrame window (window x_begin < 0 or window x_begin + window_width > image width)" << std::endl;
      throw std::invalid_argument("");
    }
    if(hf_.window_y_begin(i) < 0 || hf_.window_y_begin(i) + hf_.window_height(i) > bitmap_header_.height){
      std::cout << "error: invalid HyperFrame window (window y_begin < 0 or window y_begin + window_height > image height)" << std::endl;
      throw std::invalid_argument("");
    }
  }
  // clear the buffer
  data_.clear();
  data_indices_.clear();
  data_.resize(hf_.num_pixels_per_frame()*hf_.num_frames());
  DEBUG_MSG("HyperCine::read_buffer(): data storage size " << data_.size());

  // setup the storages
  for(std::set<int>::const_iterator set_it = hf_.frame_ids()->begin();set_it!=hf_.frame_ids()->end();++set_it){
    data_indices_.insert(std::pair<int,std::vector<size_t> >(*set_it,std::vector<size_t>(hf_.num_windows(),0)));
  }
  DEBUG_MSG("HyperCine::read_buffer(): total num pixels per frame " << hf_.num_pixels_per_frame());
  if(bitmap_header_.bit_depth==BIT_DEPTH_8)
    read_hyperframe_8_bit();
  else if (bitmap_header_.bit_depth==BIT_DEPTH_16)
    read_hyperframe_16_bit();
  else if (bitmap_header_.bit_depth==BIT_DEPTH_10_PACKED)
    read_hyperframe_10_bit_packed();
  else
    throw std::invalid_argument("invalid bit depth");
}

std::vector<storage_t>
HyperCine::get_frame(const int frame){
  DEBUG_MSG("HyperCine::get_frame(): frame " << frame);
  if(frame<(int)header_.first_image_no||frame>=(int)header_.first_image_no + (int)header_.image_count){
    std::cout << "error: frame_begin out of range" << std::endl;
    throw std::invalid_argument("invalid frame");
  }
  std::vector<storage_t> data;
  if(bitmap_header_.bit_depth==BIT_DEPTH_8)
    read_hyperframe_8_bit_full(frame,data);
  else if (bitmap_header_.bit_depth==BIT_DEPTH_16)
    read_hyperframe_16_bit_full(frame,data);
  else if (bitmap_header_.bit_depth==BIT_DEPTH_10_PACKED)
    read_hyperframe_10_bit_packed_full(frame,data);
  else
    throw std::invalid_argument("invalid bit depth");
  return data;
}

std::vector<storage_t>
HyperCine::get_avg_frame(const int frame_begin, const int frame_end){
  DEBUG_MSG("HyperCine::get_avg_frame(): frame " << frame_begin << " to " << frame_end);
  if(frame_end<frame_begin){
    std::cout << "error: frame_end < frame_begin"<<std::endl;
    throw std::invalid_argument("invalid frame range");
  }
  if(frame_begin<(int)header_.first_image_no||frame_begin>=(int)header_.first_image_no + (int)header_.image_count){
    std::cout << "error: frame_begin out of range" << std::endl;
    throw std::invalid_argument("invalid frame_begin");
  }
  if(frame_end<(int)header_.first_image_no||frame_end>=(int)header_.first_image_no + (int)header_.image_count){
    std::cout << "error: frame_end out of range" << std::endl;
    throw std::invalid_argument("invalid frame_end");
  }
  std::vector<storage_t> data(bitmap_header_.width*bitmap_header_.height,0.0);
  for(int frame=frame_begin;frame<=frame_end;++frame){
    std::vector<storage_t> temp_data = get_frame(frame);
    ASSERT_OR_EXCEPTION(temp_data.size()==data.size());
    for(size_t i=0;i<data.size();++i)
      data[i]+=temp_data[i];
  }
  for(size_t i=0;i<data.size();++i)
    data[i]/=(frame_end-frame_begin+1);
  return data;
}

void
HyperCine::read_hyperframe_8_bit_full(const int frame, std::vector<storage_t> & data){
  //
  // NOTE: in 8bit format the images are stored upside down!
  //
  DEBUG_MSG("HyperCine::read_hyperframe_8_bit_full():");
  data.clear();
  data.resize(bitmap_header_.width*bitmap_header_.height);
  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }
  std::vector<char> window_row_buffer(bitmap_header_.width);
  DEBUG_MSG("HyperCine::read_hyperframe_8_bit_full(): window row buffer storage size " << window_row_buffer.size());
  // position to the first frame in this set:
  uint8_t * window_row_buff_ptr = reinterpret_cast<uint8_t*>(&window_row_buffer[0]);
  size_t total_px_read = 0;
  // iterate the frames
  const int64_t frame_offset = image_offsets_[frame - header_.first_image_no] + header_.header_offset;
  // iterate the regions of interest
  const size_t data_window_start = total_px_read;
  for(size_t row=0;row<bitmap_header_.height;++row){
    const size_t row_inc = row*bitmap_header_.width;
    const int64_t begin_window_row = frame_offset + row_inc;
    cine_file.seekg(begin_window_row);
    cine_file.read(&window_row_buffer[0],window_row_buffer.size());
    // unpack the image data from the array
    for(size_t px=0;px<bitmap_header_.width;++px){
      const size_t data_index = (bitmap_header_.height-row-1)*bitmap_header_.width + px;
      // due to the image being stored upside down
      data[data_window_start+data_index] = window_row_buff_ptr[px];
      total_px_read++;
    } // end pixel iterator
  } // end window row iterator
  cine_file.close();
}

void
HyperCine::read_hyperframe_8_bit(){
  //
  // NOTE: in 8bit format the images are stored upside down!
  //
  // chunks of memory will be read into a buffer on region of intertest and one frame at a time
  // current strategy is to read one row of one frame region, but only the width of the frame region for each read
  // and seekg between each row of the frame region to skip the rest of the row
  DEBUG_MSG("HyperCine::read_hyperframe_8_bit():");

  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }

  // the buffer needs to be sized as big as the largest row among the windows in the hyperframe
  const int window_row_buffer_size = hf_.buffer_row_size()+1; // +1 to oversize
  std::vector<char> window_row_buffer(window_row_buffer_size);
  DEBUG_MSG("HyperCine::read_hyperframe_8_bit(): window row buffer storage size " << window_row_buffer.size());
  // position to the first frame in this set:
  uint8_t * window_row_buff_ptr = reinterpret_cast<uint8_t*>(&window_row_buffer[0]);
  size_t total_px_read = 0;
  // iterate the frames
  for(std::set<int>::const_iterator set_it = hf_.frame_ids()->begin();set_it!=hf_.frame_ids()->end();++set_it){
    const size_t frame = *set_it - header_.first_image_no;
    const int64_t frame_offset = image_offsets_[frame] + header_.header_offset;
    // iterate the regions of interest
    for(size_t window=0; window<hf_.num_windows();++window){
      const size_t data_window_start = total_px_read;
      const size_t window_width = hf_.window_width(window);
      const size_t window_height = hf_.window_height(window);
      const size_t window_x_begin = hf_.window_x_begin(window);
      const size_t window_y_begin = hf_.window_y_begin(window);
      // due to the image being stored upside down
      const size_t bottom_row_offset = (bitmap_header_.height - window_height - window_y_begin)*bitmap_header_.width;
      data_indices_.find(*set_it)->second[window] = total_px_read;
      // iterate the window rows
      for(size_t row=0;row<window_height;++row){
        const size_t row_inc = row*bitmap_header_.width;
        const int64_t begin_window_row = frame_offset + bottom_row_offset + row_inc + window_x_begin;
        cine_file.seekg(begin_window_row);
        cine_file.read(&window_row_buffer[0],window_row_buffer_size);
        // unpack the image data from the array
        for(size_t px=0;px<window_width;++px){
          const size_t data_index = (window_height-row-1)*window_width + px;
          data_[data_window_start+data_index] = window_row_buff_ptr[px];
          total_px_read++;
        } // end pixel iterator
      } // end window row iterator
    } // end window iterator
  } // end frame iterator
  cine_file.close();
}

void
HyperCine::read_hyperframe_16_bit_full(const int frame, std::vector<storage_t> & data){
  //
  // NOTE: in 16bit format the images are stored upside down!
  //
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit_full():");
  data.clear();
  data.resize(bitmap_header_.width*bitmap_header_.height);
  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit_full(): conversion factor " << conversion_factor_16_to_8_);
  // the buffer needs to be sized as big as the largest row among the windows in the hyperframe
  std::vector<char> window_row_buffer(bitmap_header_.width*bitmap_header_.height*2);
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit_full(): window row buffer storage size " << window_row_buffer.size());
  // position to the first frame in this set:
  uint16_t * window_row_buff_ptr = reinterpret_cast<uint16_t*>(&window_row_buffer[0]);
  size_t total_px_read = 0;
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit_full(): reading frame " << frame);
  const int64_t frame_offset = image_offsets_[frame-header_.first_image_no] + header_.header_offset;
  // iterate the regions of interest
  const size_t data_window_start = total_px_read;
  // iterate the window rows
  for(size_t row=0;row<bitmap_header_.height;++row){
    const size_t row_inc = row*bitmap_header_.width*2;
    const int64_t begin_window_row = frame_offset + row_inc;
    cine_file.seekg(begin_window_row);
    cine_file.read(&window_row_buffer[0],window_row_buffer.size());
    // unpack the image data from the array
    for(size_t px=0;px<bitmap_header_.width;++px){
      const size_t data_index = (bitmap_header_.height-row-1)*bitmap_header_.width + px;
      data[data_window_start+data_index] = window_row_buff_ptr[px]*conversion_factor_16_to_8_;
      total_px_read++;
    } // end pixel iterator
  } // end window row iterator
  cine_file.close();
}


void
HyperCine::read_hyperframe_16_bit(){
  //
  // NOTE: in 16bit format the images are stored upside down!
  //
  // chunks of memory will be read into a buffer on region of intertest and one frame at a time
  // current strategy is to read one row of one frame region, but only the width of the frame region for each read
  // and seekg between each row of the frame region to skip the rest of the row
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit():");

  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }

  // the buffer needs to be sized as big as the largest row among the windows in the hyperframe
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit(): conversion factor " << conversion_factor_16_to_8_);
  const int window_row_buffer_size = (hf_.buffer_row_size()+1)*2; // +1 to oversize
  std::vector<char> window_row_buffer(window_row_buffer_size);
  DEBUG_MSG("HyperCine::read_hyperframe_16_bit(): window row buffer storage size " << window_row_buffer.size());
  // position to the first frame in this set:
  uint16_t * window_row_buff_ptr = reinterpret_cast<uint16_t*>(&window_row_buffer[0]);
  size_t total_px_read = 0;
  // iterate the frames
  for(std::set<int>::const_iterator set_it = hf_.frame_ids()->begin();set_it!=hf_.frame_ids()->end();++set_it){
    DEBUG_MSG("HyperCine::read_hyperframe_16_bit(): reading frame " << *set_it);
    const size_t frame = *set_it - header_.first_image_no;
    const int64_t frame_offset = image_offsets_[frame] + header_.header_offset;
    // iterate the regions of interest
    for(size_t window=0; window<hf_.num_windows();++window){
      const size_t data_window_start = total_px_read;
      const size_t window_width = hf_.window_width(window);
      const size_t window_height = hf_.window_height(window);
      const size_t window_x_begin = hf_.window_x_begin(window);
      const size_t window_y_begin = hf_.window_y_begin(window);
      // due to the image being stored upside down
      const size_t bottom_row_offset = (bitmap_header_.height - window_height - window_y_begin)*bitmap_header_.width*2;
      data_indices_.find(*set_it)->second[window] = total_px_read;
      // iterate the window rows
      for(size_t row=0;row<window_height;++row){
        const size_t row_inc = row*bitmap_header_.width*2;
        const int64_t begin_window_row = frame_offset + bottom_row_offset + row_inc + window_x_begin*2;
        cine_file.seekg(begin_window_row);
        cine_file.read(&window_row_buffer[0],window_row_buffer_size);
        // unpack the image data from the array
        for(size_t px=0;px<window_width;++px){
          const size_t data_index = (window_height-row-1)*window_width + px;
          data_[data_window_start+data_index] = window_row_buff_ptr[px]*conversion_factor_16_to_8_;
          // below is how the buffer values needed to be split between uint8_t slots when the
          // memory buffer used to be uint8_t type, now it's uint16_t so the values don't need to be split anymore
          // data_[(data_window_start+data_index)*2] = window_row_buff_ptr[px] & 0xff; // split the 16bit value between two bits
          // data_[(data_window_start+data_index)*2+1] = (window_row_buff_ptr[px] >> 8);
          total_px_read++;
        } // end pixel iterator
      } // end window row iterator
    } // end window iterator
  } // end frame iterator
  cine_file.close();
}

void
HyperCine::read_hyperframe_10_bit_packed_full(const int frame, std::vector<storage_t> & data){
  DEBUG_MSG("HyperCine::read_hyperframe_10_bit_packed_full():");
  data.clear();
  data.resize(bitmap_header_.width*bitmap_header_.height);
  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }
  // the buffer needs to be sized as big as the largest row among the windows in the hyperframe
  const int window_row_buffer_size = ceil(((bitmap_header_.width+1)*10)/8); // 10 bits per pixel divided by 8 to get bytes, +1 to oversize
  std::vector<char> window_row_buffer(window_row_buffer_size);
  DEBUG_MSG("HyperCine::read_buffer_full(): window row buffer storage size " << window_row_buffer.size());
  // position to the first frame in this set:
  uint8_t * window_row_buff_ptr = reinterpret_cast<uint8_t*>(&window_row_buffer[0]);
  uint16_t intensity_16 = 0.0;
  uint16_t intensity_16p1 = 0.0;
  uint16_t two_byte = 0;
  size_t total_px_read = 0;
  const int64_t frame_offset = image_offsets_[frame - header_.first_image_no] + header_.header_offset;
  // iterate the window rows
  for(size_t row=0;row<bitmap_header_.height;++row){
    const size_t row_inc = row*bitmap_header_.width;
    // determine the position of the first pixel in the this row of this window
    const int64_t begin_window_row = frame_offset + (row_inc*10)/8; // convert the number of pixels up to this point to number of bytes to hold 10bit pixel values
    cine_file.seekg(begin_window_row);
    cine_file.read(&window_row_buffer[0],window_row_buffer_size);
    // unpack the 10 bit image data from the array
    for(size_t px=0;px<bitmap_header_.width;++px){
      // and now to index into the strange beast that is 10bit packed cine files...
      // this looks strange because the ten bits are shared across bytes, the first byte has 1 10bit number, with 2 bits spilling into the
      // next byte. The second byte has 2 leftover bits from the first 10bit number, 6 bits from the current 10bit value and spills 4 into the next byte
      // the third byte has 4 bits from the third 10bit value, 4 bits of its own 10bit value and spills 6 bits into the next byte
      // the fourth byte has 6 bits from the last 10bit value, 2 bits of its own, and spills 8 into the next byte, filling that byte
      // so in total it takes 5 bytes for every 4 10bit values
      const size_t col_inc = px;
      const size_t slot = px*10/8;
      const size_t chunk_offset = col_inc%4; // 5 bytes per four pixels
      // create the single 16 bit combo
      intensity_16p1 = window_row_buff_ptr[slot+1];
      intensity_16p1 <<= 8; // move the bits over to the beginning of the byte
      intensity_16 = window_row_buff_ptr[slot];
      two_byte = intensity_16 | intensity_16p1;
      endian_swap(two_byte);
      // shift the 10 bits to the right side of the 16 bit data type;
      two_byte = two_byte >> (6 - (chunk_offset*2));
      // use a mask to zero out the left 6 bits
      two_byte = two_byte & 0x3FF; // 16 bits with only the right 10 active;
      // this next step is required because the original signal was companded from 12 bits to 10,
      // now we are compressing it back to 8 bit:
      data[total_px_read++] = hash_ptr_[two_byte];
    } // end pixel iterator
  } // end window row iterator
  cine_file.close();
}

void
HyperCine::read_hyperframe_10_bit_packed(){
  DEBUG_MSG("HyperCine::read_hyperframe_10_bit_packed():");

  // chunks of memory will be read into a buffer one window and one frame at a time
  // current strategy is to read one row of one window, but only the width of the window for each read
  // and seekg between each row of the window to skip the rest of the row

  // open the file
  std::ifstream cine_file(file_name_.c_str(), std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name_ << std::endl;
    throw std::exception();
  }
  // the buffer needs to be sized as big as the largest row among the windows in the hyperframe
  const int window_row_buffer_size = ceil(((hf_.buffer_row_size()+1)*10)/8); // 10 bits per pixel divided by 8 to get bytes, +1 to oversize
  ASSERT_OR_EXCEPTION(((hf_.buffer_row_size()+1)*10)/8<=window_row_buffer_size);
  std::vector<char> window_row_buffer(window_row_buffer_size);
  DEBUG_MSG("HyperCine::read_buffer(): window row buffer storage size " << window_row_buffer.size());
  // position to the first frame in this set:
  uint8_t * window_row_buff_ptr = reinterpret_cast<uint8_t*>(&window_row_buffer[0]);
  uint16_t intensity_16 = 0.0;
  uint16_t intensity_16p1 = 0.0;
  uint16_t two_byte = 0;
  size_t total_px_read = 0;
  // iterate the frames (the frame number needs to be offset with first_image_no because hf.frame is the global frame id,
  // which is not necessarily 0-based
  for(std::set<int>::const_iterator set_it = hf_.frame_ids()->begin();set_it!=hf_.frame_ids()->end();++set_it){
    const size_t frame = *set_it - header_.first_image_no;
    const int64_t frame_offset = image_offsets_[frame] + header_.header_offset;
    // iterate the regions of interest
    for(size_t window=0; window<hf_.num_windows();++window){
      const size_t window_width = hf_.window_width(window);
      const size_t window_x_begin = hf_.window_x_begin(window);
      const int xbegin_mod_4 = hf_.window_x_begin(window)%4;
      data_indices_.find(*set_it)->second[window] = total_px_read;
      // iterate the window rows
      for(size_t row=hf_.window_y_begin(window);row<hf_.window_y_begin(window)+hf_.window_height(window);++row){
        const size_t row_inc = row*bitmap_header_.width;
        // determine the position of the first pixel in the this row of this window
        const int64_t begin_window_row = frame_offset + ((row_inc + window_x_begin)*10)/8; // convert the number of pixels up to this point to number of bytes to hold 10bit pixel values
        cine_file.seekg(begin_window_row);
        cine_file.read(&window_row_buffer[0],window_row_buffer_size);
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
          intensity_16p1 = window_row_buff_ptr[slot+1];
          intensity_16p1 <<= 8; // move the bits over to the beginning of the byte
          intensity_16 = window_row_buff_ptr[slot];
          two_byte = intensity_16 | intensity_16p1;
          endian_swap(two_byte);
          // shift the 10 bits to the right side of the 16 bit data type;
          two_byte = two_byte >> (6 - (chunk_offset*2));
          // use a mask to zero out the left 6 bits
          two_byte = two_byte & 0x3FF; // 16 bits with only the right 10 active;
          // this next step is required because the original signal was companded from 12 bits to 10,
          // now we are compressing it back to 8 bit:
          data_[total_px_read++] = hash_ptr_[two_byte];
        } // end pixel iterator
      } // end window row iterator
    } // end window iterator
  } // end frame iterator
  cine_file.close();
}

/// overload the ostream operator
std::ostream& operator<<(std::ostream& os,
  const HyperCine & hc){
  os << "HyperCine: my address: " << &hc << std::endl;
  os << "HyperCine: hyperframe address: " << &hc.hf_ << std::endl;
  os << "HyperCine: num frames in data: " << hc.hf_.num_frames() << std::endl;
  os << "HyperCine: num windows per frame: " << hc.hf_.num_windows() << std::endl;
  os << "HyperCine: " << std::left << std::setw(12) << "frame"
      << std::left << std::setw(12) << "window"
      << std::left << std::setw(12) << "data_index"
      << std::left << std::setw(12) << "width"
      << std::left << std::setw(12) << "height" << std::endl;
  for(std::set<int>::const_iterator set_it = hc.hf_.frame_ids()->begin();set_it!=hc.hf_.frame_ids()->end();++set_it){
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

void
HyperCine::write_frame(const char * file_name,
  const size_t width,
  const size_t height,
  uint16_t * data,
  const bool overwrite){ // for both 8 bit and 16 bit the frame is written upside down

  uint16_t bit_count = 16;

  // check if file exists
  std::ifstream existing_cine_file(file_name);
  uint32_t image_count = 0;
  if(overwrite||existing_cine_file.fail()){
    existing_cine_file.close(); // close the file so write_header can overwrite it if it exists
    DEBUG_MSG("HyperCine::write_frame(): writing cine header: " << file_name);
    write_header(file_name,width,height,bit_count);
  }else{
    // open file as input stream and seekg to the right place to get the image count
    DEBUG_MSG("HyperCine::initialize_write_frame(): reading existing cine header");
    existing_cine_file.seekg(IMAGE_COUNT_OFFSET);
    existing_cine_file.read(reinterpret_cast<char*>(&image_count), sizeof(image_count));
    // check the width and height
    existing_cine_file.seekg(IMAGE_WIDTH_OFFSET);
    uint32_t existing_width, existing_height;
    existing_cine_file.read(reinterpret_cast<char*>(&existing_width), sizeof(uint32_t));
    existing_cine_file.read(reinterpret_cast<char*>(&existing_height), sizeof(uint32_t));
    //DEBUG_MSG("HyperCine::write_frame(): existing image dimensions: " << existing_width << " x " << existing_height);
    existing_cine_file.close();
    if (width!=existing_width||height!=existing_height){
      std::cout << "Error, invalid image dimensions: width " << width << " existing width " << existing_width <<
          " height" << height << " existing height " << existing_height << std::endl;
      throw std::exception();
    }
  }
  // TODO check bit depth stays the same too
  DEBUG_MSG("HyperCine::write_frame(): " << file_name << " frame " << image_count);
  // update the image count
  if (image_count>=MAX_WRITE_FRAMES){
    std::cout << "Error, max number of frames in this file exceeded. Max " << image_count << ", " << file_name << std::endl;
    throw std::exception();
  }
  image_count++;

  std::ofstream cine_file(file_name, std::ios::in | std::ios::out | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name << std::endl;
    throw std::exception();
  }
  // update the image count
  cine_file.seekp(IMAGE_COUNT_OFFSET);
  cine_file.write(reinterpret_cast<char*>(&image_count), sizeof(uint32_t));
  // update the image offset
  int image_size = width*height*(bit_count/8); // in bytes

  // the 8 in the next two lines are because the offsets are stored in int64_t types which are 8 bytes
  // and there are MAX_WRITE_FRAMES of values stored (it's not related to the bit_depth)
  // offset to the start of this frames pixel values in the data memory storage
  uint64_t image_offset = HEADER_SIZE + BITMAP_HEADER_SIZE + MAX_WRITE_FRAMES*8 + (image_count - 1)*image_size;

  // offset in the file to the offset value stored for this frame
  int image_offset_offset = HEADER_SIZE + BITMAP_HEADER_SIZE + (image_count-1)*8;
  cine_file.seekp(image_offset_offset);
  cine_file.write(reinterpret_cast<char*>(&image_offset), sizeof(uint64_t));

  // write the image data at the end of the file
  cine_file.seekp(0,std::ios::end);

  // data needs to be written upside down
  for(size_t y=0;y<height;++y){
    for(size_t x=0;x<width;++x){
      cine_file.write(reinterpret_cast<char*>(&data[(height-y-1)*width+x]), sizeof(uint16_t));
    }
  }
  cine_file.close();
}

void
HyperCine::write_header(const char * file_name, const size_t width, const size_t height, uint16_t bit_count){
  std::ofstream cine_file(file_name, std::ios::out | std::ios::binary);
  if (cine_file.fail()){
    std::cout << "Error, can't open the file: " << file_name << std::endl;
    throw std::exception();
  }
  uint16_t header_size = 36 + sizeof(TIME64);
  uint32_t off_bitmap_header = 36 + sizeof(TIME64);
  uint32_t bitmap_header_size = 40;
  uint16_t header_version = 1;
  uint16_t dummy16 = 0;
  uint32_t dummy = 0;
  uint32_t off_image_offsets = header_size + bitmap_header_size; // at the end of the cine header and bitmap header
  uint32_t image_width = width;
  uint32_t image_height = height;
  uint32_t image_size = width*height*(bit_count/8);
  uint32_t clr_important = bit_count == 8 ? 256 : 65536;
  TIME64 dummy_time;
  uint64_t dummy_offset = 0;

  // fill default header info:
  cine_file.write(reinterpret_cast<char*>(&dummy16), sizeof(uint16_t));
  cine_file.write(reinterpret_cast<char*>(&header_size), sizeof(uint16_t));
  cine_file.write(reinterpret_cast<char*>(&dummy16), sizeof(uint16_t));
  cine_file.write(reinterpret_cast<char*>(&header_version), sizeof(uint16_t));
  // first_movie_image, total_image_count, first_image_no, and image_count
  for(size_t i=0;i<4;++i)
    cine_file.write(reinterpret_cast<char*>(&dummy), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&off_bitmap_header), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&dummy), sizeof(uint32_t)); // off_setup
  cine_file.write(reinterpret_cast<char*>(&off_image_offsets), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&dummy_time), sizeof(TIME64));

  // write the default bitmap header info
  cine_file.write(reinterpret_cast<char*>(&bitmap_header_size), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&image_width), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&image_height), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&dummy16), sizeof(uint16_t)); // planes
  cine_file.write(reinterpret_cast<char*>(&bit_count), sizeof(uint16_t));
  cine_file.write(reinterpret_cast<char*>(&dummy), sizeof(uint32_t)); // compression
  cine_file.write(reinterpret_cast<char*>(&image_size), sizeof(uint32_t));
  // x_pixels_per_meter, y_pixels_per_meter, clr_used
  for(size_t i=0;i<3;++i)
    cine_file.write(reinterpret_cast<char*>(&dummy), sizeof(uint32_t));
  cine_file.write(reinterpret_cast<char*>(&clr_important), sizeof(uint32_t));

  // write a buffer of image offset values
  for(size_t i=0;i<MAX_WRITE_FRAMES;++i)
    cine_file.write(reinterpret_cast<char*>(&dummy_offset), sizeof(uint64_t));
  cine_file.close();
}

} // end namespace hypercine
