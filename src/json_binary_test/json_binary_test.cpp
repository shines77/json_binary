// JSON simple example
// This example does not handle errors.

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <iostream>
#include <exception>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "json_binary.h"
#include "acl_base64.h"
#include "stop_watch.h"
#include "hex_table_256.h"

#define TEST_BIN_FILENAME       "..//..//data//test.bin"
#define TEST_BIG_BIN_FILENAME   "..//..//data//big.bin"

using namespace rapidjson;

#ifdef NDEBUG
static const int kRepeatTimes = 50;
#else
static const int kRepeatTimes = 1;
#endif

class Employee {
private:
    std::string name_;
    unsigned age_;
    bool married_;
    std::string dependents_;
    std::string binary_data_;

public:
    Employee(const std::string& name, unsigned age, bool married) : name_(name), age_(age),
        married_(married), dependents_(""), binary_data_("") {}
    Employee(const Employee& rhs) : name_(rhs.name_), age_(rhs.age_),
        married_(rhs.married_), dependents_(rhs.dependents_),
        binary_data_(rhs.binary_data_) {
        if (this != &rhs) {
            //
        }
    }
    virtual ~Employee() {};

    Employee& operator =(const Employee& rhs) {
        if (this != &rhs) {
            this->name_ = rhs.name_;
            this->age_ = rhs.age_;
            this->married_ = rhs.married_;
            this->dependents_ = rhs.dependents_;
            this->binary_data_ = rhs.binary_data_;
        }
        return *this;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("name");
        writer.String(name_.c_str());

        writer.String("age");
        writer.Uint(age_);

        writer.String("married");
        writer.Bool(married_);

        writer.String("dependents");
        writer.String(dependents_.c_str());

        writer.String("binary-hex");
        writer.RawValue(binary_data_.c_str(), binary_data_.length(), kStringType);

        writer.EndObject();
    }

    bool EncodeFromFile(const std::string & filename, bool add_quote = false) {
        return json_binary<twice_escape>::encodeFromFile(filename, binary_data_, add_quote);
    }

    bool EncodeHexFromFile(const std::string & filename, bool add_quote = false) {
        return json_binary_hex::encodeFromFile(filename, binary_data_, add_quote);
    }

    bool SaveContentToFile(const std::string & filename) {
        return utils::saveToFile(filename, binary_data_, binary_data_.length());
    }
};

bool touch_file(const std::string & filename)
{
    bool bSuccess = false;
    std::ifstream ifs;
    try {
        if (filename.empty())
            return false;
        ifs.open(filename.c_str(), std::ios::in | std::ios_base::binary);
        if (ifs.is_open()) {
            bSuccess = true;
            ifs.close();
        }
    }
    catch (const std::exception & e) {
        std::cout << "json_binary::encodeFromFile() Exception: " << e.what() << std::endl;
    }
    return bSuccess;
}

std::string find_data_dir()
{
    std::ifstream ifs;
    std::string filename;
    filename = "..//..//data//test.bin";
    if (touch_file(filename)) {
        return std::string("..//..//data");
    }
    filename = "..//..//..//data//test.bin";
    if (touch_file(filename)) {
        return std::string("..//..//..//data");
    }
    filename = "..//..//bin//data//test.bin";
    if (touch_file(filename)) {
        return std::string("..//..//bin//data");
    }
    filename = "..//..//..//bin//data//test.bin";
    if (touch_file(filename)) {
        return std::string("..//..//..//bin//data");
    }
    return std::string("");
}

std::string get_bin_file(const std::string & filename)
{
    std::string bin_file;
    static std::string data_dir = find_data_dir();
    if (!data_dir.empty()) {
        bin_file = data_dir + "//";
        bin_file += filename;
        return bin_file;
    }
    return filename;
}

std::string get_test_bin_file()
{
    return get_bin_file("test.bin");
}

std::string get_big_bin_file()
{
    return get_bin_file("big.bin");
}

std::streamsize memory_compare(const char * buf1, const char * buf2, std::size_t length)
{
    char * src = (char *)buf2;
    char * dest = (char *)buf1;
    char * src_start = src;
    char * src_end = src + length;
    while (src < src_end) {
        if (*dest++ != *src++) {
            assert(src >= src_start);
            return (src - src_start);
        }
    }
    return 0;
}

