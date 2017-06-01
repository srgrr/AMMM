#pragma once
#include <istream>
#include <iostream>
#include <sstream>
#include <vector>

class Solver {
private:
  // problem data
  int num_cities, num_types, num_locations;
  std::vector< std::vector< double > > city_coordinates;
  std::vector< std::vector< double > > location_coordinates;
  std::vector< int > city_population;
  double d_center;
  std::vector< int > type_capacity;
  std::vector< double > type_distance;
  std::vector< double > type_cost;

public:
  Solver();
  Solver(std::istream& in);

  bool is_solution_valid();
  void print_data_summary();

};
