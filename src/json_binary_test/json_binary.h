#pragma once

#include <assert.h>

#include <string>
#include <iostream>
#include <fstream>

#define LITTLE_EDAIAN   1

static const char hex_u_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'
};

static const char hex_l_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
};

#if defined(LITTLE_EDAIAN) && (LITTLE_EDAIAN > 0)
static const unsigned short hex_upper_256[] = {
    0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730,     // 00~07
    0x3830, 0x3930, 0x4130, 0x4230, 0x4330, 0x4430, 0x4530, 0x4630,     // 08~0F
    0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,     // 10~17
    0x3831, 0x3931, 0x4131, 0x4231, 0x4331, 0x4431, 0x4531, 0x4631,     // 18~1F
    0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732,     // 20~27
    0x3832, 0x3932, 0x4132, 0x4232, 0x4332, 0x4432, 0x4532, 0x4632,     // 28~2F
    0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733,     // 30~37
    0x3833, 0x3933, 0x4133, 0x4233, 0x4333, 0x4433, 0x4533, 0x4633,     // 38~3F
    0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734,     // 40~47
    0x3834, 0x3934, 0x4134, 0x4234, 0x4334, 0x4434, 0x4534, 0x4634,     // 48~4F
    0x3035, 0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735,     // 50~57
    0x3835, 0x3935, 0x4135, 0x4235, 0x4335, 0x4435, 0x4535, 0x4635,     // 58~5F
    0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736,     // 60~67
    0x3836, 0x3936, 0x4136, 0x4236, 0x4336, 0x4436, 0x4536, 0x4636,     // 68~6F
    0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737,     // 70~77
    0x3837, 0x3937, 0x4137, 0x4237, 0x4337, 0x4437, 0x4537, 0x4637,     // 78~7F
    0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738,     // 80~87
    0x3838, 0x3938, 0x4138, 0x4238, 0x4338, 0x4438, 0x4538, 0x4638,     // 88~8F
    0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739,     // 90~97
    0x3839, 0x3939, 0x4139, 0x4239, 0x4339, 0x4439, 0x4539, 0x4639,     // 98~9F
    0x3041, 0x3141, 0x3241, 0x3341, 0x3441, 0x3541, 0x3641, 0x3741,     // A0~A7
    0x3841, 0x3941, 0x4141, 0x4241, 0x4341, 0x4441, 0x4541, 0x4641,     // A8~AF
    0x3042, 0x3142, 0x3242, 0x3342, 0x3442, 0x3542, 0x3642, 0x3742,     // B0~B7
    0x3842, 0x3942, 0x4142, 0x4242, 0x4342, 0x4442, 0x4542, 0x4642,     // B8~BF
    0x3043, 0x3143, 0x3243, 0x3343, 0x3443, 0x3543, 0x3643, 0x3743,     // C0~C7
    0x3843, 0x3943, 0x4143, 0x4243, 0x4343, 0x4443, 0x4543, 0x4643,     // C8~CF
    0x3044, 0x3144, 0x3244, 0x3344, 0x3444, 0x3544, 0x3644, 0x3744,     // D0~D7
    0x3844, 0x3944, 0x4144, 0x4244, 0x4344, 0x4444, 0x4544, 0x4644,     // D8~DF
    0x3045, 0x3145, 0x3245, 0x3345, 0x3445, 0x3545, 0x3645, 0x3745,     // E0~E7
    0x3845, 0x3945, 0x4145, 0x4245, 0x4345, 0x4445, 0x4545, 0x4645,     // E8~EF
    0x3046, 0x3146, 0x3246, 0x3346, 0x3446, 0x3546, 0x3646, 0x3746,     // F0~F7
    0x3846, 0x3946, 0x4146, 0x4246, 0x4346, 0x4446, 0x4546, 0x4646,     // F8~FF
};
#else
static const unsigned short hex_upper_256[] = {
    0x3030, 0x3031, 0x3032, 0x3033, 0x3034, 0x3035, 0x3036, 0x3037,     // 00~07
    0x3038, 0x3039, 0x3041, 0x3042, 0x3043, 0x3044, 0x3045, 0x3046,     // 08~0F
    0x3130, 0x3131, 0x3132, 0x3133, 0x3134, 0x3135, 0x3136, 0x3137,     // 10~17
    0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3144, 0x3145, 0x3146,     // 18~1F
    0x3230, 0x3231, 0x3232, 0x3233, 0x3234, 0x3235, 0x3236, 0x3237,     // 20~27
    0x3238, 0x3239, 0x3241, 0x3242, 0x3243, 0x3244, 0x3245, 0x3246,     // 28~2F
    0x3330, 0x3331, 0x3332, 0x3333, 0x3334, 0x3335, 0x3336, 0x3337,     // 30~37
    0x3338, 0x3339, 0x3341, 0x3342, 0x3343, 0x3344, 0x3345, 0x3346,     // 38~3F
    0x3430, 0x3431, 0x3432, 0x3433, 0x3434, 0x3435, 0x3436, 0x3437,     // 40~47
    0x3438, 0x3439, 0x3441, 0x3442, 0x3443, 0x3444, 0x3445, 0x3446,     // 48~4F
    0x3530, 0x3531, 0x3532, 0x3533, 0x3534, 0x3535, 0x3536, 0x3537,     // 50~57
    0x3538, 0x3539, 0x3541, 0x3542, 0x3543, 0x3544, 0x3545, 0x3546,     // 58~5F
    0x3630, 0x3631, 0x3632, 0x3633, 0x3634, 0x3635, 0x3636, 0x3637,     // 60~67
    0x3638, 0x3639, 0x3641, 0x3642, 0x3643, 0x3644, 0x3645, 0x3646,     // 68~6F
    0x3730, 0x3731, 0x3732, 0x3733, 0x3734, 0x3735, 0x3736, 0x3737,     // 70~77
    0x3738, 0x3739, 0x3741, 0x3742, 0x3743, 0x3744, 0x3745, 0x3746,     // 78~7F
    0x3830, 0x3831, 0x3832, 0x3833, 0x3834, 0x3835, 0x3836, 0x3837,     // 80~87
    0x3838, 0x3839, 0x3841, 0x3842, 0x3843, 0x3844, 0x3845, 0x3846,     // 88~8F
    0x3930, 0x3931, 0x3932, 0x3933, 0x3934, 0x3935, 0x3936, 0x3937,     // 90~97
    0x3938, 0x3939, 0x3941, 0x3942, 0x3943, 0x3944, 0x3945, 0x3946,     // 98~9F
    0x4130, 0x4131, 0x4132, 0x4133, 0x4134, 0x4135, 0x4136, 0x4137,     // A0~A7
    0x4138, 0x4139, 0x4141, 0x4142, 0x4143, 0x4144, 0x4145, 0x4146,     // A8~AF
    0x4230, 0x4231, 0x4232, 0x4233, 0x4234, 0x4235, 0x4236, 0x4237,     // B0~B7
    0x4238, 0x4239, 0x4241, 0x4242, 0x4243, 0x4244, 0x4245, 0x4246,     // B8~BF
    0x4330, 0x4331, 0x4332, 0x4333, 0x4334, 0x4335, 0x4336, 0x4337,     // C0~C7
    0x4338, 0x4339, 0x4341, 0x4342, 0x4343, 0x4344, 0x4345, 0x4346,     // C8~CF
    0x4430, 0x4431, 0x4432, 0x4433, 0x4434, 0x4435, 0x4436, 0x4437,     // D0~D7
    0x4438, 0x4439, 0x4441, 0x4442, 0x4443, 0x4444, 0x4445, 0x4446,     // D8~DF
    0x4530, 0x4531, 0x4532, 0x4533, 0x4534, 0x4535, 0x4536, 0x4537,     // E0~E7
    0x4538, 0x4539, 0x4541, 0x4542, 0x4543, 0x4544, 0x4545, 0x4546,     // E8~EF
    0x4630, 0x4631, 0x4632, 0x4633, 0x4634, 0x4635, 0x4636, 0x4637,     // F0~F7
    0x4638, 0x4639, 0x4641, 0x4642, 0x4643, 0x4644, 0x4645, 0x4646,     // F8~FF
};
#endif

