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


/*
  Given a solution, computes the cheapest center that can go to a location.
  The solution can be partial
*/
void Solver::readjust_centers(Solver::solution& sol) {
  sol.solution_cost = 0;
  sol.is_valid = true;
  std::vector< double > location_population(num_locations, 0.0);
  std::vector< double > max_primary_dist(num_locations, 0.0);
  std::vector< double > max_secondary_dist(num_locations, 0.0);
  for(int i=0; i<num_cities; ++i) {
    // are we in a partial solution?
    if(sol.city_primary_center[i] == -1) continue;
    int prim = sol.city_primary_center[i];
    location_population[prim] += city_population[i];
    int secn = sol.city_secondary_center[i];
    location_population[secn] += 0.1 * double(city_population[i]);
    max_primary_dist[prim] = std::max(max_primary_dist[prim], city2loc_dist[i][prim]);
    max_secondary_dist[secn] = std::max(max_secondary_dist[secn], city2loc_dist[i][secn]);
  }
  for(int i=0; i<num_locations; ++i) {
    if(std::abs(location_population[i]) < 1e-4) {
      sol.location_center_type[i] = -1;
      continue;
    }
    double best_cost = type_cost[0];
    int best_center = 0;
    bool ok = false;
    for(int j=0; j<num_types; ++j) {
      // check that the proposed type fulfills all the distance and population
      // constraints
      if(type_distance[j] < max_primary_dist[i]) continue;
      if(3.0*type_distance[j] < max_secondary_dist[i]) continue;
      if(location_population[i] > type_capacity[j]) continue;
      if(!ok || type_cost[j] <= best_cost) {
        ok = true;
        best_cost = type_cost[j];
        best_center = j;
      }
    }
    if(!ok) {
      sol.is_valid = false;
      return;
    }
    sol.location_center_type[i] = best_center;
    sol.solution_cost += best_cost;
  }
  sol.is_valid = is_solution_valid(sol, true, false);
}

Solver::solution GRASP::get_randomized_solution(bool greedy, double alpha) {
  if(!greedy) alpha = 1.0;
  Solver::solution ret(num_locations, num_cities);
  for(int i=0; i<num_cities; ++i) {
    std::vector< std::pair<double, std::pair<int, int> > > candidate_list;
    for(int primary=0; primary<num_locations; ++primary) {
      for(int secondary=0; secondary<num_locations; ++secondary) {
        ret.city_primary_center[i] = primary;
        ret.city_secondary_center[i] = secondary;
        readjust_centers(ret);
        if(ret.is_valid) {
          candidate_list.push_back({ret.solution_cost, {primary, secondary}});
        }
      }
    }
    std::cout << "[ RANDOM GENERATION ]: Found " << int(candidate_list.size()) <<
    " candidates for city " << i << "." << std::endl;
    if(greedy) {
      std::sort(candidate_list.begin(), candidate_list.end());
    }
    if(candidate_list.empty()) {
      ret.is_valid = false;
      return ret;
    }
    int max_val = std::max(1, int(double(candidate_list.size()*alpha)));
    int chosen = rand()%max_val;
    ret.solution_cost = candidate_list[chosen].first;
    ret.city_primary_center[i] = candidate_list[chosen].second.first;
    ret.city_secondary_center[i] = candidate_list[chosen].second.second;
    readjust_centers(ret);
  }
  return ret;
}

/*
  Given a solution, computes all its possible neighbors and returns the ones
  that show some improvement on the solution cost

  Here, two solutions A and B are neighbors if they are identical except for a
  city that uses a different pair of centers as primary and secondary (note that
  there is no need to change both centers, a solution can be considered a
  neighbor if we change only one of them, too).
*/
std::vector< Solver::solution > GRASP::generate_neighbors(Solver::solution& sol) {
  std::vector< Solver::solution > ret;
  /*
    Pairs of cities, swap primary-primary
  */
  #pragma omp parallel for shared(ret, sol)
  for(int i=0; i<num_cities; ++i) {
    for(int j=i+1; j<num_cities; ++j) {
      Solver::solution cand = sol;
      std::swap(cand.city_primary_center[i], cand.city_primary_center[j]);
      readjust_centers(cand);
      if(cand < sol) {
        #pragma omp critical
          ret.push_back(cand);
      }
    }
  }
  /*
    Pairs of cities, swap primary-secondary
  */
  #pragma omp parallel for shared(ret, sol)
  for(int i=0; i<num_cities; ++i) {
    for(int j=0; j<num_cities; ++j) {
      if(i == j) continue;
      Solver::solution cand = sol;
      std::swap(cand.city_primary_center[i], cand.city_secondary_center[j]);
      readjust_centers(cand);
      if(cand < sol) {
        #pragma omp critical
          ret.push_back(cand);
      }
    }
  }
  /*
    Pairs of cities, swap secondary-secondary
  */
  #pragma omp parallel for shared(ret, sol)
  for(int i=0; i<num_cities; ++i) {
    for(int j=i+1; j<num_cities; ++j) {
      Solver::solution cand = sol;
      std::swap(cand.city_secondary_center[i], cand.city_secondary_center[j]);
      readjust_centers(cand);
      if(cand < sol) {
        #pragma omp critical
          ret.push_back(cand);
      }
    }
  }
  /*
    Single city, change its primary and secondary centers
    (or only one of them)
  */
  #pragma omp parallel for shared(ret, sol)
  for(int i=0; i<num_cities; ++i) {
    for(int primary=0; primary<num_locations; ++primary) {
      for(int secondary=0; secondary<num_locations; ++secondary) {
        Solver::solution cand = sol;
        cand.city_primary_center[i] = primary;
        cand.city_secondary_center[i] = secondary;
        readjust_centers(cand);
        if(cand < sol) {
          #pragma omp critical
            ret.push_back(cand);
        }
      }
    }
  }
  #pragma omp barrier
  return ret;
}
