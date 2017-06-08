#include "grasp.h"


/*
  Given a feasible solution, performs a beam-search that tries to improve
  it
*/
void GRASP::local_search(Solver::solution& current_solution) {
  std::set< Solver::solution > S;
  S.insert(current_solution);
  while(!S.empty()) {
    Solver::solution cur = *S.begin();
    if(cur < best_solution) {
      best_solution = cur;
      std::cout << "MILESTONE | COST " << best_solution.solution_cost <<  " TIME " << _ellapsed_time() << std::endl;
    }
    S.erase(cur);
    std::vector< Solver::solution > neighbors = generate_neighbors(cur);
    for(Solver::solution& neighbor : neighbors) {
      S.insert(neighbor);
      while(int(S.size()) > beam_size) {
        S.erase(--S.end());
      }
    }
  }
}

Solver::solution GRASP::solve() {
  for(int i=0; i<max_grasp_iterations; ++i) {
    solution sol(num_locations, num_cities);
    sol = get_randomized_solution(alpha);
    if(sol.is_valid) {
      local_search(sol);
    }
  }
  return best_solution;
}

Solver::solution GRASP::get_randomized_solution(double alpha) {
  Solver::solution ret(num_locations, num_cities);
  std::vector< int > city_order(num_cities);
  for(int i=0; i<num_cities; ++i) {
    city_order[i] = i;
  }
  std::random_shuffle(city_order.begin(), city_order.end());
  for(int i=0; i<num_cities; ++i) {
    int current_city = city_order[i];
    std::vector< std::pair<double, std::pair<int, int> > > candidate_list;
    for(int primary=0; primary<num_locations; ++primary) {
      for(int secondary=0; secondary<num_locations; ++secondary) {
        ret.city_primary_center[current_city] = primary;
        ret.city_secondary_center[current_city] = secondary;
        readjust_centers(ret);
        if(ret.is_valid) {
          candidate_list.push_back({ret.solution_cost, {primary, secondary}});
        }
      }
    }
    std::sort(candidate_list.begin(), candidate_list.end());
    if(candidate_list.empty()) {
      ret.is_valid = false;
      return ret;
    }
    int max_val = std::max(1, int(double(candidate_list.size()*alpha)));
    int chosen = rand()%max_val;
    ret.solution_cost = candidate_list[chosen].first;
    ret.city_primary_center[current_city] = candidate_list[chosen].second.first;
    ret.city_secondary_center[current_city] = candidate_list[chosen].second.second;
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
