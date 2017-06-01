#include <iostream>
#include <cstdlib>
using namespace std;

double random_float() {
  return ((double) rand() / (RAND_MAX));
}

int main(int argc, char** argv) {
  cout.setf(ios::fixed);
  cout.precision(2);
  int num_cities = atoi(argv[1]);
  int num_locations = atoi(argv[2]);
  int num_types = atoi(argv[3]);
  int seed = atoi(argv[4]);
  srand(seed);
  cout << "numCities = " << num_cities << ";" << endl;
  cout << "numLocations = " << num_locations << ";" << endl;
  cout << "numTypes = " << num_types << ";" << endl;
  cout << "cityCoordinates = [" << endl;
  for(int i=0; i<num_cities; ++i) {
    cout << "[" << random_float() << " " << random_float() << "]" << endl;
  }
  cout << "];" << endl;
  cout << "locationCoordinates = [" << endl;
  for(int i=0; i<num_locations; ++i) {
    cout << "[" << random_float() << " " << random_float() << "]" << endl;
  }
  cout << "];" << endl;
  cout << "cityPopulation = [";
  for(int i=0; i<num_cities; ++i) {
    cout << rand()%100 << " ";
  }
  cout << "];" << endl;
  cout << "d_center = " << random_float()/4.0 << ";" << endl;
  cout << "typeCapacity = [";
  for(int i=0; i<num_types; ++i) {
    cout << rand()%100 << " ";
  }
  cout << "];" << endl;
  cout << "typeDistance = [";
  for(int i=0; i<num_types; ++i) {
    cout << random_float() << " ";
  }
  cout << "];" << endl;
  cout << "typeCost = [";
  for(int i=0; i<num_types; ++i) {
    cout << rand()%10 + 1 << " ";
  }
  cout << "];" << endl;
}

