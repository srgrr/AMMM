#pragma once
#include "solver.h"

class BruteForce : public Solver {
private:

public:
  BruteForce(std::istream& in) : Solver(in) {}
  Solver::solution solve();
};
