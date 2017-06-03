#pragma once
#include "solver.h"

class BRKGA : public Solver {
private:
  int num_individuals = 1000;
  int num_generations = 100;
  double elite_p = 0.05;
  double mutant_p = 0.05;
  std::vector< double > generate_random_vector(int vector_size);
  std::vector< double > generate_individual();
  Solver::solution decode_individual(std::vector< double >& ind);

public:
  BRKGA(std::istream& in) : Solver(in) {};
  Solver::solution solve();
};
