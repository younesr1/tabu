#include <iostream>
#include "qap_solver.hpp"

#include "matrix.hpp"

namespace matrix_tests
{
bool test_shape()
{
    const auto foo = Matrix<20, 20, int>().shape();
    return foo.first == 20 && foo.second == 20;
}

bool test_at()
{
    const auto foo = Matrix<2, 2, int>({{1, 2}, {3, 4}});
    bool success = foo.at(0, 0) == 1;
    success &= foo.at(0, 1) == 2;
    success &= foo.at(1, 0) == 3;
    success &= foo.at(1, 1) == 4;
    return success;
}

bool test_symmetric()
{
    const auto clean = Matrix<3, 3, int>({{1, 2, 3}, {2, 1, 4}, {3, 4, 1}});
    bool success = clean.is_symmetric();
    const auto rect =
        Matrix<4, 3, int>({{1, 2, 3}, {2, 1, 4}, {3, 4, 1}, {1, 2, 3}});
    success &= !rect.is_symmetric();
    const auto non_sym = Matrix<3, 3, int>({{1, 2, 10}, {2, 1, 4}, {3, 4, 1}});
    success &= !non_sym.is_symmetric();
    return success;
}

bool test_unique()
{
    const auto unique = Matrix<2, 3, int>({{1, 2, 3}, {4, 5, 6}});
    bool success = unique.has_unique_entries();
    const auto non_unique = Matrix<20, 20, int>();
    success &= !non_unique.has_unique_entries();
    return success;
}

bool test_find()
{
    const auto mat = Matrix<3, 3, int>({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    bool success = mat.find(1).value() == std::pair<size_t, size_t>(0, 0);
    success &= mat.find(2).value() == std::pair<size_t, size_t>(0, 1);
    success &= mat.find(3).value() == std::pair<size_t, size_t>(0, 2);
    success &= mat.find(4).value() == std::pair<size_t, size_t>(1, 0);
    success &= mat.find(5).value() == std::pair<size_t, size_t>(1, 1);
    success &= mat.find(6).value() == std::pair<size_t, size_t>(1, 2);
    success &= mat.find(7).value() == std::pair<size_t, size_t>(2, 0);
    success &= mat.find(8).value() == std::pair<size_t, size_t>(2, 1);
    success &= mat.find(9).value() == std::pair<size_t, size_t>(2, 2);
    return success;
}
}  // namespace matrix_tests

class TestQAPSolver
{
    public:
    bool run() const
    {
        QAPSolver solver(Matrixi<4,5>(), 0);
        const auto ret = solver.cost(Matrixi<4,5>({{0,1,2,3,4},{5,6,7,8,9},{10,11,12,13,14},{15,16,17,18,19}}));
        return ret == 3444;
    }
};

int main()
{
    // matrix tests
    std::cout << "testing matrix lib" << std::endl;
    assert(matrix_tests::test_shape());
    assert(matrix_tests::test_at());
    assert(matrix_tests::test_symmetric());
    assert(matrix_tests::test_unique());
    assert(matrix_tests::test_find());
    std::cout << "testing qap solver" << std::endl;
    const TestQAPSolver solver;
    assert(solver.run());
    std::cout << "done" << std::endl;
}
