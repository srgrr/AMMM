#pragma once
#include "solver.h"
#include <set>
#include <algorithm>


class GRASP : public Solver {
private:
  int max_grasp_iterations = 10;
  int max_solution_generation_attempts = 2;
  int beam_size = 10;
  double alpha = 0.25;
  Solver::solution get_randomized_solution();
  Solver::solution local_search(solution& sol);
  std::vector< Solver::solution > generate_neighbors(Solver::solution& sol);
  void readjust_centers(Solver::solution& sol);


public:
  GRASP(std::istream& in) : Solver(in) {}
  Solver::solution solve();
};
