#include <iostream>

#include "assignment_data.hpp"
#include "matrix.hpp"
#include "qap_solver.hpp"

int main()
{
    constexpr Matrixi<4, 5> initial_soln({
        {
            0,
            1,
            2,
            3,
            4,
        },
        {
            5,
            6,
            7,
            8,
            9,
        },
        {
            10,
            11,
            12,
            13,
            14,
        },
        {
            15,
            16,
            17,
            18,
            19,
        },
    });
    assert(initial_soln.has_unique_entries());
    auto solver = QAPSolver(initial_soln, 0);
    const auto [soln, cost] = solver.solve();
    std::cout << "cost: " << cost << std::endl;
    std::cout << "solution: " << soln << std::endl;
    return 0;
}
