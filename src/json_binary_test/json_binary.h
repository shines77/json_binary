#pragma once

#include <assert.h>

#include <string>
#include <iostream>
#include <fstream>
#include <exception>

#include "json_hex16.h"
#include "bin_escape.h"

namespace utils {

static bool readFromFile(const std::string & filename, std::string & content,
                         std::size_t buffer_size = 32768) {
    bool bSuccess = false;
    if (filename.empty())
        return false;
    try {
        std::ifstream ifs;
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
            char * current = &content[0];
            char * content_end = current + content.capacity();
            std::streamsize read_total = 0;
            ifs.seekg(0, std::ios_base::beg);
            while (!ifs.eof()) {
                ifs.read(current, buffer_size);
                std::streamsize read_bytes = ifs.gcount();
                current += read_bytes;
                read_total += read_bytes;
            }
#ifndef NDEBUG
            std::cout << "totalReadBytes = " << read_total << std::endl;
#endif
            content.resize(read_total);
            ifs.close();
            bSuccess = true;
        }
    }
    catch (const std::exception & e) {
        std::cout << "utils::readFromFile() Exception: " << e.what() << std::endl;
    }
    return bSuccess;
}

static bool saveToFile(const std::string & filename, std::string & content,
                       std::size_t content_size, std::size_t buffer_size = 32768) {
    bool bSuccess = false;
    if (filename.empty())
        return false;
    try {
        std::ofstream ofs;
        ofs.open(filename.c_str(), std::ios::out | std::ios::trunc | std::ios_base::binary);
        if (ofs.is_open()) {
            std::streamsize content_left = content_size;
            char * current = &content[0];
            char * content_end = current + content.capacity();
            std::streamsize write_bytes;
            std::streamsize written_total = 0;
            ofs.seekp(0, std::ios_base::beg);
            while (!ofs.eof()) {
                if (content_left >= (std::streamsize)buffer_size)
                    write_bytes = buffer_size;
                else
                    write_bytes = content_left;
                ofs.write(current, write_bytes);
                std::streampos written_bytes = ofs.tellp();
                current += written_bytes;
                written_total += written_bytes;
                content_left -= write_bytes;
                if (content_left <= 0)
                    break;
            }
#ifndef NDEBUG
            std::cout << "TotalWrittenBytes = " << written_total << std::endl;
#endif
            ofs.close();
            bSuccess = true;
        }
    }
    catch (const std::exception & e) {
        std::cout << "utils::saveToFile() Exception: " << e.what() << std::endl;
    }
    return bSuccess;
}

} // namespace utils

