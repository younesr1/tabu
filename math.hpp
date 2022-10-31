#pragma once
#include <cmath>
namespace math
{
constexpr size_t choose(const size_t n, const size_t k)
{
    double ret = 1;
    for (size_t i = 1; i <= k; i++)
    {
        ret *= (n + 1 - i) / static_cast<double>(i);
    }
    return std::round(ret);
}

}  // namespace math
