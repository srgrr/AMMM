#pragma once
#include "solver.h"
#include <set>
#include <algorithm>


class GRASP : public Solver {
private:
  int max_grasp_iterations = 10;
  int max_solution_generation_attempts = 100;
  int beam_size = 5;
  double alpha = 0.2;
  inline Solver::solution get_randomized_solution();
  inline Solver::solution local_search(solution& sol);
  inline std::vector< Solver::solution > generate_neighbors(Solver::solution& sol);
  inline void readjust_centers(Solver::solution& sol);


public:
  GRASP(std::istream& in) : Solver(in) {}
  Solver::solution solve();
};