void test_simple_dom()
{
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\": \"rapidjson\",\"stars\": 10, \"binary-hex\": \"\"}";
    Document doc;
    doc.Parse(json);

    // 2. Modify it by DOM.
    Value& s = doc["stars"];
    s.SetInt(s.GetInt() + 1);

    Value& bin = doc["binary-hex"];
    std::string content;
    std::size_t content_size = 0;
    json_binary<one_escape>::encodeFromFile(get_test_bin_file(), content, false);
    bin.SetString(content.c_str(), (rapidjson::SizeType)content.length());  

    // 3. Stringify the DOM
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << std::endl;
    std::cout << buffer.GetString() << std::endl;
    std::cout << std::endl;
}

void test_serialize()
{
    std::vector<Employee> employees;

    Employee milo("Milo YIP", 34, true);
    milo.EncodeFromFile(get_test_bin_file(), true);
    milo.SaveContentToFile("C:\\test_json_binary_out.bin");

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    milo.Serialize(writer);

    std::cout << std::endl;
    ::puts(sb.GetString());
    std::cout << std::endl;
}

void test_simple_dom_hex()
{
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\": \"rapidjson\",\"stars\": 10, \"binary-hex\": \"\"}";
    Document doc;
    doc.Parse(json);

    // 2. Modify it by DOM.
    Value& s = doc["stars"];
    s.SetInt(s.GetInt() + 1);

    Value& bin = doc["binary-hex"];
    std::string content;
    std::size_t content_size = 0;
    json_binary_hex::encodeFromFile(get_test_bin_file(), content, false);
    bin.SetString(content.c_str(), (rapidjson::SizeType)content.length());  

    // 3. Stringify the DOM
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << std::endl;
    std::cout << buffer.GetString() << std::endl;
    std::cout << std::endl;
}

void test_serialize_hex()
{
    std::vector<Employee> employees;

    Employee milo("Milo YIP", 34, true);
    milo.EncodeHexFromFile(get_test_bin_file(), true);
    milo.SaveContentToFile("C:\\test_json_binary_hex_out.bin");

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    milo.Serialize(writer);

    std::cout << std::endl;
    ::puts(sb.GetString());
    std::cout << std::endl;
}

