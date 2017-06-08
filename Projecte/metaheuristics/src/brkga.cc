#include "brkga.h"

std::vector< double > BRKGA::generate_random_vector(int vector_size) {
  std::vector< double > ret(vector_size);
  for(double& x : ret) x = ((double) rand() / (RAND_MAX));
  return ret;
}

std::vector< double > BRKGA::generate_individual() {
  // an individual is represented as a vector of num_cities entries
  std::vector< double > ret = generate_random_vector(num_cities);
  return ret;
}

std::vector< double > BRKGA::crossover(std::vector< double >& elite, std::vector< double >& non_elite) {
  std::vector< double > ret(num_cities, 0.0);
  for(int i=0; i<num_cities; ++i) {
    double prob = ((double) rand() / (RAND_MAX));
    if(prob <= elite_inheritance_prob) ret[i] = elite[i];
    else ret[i] = non_elite[i];
  }
  return ret;
}

Solver::solution BRKGA::decode_individual(std::vector< double >& individual) {
  Solver::solution ret(num_locations, num_cities);
  std::vector< std::pair< double, int > > sorted_cities(num_cities);
  for(int i=0; i<num_cities; ++i) {
    sorted_cities[i] = {double(city_population[i])*individual[i], i};
  }
  std::sort(sorted_cities.begin(), sorted_cities.end());
  ret.is_valid = true;
  for(int i=0; i<num_cities; ++i) {
    int current_city = sorted_cities[i].second;
    std::pair<int, int> best_pair = {-1, -1};
    double best_cost = -1.0;
    for(int primary = 0; primary<num_locations; ++primary) {
      for(int secondary = 0; secondary<num_locations; ++secondary) {
        ret.city_primary_center[current_city] = primary;
        ret.city_secondary_center[current_city] = secondary;
        readjust_centers(ret);
        if(ret.is_valid) {
          if(best_cost < 0.0 || ret.solution_cost < best_cost) {
            best_cost = ret.solution_cost;
            best_pair = {primary, secondary};
          }
        }
      }
    }
    if(best_pair == std::pair<int, int>(-1, -1)) {
      ret.is_valid = false;
      return ret;
    }
    ret.city_primary_center[current_city] = best_pair.first;
    ret.city_secondary_center[current_city] = best_pair.second;
  }
  readjust_centers(ret);
  return ret;
}

/*
  A chromosome here is a vector of num_cities reals and is decoded as follow:
  Assign each gene to a city and sort them according to the following formula:
    citypopulation * genevalue
  Then greedily build a solution exactly the same way as we do in the GRASP
  source code, but with alpha = 1 and assigning the cities in the order that
  we got by sorting them according to the previous formula.
*/
Solver::solution BRKGA::solve() {
  std::vector< std::vector< double > > individuals(num_individuals);
  std::vector< std::vector< double > > new_individuals(num_individuals);
  std::vector< std::vector< double > >& current = individuals;
  std::vector< std::vector< double > >& next_v = new_individuals;
  solution ret(num_locations, num_cities);
  // generate the initial population
  std::cout << "[INFO]: Generating initial population..." << std::endl;
  #pragma omp parallel for shared(individuals)
  for(int i=0; i<num_individuals; ++i) {
    individuals[i] = generate_individual();
    for(int j=0; j<num_retries && !decode_individual(individuals[i]).is_valid; ++j) {
      individuals[i] = generate_individual();
    }
  }

  int num_elite = int(double(num_individuals) * elite_p);
  assert(num_elite > 0);
  int num_crossover = int(double(num_individuals) * (1.0 - (elite_p + mutant_p)));
  int num_mutants   = num_individuals - (num_elite + num_crossover);
  assert(num_crossover + num_mutants > 0);

  for(int gen=0; gen<num_generations; ++gen) {
    std::cout << "[INFO]: Starting generation " << gen << "..." << std::endl;
    // decode the solutions
    std::vector< std::pair< Solver::solution, std::vector< double > > > solutions(num_individuals);
    std::cout << "\tDecoding solutions..." << std::endl;
    #pragma omp parallel for shared(solutions)
    for(int i=0; i<num_individuals; ++i) {
      solutions[i] = {decode_individual(current[i]), current[i]};
    }
    // sort them according to their fitness
    std::cout << "\tSorting..." << std::endl;
    std::sort(solutions.begin(), solutions.end());
    // update best found solution
    std::cout << "\tUpdated best found solution..." << std::endl;
    std::cout << "\t";
    for(int i=0; i<num_individuals; ++i) {
      std::cout << solutions[i].first.solution_cost << " ";
      if(solutions[i].first < ret) {
        ret = solutions[i].first;
      }
      current[i] = solutions[i].second;
    }
    std::cout << std::endl;
    std::cout << "\tBest solution cost so far: " << ret.solution_cost << std::endl;
    // copy intact the elite
    std::cout << "\tCopying the elite..." << std::endl;
    for(int i=0; i<num_elite; ++i) {
      next_v[i] = current[i];
    }
    // create crossover individuals
    std::cout << "\tCrossing over..." << std::endl;
    for(int i=0; i<num_crossover; ++i) {
      int elite_member = std::rand() % num_elite;
      int non_elite_member = std::rand() % (num_individuals - num_elite) + num_elite;
      next_v[i + num_elite] = crossover(individuals[elite_member], individuals[non_elite_member]);
    }
    // create mutant individuals
    std::cout << "\tCreating random mutants" << std::endl;
    for(int i=0; i<num_mutants; ++i) {
      next_v[i + num_elite + num_crossover] = generate_individual();
    }
    std::swap(current, next_v);
  }
  return ret;
}
