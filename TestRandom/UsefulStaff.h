#pragma once

const int MAX_BOUNCE = 8;
size_t MAX_PATHS = 0;

const float EPSILON = 1e-7;

enum class Generator : uint8_t {
    Standard, Halton, HaltonRandomDigit, HaltonOwen, BlueNoise
};

enum struct SampleDimension : uint32_t {
    ePixelX,
    ePixelY,
    eLightId,
    eLightPointX,
    eLightPointY,
    eBSDF0,
    eBSDF1,
    eBSDF2,
    eBSDF3,
    eGetRayX,
    eGetRayY,
    eRussianRoulette,
    eNUM_DIMENSIONS
};

struct SamplerState {
    uint32_t seed = 0;
    uint32_t sampleIdx = 0;
    uint32_t depth = 0;
};

static std::vector<std::vector<uint32_t>> permutations_scrambling = {{1,   0},
                                                                     {2,   0,   1},
                                                                     {1,   0,   4,   3,   2},
                                                                     {4,   1,   6,   2,   0,  5,   3},
                                                                     {5,   6,   2,   1,   7,  8,   9,   3,  10,  0,  4},
                                                                     {4,   12,  8,   6,   1,  0,   3,   2,  7,   11, 5,   9,   10},
                                                                     {16,  3,   10,  9,   6,  7,   13,  2,  14,  11, 5,   0,   1,  4,   8,  15,  12},
                                                                     {4,   17,  0,   15,  18, 5,   8,   16, 2,   11, 6,   1,   13, 10,  7,  3,   12,  14,  9},
                                                                     {21,  6,   22,  5,   2,  3,   18,  12, 9,   19, 15,  17,  20, 11,  14, 10,  7,   1,   0,   8,   4,   13, 16},
                                                                     {5,   28,  21,  8,   2,  25,  22,  7,  24,  26, 19,  10,  16, 9,   11, 0,   27,  14,  18,  3,   23,  12, 15,  6,   4,   1,   13, 20,  17},
                                                                     {12,  4,   24,  25,  20, 8,   11,  17, 30,  26, 18,  10,  16, 14,  5,  2,   19,  3,   1,   15,  21,  22, 23,  13,  0,   9,   7,  27,  6,  29, 28},
                                                                     {4,   36,  2,   1,   30, 17,  3,   23, 9,   7,  18,  24,  6,  26,  0,  32,  11,  14,  22,  21,  12,  13, 15,  33,  10,  27,  20, 5,   19, 28, 16, 29, 25, 34, 8,  31,  35},
                                                                     {21,  32,  35,  1,   25, 18,  16,  5,  30,  28, 20,  2,   4,  39,  9,  14,  34,  29,  11,  27,  0,   31, 23,  19,  38,  17,  12, 37,  8,  40, 33, 3,  24, 36, 22, 6,   26, 10, 15, 7,  13},
                                                                     {2,   41,  30,  8,   12, 6,   29,  13, 1,   19, 31,  27,  34, 26,  24, 16,  5,   3,   17,  32,  4,   40, 15,  10,  25,  33,  0,  39,  38, 22, 11, 21, 14, 42, 7,  18,  36, 9,  28, 35, 37,  20, 23},
                                                                     {32,  17,  46,  38,  8,  13,  4,   39, 36,  30, 9,   31,  29, 34,  44, 11,  15,  43,  41,  0,   22,  25, 20,  23,  7,   45,  18, 37,  14, 10, 40, 5,  42, 28, 12, 21,  33, 35, 3,  2,  27,  16, 6,   24, 19, 26,  1},
                                                                     {36,  8,   6,   24,  34, 7,   26,  32, 16,  48, 14,  15,  39, 18,  23, 25,  45,  17,  38,  3,   47,  5,  40,  9,   21,  2,   11, 28,  50, 4,  10, 29, 31, 46, 30, 51,  27, 49, 37, 42, 22,  0,  13,  52, 19, 43,  35, 41,  20, 12, 44, 1,   33},
                                                                     {26, 32, 42, 54, 7, 45, 58, 10, 43, 19, 8, 25, 16, 36, 37, 24, 12, 51, 35, 0, 39, 2, 1, 27, 56, 22, 11, 49, 41, 3, 34, 5, 48, 4, 9, 29, 13, 21, 18, 38, 28, 23, 6, 47, 17, 50, 53, 14, 57, 20, 15, 40, 44,                                       52, 31, 55,  30, 33, 46},
                                                                     {47,  13,  60,  54,  5,  58,  49,  48, 36,  46, 51,  11,  7,  0,   22, 42,  56,  27,  50,  20,  15,  1,  39,  35,  33,  21,  29, 52,  24, 45, 28, 30, 4,  38, 8,  12,  19, 26, 55, 25, 2,   41, 37,  16, 14, 43,  17, 3,   31, 34, 57, 59,  10,  23, 44, 40,  18, 32, 6,  53,  9},
                                                                     {44, 57, 58, 21, 19, 32, 22, 9, 14, 36, 35, 10, 56, 53, 47, 66, 52, 48, 24, 34, 5, 37, 8, 17, 51, 39, 0, 18, 4, 27, 33, 61, 41, 16, 23, 60, 45, 40, 13, 63, 46, 59, 31, 55, 12, 20, 11, 29, 49, 26, 28, 65, 3, 25, 38, 62, 2, 42, 1, 50, 64,                                       7,   6,   43,  15, 54, 30},
                                                                     {43, 24, 48, 22, 64, 2, 28, 1, 15, 25, 39, 12, 57, 46, 8, 70, 65, 10, 44, 27, 21, 41, 45, 61, 34, 69, 3, 23, 42, 19, 50, 16, 30, 68, 36, 55, 26, 18, 63, 53, 4, 62, 31, 38, 32, 11, 67, 35, 20, 14, 66, 5, 56, 13, 6, 47, 58, 29, 7, 37, 51, 54, 60, 9, 49, 17, 40,                                           59, 0,  33, 52},
                                                                     {50, 32, 11, 71, 6, 2, 8, 60, 36, 63, 33, 30, 41, 46, 7, 35, 53, 65, 20, 40, 19, 0, 29, 47, 70, 25, 62, 9, 55, 58, 56, 43, 57, 16, 44, 61, 39, 59, 72, 51, 3, 13, 1, 21, 15, 18, 27, 31, 12, 45, 42, 24, 68, 22, 4, 49, 14, 67, 54, 5, 69, 10, 26, 34, 37, 38, 23, 28, 66, 64, 17,                                             52, 48},
                                                                     {3, 10, 55, 54, 76, 52, 32, 68, 9, 46, 69, 66, 58, 43, 36, 70, 42, 21, 56, 12, 25, 53, 74, 37, 75, 44, 30, 62, 57, 5, 51, 14, 19, 31, 38, 40, 78, 23, 29, 64, 45, 18, 71, 49, 27, 48, 47, 72, 77, 11, 17, 4, 59, 35, 61, 60, 41, 24, 39, 6, 50, 34, 65, 20, 0, 28, 63, 7, 1, 2, 16, 26, 22,                                             67, 13,  33, 73,  15, 8},
                                                                     {58, 77, 70, 26, 9, 24, 6, 44, 74, 8, 31, 27, 4, 38, 80, 65, 50, 36, 2, 18, 63, 46, 25, 41, 14, 7, 76, 40, 61, 78, 51, 42, 10, 22, 66, 3, 57, 53, 43, 1, 29, 73, 28, 12, 35, 32, 39, 56, 0, 13, 69, 16, 59, 62, 55, 34, 15, 23, 82, 17, 47, 37, 45, 11, 64, 48, 81, 79, 19, 67, 68, 52, 33, 21, 5, 49, 30, 72, 20,                                                60, 75, 71, 54},
                                                                     {64, 15, 76, 63, 83, 18, 24, 33, 49, 20, 65, 30, 58, 17, 47, 43, 55, 37, 21, 34, 12, 57, 22, 66, 41, 46, 68, 86, 56, 69, 23, 14, 75, 78, 62, 13, 59, 8, 29, 84, 7, 77, 82, 3, 4, 48, 31, 61, 81, 88, 39, 0, 67, 19, 16, 6, 85, 51, 27, 73, 40, 52, 42, 79, 54, 50, 2, 60, 9, 80, 36, 5, 44, 70, 35, 74, 53, 10, 1, 32, 38, 28, 45,                                                72, 11, 71,  87, 26, 25},
                                                                     {84, 75, 92, 56, 68, 58, 81, 59, 9, 60, 4, 42, 57, 80, 26, 41, 64, 52, 50, 71, 73, 34, 67, 54, 72, 38, 13, 89, 12, 22, 82, 11, 91, 90, 7, 74, 15, 18, 51, 20, 88, 49, 32, 76, 0, 25, 16, 46, 66, 35, 8, 45, 28, 2, 37, 27, 93, 53, 3, 96, 39, 77, 47, 33, 14, 36, 65, 63, 61, 48, 78, 62, 83, 17, 1, 19, 23, 30, 6, 10, 43, 24, 5, 95, 70, 29, 69, 31, 85,                                                  79,  44, 21, 40,  87, 55,  86,  94},
                                                                     {61, 94, 11, 53, 38, 18, 17, 84, 68, 71, 37, 47, 87, 27, 33, 89, 67, 72, 41, 45, 58, 62, 13, 28, 3, 32, 75, 59, 52, 56, 9, 30, 25, 24, 40, 5, 23, 97, 4, 21, 74, 73, 1, 46, 35, 85, 76, 29, 96, 42, 92, 55, 12, 10, 70, 51, 95, 26, 22, 39, 34, 69, 98, 44, 19, 99, 78, 57, 43, 31, 20, 65, 81, 50, 79, 100, 14, 54, 63, 6, 36, 77, 82, 60, 66, 2, 86, 88, 48, 16, 49, 15, 90, 91, 7, 8, 0,                                                     64, 83, 80, 93},
                                                                     {60, 91, 66, 34, 40, 6, 51, 98, 100, 14, 59, 67, 25, 71, 21, 7, 35, 95, 76, 50, 70, 82, 8, 89, 24, 27, 87, 17, 101, 75, 96, 5, 16, 93, 94, 97, 44, 58, 0, 22, 20, 74, 29, 85, 23, 61, 10, 18, 92, 99, 48, 15, 19, 79, 4, 12, 13, 28, 77, 11, 84, 64, 63, 30, 46, 54, 73, 86, 42, 88, 55, 47, 43, 72, 53, 57, 62, 41, 31, 45, 56, 90, 37, 32, 83, 3, 81, 33, 69, 68, 49, 38, 1, 80, 78, 39, 52, 2, 9, 102, 36,                                                    65, 26},
                                                                     {64, 44, 104, 43, 67, 103, 5, 50, 87, 56, 24, 37, 73, 88, 82, 45, 63, 78, 66, 2, 32, 85, 99, 71, 61, 89, 101, 93, 10, 13, 81, 98, 19, 91, 72, 41, 14, 1, 102, 27, 105, 18, 59, 65, 38, 34, 52, 106, 23, 54, 12, 90, 69, 6, 62, 94, 7, 3, 11, 53, 49, 86, 4, 96, 48, 33, 35, 47, 57, 25, 42, 46, 17, 8, 75, 39, 58, 79, 40, 70, 36, 28, 97, 80, 55, 16, 74, 83, 31, 0, 9, 20, 15, 30, 21, 51, 100, 92, 84, 29, 68, 95, 60,                                                 22,  76, 77,  26},
                                                                     {45, 108, 102, 77, 40, 86, 4, 18, 79, 8, 50, 72, 3, 27, 5, 57, 93, 106, 80, 96, 90, 19, 74, 92, 104, 75, 98, 84, 43, 103, 97, 22, 31, 23, 53, 81, 37, 21, 66, 32, 29, 20, 60, 83, 1, 62, 2, 58, 68, 30, 99, 76, 70, 24, 54, 82, 55, 47, 17, 41, 69, 46, 89, 95, 67, 28, 73, 7, 85, 63, 26, 51, 39, 42, 10, 9, 36, 11, 61, 15, 105, 94, 56, 38, 34, 91, 88, 35, 71, 78, 25, 0, 13, 64, 14, 6, 107, 33, 16, 44, 87, 49, 12, 52, 59, 101, 65,                                                   100, 48},
                                                                     {111, 26,  107, 29,  22, 102, 41,  4,  93,  89, 106, 97,  94, 20,  54, 110, 56,  35,  6,   112, 52,  96, 43,  64,  51,  101, 5,  103, 78, 88, 30, 12, 3,  18, 15, 65,  53, 25, 99, 14, 81,  67, 87,  7,  16, 69,  19, 76,  61, 75, 17, 98,  48,  45, 70, 39,  11, 37, 42, 47,  90, 59,  10,  32,  73, 2,  50, 57, 80, 21, 84,  23, 85,  91, 0,   28, 60,  92, 79, 9,  34, 83, 36, 44, 46, 71,  24, 1,  8,   105, 33, 49, 104, 62, 108, 55,  31, 86, 40, 66, 95,  58, 100, 72,  74, 82,  13,  27,  77, 38,  109, 63,  68},
                                                                     {22,  68,  87,  46,  48, 23,  40,  77, 88,  16, 0,   63,  95, 49,  90, 114, 24,  96,  103, 11,  125, 38, 81,  107, 112, 126, 33, 42,  32, 17, 61, 41, 37, 4,  15, 93,  1,  66, 67, 76, 101, 43, 12,  51, 3,  7,   83, 105, 20, 78, 39, 118, 111, 69, 62, 120, 21, 91, 53, 106, 72, 80,  123, 117, 65, 36, 60, 64, 50, 56, 109, 27, 115, 73, 121, 99, 110, 58, 9,  2,  70, 92, 29, 86, 89, 35,  31, 47, 124, 84,  19, 28, 8,   82, 119, 102, 26, 94, 52, 18, 100, 54, 25,  59,  30, 45,  122, 74,  6,  104, 98,  10,  85, 13, 14, 71, 34, 97, 116, 113, 44, 55, 79, 75, 57, 108, 5},
                                                                     {23,  106, 102, 115, 15, 7,   111, 16, 117, 27, 51,  105, 52, 123, 71, 88,  121, 112, 60,  124, 89,  79, 100, 125, 1,   101, 98, 24,  73, 22, 55, 35, 13, 28, 82, 128, 26, 81, 34, 69, 104, 2,  108, 78, 9,  130, 29, 44,  97, 65, 33, 116, 54,  39, 80, 5,   66, 59, 63, 107, 32, 122, 30,  70,  75, 21, 68, 62, 17, 72, 36,  64, 19,  14, 85,  18, 41,  76, 43, 42, 50, 11, 6,  56, 25, 109, 99, 57, 31,  103, 74, 3,  91,  53, 114, 47,  0,  20, 61, 58, 92,  48, 87,  118, 10, 110, 12,  129, 67, 94,  86,  120, 90, 93, 77, 95, 96, 40, 113, 46,  37, 83, 38, 45, 49, 8,   126, 127, 84, 119, 4}};

static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                          67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};

static const std::vector<uint32_t> OwenHashes = {0xb694b1f4, 0x7eb7d41c, 0x8bf4251c, 0xf0ea7299, 0xc551ce33,
                                                 0xe75d6a6b, 0xd239f9f0, 0xcc2775fd, 0x6713db31, 0x41ecec7,
                                                 0xae553148, 0xe17f90cd, 0xe720c4d5, 0xe697db5b, 0x1296bb16,
                                                 0xe07d213c, 0x5fa8d144, 0xf3a5e3a8, 0x806b8b34, 0x3875e60d,
                                                 0xfe9d2916, 0x2da800cf, 0xa43393fb, 0xb7403e48, 0xf5547c8f,
                                                 0x324a1e77, 0x497c0deb, 0x29b4427c, 0x4b17bdff, 0xf6d78aa3,
                                                 0x746e10b4, 0x89c231e4};
