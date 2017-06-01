#include <iostream>
#include "solver.h"
#include "grasp.h"
#include "brkga.h"


int main(int argc, char** argv) {
  if(argc<2) {
    std::cout << "Error, no algorithm specified!" << std::endl;
    std::exit(1);
  }
  std::string heur_algorithm(argv[1]);
  Solver solver;
  if(heur_algorithm == "GRASP") solver = GRASP(std::cin);
  else if(heur_algorithm == "BRKGA") solver = BRKGA(std::cin);
  else {
    std::cout << "Error, wrong algorithm name!" << std::endl;
    std::exit(2);
  }
  solver.print_data_summary();
}
