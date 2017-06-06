#include "grasp.h"


/*
  Given a feasible solution, performs a beam-search that tries to improve
  it
*/
Solver::solution GRASP::local_search(Solver::solution& current_solution) {
  std::set< Solver::solution > S;
  S.insert(current_solution);
  Solver::solution best_solution = current_solution;
  std::cout << "[ LOCAL SEARCH ]" << std::endl;
  int it_count = 0;
  while(!S.empty()) {
    std::cout << "\tIteration " << ++it_count << std::endl;
    std::cout << "\tBest solution cost " << best_solution.solution_cost << std::endl;
    std::cout << "\tSolution bag size is " << S.size() << std::endl;
    std::cout << "Operating at interval [" << (*S.begin()).solution_cost << ", " << (*--S.end()).solution_cost << "]" << std::endl;
    Solver::solution cur = *S.begin();
    if(cur.solution_cost < best_solution.solution_cost) {
      best_solution = cur;
    }
    S.erase(cur);
    std::vector< Solver::solution > neighbors = generate_neighbors(cur);
    std::cout << "\tFound " << int(neighbors.size()) << " neighbors." << std::endl;
    for(Solver::solution& neighbor : neighbors) {
      S.insert(neighbor);
      while(int(S.size()) > beam_size) {
        S.erase(--S.end());
      }
    }
  }
  return best_solution;
}

Solver::solution GRASP::solve() {
  solution best_solution(num_locations, num_cities);
  for(int i=0; i<max_grasp_iterations; ++i) {
    solution sol(num_locations, num_cities);
    sol = get_randomized_solution(true, this->alpha);
    if(sol.is_valid) {
      sol = local_search(sol);
      if(sol < best_solution) {
        best_solution = sol;
      }
    }
    std::cout << "Iteration " << i << " | Best cost: " << best_solution.solution_cost << std::endl;
  }
  return best_solution;
}
