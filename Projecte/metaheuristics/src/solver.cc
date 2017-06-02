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