#define F16 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
static const char hex_lookup_256[256] = {
    // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
     F16,                                                                               // 00~0F
     F16,                                                                               // 10~1F
     F16,                                                                               // 20~2F
       0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  -1,  -1,  -1,  -1,  -1,  -1,    // 30~3F
      10,  11,  12,  13,  14,  15,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,    // 40~4F
     F16,                                                                               // 50~5F
      10,  11,  12,  13,  14,  15,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,    // 60~6F
     F16,                                                                               // 70~7F
     F16, F16, F16, F16, F16, F16, F16, F16                                             // 80~FF
};
#undef  F16

#define Z16 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
static const char escape_table[256] = {
    // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
      '0',  0,   0,   0,   0,   0,   0,   0,  'b', 't', 'n',   0, 'f', 'r',  0,   0,    // 00~0F
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    // 10~1F
       0,   0,  '"',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  '/',   // 20~2F
     Z16,                                                                               // 30~3F
     Z16,                                                                               // 40~4F
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  '\\', 0,   0,   0,    // 50~5F
     Z16,                                                                               // 60~6F
     Z16,                                                                               // 70~7F
     Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16                                             // 80~FF
};
#undef Z16

struct json_binary_utils
{
    json_binary_utils() = default;
    ~json_binary_utils() = default;

