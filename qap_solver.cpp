#include "qap_solver.hpp"

#include <cstdlib>

#include "assignment_data.hpp"

QAPSolver::QAPSolver(const Matrixi<4, 5> &initial_soln,
                     const double allowed_percent_error)
    : initial_soln_(initial_soln),
      tabu_(),
      min_allowed_cost_((allowed_percent_error + 1) *
                        assignment_data::optimal_cost)
{
    assert(allowed_percent_error >= 0 && allowed_percent_error <= 1);
}

std::pair<Matrixi<4, 5>, int32_t> QAPSolver::solve()
{
    constexpr auto combinations = get_all_possible_combinations();
    assert(std::all_of(combinations.cbegin(), combinations.cend(), [](const auto &c) { return c.first < ELEMENTS && c.second < ELEMENTS; }));
    uint64_t iteration = 0;
    auto cur_soln = std::move(initial_soln_);
    auto best_ever_soln = cur_soln;
    int32_t best_ever_cost = std::numeric_limits<int32_t>::max();
    while (cost(cur_soln) > min_allowed_cost_)
    {
        ++iteration;
        assert(cur_soln.has_unique_entries());
        int32_t best_local_cost = std::numeric_limits<int32_t>::max();
        auto best_local_soln = cur_soln;
        std::pair<size_t, size_t> best_local_action;
        bool no_available_choices = true;
        for (const auto &[val1, val2] : combinations)
        {
            no_available_choices = false;
            auto hypothesis = cur_soln;
            const auto loc1 = hypothesis.find(val1).value();
            const auto loc2 = hypothesis.find(val2).value();
            hypothesis.swap(loc1, loc2);
            const auto cost = this->cost(hypothesis);
            if (tabu_.at(std::min(val1, val2), std::max(val1, val2)) == 0)
                // || cost < best_ever_cost)
            {
                if (cost < best_local_cost)
                {
                    best_local_action = std::make_pair(std::min(val1, val2),
                                                       std::max(val1, val2));
                    best_local_cost = cost;
                    best_local_soln = std::move(hypothesis);
                }
            }
        }
        if (no_available_choices)
        {
            std::cout << "ran out of available choices" << std::endl;
            break;
        }
        if (iteration > MAX_ITERATIONS)
        {
            std::cout << "exceeded max iterations" << std::endl;
            break;
        }
        this->update_tabu_list(best_local_action);
        cur_soln = best_local_soln;
        if (best_ever_cost > best_local_cost)
        {
            best_ever_cost = best_local_cost;
            best_ever_soln = cur_soln;
        }
        std::cout << "iteration: " << iteration << " cost: " << best_local_cost << std::endl;
    }
    return {best_ever_soln, best_ever_cost};
}

void QAPSolver::update_tabu_list(const std::pair<size_t,size_t> &action)
{
    const auto [rows,cols] = tabu_.shape();
    for (size_t j = 0; j < rows; j++)
    {
        for (size_t i = 0; i < j; i++)
        {
            tabu_.at(i,j) = std::max(static_cast<ssize_t>(0),static_cast<ssize_t>(tabu_.at(i,j))-1);
        }
    }
    assert(action.second > action.first);
    tabu_.at(action) = TABU_TENURE;
}

int32_t QAPSolver::cost(const Matrixi<4, 5> &soln) const
{
    assert(soln.has_unique_entries());
    auto distances = Mat();
    const auto [rows, cols] = distances.shape();
    // fill the upper triangular matrix
    for (size_t i = 0; i < rows; i++)
    {
        using signed_coord = std::pair<ssize_t, ssize_t>;
        const signed_coord i_coordinate = soln.find(i).value();
        for (size_t j = 0; j < i; j++)
        {
            const signed_coord j_coordinate = soln.find(j).value();
            const auto manhattan_distance =
                std::abs(i_coordinate.first - j_coordinate.first) +
                std::abs(i_coordinate.second - j_coordinate.second);
            distances.at(i, j) = manhattan_distance;
            distances.at(j, i) = manhattan_distance;
        }
    }
    for (size_t i = 0; i < rows; i++)
    {
        distances.at(i,i) = 0;
    }
    assert(distances.is_symmetric());
    const auto cost = distances.frobenius_inner_product(assignment_data::flows);
    assert((cost % 2) == 0);
    return cost;
}
