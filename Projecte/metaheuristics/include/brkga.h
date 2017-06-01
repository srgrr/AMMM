#pragma once
#include "solver.h"

class BRKGA : public Solver {

public:
  BRKGA(std::istream& in) : Solver(in) {};
  Solver::solution solve();
};
