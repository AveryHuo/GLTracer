#pragma once
#include <array>
#include<string>

template <typename T, std::size_t N>
inline static decltype(auto) to_raw_array(const std::array<T, N>& arr_v) {
    return reinterpret_cast<const T(&)[N]>(*arr_v.data());
}

template< typename... Args >
std::string string_format(const char* format, Args... args)
{
    size_t length = std::snprintf(nullptr, 0, format, args...);
    if (length <= 0)
    {
        return "";
    }

    char* buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);

    std::string str(buf);
    delete[] buf;
    return std::move(str);
}