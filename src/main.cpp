#include "../include/vec_math.hpp"
#include "../include/boid.hpp"
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
  Boid test(ZeroVec, Vector(1, 0));
  for (int i = 0; i < 2; i++) {
    test.print();
    test.update(1);
    cout << endl;
  }
  test.print();

  test.pos = test.pos.rotate(M_PI / 2);

  for (int i = 0; i < 2; i++) {
    test.print();
    test.update(1);
    cout << endl;
  }
  test.print();
  return 0;
}
