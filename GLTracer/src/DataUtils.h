#pragma once
#include <array>

template <typename T, std::size_t N>
inline static decltype(auto) to_raw_array(const std::array<T, N>& arr_v) {
    return reinterpret_cast<const T(&)[N]>(*arr_v.data());
}