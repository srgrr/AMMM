#pragma once
#include "solver.h"

class BRKGA : public Solver {
private:
  int num_individuals = 40;
  int num_generations = 100;
  double elite_p = 0.2;
  double elite_inheritance_prob = 0.7;
  double mutant_p = 0.15;
  std::vector< double > generate_random_vector(int vector_size);
  std::vector< double > generate_individual();
  std::vector< double > crossover(std::vector< double >& elite, std::vector< double >& non_elite);
  Solver::solution decode_individual(std::vector< double >& ind);

public:
  BRKGA(std::istream& in) : Solver(in) {};
  Solver::solution solve();
};
