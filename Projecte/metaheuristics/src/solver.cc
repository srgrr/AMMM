#include "solver.h"

Solver::Solver() {

}

static double _dist(double x1, double y1, double x2, double y2) {
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

Solver::Solver(std::istream& in) {
  in >> num_cities >> num_types >> num_locations;
  city_coordinates =
    std::vector< std::vector< double > >(num_cities, std::vector< double >(2, 0.0));

  for(auto& elem : city_coordinates) {
    in >> elem[0] >> elem[1];
  }

  location_coordinates =
    std::vector< std::vector< double > >(num_locations, std::vector< double >(2, 0.0));

  for(auto& elem : location_coordinates) {
    in >> elem[0] >> elem[1];
  }

  city_population = std::vector< int >(num_cities, 0);

  for(auto& elem : city_population) {
    in >> elem;
  }

  in >> d_center;

  type_capacity = std::vector< int >(num_types, 0);

  for(auto& elem : type_capacity) {
    in >> elem;
  }

  type_distance = std::vector< double >(num_types, 0.0);

  for(auto& elem : type_distance) {
    in >> elem;
  }

  type_cost = std::vector< double >(num_types, 0.0);

  for(auto& elem : type_cost) {
    in >> elem;
  }

  loc2loc_dist =
    std::vector< std::vector< double > >(num_locations, std::vector< double >(num_locations, 0.0));

  for(int i=0; i<num_locations; ++i) {
    for(int j=0; j<i; ++j) {
      loc2loc_dist[i][j] = loc2loc_dist[j][i] =
        _dist(location_coordinates[i][0], location_coordinates[i][1],
        location_coordinates[j][0], location_coordinates[j][1]);
    }
  }

  city2loc_dist =
    std::vector< std::vector< double > >(num_cities, std::vector< double >(num_locations, 0.0));

  for(int i=0; i<num_cities; ++i) {
    for(int j=0; j<num_locations; ++j) {
      city2loc_dist[i][j] =
        _dist(city_coordinates[i][0], city_coordinates[i][1],
              location_coordinates[j][0], location_coordinates[j][1]);
    }
  }

}

bool Solver::is_solution_valid(Solver::solution& sol, bool accept_partial, bool verbose = false) {
  /*
    This function checks that a solution satisfies the constraints
    specified on the PDFs equations
  */

  // equation 2 is always true given that we store Xlt as a vvdouble,
  // in fact, it is not possible to violate it here

  // check that each city has a primary and a secondary center
  // equation 3 and 4
  if(!accept_partial) {
    for(int i=0; i<num_cities; ++i) {
      if(sol.city_primary_center[i] == -1) {
        if(verbose) std::cout << "[ERROR]: City " << i << " has no primary center." << std::endl;
      }
      if(sol.city_secondary_center[i] == -1) {
        if(verbose) std::cout << "[ERROR]. City " << i << " has no secondary center." << std::endl;
      }
    }
  }
  // equation 5
  for(int i=0; i<num_cities; ++i) {
    if(sol.city_primary_center[i] == -1) continue;
    if(sol.city_primary_center[i] == sol.city_secondary_center[i]) {
      if(verbose) std::cout << "[ERROR]: City " << i << " has " <<
      sol.city_primary_center[i] << " as both primary and secondary center." <<
      std::endl;
      return false;
    }
  }

  // equation 6
  for(int i=0; i<num_cities; ++i) {
    if(sol.city_primary_center[i] != -1 && sol.location_center_type[sol.city_primary_center[i]] == -1) {
      if(verbose) std::cout << "[ERROR]: City " << i << " has " << sol.city_primary_center[i] <<
      " as a primary center, but this location has no center" << std::endl;
      return false;
    }
    if(sol.city_secondary_center[i] != -1 && sol.location_center_type[sol.city_secondary_center[i]] == -1) {
      if(verbose) std::cout << "[ERROR]: City " << i << " has " << sol.city_secondary_center[i] <<
      " as a primary center, but this location has no center" << std::endl;
      return false;
    }
  }

  // equation 7
  for(int i=0; i<num_locations; ++i) {
    for(int j=i+1; j<num_locations; ++j) {
      if(sol.location_center_type[i] != -1 && sol.location_center_type[j] != -1 &&
      loc2loc_dist[i][j] < d_center) {
        if(verbose) std::cout << "[ERROR]: Locations " << i << " and " << j <<
        " both have a center, but they are too near wrt d_center!" << std::endl;
        return false;
      }
    }
  }

  // equation 8
  std::vector< double > total_population(num_locations);
  for(int i=0; i<num_cities; ++i) {
    if(sol.city_primary_center[i] == -1) continue;
    total_population[sol.city_primary_center[i]] += city_population[i];
    total_population[sol.city_secondary_center[i]] += 0.1 * double(city_population[i]);
  }
  for(int i=0; i<num_locations; ++i) {
    if(sol.location_center_type[i] == -1) continue;
    if(total_population[i] > type_capacity[sol.location_center_type[i]]) {
      if(verbose) std::cout << "[ERROR]: Location " << i << " has too many population assigned." << std::endl;
      if(verbose) std::cout << "Assigned: " << total_population[i] << ", Capacity: " << type_capacity[sol.location_center_type[i]] << std::endl;
      return false;
    }
  }

  // equation 9
  for(int i=0; i<num_cities; ++i) {
    if(sol.city_primary_center[i] == -1) continue;
    if(city2loc_dist[i][sol.city_primary_center[i]] > type_distance[sol.location_center_type[sol.city_primary_center[i]]]) {
      if(verbose) std::cout << "[ERROR]: City " << i << " has assigned as primary center " << sol.city_primary_center[i] << ", which is too far" << std::endl;
    }
    if(city2loc_dist[i][sol.city_secondary_center[i]] > 3.0 * type_distance[sol.location_center_type[sol.city_secondary_center[i]]]) {
      if(verbose) std::cout << "[ERROR]: City " << i << " has assigned as secondary center " << sol.city_secondary_center[i] << ", which is too far" << std::endl;
    }

  }

  return true;
}

void Solver::print_solution(const Solver::solution& sol) {
    if(!sol.is_valid) {
      std::cout << "No solution found" << std::endl;
      return;
    }
    std::cout << "Solution cost: " << sol.solution_cost << std::endl;
    std::cout << "Location center type: ";
    for(int i=0; i<int(sol.location_center_type.size()); ++i) {
      if(i>0) std::cout << " ";
      std::cout << sol.location_center_type[i];
    }
    std::cout << std::endl;
    std::cout << "City primary center: ";
    for(int i=0; i<int(sol.city_primary_center.size()); ++i) {
      if(i>0) std::cout << " ";
      std::cout << sol.city_primary_center[i];
    }
    std::cout << std::endl;
    std::cout << "City secondary center: ";
    for(int i=0; i<int(sol.city_secondary_center.size()); ++i) {
      if(i>0) std::cout << " ";
      std::cout << sol.city_secondary_center[i];
    }
    std::cout << std::endl;
}

Solver::solution Solver::solve() {
  std::cout << "[ ERROR ]: You shall not call this method!!!" << std::endl;
  std::exit(1);
}

void Solver::print_data_summary() {
  std::cout.setf(std::ios::fixed);
  std::cout.precision(4);
  std::cout << "Num cities: " << num_cities << std::endl;
  std::cout << "Num types:" << num_types << std::endl;
  std::cout << "Num locations:" << num_locations << std::endl;
  std::cout << std::endl;
  std::cout << "City coordinates:" << std::endl;
  for(auto elem : city_coordinates) {
    std::cout << "\t" << elem[0] << ", " << elem[1] << std::endl;
  }
  std::cout << "Location coordinates:" << std::endl;
  for(auto elem : location_coordinates) {
    std::cout << "\t" << elem[0] << ", " << elem[1] << std::endl;
  }
  std::cout << "d_center: " << d_center << std::endl;
  std::cout << "City population:" << std::endl;
  for(auto elem : city_population) {
    std::cout << "\t" << elem << std::endl;
  }
  std::cout << "Type capacity:" << std::endl;
  for(auto elem : type_capacity) {
    std::cout << "\t" << elem << std::endl;
  }
  std::cout << "Type distance:" << std::endl;
  for(auto elem : type_distance) {
    std::cout << "\t" << elem << std::endl;
  }
  std::cout << "Type cost:" << std::endl;
  for(auto elem : type_cost) {
    std::cout << "\t" << elem << std::endl;
  }
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

Solver::solution Solver::get_randomized_solution(bool greedy, double alpha) {
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
std::vector< Solver::solution > Solver::generate_neighbors(Solver::solution& sol) {
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
