#pragma once
#include "solver.h"

class GRASP : public Solver {

public:
  GRASP(std::istream& in) : Solver(in) {};
};
