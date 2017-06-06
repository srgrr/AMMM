#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

double random_float() {
  return ((double) rand() / (RAND_MAX));
}

struct point {
  double x, y;
  point(double _x, double _y) {
    x = _x;
    y = _y;
  }
};

int main(int argc, char** argv) {
  cout.setf(ios::fixed);
  cout.precision(2);
  int num_points = atoi(argv[1]);
  int num_types = atoi(argv[2]);
  int seed = atoi(argv[3]);
  srand(seed);
  std::vector< point > points(num_points);
  for(point& p : points) {
    
  }
}
