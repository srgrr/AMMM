#include <bits/stdc++.h>
using namespace std;

double rnd() {
  return double(rand()) / double(RAND_MAX);
}

int main(int argc, char** argv) {
  cout.setf(ios::fixed);
  cout.precision(2);
  int nTasks = atoi(argv[1]);
  int nCpus  = atoi(argv[2]);
  int K      = atoi(argv[3]);
  int seed;
  if(argc > 4) {
    seed = atoi(argv[4]);
  }
  else {
    seed = time(NULL);
  }
  srand(seed);
  cout << "nTasks=" << nTasks << ";" << endl;
  cout << "nCpus=" << nCpus << ";" << endl;
  cout << "K=" << K << ";" << endl;
  cout << "rt=[";
  for(int i=0; i<nTasks; ++i) {
    if(i>0) cout << " ";
    cout << rnd();
  }
  cout << "];" << endl;
  cout << "rc=[";
  for(int i=0; i<nCpus; ++i) {
    if(i>0) cout << " ";
    cout << rnd();
  }
  cout << "];" << endl;
}