void json_binary_big_file_test()
{
    StopWatch sw;
    std::string original;

    std::cout << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    //
    // json_binary_utils::readFromFile()
    //
    {
        std::string content;

        sw.start();
        if (utils::readFromFile(get_big_bin_file(), content)) {
            sw.stop();
            std::cout << std::endl;
            std::cout << "json_binary_utils::readFromFile():" << std::endl << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary_utils::readFromFile(): failure." << std::endl;
        }
        std::cout << std::endl;
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;

        original = content;
    }

    std::cout << "--------------------------------------------" << std::endl;

    //
    // json_binary<single_escape>
    //
    {
        std::string content;

        sw.start();
        if (json_binary<one_escape>::encodeFromFile(get_big_bin_file(), content)) {
            sw.stop();
            std::cout << std::endl;
            std::cout << "json_binary<one_escape>::encodeFromFile():" << std::endl << std::endl;
            std::cout << "encode_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary<one_escape>::encodeFromFile(): failure." << std::endl;
        }
        std::cout << std::endl;
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "--------------------------------------------" << std::endl;

    //
    // json_binary<double_escape>
    //
    {
        std::string content;

        sw.start();
        if (json_binary<twice_escape>::encodeFromFile(get_big_bin_file(), content)) {
            sw.stop();
            std::cout << std::endl;
            std::cout << "json_binary<twice_escape>::encodeFromFile():" << std::endl << std::endl;
            std::cout << "encode_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary<twice_escape>::encodeFromFile(): failure." << std::endl;
        }
        std::cout << std::endl;
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "--------------------------------------------" << std::endl;

    //
    // json_binary_hex::encode()
    //
    {
        std::string content, source;

        sw.start();
        if (json_binary_hex::encodeFromFile(get_big_bin_file(), content)) {
            sw.stop();
            std::cout << std::endl;
            std::cout << "json_binary_hex::encodeFromFile():" << std::endl << std::endl;
            std::cout << "encode_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary_hex::encodeFromFile(): failure." << std::endl;
        }
        std::cout << std::endl;
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "--------------------------------------------" << std::endl;

    {
        std::string content = original;
        {
            std::size_t buf_size = json_binary_hex::get_encode_capacity(content.length());
            std::unique_ptr<char> buffer(new char[buf_size]);
            sw.start();
            std::streamsize encode_size = json_binary_hex::encode(content.c_str(), content.length(), buffer.get(), buf_size);
            if (encode_size >= 0) {
                sw.stop();
                std::cout << std::endl;
                std::cout << "json_binary_hex::encode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "encode_size  = " << encode_size << std::endl;
                content = buffer.get();
            }
            else {
                sw.stop();
                std::cout << "json_binary_hex::encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            std::size_t buf_size = json_binary_hex::get_decode_capacity(content.length());
            std::unique_ptr<char> buffer(new char[buf_size]);
            sw.start();
            std::streamsize decode_size = json_binary_hex::decode(content.c_str(), content.length(), buffer.get(), buf_size);
            if (decode_size >= 0) {
                sw.stop();
                std::cout << "json_binary_hex::decode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size  = " << decode_size << std::endl;
            }
            else {
                sw.stop();
                std::cout << "json_binary_hex::decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;

            std::streamsize result = memory_compare(buffer.get(), original.c_str(), original.length());
            if (result == 0 && std::memcmp(buffer.get(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;

#if 0
    //
    // json_binary_hex::encode_std()
    //
    {
        std::string content;

        sw.start();
        if (json_binary_hex::encodeStdFromFile(get_big_bin_file(), content)) {
            sw.stop();
            std::cout << std::endl;
            std::cout << "json_binary_hex::encodeStdFromFile():" << std::endl << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary_hex::encodeStdFromFile(): failure." << std::endl;
        }
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;

        {
            std::size_t buf_size = json_binary_hex::get_decode_capacity(content.length());
            std::unique_ptr<char> buffer(new char[buf_size]);
            sw.start();
            std::streamsize decode_size = json_binary_hex::decode_std(content.c_str(), content.length(), buffer.get(), buf_size);
            if (decode_size >= 0) {
                sw.stop();
                std::cout << "json_binary_hex::decode_std():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size  = " << decode_size << std::endl;
            }
            else {
                sw.stop();
                std::cout << "json_binary_hex::decode_std(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;
#endif

#ifdef NDEBUG
    //
    // acl_base64 for C++
    //
    {
        std::string content = original;
        {
            std::string encoded;
            sw.start();
            encoded = base64_encode(content.c_str(), (int)content.length());
            if (encoded.length() > 0) {
                sw.stop();
                std::cout << std::endl;
                std::cout << "acl_base64_cpp::encode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "encode_size  = " << encoded.length() << std::endl;
                content = encoded;
            }
            else {
                sw.stop();
                std::cout << "acl_base64_cpp::encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            std::string decoded;
            sw.start();
            decoded = base64_decode(content);
            if (decoded.length() > 0) {
                sw.stop();
                std::cout << "acl_base64_cpp::decode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size  = " << decoded.length() << std::endl;
                //std::cout << "buffer = 0x" << std::hex << (std::size_t)decoded.c_str() << std::dec << std::endl;
            }
            else {
                sw.stop();
                std::cout << "acl_base64_cpp::decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;

            if (std::memcmp(decoded.c_str(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;
#endif

    //
    // json_binary_hex::encode() & decode()
    //
    {
        std::string content = original;
        {
            std::string encoded;
            sw.start();
            //encoded = hex16_encode(content.c_str(), (int)content.length());
            std::streamsize encode_size = hex16_encode(content.c_str(), (int)content.length(), encoded);
            if (encoded.length() > 0) {
                sw.stop();
                std::cout << std::endl;
                std::cout << "json_hex16_cpp::encode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "encode_size  = " << encoded.length() << std::endl;
                content = encoded;
            }
            else {
                sw.stop();
                std::cout << "json_hex16_cpp::encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            std::string decoded;
            sw.start();
            //decoded = hex16_decode(content);
            std::streamsize decode_size = hex16_decode(content, decoded);
            if (decoded.length() > 0) {
                sw.stop();
                std::cout << "json_hex16_cpp::decode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size  = " << decoded.length() << std::endl;
                //std::cout << "buffer = 0x" << std::hex << (std::size_t)decoded.c_str() << std::dec << std::endl;
            }
            else {
                sw.stop();
                std::cout << "json_hex16_cpp::decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;

            std::streamsize result = memory_compare(decoded.c_str(), original.c_str(), original.length());
            if ((result == 0) && std::memcmp(decoded.c_str(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;

#if 0
    //
    // acl_base64
    //
    {
        std::string content = original;
        {
            sw.start();
            unsigned char * buffer = acl_base64_encode(content.c_str(), (int)content.length());
            if (buffer != nullptr) {
                sw.stop();
                std::cout << std::endl;
                std::cout << "acl_base64::encode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "encode_size  = " << std::strlen((const char *)buffer) << std::endl;
                content = (const char *)buffer;
            }
            else {
                sw.stop();
                std::cout << "acl_base64::encode(): failure." << std::endl;
            }
            if (buffer)
                ::free(buffer);
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            char * buffer = nullptr;
            sw.start();
            int decode_size = acl_base64_decode(content.c_str(), &buffer);
            if (decode_size >= 0) {
                sw.stop();
                std::cout << "acl_base64::decode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size  = " << decode_size << std::endl;
                //std::cout << "buffer = 0x" << std::hex << (std::size_t)buffer << std::dec << std::endl;
            }
            else {
                sw.stop();
                std::cout << "acl_base64::decode(): failure." << std::endl;
            }
            if (buffer)
                ::free(buffer);
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;

            if (std::memcmp(buffer, original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;
#endif

    //
    // bin_escape::encode() & decode()
    //
    {
        std::string content = original;
        {
            std::string encoded;
            sw.start();
            //encoded = bin_escape_encode(content.c_str(), (int)content.length());
            std::streamsize encode_size = bin_escape_encode(content.c_str(), (int)content.length(), encoded);
            if (encoded.length() > 0) {
                sw.stop();
                std::cout << std::endl;
                std::cout << "bin_escape_encode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "encode_size  = " << encoded.length() << std::endl;
                content = encoded;
            }
            else {
                sw.stop();
                std::cout << "bin_escape_encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            std::string decoded;
            sw.start();
            //decoded = bin_escape_decode(content);
            std::streamsize decode_size = bin_escape_decode(content, decoded);
            if (decoded.length() > 0) {
                sw.stop();
                std::cout << "bin_escape_decode():" << std::endl << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size  = " << decoded.length() << std::endl;
                //std::cout << "buffer = 0x" << std::hex << (std::size_t)decoded.c_str() << std::dec << std::endl;
            }
            else {
                sw.stop();
                std::cout << "bin_escape_decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;

            std::streamsize result = memory_compare(decoded.c_str(), original.c_str(), original.length());
            if ((result == 0) && std::memcmp(decoded.c_str(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error. [pos = " << result << "]" << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;
}

void big_file_test()
{
    std::string original;

    std::cout << "--------------------------------------------" << std::endl;
    //
    // utils::readFromFile()
    //
    {
        StopWatch sw;
        std::string content;

        sw.start();
        bool success = utils::readFromFile(get_big_bin_file(), content, 65536);
        sw.stop();

        if (success) {
            std::cout << std::endl;
            std::cout << "utils::readFromFile():" << std::endl << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            std::cout << "utils::readFromFile(): failure." << std::endl;
        }
        std::cout << std::endl;
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;

        original.swap(content);
    }

    std::cout << "--------------------------------------------" << std::endl;

#ifdef NDEBUG
    //
    // acl_base64 for C++
    //
    {
        std::string content = original;

        {
            StopWatch sw;
            std::string encoded;
            std::size_t sum_encoded = 0;
        
            sw.reset();
            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                //encoded = base64_encode(content.c_str(), (int)content.length());
				std::ptrdiff_t encoded_size = base64_encode(content.c_str(), (int)content.length(), encoded);
                sw.stop();
                sw.again();
                sum_encoded += encoded.length();
            }

            if (encoded.length() > 0) {
                std::cout << std::endl;
                std::cout << "base64_encode(): " << (sum_encoded % 16) << std::endl;
                std::cout << std::endl;
                std::cout << "encode_size = " << encoded.length() << std::endl;
                content.swap(encoded);
            }
            else {
                std::cout << "base64_encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "avg. time spent: " << (sw.getTotalMillisec() / (double)kRepeatTimes) << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            StopWatch sw;
            std::string decoded;
            std::size_t sum_decoded = 0;
        
            sw.reset();
            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                //decoded = base64_decode(content);
				std::ptrdiff_t decoded_size = base64_decode(content, decoded);
                sw.stop();
                sw.again();
                sum_decoded += decoded.length();
            }

            if (decoded.length() > 0) {
                std::cout << "base64_decode(): " << (sum_decoded % 16) << std::endl;
                std::cout << std::endl;
                std::cout << "decode_size = " << decoded.length() << std::endl;
            }
            else {
                std::cout << "base64_decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "avg. time spent: " << (sw.getTotalMillisec() / (double)kRepeatTimes) << " ms." << std::endl;
            std::cout << std::endl;

            if (std::memcmp(decoded.c_str(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;
#endif

    //
    // hex16_encode() && hex16_decode()
    //
    {
        std::string content = original;

        {
            StopWatch sw;
            std::string encoded;
            std::size_t sum_encoded = 0;
        
            sw.reset();
            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                //encoded = hex16_encode(content.c_str(), (int)content.length());
                std::streamsize encode_size = hex16_encode(content.c_str(), (int)content.length(), encoded);
                sw.stop();
                sw.again();
                sum_encoded += encoded.length();
            }

            if (encoded.length() > 0) {
                std::cout << std::endl;
                std::cout << "hex16_encode(): " << (sum_encoded % 16) << std::endl;
                std::cout << std::endl;
                std::cout << "encode_size = " << encoded.length() << std::endl;
                content.swap(encoded);
            }
            else {
                std::cout << "hex16_encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "avg. time spent: " << (sw.getTotalMillisec() / (double)kRepeatTimes) << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            StopWatch sw;
            std::string decoded;
            std::size_t sum_decoded = 0;
        
            sw.reset();
            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                //decoded = hex16_decode(content);
                std::streamsize decode_size = hex16_decode(content, decoded);
                sw.stop();
                sw.again();
                sum_decoded += decoded.length();
            }

            if (decoded.length() > 0) {
                std::cout << "hex16_decode(): " << (sum_decoded % 16) << std::endl;
                std::cout << std::endl;
                std::cout << "decode_size = " << decoded.length() << std::endl;
            }
            else {
                std::cout << "hex16_decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "avg. time spent: " << (sw.getTotalMillisec() / (double)kRepeatTimes) << " ms." << std::endl;
            std::cout << std::endl;

            if (std::memcmp(decoded.c_str(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;

    //
    // bin_escape::encode() & decode()
    //
    {
        std::string content = original;

        {
            StopWatch sw;
            std::string encoded;
            std::size_t sum_encoded = 0;
        
            sw.reset();
            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                //encoded = bin_escape_encode(content.c_str(), (int)content.length());
                std::streamsize encode_size = bin_escape_encode(content.c_str(), (int)content.length(), encoded);
                sw.stop();
                sw.again();
                sum_encoded += encoded.length();
            }

            if (encoded.length() > 0) {
                std::cout << std::endl;
                std::cout << "bin_escape_encode(): " << (sum_encoded % 16) << std::endl;
                std::cout << std::endl;
                std::cout << "encode_size = " << encoded.length() << std::endl;
                content.swap(encoded);
            }
            else {
                std::cout << "bin_escape_encode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "avg. time spent: " << (sw.getTotalMillisec() / (double)kRepeatTimes) << " ms." << std::endl;
            std::cout << std::endl;
        }

        {
            StopWatch sw;
            std::string decoded;
            std::size_t sum_decoded = 0;
        
            sw.reset();
            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                //decoded = bin_escape_decode(content);
                std::streamsize decode_size = bin_escape_decode(content, decoded);
                sw.stop();
                sw.again();
                sum_decoded += decoded.length();
            }

            if (decoded.length() > 0) {
                std::cout << "bin_escape_decode(): " << (sum_decoded % 16) << std::endl;
                std::cout << std::endl;
                std::cout << "decode_size = " << decoded.length() << std::endl;
            }
            else {
                std::cout << "bin_escape_decode(): failure." << std::endl;
            }
            std::cout << std::endl;
            std::cout << "avg. time spent: " << (sw.getTotalMillisec() / (double)kRepeatTimes) << " ms." << std::endl;
            std::cout << std::endl;

            if (std::memcmp(decoded.c_str(), original.c_str(), original.length()) == 0)
                std::cout << "decode() correctly." << std::endl;
            else
                std::cout << "decode() error." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "--------------------------------------------" << std::endl;
}

int main(int argc, char * argv[])
{
    //test_std_string();

    /*
    test_simple_dom();
    test_serialize();

    test_simple_dom_hex();
    test_serialize_hex();
    //*/

    //json_binary_big_file_test();
    big_file_test();

    ::system("pause");
    return 0;
}