namespace json_binary_hex {

static std::size_t get_encode_capacity(std::size_t data_length, bool add_quote = false) {
    std::size_t capacity;
    // One char convent to two Hex strings.
    capacity = data_length * 2 + 1;
    // Add double quote and nullor format like: "xxxxxx" '\0'
    if (add_quote)
        capacity += 2;
    return capacity;
}

static std::size_t get_decode_capacity(std::size_t data_length) {
    std::size_t capacity;
    // One char convent to two Hex strings.
    capacity = (data_length + 1) / 2;
    return capacity;
}

// encode to upper HEX strings, like "AABBCCF0E5D9".
static std::streamsize encode(const char * src, std::size_t src_len, char * buffer,
    std::size_t buf_size, bool fill_null = true) {
    assert(src != nullptr);
    assert(buffer != nullptr);
    const char * src_end = src + src_len;
    char * dest = buffer;
    if (((std::size_t)buffer & 0x02U) == 0) {
        unsigned short * dest16 = (unsigned short *)buffer;
        while (src < src_end) {
            unsigned char c = (unsigned char)(*src);
            *dest16++ = hex_upper_256_little_endian[c];
            src++;
        }
        dest = (char * )dest16;
    }
    else {
        while (src < src_end) {
            unsigned char c = (unsigned char)(*src);
            *dest++ = hex_u_tbl_16[ c >> 4U   ];
            *dest++ = hex_u_tbl_16[ c & 0x0FU ];
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
        *dest++ = hex_l_tbl_16[ c >> 4U   ];
        *dest++ = hex_l_tbl_16[ c & 0x0FU ];
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
        hex1 = c >> 4U;
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
    if (encode_size >= 0)
        dest.resize(encode_size);
    else
        dest.clear();
    return encode_size;
}

static std::streamsize encode(const std::string & src, std::string & dest, bool fill_null = true) {
    return json_binary_hex::encode(src.c_str(), src.length(), dest, fill_null);
}

static std::streamsize decode(const char * src, std::size_t src_len, char * buffer, std::size_t buf_size) {
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
        unsigned char hex, hex1, hex2;
        unsigned char c1, c2;
        // High Hex charactor
        c1 = (unsigned char)(*src);
        hex1 = hex_lookup_256[c1];
        if (hex1 == (unsigned char)(char)(-1)) {
            // Error: include non-hex chars.
            std::cout << "Error: json_binary_hex::decode(), high Hex include non-hex chars: [" << c1 << "]." << std::endl;
            return -1;
        }
        // Low Hex charactor
        c2 = (unsigned char)(*(src + 1));
        hex2 = hex_lookup_256[c2];
        if (hex2 == (unsigned char)(char)(-1)) {
            // Error: include non-hex chars.
            std::cout << "Error: json_binary_hex::decode(), low Hex include non-hex chars: [" << c2 << "]." << std::endl;
            return -1;
        }
        hex = (hex1 << 4) | hex2;
        *dest++ = hex;
        src += 2;
    }
    assert(dest >= buffer);
    return (dest - buffer);
}

static std::streamsize decode_std(const char * src, std::size_t src_len, char * buffer, std::size_t buf_size) {
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
        unsigned int hex, hex1, hex2;
        unsigned char c1, c2;
        // High Hex charactor
        c1 = (unsigned char)(*src);
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
        c2 = (unsigned char)(*(src + 1));
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
        *dest++ = (unsigned char)hex;
        src += 2;
    }
    assert(dest >= buffer);
    return (dest - buffer);
}

static std::streamsize decode(const char * src, std::size_t src_len, std::string & dest) {
    std::size_t alloc_size = json_binary_hex::get_decode_capacity(src_len);
    dest.resize(alloc_size);
    char * buffer = &dest[0];
    std::streamsize decode_size = json_binary_hex::decode(src, src_len, buffer, dest.capacity());
    if (decode_size >= 0)
        dest.resize(decode_size);
    else
        dest.clear();
    return decode_size;
}

static std::streamsize decode(const std::string & src, std::string & dest) {
    return json_binary_hex::decode(src.c_str(), src.length(), dest);
}

static std::string decode(const std::string & src) {
    std::string dest;
    std::size_t alloc_size = json_binary_hex::get_decode_capacity(src.length());
    dest.resize(alloc_size);
    char * buffer = &dest[0];
    std::streamsize decode_size = hex16_decode(src.c_str(), src.length(), buffer, dest.capacity());
    if (decode_size >= 0)
        dest.resize(decode_size);
    else
        dest.clear();
    return dest;
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

} // namespace json_binary_hex

enum double_escape_t {
    one_escape = false,
    twice_escape = true
};

template <bool isTwiceEscape = false>
struct json_binary
{
    json_binary() = default;
    ~json_binary() = default;

