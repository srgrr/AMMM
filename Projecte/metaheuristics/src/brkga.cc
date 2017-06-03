#include "brkga.h"

std::vector< double > BRKGA::generate_random_vector(int vector_size) {
  std::vector< double > ret(vector_size);
  for(double& x : ret) x = ((double) rand() / (RAND_MAX));
  return ret;
}

std::vector< double > BRKGA::generate_individual() {
  // an individual is represented as a vector of 2*num_cities entries
  std::vector< double > ret = generate_random_vector(2 * num_cities);
  return ret;
}

Solver::solution BRKGA::decode_individual(std::vector< double >& ind) {
  Solver::solution ret(num_locations, num_cities);
  for(int i=0; i<num_cities; ++i) {
    ret.city_primary_center[i] = int(ind[2*i] * double(num_locations));
    ret.city_secondary_center[i] = int(ind[2*i + 1] * double(num_locations));
  }
  readjust_centers(ret);
  return ret;
}

/*
  A chromosome here is a vector of 2*num_cities reals and is decoded as follow:
  The numbers on the positions 2*k and 2*k + 1 represent the primary and secondary
  location index for the kth city
*/
Solver::solution BRKGA::solve() {
  std::vector< std::vector< double > > individuals(num_individuals);
  solution ret(num_locations, num_cities);
  // generate the initial population
  for(auto& indv : individuals) {
    indv = generate_individual();
  }

  for(int gen=0; gen<num_generations; ++gen) {
    // decode the solutions
    std::vector< std::pair< Solver::solution, std::vector< double > > > solutions(individuals.size());
    for(int i=0; i<num_cities; ++i) {
      solutions[i].first = decode_individual(individuals[i]);
      solutions[i].second = individuals[i];
    }
    // sort them according to their fitness
    std::sort(solutions.begin(), solutions.end());
    // update best found solution
    for(int j=0; j<int(solutions.size()); ++j) {
      if(solutions[j].first < ret) {
        ret = solutions[j].first;
      }
    }
  }
  return ret;
}
