#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <tuple>

template <size_t rows, size_t cols, typename T>
class Matrix
{
   public:
    static_assert(rows > 0 && cols > 0);
    Matrix() : data_{0} {}
    ~Matrix() = default;

    constexpr Matrix(std::initializer_list<std::initializer_list<T>> list)
        : data_{0}
    {
        size_t i = 0, j = 0;
        for (const auto row : list)
        {
            j = 0;
            for (const auto val : row)
            {
                this->at(i, j) = val;
                ++j;
            }
            ++i;
        }
    }

    Matrix(const Matrix &m) = default;
    Matrix &operator=(const Matrix &m) = default;

    Matrix(Matrix &&m) = default;
    Matrix &operator=(Matrix &&m) = default;

    constexpr T &at(const size_t i, const size_t j)
    {
        return data_.at(i * cols + j);
    }

    constexpr const T &at(const size_t i, const size_t j) const
    {
        return data_.at(i * cols + j);
    }

    constexpr T &at(const std::pair<size_t, size_t> &loc)
    {
        const auto [i, j] = loc;
        return this->at(i, j);
    }

    constexpr const T &at(const std::pair<size_t, size_t> &loc) const
    {
        const auto [i, j] = loc;
        return this->at(i, j);
    }

    constexpr bool is_symmetric() const
    {
        if constexpr (rows != cols)
        {
            return false;
        }
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < i + 1; j++)
            {
                if (this->at(i, j) != this->at(j, i))
                {
                    return false;
                }
            }
        }
        return true;
    }

    constexpr bool operator==(const Matrix<rows,cols,T> &other) const
    {
        return this->data_ == other.data_;
    }

    constexpr bool has_unique_entries() const
    {
        std::array<T, rows * cols> sorted{0};
        std::partial_sort_copy(data_.cbegin(), data_.cend(), sorted.begin(),
                               sorted.end());
        return std::adjacent_find(sorted.cbegin(), sorted.cend()) ==
               sorted.cend();
    }

    constexpr auto shape() const
    {
        return std::pair<size_t, size_t>(rows, cols);
    }

    constexpr std::optional<std::pair<size_t, size_t>> find(const T &val) const
    {
        const auto it = std::find(data_.cbegin(), data_.cend(), val);
        if (it == data_.cend())
        {
            return std::nullopt;
        }
        const auto index = std::distance(data_.cbegin(), it);
        const size_t row = index / cols;
        const size_t col = index % cols;
        assert(row < rows && col < cols);
        assert(row >= 0 && col >= 0);
        return std::make_pair(row, col);
    }

    constexpr T frobenius_inner_product(const Matrix<rows, cols, T> &mat) const
    {
        return std::inner_product(data_.begin(), data_.end(),
                                  mat.data_.cbegin(), T(0));
    }

    constexpr void swap(const std::pair<size_t, size_t> &a,
                        const std::pair<size_t, size_t> &b)
    {
        const auto temp = this->at(a);
        this->at(a) = this->at(b);
        this->at(b) = temp;
    }

   private:
    std::array<T, cols * rows> data_;
};

template <size_t rows, size_t cols>
using Matrixd = Matrix<rows, cols, double>;

template <size_t rows, size_t cols>
using Matrixi = Matrix<rows, cols, int32_t>;

template <size_t rows, size_t cols, typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<rows, cols, T> &mat)
{
    os << "{\n";
    for (size_t i = 0; i < rows; i++)
    {
        os << "{";
        for (size_t j = 0; j < cols; j++)
        {
            os << mat.at(i, j) << " ";
        }
        os << "}\n";
    }
    os << "}";
    return os;
}