    static std::size_t encode(const char * data, std::size_t data_len,
        char * json, std::size_t max_size, bool fill_null = true) {
        unsigned char * src = reinterpret_cast<unsigned char *>(const_cast<char *>(data));
        unsigned char * src_end = src + data_len;
        char * dest = json;
        char * dest_max = json + max_size;
        if (fill_null)
            dest_max -= 1;
        // The json buffer size must be large than data length.
        assert(max_size >= data_len);
        // dest can not overflow dest_max forever.
        while (src < src_end) {
            unsigned char c = *src;
            unsigned char escape = escape_table_256[c];
            if (escape == 0) {
                *dest++ = c;
                assert(dest <= dest_max);
                src++;
            }
            else {
                if (isTwiceEscape) {
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
        unsigned char * src = reinterpret_cast<unsigned char *>(const_cast<char *>(data));
        unsigned char * src_end = src + data_len;
        char * dest = json;
        char * dest_max = json + max_size;;
        if (fill_null)
            dest_max -= 1;
        // The json buffer size can less than the data length.
        while (src < src_end) {
            unsigned char c = *src;
            unsigned char escape = escape_table_256[c];
            if (escape == 0) {
                *dest++ = c;
                if (dest >= dest_max) {
                    dest = dest_max;
                    break;
                }
                src++;
            }
            else {
                if (isTwiceEscape) {
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

    static std::size_t encode(const char * src, std::size_t src_len,
        std::string & dest, bool fill_null = true) {
        char * dest_ptr = &dest[0];
        return json_binary::encode(src, src_len, dest_ptr, std::string::max_size(), fill_null);
    }

    static std::size_t encode(const std::string & src, std::string & dest, bool fill_null = true) {
        return json_binary::encode(src, src.length(), dest, std::string::max_size(), fill_null);
    }

    static std::size_t decode_one_escape(const char * json, std::size_t json_len,
    char * dest, std::size_t max_size, bool fill_null = true, bool skip_quote = false) {
        unsigned char * src = reinterpret_cast<unsigned char *>(const_cast<char *>(data));
        unsigned char * src_end = src + data_len;
        char * dest_start = dest;
        char * dest_max = dest + max_size;
        if (fill_null)
            dest_max -= 1;
        // The dest json decode buffer size must be less than src json length.
        assert(max_size >= json_len);
        // dest can not overflow dest_max forever.
        while (src < src_end) {
            unsigned char c = *src;
            if (c != '\\') {
                // c is non '\\'.
                *dest++ = c;
                src++;
            }
            else {
                // c is '\\'.
                src++;
                unsigned char e = *src;
                char unescape = unescape_table_256[e];
                if (unescape != 1) {
                    // It's a valid unescape char.
                    *dest++ = unescape;
                    src++;
                }
                else {
                    // Error: Parse string escape invalid.
                }
            }
        }
        assert(dest != nullptr);
        if (fill_null)
            *dest = '\0';
        assert(dest >= dest_start);
        return (dest - dest_start);
    }

    static std::size_t decode_twice_escape(const char * json, std::size_t json_len,
        char * dest, std::size_t max_size, bool fill_null = true, bool skip_quote = false) {
        unsigned char * src = reinterpret_cast<unsigned char *>(const_cast<char *>(data));
        unsigned char * src_end = src + data_len;
        char * dest_start = dest;
        char * dest_max = dest + max_size;
        if (fill_null)
            dest_max -= 1;
        // The dest json decode buffer size must be less than src json length.
        assert(max_size >= json_len);
        // dest can not overflow dest_max forever.
        if (skip_quote && (*src == '\"'))
            src++;
        while (src < src_end) {
            unsigned char c = *src;
            unsigned char e, unescape;
            if (c != '\\') {
                // c is non '\\'.
                *dest++ = c;
                src++;
            }
            else if (skip_quote && (c == '\"')) {
                src++;
                break;
            }
            else {
                // c is '\\'.
                src++;
                c = *src;
                if (c == '\\') {
                    // src is two "\\".
                    src++;
                    e = *src;
                    if (e != '\\') {
                        // "\\x" -> "\x", "\n", "\r"
                        unescape = unescape_table_256[e];
                        if (unescape != 1) {
                            // It's a valid unescape char.
                            *dest++ = unescape;
                        }
                        else {
                            // Error: Parse string escape invalid.
                        }
                        src++;
                    }
                    else {
                        // src is three "\\".
                        // "\\\x" -> "\/", "\"", "\\"
                        src++;
                        e = *src;
                        unescape = unescape_table_256[e];
                        if (unescape != 1) {
                            // It's a valid unescape char.
                            *dest++ = unescape;
                        }
                        else {
                            // Error: Parse string escape invalid.
                        }
                        src++;
                    }
                }
                else {
                    // Error: Parse string escape invalid. --> "\x"
                }
            }
        }
        assert(dest != nullptr);
        if (fill_null)
            *dest = '\0';
        assert(dest >= dest_start);
        return (dest - dest_start);
    }

    static std::size_t decode(const char * src, std::size_t src_len, const std::string & dest, bool fill_null = true) {
        char * dest_buf = &dest[0];
        return decode(src, src_len, dest_buf, std::string::max_size(), fill_null);
    }

    static std::size_t decode(std::string & src, const std::string & dest, bool fill_null = true) {
        char * dest_buf = &dest[0];
        return decode(content.c_str(), content.length(), dest, std::string::max_size(), fill_null);
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
                if (isTwiceEscape)
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
