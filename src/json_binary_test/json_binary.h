#pragma once

#include <assert.h>

#include <string>
#include <iostream>
#include <fstream>

static const char hex_u_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'
};

static const char hex_l_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
};

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

    static bool readFromFile(const std::string & filename, std::string & content, std::size_t & content_size) {
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
                    std::cout << "ReadBytes = " << read_bytes << std::endl;
#endif
                    current += read_bytes;
                    read_total += read_bytes;
                }
                content_size = read_total;
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
                    std::cout << "WrittenBytes = " << written_bytes << std::endl;
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

struct json_binary_hex
{
    json_binary_hex() = default;
    ~json_binary_hex() = default;

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
        while (src < src_end) {
            unsigned char c = (unsigned char)(*src);
            *dest++ = hex_u_table[ c >> 4U   ];
            *dest++ = hex_u_table[ c & 0x0FU ];
            src++;
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

    static std::streamsize decode(const char * src, std::size_t src_len, char * buffer,
        std::size_t buf_size, bool fill_null = true) {
        assert(src != nullptr);
        assert(buffer != nullptr);
        // src_len must be multiply of 2.
        if ((src_len & 1UL) != 0) {
            std::cout << "Error: binary_hex::decode(), src_len = " << src_len << " is not multiply of 2." << std::endl;
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
                std::cout << "Error: binary_hex::decode(), high Hex include non-hex chars: [" << c1 << "]." << std::endl;
                return -1;
            }
            // Low Hex charactor
            c2 = (unsigned int)(unsigned char)(*(src + 1));
            if (c1 != -1) {
                hex2 = hex_lookup_256[c2];
            }
            else {
                // Error: include non-hex chars.
                std::cout << "Error: binary_hex::decode(), low Hex include non-hex chars: [" << c2 << "]." << std::endl;
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
        std::size_t buf_size, bool fill_null = true) {
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

    static bool encodeFromFile(const std::string & filename, std::string & content,
        std::size_t & encode_size, bool add_quote = false) {
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
                    std::cout << "ReadBytes = " << read_bytes << ", EscapeBytes = " << encode_bytes << std::endl;
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
                encode_size = json - json_start;
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

    static std::size_t encode(const std::string & src, std::size_t length,
        std::string & dest, std::size_t offset, std::size_t max_size,
        bool fill_null = true) {
        std::string::iterator & dest_begin = dest.begin() + offset;
        std::string::iterator & dest_end = dest_begin + max_size;
        std::string::const_iterator & src_begin = src.begin();
        std::string::const_iterator & src_end = src_begin + length;
        return json_binary::encode(src_begin, src_end, dest_begin, dest_end, fill_null);
    }

    static std::size_t encode(std::string & dest, std::size_t max_size,
        const std::string & src, std::size_t length, bool fill_null = true) {
        return json_binary::encode(dest, 0, max_size, src, length, fill_null);
    }

    static bool decode(std::string & content, const std::string & filename) {
        return true;
    }

    static bool encodeFromFile(const std::string & filename, std::string & content,
        std::size_t & encode_size, bool add_quote = false) {
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
                    std::cout << "ReadBytes = " << read_bytes << ", EscapeBytes = " << encode_bytes << std::endl;
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
                encode_size = json - json_start;
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
