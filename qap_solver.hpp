#pragma once
#include "math.hpp"
#include "matrix.hpp"

// This quadratic assignment problem uses Tabu search
class QAPSolver
{
   private:
    using Mat = Matrixi<20, 20>;
    friend class TestQAPSolver;

   public:
    QAPSolver(const Matrixi<4, 5> &initial_soln,
              const double allowed_percent_error);
    ~QAPSolver() = default;

    QAPSolver(const QAPSolver &m) = delete;
    QAPSolver &operator=(const QAPSolver &m) = delete;

    QAPSolver(QAPSolver &&m) = delete;
    QAPSolver &operator=(QAPSolver &&m) = delete;

    // returns the best configuration found and the associated cost
    std::pair<Matrixi<4, 5>, int32_t> solve();

   private:
    static constexpr auto TABU_TENURE = 12;
    static constexpr auto ELEMENTS = 20;
    static constexpr auto ELEMENTS_PER_SWAP = 2;
    static constexpr auto MAX_ITERATIONS = 10000;
    Matrixi<4, 5> initial_soln_;
    Mat tabu_;
    const double min_allowed_cost_;

    int32_t cost(const Matrixi<4, 5> &soln) const;
    void update_tabu_list(const std::pair<size_t,size_t> &action);
    static constexpr auto get_all_possible_combinations()
    {
        constexpr auto N_CHOOSE_K = math::choose(ELEMENTS, ELEMENTS_PER_SWAP);
        static_assert(N_CHOOSE_K == 190);
        using comb = std::pair<size_t, size_t>;
        std::array<comb, N_CHOOSE_K> combinations;
        size_t k = 0;
        for (size_t i = 0; i < ELEMENTS; i++)
        {
            for (size_t j = 0; j < i; j++)
            {
                combinations.at(k).first = i;
                combinations.at(k).second = j;
                ++k;
            }
        }
        assert(k == 190);
        return combinations;
    }
};
