#include <iostream>
#include <vector>
#include <cassert>

struct A
{
  template <class T>
  void foo (T a)
  {
    assert(count++ == 1);
  }

  template <class T, class A,  template <class U, class V> class C>
  void foo (C<T, A> a)
  {
    assert(count++ == 0);
  }
  int count;
};


int
main (int argc, char **argv)
{
  A foo;
  foo.count=0;
  foo.foo(std::vector<float> ());
  foo.foo(1);
}
