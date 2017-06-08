#pragma once
#include "solver.h"
#include <set>
#include <algorithm>


class GRASP : public Solver {
private:
  int max_grasp_iterations = 10;
  int max_solution_generation_attempts = 100;
  int beam_size = 40;
  double alpha = 0.4;
  Solver::solution local_search(solution& sol);
  Solver::solution get_randomized_solution(double alpha);
  std::vector< Solver::solution > generate_neighbors(Solver::solution& sol);

public:
  GRASP(std::istream& in) : Solver(in) {}
  Solver::solution solve();
};
