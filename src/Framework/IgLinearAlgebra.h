
#ifndef IGUANA_IG_LINEAR_ALGEBRA_H
#define IGUANA_IG_LINEAR_ALGEBRA_H


#include <cassert>

template <class T, int N>
class StoragePolicy
{
public:
  typedef T TYPE;
  typedef T STORAGE_TYPE;
  static const int ELEMENTS = N;
  static const int SIZE = ELEMENTS * sizeof(TYPE);
  typedef StoragePolicy<T, N> STORAGE_CLASS;
  StoragePolicy(TYPE a, ...)
    {
      // TODO: implement using var args
      assert(false);
    }
  TYPE *data(void) {return m_array; }
protected:
  T m_array[ELEMENTS];
};

template <class T>
class StoragePolicy<T, 2>
{
public:
  typedef T TYPE;
  typedef T STORAGE_TYPE;
  static const int ELEMENTS = 2;
  static const int SIZE = ELEMENTS * sizeof(TYPE);
  StoragePolicy(TYPE x, TYPE y)
    {
      m_array[0] = x;
      m_array[1] = y;
    }
  TYPE *data(void) {return m_array; }
protected:
  TYPE m_array[2];
};


template <class T>
class StoragePolicy<T, 3>
{
public:
  typedef T TYPE;
  static const int ELEMENTS=3;
  static const int SIZE = ELEMENTS * sizeof(TYPE);
  StoragePolicy(TYPE x, TYPE y, TYPE z)
    {
      m_array[0] = x;
      m_array[1] = y;
      m_array[2] = z;
    }
  TYPE *data(void) {return m_array; }
protected:
  TYPE m_array[3];
};

template <class T>
class StoragePolicy<T, 4>
{
public:
  typedef T TYPE;
  static const int ELEMENTS=4;
  static const int SIZE = ELEMENTS * sizeof(TYPE);

  StoragePolicy(TYPE x, TYPE y, TYPE z, TYPE w)
    {
      m_array[0] = x;
      m_array[1] = y;
      m_array[2] = z;
      m_array[3] = w;
    }
  TYPE *data(void) {return m_array; }
protected:
  TYPE m_array[ELEMENTS];
};

template <template <class U, int V> class STORAGE,
  class T,
  int N>
class GetterPolicy : public STORAGE<T, N>
{
public:
  typedef STORAGE<T, N> S;
  inline typename S::TYPE &operator[](int index) { return S::m_array[index]; }
};


template <template <class U, int V> class STORAGE, class T>
class GetterPolicy<STORAGE, T, 2> : public STORAGE<T, 2>
{
public:
  GetterPolicy(T x = 0, T y = 0)
    :S(x,y) {}
  typedef STORAGE<T, 2> S;
  inline typename S::TYPE &operator[](int index) { return S::m_array[index]; }
  inline typename S::TYPE &x(void) { return S::m_array[0]; }
  inline typename S::TYPE &y(void) { return S::m_array[1]; }
};


template <template <class U, int V> class STORAGE, class T>
class GetterPolicy<STORAGE, T, 3> : public STORAGE<T, 3>
{
public:
  GetterPolicy(T x = 0, T y = 0, T z = 0)
    :S(x,y,z) {}
  typedef STORAGE<T, 3> S;
  inline typename S::TYPE &operator[](int index) { return S::m_array[index]; }
  inline typename S::TYPE &x(void) { return S::m_array[0]; }
  inline typename S::TYPE &y(void) { return S::m_array[1]; }
  inline typename S::TYPE &z(void) { return S::m_array[2]; }
};

template <template <class U, int V> class STORAGE, class T>
class GetterPolicy<STORAGE, T, 4> : public STORAGE<T, 4>
{
public:
  typedef STORAGE<T, 4> S;
  GetterPolicy(T x = 0, T y = 0, T z = 0, T w = 0)
    :S(x,y,z,w) {}
  inline typename S::TYPE &operator[](int index) { return S::m_array[index]; }
  inline typename S::TYPE &x(void) { return S::m_array[0]; }
  inline typename S::TYPE &y(void) { return S::m_array[1]; }
  inline typename S::TYPE &z(void) { return S::m_array[2]; }
  inline typename S::TYPE &w(void) { return S::m_array[3]; }
};

typedef GetterPolicy<StoragePolicy, double, 2> IgV2d;
typedef GetterPolicy<StoragePolicy, double, 3> IgV3d;
typedef GetterPolicy<StoragePolicy, double, 4> IgV4d;
typedef GetterPolicy<StoragePolicy, int, 2> IgV2i;
typedef GetterPolicy<StoragePolicy, int, 3> IgV3i;
typedef GetterPolicy<StoragePolicy, int, 4> IgV4i;
typedef GetterPolicy<StoragePolicy, float, 2> IgV2f;
typedef GetterPolicy<StoragePolicy, float, 3> IgV3f;
typedef GetterPolicy<StoragePolicy, float, 4> IgV4f;

#endif // IGUANA_IG_LINEAR_ALGEBRA_H