    static bool readFromFile(const std::string & filename, std::string & content) {
        bool bSuccess = false;
        std::ifstream ifs;
        try {
            if (filename.empty())
                return false;
            ifs.open(filename.c_str(), std::ios::in | std::ios_base::binary);
            if (ifs.is_open()) {
                ifs.seekg(0, std::ios_base::end);
                std::streampos spos_end = ifs.tellg();
                std::streamsize file_size = spos_end;
                file_size += 1;
                if ((std::streamsize)content.length() < file_size)
                    content.resize(file_size);
                if ((std::streamsize)content.length() < file_size) {
                    return (file_size == 0);
                }
                static const std::streamsize kReadSize = 65536;
                char * current = &content[0];
                char * content_end = current + content.capacity();
                std::streamsize read_total = 0;
                ifs.seekg(0, std::ios_base::beg);
                while (!ifs.eof()) {
                    ifs.read(current, kReadSize);
                    std::streamsize read_bytes = ifs.gcount();
#ifndef NDEBUG
                    //std::cout << "ReadBytes = " << read_bytes << std::endl;
#endif
                    current += read_bytes;
                    read_total += read_bytes;
                }
                content.resize(read_total);
                ifs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary_utils::readFromFile() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
    }

    static bool saveToFile(const std::string & filename, std::string & content, std::size_t content_size) {
        bool bSuccess = false;
        std::ofstream ofs;
        try {
            if (filename.empty())
                return false;
            ofs.open(filename.c_str(), std::ios::out | std::ios::trunc | std::ios_base::binary);
            if (ofs.is_open()) {
                static const std::streamsize kWriteSize = 65536;
                std::streamsize content_left = content_size;
                char * current = &content[0];
                char * content_end = current + content.capacity();
                std::streamsize write_bytes;
                std::streamsize written_total = 0;
                ofs.seekp(0, std::ios_base::beg);
                while (!ofs.eof()) {
                    if (content_left >= kWriteSize)
                        write_bytes = kWriteSize;
                    else
                        write_bytes = content_left;
                    ofs.write(current, write_bytes);
                    std::streampos written_bytes = ofs.tellp();
#ifndef NDEBUG
                    //std::cout << "WrittenBytes = " << written_bytes << std::endl;
#endif
                    current += written_bytes;
                    written_total += written_bytes;
                    content_left -= write_bytes;
                    if (content_left <= 0)
                        break;
                }
                ofs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary_utils::saveToFile() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
    }
};

namespace json_binary_hex
{
    static std::size_t get_encode_capacity(std::size_t data_length, bool add_quote = false) {
        std::size_t capacity = data_length;
        // One char convent to two Hex strings.
        capacity *= 2;
        // Add double quote and nullor format like: "xxxxxx" '\0'
        if (add_quote)
            capacity += 3;
        else
            capacity += 1;
        return capacity;
    }

    static std::size_t get_decode_capacity(std::size_t data_length, bool add_quote = false) {
        std::size_t capacity = data_length;
        // One char convent to two Hex strings.
        capacity = (capacity + 1) / 2;
        // Add double quote and nullor format like: "xxxxxx" '\0'
        if (add_quote)
            capacity += 3;
        else
            capacity += 1;
        return capacity;
    }

    // encode to upper HEX strings, like "AABBCCF0E5D9".
    static std::streamsize encode(const char * src, std::size_t src_len, char * buffer,
        std::size_t buf_size, bool fill_null = true) {
        assert(src != nullptr);
        assert(buffer != nullptr);
        const char * src_end = src + src_len;
        char * dest = buffer;
        if (((std::size_t)buffer & 02U) == 0) {
            unsigned short * dest16 = (unsigned short *)buffer;
            while (src < src_end) {
                unsigned char c = (unsigned char)(*src);
                *dest16++ = hex_upper_256[c];
                src++;
            }
            dest = (char * )dest16;
        }
        else {
            while (src < src_end) {
                unsigned char c = (unsigned char)(*src);
                *dest++ = hex_u_table[ c >> 4U   ];
                *dest++ = hex_u_table[ c & 0x0FU ];
                src++;
            }
        }
        if (fill_null)
            *dest = '\0';
        assert(dest >= buffer);
        return (dest - buffer);
    }

    // encode to lower HEX strings, like "aabbccf0e5d9".
    static std::streamsize encode_lower(const char * src, std::size_t src_len, char * buffer,
        std::size_t buf_size, bool fill_null = true) {
        assert(src != nullptr);
        assert(buffer != nullptr);
        const char * src_end = src + src_len;
        char * dest = buffer;
        while (src < src_end) {
            unsigned char c = (unsigned char)(*src);
            *dest++ = hex_l_table[ c >> 4U   ];
            *dest++ = hex_l_table[ c & 0x0FU ];
            src++;
        }
        if (fill_null)
            *dest = '\0';
        assert(dest >= buffer);
        return (dest - buffer);
    }

    // encode to upper HEX strings, like "AABBCCF0E5D9".
    static std::streamsize encode_std(const char * src, std::size_t src_len, char * buffer,
        std::size_t buf_size, bool fill_null = true) {
        assert(src != nullptr);
        assert(buffer != nullptr);
        const char * src_end = src + src_len;
        char * dest = buffer;
        while (src < src_end) {
            unsigned char c = (unsigned char)(*src);
            unsigned int hex1, hex2;
            hex1 = c >> 4;
            hex2 = c & 0x0FU;
            if (hex1 <= 9)
                *dest = hex1 + '0';
            else
                *dest = hex1 - 10 + 'A';
            if (hex2 <= 9)
                *(dest + 1) = hex2 + '0';
            else
                *(dest + 1) = hex2 - 10 + 'A';
            dest += 2;
            src++;
        }
        if (fill_null)
            *dest = '\0';
        assert(dest >= buffer);
        return (dest - buffer);
    }

    static std::streamsize encode(const char * src, std::size_t src_len, std::string & dest, bool fill_null = true) {
        std::size_t alloc_size = json_binary_hex::get_encode_capacity(src_len);
        dest.resize(alloc_size);
        char * buffer = &dest[0];
        std::streamsize encode_size = json_binary_hex::encode(src, src_len, buffer, dest.capacity(), fill_null);
        if (encode_size >= 0) {
            if (fill_null)
                dest.resize(encode_size + 1);
            else
                dest.resize(encode_size);
        }
        return encode_size;
    }

    static std::streamsize encode(const std::string & src, std::string & dest, bool fill_null = true) {
        return json_binary_hex::encode(src.c_str(), src.length(), dest, fill_null);
    }

    static std::streamsize decode(const char * src, std::size_t src_len, char * buffer,
        std::size_t buf_size, bool fill_null = false) {
        assert(src != nullptr);
        assert(buffer != nullptr);
        // src_len must be multiply of 2.
        if ((src_len & 1UL) != 0) {
            std::cout << "Error: json_binary_hex::decode(), src_len = " << src_len << " is not multiply of 2." << std::endl;
            return -2;
        }
        src_len -= (src_len & 1U);
        const char * src_end = src + src_len;
        char * dest = buffer;
        while (src < src_end) {
            unsigned int hex, hex1 = 0, hex2 = 0;
            unsigned int c1, c2;
            // High Hex charactor
            c1 = (unsigned int)(unsigned char)(*src);
            if (c1 != -1) {
                hex1 = hex_lookup_256[c1];
            }
            else {
                // Error: include non-hex chars.
                std::cout << "Error: json_binary_hex::decode(), high Hex include non-hex chars: [" << c1 << "]." << std::endl;
                return -1;
            }
            // Low Hex charactor
            c2 = (unsigned int)(unsigned char)(*(src + 1));
            if (c1 != -1) {
                hex2 = hex_lookup_256[c2];
            }
            else {
                // Error: include non-hex chars.
                std::cout << "Error: json_binary_hex::decode(), low Hex include non-hex chars: [" << c2 << "]." << std::endl;
                return -1;
            }
            hex = (hex1 << 4) | hex2;
            *dest++ = hex;
            src += 2;
        }
        if (fill_null)
            *dest = '\0';
        assert(dest >= buffer);
        return (dest - buffer);
    }

    static std::streamsize decode_std(const char * src, std::size_t src_len, char * buffer,
        std::size_t buf_size, bool fill_null = false) {
        assert(src != nullptr);
        assert(buffer != nullptr);
        // src_len must be multiply of 2.
        if ((src_len & 1UL) != 0) {
            std::cout << "Error: binary_hex::decode_std(), src_len = " << src_len << " is not multiply of 2." << std::endl;
            return -2;
        }
        src_len -= (src_len & 1UL);
        const char * src_end = src + src_len;
        char * dest = buffer;
        while (src < src_end) {
            unsigned int hex, hex1 = 0, hex2 = 0;
            unsigned int c1, c2;
            // High Hex charactor
            c1 = (unsigned int)(unsigned char)(*src);
            if (c1 <= '9' && c1 >= '0') {
                hex1 = c1 - '0';
            }
            else if (c1 <= 'F' && c1 >= 'A') {
                hex1 = c1 - 'A';
            }
            else if (c1 <= 'f' && c1 >= 'a') {
                hex1 = c1 - 'a';
            }
            else {
                // Error: include non-hex chars.
                std::cout << "Error: binary_hex::decode_std(), high Hex include non-hex chars: [" << c1 << "]." << std::endl;
                return -1;
            }
            // Low Hex charactor
            c2 = (unsigned int)(unsigned char)(*(src + 1));
            if (c2 <= '9' && c2 >= '0') {
                hex2 = c2 - '0';
            }
            else if (c2 <= 'F' && c2 >= 'A') {
                hex2 = c2 - 'A';
            }
            else if (c2 <= 'f' && c2 >= 'a') {
                hex2 = c2 - 'a';
            }
            else {
                // Error: include non HEX chars.
                std::cout << "Error: binary_hex::decode_std(), low Hex include non-hex chars: [" << c2 << "]." << std::endl;
                return -1;
            }
            hex = (hex1 << 4) | hex2;
            *dest++ = hex;
            src += 2;
        }
        if (fill_null)
            *dest = '\0';
        assert(dest >= buffer);
        return (dest - buffer);
    }

    static std::streamsize decode(const char * src, std::size_t src_len, std::string & dest, bool fill_null = false) {
        std::size_t alloc_size = json_binary_hex::get_decode_capacity(src_len);
        dest.resize(alloc_size);
        char * buffer = &dest[0];
        std::streamsize decode_size = json_binary_hex::decode(src, src_len, buffer, dest.capacity(), fill_null);
        if (decode_size >= 0) {
            if (fill_null)
                dest.resize(decode_size);
            else
                dest.resize(decode_size + 1);
        }
        return decode_size;
    }

    static std::streamsize decode(const std::string & src, std::string & dest, bool fill_null = false) {
        return json_binary_hex::decode(src.c_str(), src.length(), dest, fill_null);
    }

    static bool encodeFromFile(const std::string & filename, std::string & content, bool add_quote = false) {
        bool bSuccess = false;
        std::ifstream ifs;
        try {
            if (filename.empty())
                return false;
            ifs.open(filename.c_str(), std::ios::in | std::ios_base::binary);
            if (ifs.is_open()) {
                ifs.seekg(0, std::ios_base::end);
                std::streampos spos_end = ifs.tellg();
                std::streamsize file_size = spos_end;
                
                std::string json_bin;
                std::size_t json_size;
                json_size = file_size * 2 + 1;
                if (add_quote)
                    json_size += 3;
                json_bin.resize(json_size);

                static const std::streamsize kReadSize = 65536;
                char buffer[kReadSize];
                char * json_start = &json_bin[0];
                char * json_end = json_start + json_bin.capacity();
                char * json = json_start;
                std::streamsize json_capacity = (std::streamsize)json_bin.capacity();
                std::size_t encode_total = 0;
                if (add_quote)
                    *json++ = '\"';
                ifs.seekg(0, std::ios_base::beg);
                while (!ifs.eof()) {
                    ifs.read(buffer, kReadSize);
                    std::streamsize read_bytes = ifs.gcount();
                    assert(json <= json_end);
                    std::size_t encode_bytes = json_binary_hex::encode(buffer, read_bytes, json, json_capacity, false);
                    encode_total += encode_bytes;
#ifndef NDEBUG
                    //std::cout << "ReadBytes = " << read_bytes << ", EscapeBytes = " << encode_bytes << std::endl;
#endif
                    json += encode_bytes;
                    json_capacity -= encode_bytes;
                    if (json_capacity <= 0)
                        break;
                }
                if (add_quote) {
                    *json++ = '\"';
                    *json = '\0';
                }
                std::size_t encode_size = json - json_start;
                if (json >= json_start)
                    json_bin.resize(encode_size);
                json_bin.resize(encode_size);
                content.swap(json_bin);
                ifs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary_hex::encodeFromFile() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
    }

    static bool encodeStdFromFile(const std::string & filename, std::string & content, bool add_quote = false) {
        bool bSuccess = false;
        std::ifstream ifs;
        try {
            if (filename.empty())
                return false;
            ifs.open(filename.c_str(), std::ios::in | std::ios_base::binary);
            if (ifs.is_open()) {
                ifs.seekg(0, std::ios_base::end);
                std::streampos spos_end = ifs.tellg();
                std::streamsize file_size = spos_end;
                
                std::string json_bin;
                std::size_t json_size;
                json_size = file_size * 2 + 1;
                if (add_quote)
                    json_size += 3;
                json_bin.resize(json_size);

                static const std::streamsize kReadSize = 65536;
                char buffer[kReadSize];
                char * json_start = &json_bin[0];
                char * json_end = json_start + json_bin.capacity();
                char * json = json_start;
                std::streamsize json_capacity = (std::streamsize)json_bin.capacity();
                std::size_t encode_total = 0;
                if (add_quote)
                    *json++ = '\"';
                ifs.seekg(0, std::ios_base::beg);
                while (!ifs.eof()) {
                    ifs.read(buffer, kReadSize);
                    std::streamsize read_bytes = ifs.gcount();
                    assert(json <= json_end);
                    std::size_t encode_bytes = json_binary_hex::encode_std(buffer, read_bytes, json, json_capacity, false);
                    encode_total += encode_bytes;
#ifndef NDEBUG
                    //std::cout << "ReadBytes = " << read_bytes << ", EscapeBytes = " << encode_bytes << std::endl;
#endif
                    json += encode_bytes;
                    json_capacity -= encode_bytes;
                    if (json_capacity <= 0)
                        break;
                }
                if (add_quote) {
                    *json++ = '\"';
                    *json = '\0';
                }
                std::size_t encode_size = json - json_start;
                if (json >= json_start)
                    json_bin.resize(encode_size);
                json_bin.resize(encode_size);
                content.swap(json_bin);
                ifs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary_hex::encodeFromFile() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
    }
};

enum double_escape_t {
    single_escape = false,
    double_escape = true
};

template <bool isDoubleEscape = single_escape>
struct json_binary
{
    json_binary() = default;
    ~json_binary() = default;

    static std::size_t encode(const char * data, std::size_t data_len,
        char * json, std::size_t max_size, bool fill_null = true) {
        char * src = const_cast<char *>(data);
        char * src_end = src + data_len;
        char * dest = json;
        char * dest_max = json + max_size;
        if (fill_null)
            dest_max -= 1;
        // The json buffer size must be large than data length.
        assert(max_size >= data_len);
        // dest can not overflow dest_max forever.
        while (src < src_end) {
            char c = *src;
            char escape = escape_table[c];
            if (escape == 0) {
                *dest++ = c;
                assert(dest <= dest_max);
                src++;
            }
            else {
                if (isDoubleEscape) {
                    // '\\', '\"', '\/'
                    if (c > 32)
                        *dest++ = '\\';
                    *dest++ = '\\';
                }
                *dest++ = '\\';
                *dest++ = escape;
                assert(dest <= dest_max);
                src++;
            }
        }
        assert(dest != nullptr);
        if (fill_null)
            *dest = '\0';
        assert(dest >= json);
        return (dest - json);
    }

    static std::size_t encode_safe(const char * data, std::size_t data_len,
        char * json, std::size_t max_size, bool fill_null = true) {
        char * src = const_cast<char *>(data);
        char * src_end = src + data_len;
        char * dest = json;
        char * dest_max = json + max_size;;
        if (fill_null)
            dest_max -= 1;
        // The json buffer size can less than the data length.
        while (src < src_end) {
            char c = *src;
            char escape = escape_table[c];
            if (escape == 0) {
                *dest++ = c;
                if (dest >= dest_max) {
                    dest = dest_max;
                    break;
                }
                src++;
            }
            else {
                if (isDoubleEscape) {
                    // '\\', '\"', '\/'
                    if (c > 32)
                        *dest++ = '\\';
                    *dest++ = '\\';
                }
                *dest++ = '\\';
                *dest++ = escape;
                if (dest >= dest_max) {
                    dest = dest_max;
                    break;
                }
                src++;
            }
        }
        assert(dest != nullptr);
        if (fill_null)
            *dest = '\0';
        assert(dest >= json);
        return (dest - json);
    }

    static std::size_t encode(const char * src, std::size_t src_len,
        char * dest, std::size_t offset, std::size_t max_size,
        bool fill_null = true) {
        return json_binary::encode(src, src_len, dest + offset, max_size, fill_null);
    }

    static std::size_t encode(const std::string::const_iterator & src_begin,
        const std::string::const_iterator & src_end,
        std::string::iterator & dest_begin, std::string::iterator & dest_end,
        bool fill_null = true) {
        std::string::const_iterator src;
        std::string::iterator & dest = dest_begin;
        // dest must be not overflow dest_end forever.
        for (src = src_begin; src != src_end; ++src) {
            char c = *src;
            char escape = escape_table[c];
            if (escape == 0) {
                *dest++ = c;
                assert(dest != dest_end);
                src++;
            }
            else {
                if (isDoubleEscape) {
                    // '\\', '\"', '\/'
                    if (c > 32)
                        *dest++ = '\\';
                    *dest++ = '\\';
                }
                *dest++ = '\\';
                *dest++ = escape;
                assert(dest != dest_end);
                src++;
            }
        }
        if (fill_null)
            *dest = '\0';
        return (dest - dest_begin);
    }

    static std::size_t encode(const std::string & src, std::size_t src_len,
        std::string & dest, std::size_t offset, std::size_t max_size,
        bool fill_null = true) {
        std::string::iterator & dest_begin = dest.begin() + offset;
        std::string::iterator & dest_end = dest_begin + max_size;
        std::string::const_iterator & src_begin = src.begin();
        std::string::const_iterator & src_end = src_begin + src_len;
        return json_binary::encode(src_begin, src_end, dest_begin, dest_end, fill_null);
    }

    static std::size_t encode(const std::string & src, std::size_t src_len,
        std::string & dest, std::size_t max_size, bool fill_null = true) {
        return json_binary::encode(src, src_len, dest, 0, max_size, fill_null);
    }

    static bool decode(std::string & content, const std::string & filename) {
        return true;
    }

    static bool encodeFromFile(const std::string & filename, std::string & content, bool add_quote = false) {
        bool bSuccess = false;
        std::ifstream ifs;
        try {
            if (filename.empty())
                return false;
            ifs.open(filename.c_str(), std::ios::in | std::ios_base::binary);
            if (ifs.is_open()) {
                ifs.seekg(0, std::ios_base::end);
                std::streampos spos_end = ifs.tellg();
                std::streamsize file_size = spos_end;

                std::string json_bin;
                std::size_t json_size;
                if (isDoubleEscape)
                    json_size = file_size * 4 + 1;
                else
                    json_size = file_size * 2 + 1;
                if (add_quote)
                    json_size += 3;
                json_bin.resize(json_size);

                static const std::streamsize kReadSize = 65536;
                char buffer[kReadSize];
                char * json_start = &json_bin[0];
                char * json_end = json_start + json_bin.capacity();
                char * json = json_start;
                std::streamsize json_capacity = (std::streamsize)json_bin.capacity();
                std::size_t encode_total = 0;
                if (add_quote)
                    *json++ = '\"';
                ifs.seekg(0, std::ios_base::beg);
                while (!ifs.eof()) {
                    ifs.read(buffer, kReadSize);
                    std::streamsize read_bytes = ifs.gcount();
                    assert(json <= json_end);
                    std::size_t encode_bytes = json_binary::encode(buffer, read_bytes, json, json_capacity, false);
                    encode_total += encode_bytes;
#ifndef NDEBUG
                    //std::cout << "ReadBytes = " << read_bytes << ", EscapeBytes = " << encode_bytes << std::endl;
#endif
                    json += encode_bytes;
                    json_capacity -= encode_bytes;
                    if (json_capacity <= 0)
                        break;
                }
                if (add_quote) {
                    *json++ = '\"';
                    *json = '\0';
                }
                std::size_t encode_size = json - json_start;
                if (json >= json_start)
                    json_bin.resize(encode_size);
                content.swap(json_bin);
                ifs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary::encodeFromFile() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
    }
};
