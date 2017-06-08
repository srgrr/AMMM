#pragma once
#include "solver.h"
#include <set>
#include <algorithm>


class GRASP : public Solver {
private:
  int max_grasp_iterations = 20;
  int max_solution_generation_attempts = 100;
  int beam_size = 100;
  double alpha = 0.1;
  void local_search(solution& sol);
  Solver::solution get_randomized_solution(double alpha);
  std::vector< Solver::solution > generate_neighbors(Solver::solution& sol);

public:
  GRASP(std::istream& in) : Solver(in) {}
  Solver::solution solve();
};
