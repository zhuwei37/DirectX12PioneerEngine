#pragma once
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint>
#include<objbase.h>
#include <windows.h>
#include <unordered_map>
#include <functional> // for std::hash
#include <iomanip>

#include<regex>
struct GUIDHash {
    std::size_t operator()(const GUID& guid) const {
        // 使用 GUID 的 Data1, Data2, Data3, Data4 生成哈希
        std::size_t h1 = std::hash<DWORD>()(guid.Data1);
        std::size_t h2 = std::hash<WORD>()(guid.Data2);
        std::size_t h3 = std::hash<WORD>()(guid.Data3);

        // 对 Data4 数组进行哈希
        std::size_t h4 = 0;
        for (int i = 0; i < 8; ++i) {
            h4 ^= std::hash<BYTE>()(guid.Data4[i]) + 0x9e3779b9 + (h4 << 6) + (h4 >> 2);
        }

        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};
struct GUIDEqual {
    bool operator()(const GUID& lhs, const GUID& rhs) const {
        return IsEqualGUID(lhs, rhs); // Windows API 函数
        // 或者手动比较：
        // return lhs.Data1 == rhs.Data1 &&
        //        lhs.Data2 == rhs.Data2 &&
        //        lhs.Data3 == rhs.Data3 &&
        //        memcmp(lhs.Data4, rhs.Data4, 8) == 0;
    }
};
class GUIDHelper
{
public:
   static std::string ResourceIdToString(const GUID& guid) {
        std::ostringstream oss;

        // 格式: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
        oss << std::hex << std::setfill('0')
            << std::setw(8) << guid.Data1 << "-"
            << std::setw(4) << guid.Data2 << "-"
            << std::setw(4) << guid.Data3 << "-"
            << std::setw(2) << static_cast<int>(guid.Data4[0])
            << std::setw(2) << static_cast<int>(guid.Data4[1]) << "-"
            << std::setw(2) << static_cast<int>(guid.Data4[2])
            << std::setw(2) << static_cast<int>(guid.Data4[3])
            << std::setw(2) << static_cast<int>(guid.Data4[4])
            << std::setw(2) << static_cast<int>(guid.Data4[5])
            << std::setw(2) << static_cast<int>(guid.Data4[6])
            << std::setw(2) << static_cast<int>(guid.Data4[7]);
        return oss.str();
    }

    // 从字符串解析GUID（反向操作）
   static  GUID StringToResourceId(const std::string& str) {
       

        GUID guid1;

        std::regex guid_regex(
            R"(\{?([0-9a-fA-F]{8})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{12})\}?)"
        );

        std::smatch match;
        if (!std::regex_match(str, match, guid_regex)) {
           
        }

        // 解析各个部分
        std::stringstream ss;

        // Data1
        ss << std::hex << match[1].str();
        ss >> guid1.Data1;
        ss.clear();

        // Data2
        ss << std::hex << match[2].str();
        ss >> guid1.Data2;
        ss.clear();

        // Data3
        ss << std::hex << match[3].str();
        ss >> guid1.Data3;
        ss.clear();

        // Data4（两部分）
        std::string data4_1 = match[4].str();
        std::string data4_2 = match[5].str();

        // 第一部分：2个字节
        for (size_t i = 0; i < 2; ++i) {
            std::string byteStr = data4_1.substr(i * 2, 2);
            ss << std::hex << byteStr;
            int byteVal;
            ss >> byteVal;
            guid1.Data4[i] = static_cast<uint8_t>(byteVal);
            ss.clear();
        }

        // 第二部分：6个字节
        for (size_t i = 0; i < 6; ++i) {
            std::string byteStr = data4_2.substr(i * 2, 2);
            ss << std::hex << byteStr;
            int byteVal;
            ss >> byteVal;
            guid1.Data4[i + 2] = static_cast<uint8_t>(byteVal);
            ss.clear();
        }

        return guid1;
    }
};