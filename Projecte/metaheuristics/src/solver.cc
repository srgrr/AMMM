#include "solver.h"

Solver::Solver() {

}

double _dist(double x1, double y1, double x2, double y2) {
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

  std::vector< std::vector< double > > loc2loc_dist;
  std::vector< std::vector< double > > city2loc_dist;

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

bool Solver::is_solution_valid() {
  return true;
}

void Solver::print_solution(const Solver::solution& sol) {
    std::cout << "Solution cost: " << sol.solution_cost << std::endl;
    std::cout << "Location center type:";
    for(int i=0; i<int(sol.location_center_type.size()); ++i) {
      if(i>0) std::cout << " ";
      std::cout << sol.location_center_type[i];
    }
    std::cout << std::endl;
    std::cout << "City primary center:";
    for(int i=0; i<int(sol.city_primary_center.size()); ++i) {
      if(i>0) std::cout << " ";
      std::cout << sol.city_primary_center[i];
    }
    std::cout << std::endl;
    std::cout << "City secondary center:";
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