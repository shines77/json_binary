#pragma once

#include <iostream>
#include <string>
#include <fstream>

static inline
void get_hex_upper_256()
{
    static const char hex_u_16[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };
    std::ofstream ofs;
    try {
        ofs.open("hex_upper_256.txt", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
        if (ofs.is_open()) {
            char * table_head = "static const unsigned short hex_upper_256[] = {\r\n";
            char * table_tail = "};\r\n\r\n";
            ofs.write(table_head, std::strlen(table_head));
            char buff[256];
            std::string short_line;
            for (int row = 0; row < 16; ++row) {
                short_line.clear();
                short_line = "    ";
                for (int col = 0; col < 8; ++col) {
                    std::snprintf(buff, sizeof(buff) - 1, "0x%02X%02X, ", hex_u_16[col], hex_u_16[row]);
                    short_line += buff;
                }
                std::snprintf(buff, sizeof(buff) - 1, "    // %02X~%02X\r\n", row * 16, row * 16 + 7);
                short_line += buff;
                short_line += "    ";
                for (int col = 8; col < 16; ++col) {
                    std::snprintf(buff, sizeof(buff) - 1, "0x%02X%02X, ", hex_u_16[col], hex_u_16[row]);
                    short_line += buff;
                }
                std::snprintf(buff, sizeof(buff) - 1, "    // %02X~%02X\r\n", row * 16 + 8, row * 16 + 15);
                short_line += buff;
                ofs.write(short_line.c_str(), short_line.length());
            }
            ofs.write(table_tail, std::strlen(table_tail));
            ofs.close();
        }
    }
    catch (const std::exception & e) {
        std::cout << "get_hex_upper_256() Exception: " << e.what() << std::endl;
    }
}

static inline
void get_hex_upper_256_big()
{
    static const char hex_u_16[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };
    std::ofstream ofs;
    try {
        ofs.open("hex_upper_256_big.txt", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
        if (ofs.is_open()) {
            char * table_head = "static const unsigned short hex_upper_256[] = {\r\n";
            char * table_tail = "};\r\n\r\n";
            ofs.write(table_head, std::strlen(table_head));
            char buff[256];
            std::string short_line;
            for (int row = 0; row < 16; ++row) {
                short_line.clear();
                short_line = "    ";
                for (int col = 0; col < 8; ++col) {
                    std::snprintf(buff, sizeof(buff) - 1, "0x%02X%02X, ", hex_u_16[row], hex_u_16[col]);
                    short_line += buff;
                }
                std::snprintf(buff, sizeof(buff) - 1, "    // %02X~%02X\r\n", row * 16, row * 16 + 7);
                short_line += buff;
                short_line += "    ";
                for (int col = 8; col < 16; ++col) {
                    std::snprintf(buff, sizeof(buff) - 1, "0x%02X%02X, ", hex_u_16[row], hex_u_16[col]);
                    short_line += buff;
                }
                std::snprintf(buff, sizeof(buff) - 1, "    // %02X~%02X\r\n", row * 16 + 8, row * 16 + 15);
                short_line += buff;
                ofs.write(short_line.c_str(), short_line.length());
            }
            ofs.write(table_tail, std::strlen(table_tail));
            ofs.close();
        }
    }
    catch (const std::exception & e) {
        std::cout << "get_hex_upper_256() Exception: " << e.what() << std::endl;
    }
}

void test_std_string()
{
    {
        std::string str;
        str = "";
        str += "0";
        str += "abcdef";

        char * buffer = &str[0];
        const char * data = str.data();
        const char * c_str = str.c_str();

        std::cout << "str = " << str << std::endl;
        std::cout << "str.data() = " << str.data() << std::endl;
        std::cout << "str.c_str() = " << str.c_str() << std::endl;

        std::cout << "&str[0] = 0x" << std::hex << (std::size_t)buffer << std::endl;
        std::cout << "str.data() = 0x" << std::hex << (std::size_t)data << std::endl;
        std::cout << "str.c_str() = 0x" << std::hex << (std::size_t)c_str << std::endl;
        std::cout << std::dec << std::endl;
        std::cout << std::endl;
    }

    {
        std::string str;
        str = "abc\0de";
        str += "\0\0";
        str += "abcdefghijklmnopqrstuvwxyz123456";

        char * buffer = &str[0];
        const char * data = str.data();
        const char * c_str = str.c_str();

        std::cout << "str = " << str << std::endl;
        std::cout << "str.data() = " << str.data() << std::endl;
        std::cout << "str.c_str() = " << str.c_str() << std::endl;

        std::cout << "&str[0] = 0x" << std::hex << (std::size_t)buffer << std::endl;
        std::cout << "str.data() = 0x" << std::hex << (std::size_t)data << std::endl;
        std::cout << "str.c_str() = 0x" << std::hex << (std::size_t)c_str << std::endl;
        std::cout << std::dec << str.capacity() << std::endl;
        std::cout << std::endl;
    }
}
