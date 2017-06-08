#pragma once
#include <omp.h>
#include <istream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <queue>
#include <vector>
#include <cmath>
#include <ctime>
#include <cassert>

class Solver {
public:
  // solution container
  // its default constructor builds an invalid solution
  struct solution {
      std::vector< int > location_center_type;
      std::vector< int > city_primary_center;
      std::vector< int > city_secondary_center;
      bool is_valid;
      double solution_cost;
      solution(int num_locs, int num_cities) {
        location_center_type = std::vector< int >(num_locs, -1);
        city_primary_center = std::vector< int >(num_cities, -1);
        city_secondary_center = std::vector< int >(num_cities, -1);
        is_valid = false;
        solution_cost = 0.0;
      }
      solution() {
        is_valid = false;
      }
      bool operator<(const Solver::solution& other) const {
        if(!is_valid) return false;
        if(!other.is_valid) return true;
        return solution_cost < other.solution_cost;
      }
  };
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

  void readjust_centers(Solver::solution& sol);

  Solver::solution best_solution;

  std::time_t _start_time;

  double _ellapsed_time() {
    std::time_t cur;
    std::time(&cur);
    return cur - _start_time;
  }

public:
  Solver();
  Solver(std::istream& in);
  void print_data_summary();
  bool is_solution_valid(Solver::solution& sol, bool accept_partial, bool verbose);
  virtual Solver::solution solve();
  void print_solution(const Solver::solution& sol);
};
