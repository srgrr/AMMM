#include "grasp.h"

Solver::solution GRASP::get_randomized_solution() {
    // current population that a location must serve
    std::vector< double > location_population(num_locations, 0.0);
    // maximum distance from a city (primary)
    std::vector< double > max_primary_dist(num_locations, 0.0);
    // maximum distance from a city (secondary)
    std::vector< double > max_secondary_dist(num_locations, 0.0);

    solution ret(num_locations, num_cities);
    /*
      For each city, get all the valid pairs (primary, secondary),
      sort them according to the following function:

      cost = sum of all minimum location prices

      minimum location price = the price of the cheapest location that can be
      placed and still satisfy the problem's constraints


    */
    for(int i=0; i<num_cities; ++i) {
      // cost < < primary_location, secondary_location >, < primary_type, secondary_type > >
      std::vector< std::pair< double, std::pair< std::pair< int, int >, std::pair< int, int > > > > candidate_list;
      for(int primary=0; primary<num_locations; ++primary) {
        for(int secondary=0; secondary<num_locations; ++secondary) {
          // ... which of course must be different
          if(primary == secondary) continue;
          /*
            For each candidate, compute the cost function of the partial solution
            that consists of the current solution + the candidate element
          */

          // this distance comes from the constraint imposed in equation 7
          if(loc2loc_dist[primary][secondary] < d_center) continue;
          // if any of these two locations had no center before, then we must
          // check if there is any location which has a center such that there
          // is at distance < d_center from any of our two chosen centers
          int to_check[2] = {primary, secondary};
          bool can = true;
          for(int k=0; k<2 && can; ++k) {
            // is this location already assigned?
            if(ret.location_center_type[to_check[k]] != -1) continue;
            for(int l=0; l<num_locations && can; ++l) {
              if(l == to_check[k]) continue;
              can &= (ret.location_center_type[l] == -1 || loc2loc_dist[l][to_check[k]] >= d_center);
            }
          }
          if(!can) continue;
          // lets find the cheapests center types that satisfies the population requeriments
          double current_cost = ret.solution_cost;
          if(ret.location_center_type[primary] != -1) {
            current_cost -= type_cost[ret.location_center_type[primary]];
          }
          if(ret.location_center_type[secondary] != -1) {
            current_cost -= type_cost[ret.location_center_type[secondary]];
          }
          // check if the primary location is suitable
          double population_primary =
            location_population[primary] + city_population[i];
          double cur_max_primary_dist =
            std::max(max_primary_dist[primary], city2loc_dist[i][primary]);
          double best_primary = -1.0;
          int best_primary_type = -1;
          for(int t=0; t<num_types; ++t) {
            if(type_capacity[t] >= population_primary && type_distance[t] >= cur_max_primary_dist) {
              if(best_primary < 0.0 || type_cost[t] < best_primary) {
                best_primary = type_cost[t];
                best_primary_type = t;
              }
            }
          }
          if(best_primary < 0.0) continue;

          // check if the secondary lcoation is suitable
          double population_secondary =
            location_population[secondary] + 0.1*double(city_population[i]);
          double cur_max_secondary_dist =
            std::max(max_secondary_dist[secondary], city2loc_dist[i][secondary]);
          double best_secondary = -1.0;
          int best_secondary_type = -1;
          for(int t=0; t<num_types; ++t) {
            if(type_capacity[t] >= population_secondary && 3.0*type_distance[t] >= cur_max_secondary_dist) {
              if(best_secondary < 0.0 || type_cost[t] < best_secondary) {
                best_secondary = type_cost[t];
                best_secondary_type = t;
              }
            }
          }
          if(best_secondary < 0.0) continue;
          // did we find two center types suitable to our candidate?
          if(best_primary >= 0.0 && best_secondary >= 0.0) {
            current_cost += best_primary + best_secondary;
            // add this candidate to the list
            candidate_list.push_back({current_cost, {{primary, secondary}, {best_primary_type, best_secondary_type}}});
          }
        // end of big inner loop
        }
      }
      if(candidate_list.empty()) return ret;
      std::sort(candidate_list.begin(), candidate_list.end());
      int max_val = std::max(1, int(double(candidate_list.size()*alpha)));
      int chosen = rand()%max_val;
      double new_cost   = candidate_list[chosen].first;
      int prim_location = candidate_list[chosen].second.first.first;
      int secn_location = candidate_list[chosen].second.first.second;
      int prim_type     = candidate_list[chosen].second.second.first;
      int secn_type     = candidate_list[chosen].second.second.second;
      // apply the chosen step to the solution
      ret.city_primary_center[i] = prim_location;
      ret.city_secondary_center[i] = secn_location;
      ret.location_center_type[prim_location] = prim_type;
      ret.location_center_type[secn_location] = secn_type;
      ret.solution_cost = new_cost;
      // update the auxiliary data structures
      location_population[prim_location] += city_population[i];
      location_population[secn_location] += 0.1 * double(city_population[i]);
      max_primary_dist[prim_location] = std::max(max_primary_dist[prim_location], city2loc_dist[i][prim_location]);
      max_secondary_dist[prim_location] = std::max(max_secondary_dist[secn_location], city2loc_dist[i][secn_location]);
    }
    ret.is_valid = true;
    return ret;
}

void GRASP::local_search(Solver::solution& current_solution) {

}

Solver::solution GRASP::solve() {


  solution best_solution(num_locations, num_cities);

  for(int i=0; i<max_grasp_iterations; ++i) {
    solution sol(num_locations, num_cities);
    for(int j=0; j<max_solution_generation_attempts && !sol.is_valid; ++j) {
      sol = get_randomized_solution();
    }
    local_search(sol);
    if(sol.is_valid) {
      if(!best_solution.is_valid || sol.solution_cost < best_solution.solution_cost) {
        best_solution = sol;
      }
    }
  }

  return best_solution;

}
