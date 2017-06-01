#pragma once
#include <istream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

class Solver {
protected:
  // problem data
  int num_cities, num_types, num_locations;
  std::vector< std::vector< double > > city_coordinates;
  std::vector< std::vector< double > > location_coordinates;
  std::vector< int > city_population;
  double d_center;
  std::vector< int > type_capacity;
  std::vector< double > type_distance;
  std::vector< double > type_cost;
  std::vector< std::vector< double > > loc2loc_dist;
  std::vector< std::vector< double > > city2loc_dist;

public:
  Solver();
  Solver(std::istream& in);

  bool is_solution_valid();
  void print_data_summary();

  // solution container
  // its default constructor builds an invalid solution
  struct solution {
      std::vector< int > location_center_type;
      std::vector< int > city_primary_center;
      std::vector< int > city_secondary_center;
      bool is_valid;
      double solution_cost;
      solution(int num_locs, int num_cities) {
        location_center_type = std::vector< int >(num_locs);
        city_primary_center = std::vector< int >(num_cities);
        city_secondary_center = std::vector< int >(num_cities);
        is_valid = false;
        solution_cost = 0.0;
      }
  };


  virtual Solver::solution solve();

  void print_solution(const Solver::solution& sol);


};
