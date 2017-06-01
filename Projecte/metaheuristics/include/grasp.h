#pragma once
#include "solver.h"
#include <algorithm>

class GRASP : public Solver {
private:
  int max_grasp_iterations = 10000;
  int max_solution_generation_attempts = 2;
  double alpha = 0.25;

public:
  GRASP(std::istream& in) : Solver(in) {}
  Solver::solution solve();
  Solver::solution get_randomized_solution();
  void local_search(solution& sol);
};
