#include <iostream>
#include <random>
#include <chrono>

#define sumSquare(x, y) ((x*x) + (y*y))

const int epoch = 1000000000;



using namespace std;

int main()
{
  int counter = 0;
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  uniform_real_distribution<double> distribution(-1.0, 1.0); 
  for (int i = 0; i < epoch; i++)
  {
    double x = distribution(generator);
    distribution.reset();
    double y = distribution(generator);
    if (sumSquare(x, y) <= 1.0) counter += 1;
  }
  double pi = 4.0 * counter / epoch;
  cout << "pi = " << pi << endl;
  return 0;
}
