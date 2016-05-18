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

#define Z16 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
static const char escape_table[256] = {
    // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
      '0',  0,   0,   0,   0,   0,   0,   0,  'b', 't', 'n',   0, 'f', 'r',  0,   0,    // 00~0F
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    // 10~1F
       0,   0,  '"',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  '/',   // 20~2F
     Z16,                                                                               // 30~3F
     Z16,                                                                               // 40~4F
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  '\\', 0,   0,   0,    // 50~5F

     Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16                                   // 60~FF
};
#undef Z16

struct binary_hex
{
    binary_hex() = default;
    ~binary_hex() = default;
};

enum double_escape_t {
    single_escape = false,
    double_escape = true
};

template <bool DoubleEscape = single_escape>
struct json_binary
{
    json_binary() = default;
    ~json_binary() = default;

    static std::size_t encode(char * json, std::size_t max_size, const char * data,
        std::size_t length, bool fill_null = true) {
        char * src = const_cast<char *>(data);
        char * src_end = src + length;
        char * dest = json;
        char * dest_max = json + max_size;
        if (fill_null)
            dest_max -= 1;
        // The json buffer size must be large than data length.
        assert(max_size >= length);
        // dest can not overflow dest_max forever.
        while (src < src_end) {
            char ch = *src;
            char escape = escape_table[ch];
            if (escape == 0) {
                *dest++ = ch;
                assert(dest <= dest_max);
                src++;
            }
            else {
                if (DoubleEscape) {
                    // '\\', '\"', '\/'
                    if (ch > 32)
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
            *dest++ = '\0';
        assert(dest >= json);
        return (dest - json);
    }

    static std::size_t encode_safe(char * json, std::size_t max_size, const char * data,
        std::size_t length, bool fill_null = true) {
        char * src = const_cast<char *>(data);
        char * src_end = src + length;
        char * dest = json;
        char * dest_max = json + max_size;;
        if (fill_null)
            dest_max -= 1;
        // The json buffer size can less than the data length.
        while (src < src_end) {
            char ch = *src;
            char escape = escape_table[ch];
            if (escape == 0) {
                *dest++ = ch;
                if (dest >= dest_max) {
                    dest = dest_max;
                    break;
                }
                src++;
            }
            else {
                if (DoubleEscape) {
                    // '\\', '\"', '\/'
                    if (ch > 32)
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
            *dest++ = '\0';
        assert(dest >= json);
        return (dest - json);
    }

    static std::size_t encode(char * dest, std::size_t offset, std::size_t max_size,
        const char * src, std::size_t length, bool fill_null = true) {
        return json_binary::encode(dest + offset, max_size, src, length, fill_null);
    }

    static std::size_t encode(std::string::iterator & dest_begin, std::string::iterator & dest_end,
        const std::string::const_iterator & src_begin, const std::string::const_iterator & src_end,
        bool fill_null = true) {
        std::string::const_iterator src;
        std::string::iterator & dest = dest_begin;
        // dest must be not overflow dest_end forever.
        for (src = src_begin; src != src_end; ++src) {
            char ch = *src;
            char escape = escape_table[ch];
            if (escape == 0) {
                *dest++ = ch;
                assert(dest != dest_end);
                src++;
            }
            else {
                if (DoubleEscape) {
                    // '\\', '\"', '\/'
                    if (ch > 32)
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
            *dest++ = '\0';
        return (dest - dest_begin);
    }

    static std::size_t encode(std::string & dest, std::size_t offset, std::size_t max_size,
        const std::string & src, std::size_t length, bool fill_null = true) {
        std::string::iterator & dest_begin = dest.begin() + offset;
        std::string::iterator & dest_end = dest_begin + max_size;
        std::string::const_iterator & src_begin = src.begin();
        std::string::const_iterator & src_end = src_begin + length;
        return json_binary::encode(dest_begin, dest_end, src_begin, src_end, fill_null);
    }

    static std::size_t encode(std::string & dest, std::size_t max_size,
        const std::string & src, std::size_t length, bool fill_null = true) {
        return json_binary::encode(dest, 0, max_size, src, length, fill_null);
    }

    static bool decode(std::string & content, const std::string & filename) {
        return true;
    }

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
                    std::cout << "ReadBytes = " << read_bytes << std::endl;
                    current += read_bytes;
                    read_total += read_bytes;
                }
                content_size = read_total;
                ifs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary::readFromFile() Exception: " << e.what() << std::endl;
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
                    std::cout << "WrittenBytes = " << written_bytes << std::endl;
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
            std::cout << "json_binary::saveToFile() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
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
                file_size += 1;
                if ((std::streamsize)content.capacity() < file_size)
                    content.reserve(file_size);
                if ((std::streamsize)content.capacity() < file_size) {
                    return (file_size == 0);
                }
                static const std::streamsize kReadSize = 65536;
                std::string json_bin;
                std::size_t json_size;
                if (DoubleEscape)
                    json_size = file_size * 4 + 1;
                else
                    json_size = file_size * 2 + 1;
                if (add_quote)
                    json_size += 3;
                json_bin.reserve(json_size);
                char * current = &content[0];
                char * content_end = current + content.capacity();
                char * json_start = &json_bin[0];
                char * json_end = json_start + json_bin.capacity();
                char * json = json_start;
                std::size_t encode_total = 0;
                if (add_quote)
                    *json++ = '\"';
                ifs.seekg(0, std::ios_base::beg);
                while (!ifs.eof()) {
                    ifs.read(current, kReadSize);
                    std::streamsize read_bytes = ifs.gcount();
                    assert(json <= json_end);
                    assert(current <= content_end);
                    std::size_t encode_bytes = json_binary::encode(json, json_end - json, current, read_bytes, false);
                    encode_total += encode_bytes;
                    std::cout << "ReadBytes = " << read_bytes << ", EscapeBytes = " << encode_bytes << std::endl;
                    current += read_bytes;
                    json += encode_bytes;
                    if (json > json_end)
                        break;
                }
                if (add_quote) {
                    *json++ = '\"';
                    *json = '\0';
                }
                encode_size = json - json_start;
                content.swap(json_bin);
                ifs.close();
                bSuccess = true;
            }
        }
        catch (const std::exception & e) {
            std::cout << "json_binary::readFromFileAndEncode() Exception: " << e.what() << std::endl;
        }
        return bSuccess;
    }
};
