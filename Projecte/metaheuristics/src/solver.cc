#include "solver.h"

Solver::Solver() {

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

}

bool Solver::is_solution_valid() {

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
