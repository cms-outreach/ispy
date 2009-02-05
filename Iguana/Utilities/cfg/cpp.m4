dnl +++######################################################################
AC_DEFUN(LAT_CHECK_LONG_LONG,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements long long)
  AC_CACHE_VAL(lat_cv_check_long_long,
   [AC_TRY_COMPILE(,[long long ll = 16LL;],
     lat_cv_check_long_long=yes,
     lat_cv_check_long_long=no)])
  AC_MSG_RESULT($lat_cv_check_long_long)
  if test $lat_cv_check_long_long = yes ; then
    AC_DEFINE(HAVE_LONG_LONG)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_INT64,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements __int64)
  AC_CACHE_VAL(lat_cv_check_int64,
   [AC_TRY_COMPILE(,[__int64 ll = 16i64; unsigned __int64 ull = 16ui64;],
     lat_cv_check_int64=yes,
     lat_cv_check_int64=no)])
  AC_MSG_RESULT($lat_cv_check_int64)
  if test $lat_cv_check_int64 = yes ; then
    AC_DEFINE(HAVE_INT64)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_INT128,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements __int128)
  AC_CACHE_VAL(lat_cv_check_int64,
   [AC_TRY_COMPILE(,[__int128 ll = 16i128; unsigned __int128 ull = 16ui128;],
     lat_cv_check_int128=yes,
     lat_cv_check_int128=no)])
  AC_MSG_RESULT($lat_cv_check_int128)
  if test $lat_cv_check_int128 = yes ; then
    AC_DEFINE(HAVE_INT128)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SIGNED,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements signed keyword)
  AC_CACHE_VAL(lat_cv_check_signed,
   [AC_TRY_COMPILE([void f(signed char *p){} void f(unsigned char *p){}
	void f(char *p){}], [], lat_cv_check_signed=yes, lat_cv_check_signed=no)])
  AC_MSG_RESULT($lat_cv_check_signed)
  if test $lat_cv_check_signed = yes ; then
    AC_DEFINE(HAVE_SIGNED)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_EXPLICIT,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements explicit keyword)
  AC_CACHE_VAL(lat_cv_check_explicit,
   [AC_TRY_COMPILE([class A { explicit A(int j):i(j){} A():i(0){} int i;};],
     [], lat_cv_check_explicit=yes, lat_cv_check_explicit=no)])
  AC_MSG_RESULT($lat_cv_check_explicit)
  if test "$lat_cv_check_explicit" = yes; then
    AC_DEFINE(HAVE_EXPLICIT)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_MUTABLE,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements mutable keyword)
  AC_CACHE_VAL(lat_cv_check_mutable,
   [AC_TRY_COMPILE([class A {public:A():i(0){} void f()const{i=1;} mutable int i;};],
     [A a; a.f();], lat_cv_check_mutable=yes, lat_cv_check_mutable=no)])
  AC_MSG_RESULT($lat_cv_check_mutable)
  if test "$lat_cv_check_mutable" = yes; then
    AC_DEFINE(HAVE_MUTABLE)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_RESTRICT,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements restrict keyword)
  AC_CACHE_VAL(lat_cv_check_restrict,
   [AC_TRY_COMPILE([void f (int * restrict x, int * restrict y) { *x = *y; }],
     [int x = 0, y = 1; f(&x, &y);],
     lat_cv_check_restrict=yes,
     lat_cv_check_restrict=no)])
  AC_MSG_RESULT($lat_cv_check_restrict)
  if test "$lat_cv_check_restrict" = yes; then
    AC_DEFINE(HAVE_RESTRICT)
  else
    AC_MSG_CHECKING(whether ${CXX-CC} implements __restrict keyword)
    AC_CACHE_VAL(lat_cv_check___restrict,
     [AC_TRY_COMPILE([void f (int * __restrict x, int * __restrict y) { *x = *y; }],
       [int x = 0, y = 1; f(&x, &y);],
       lat_cv_check___restrict=yes,
       lat_cv_check___restrict=no)])
    AC_MSG_RESULT($lat_cv_check___restrict)
    if test "$lat_cv_check___restrict" = yes; then
      AC_DEFINE(HAVE_PRIVATE_RESTRICT)
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TEMPLATE_INSTANTIATION,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements ANSI template instantiation)
  AC_CACHE_VAL(lat_cv_check_ansi_template_instantiation,
   [AC_TRY_COMPILE(
      [template<class T>struct A{A(T t){t;}};template struct A<int>;], [],
      lat_cv_check_ansi_template_instantiation=yes,
      lat_cv_check_ansi_template_instantiation=no)])
  AC_MSG_RESULT($lat_cv_check_ansi_template_instantiation)
  if test $lat_cv_check_ansi_template_instantiation = yes ; then
    AC_DEFINE(HAVE_ANSI_TEMPLATE_INSTANTIATION)
  fi

  AC_MSG_CHECKING(whether ${CXX-CC} distinguishes template specialisation and instantiation)
  AC_CACHE_VAL(lat_cv_check_ansi_template_spec_inst,
   [AC_TRY_COMPILE([template<class T>struct A{static T array[];};
	template<>float A<float>::array[];float*foo=A<float>::array;
	template<>float A<float>::array[]={0.0,1.0,2.0,3.0};],
      [],
      lat_cv_check_ansi_template_spec_inst=yes,
      lat_cv_check_ansi_template_spec_inst=no)])
  AC_MSG_RESULT($lat_cv_check_ansi_template_spec_inst)
  if test $lat_cv_check_ansi_template_spec_inst = yes; then
    AC_DEFINE(HAVE_SEPARATE_TEMPLATE_SPEC_INST)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TEMPLATE_RESCOPE,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements rescoping in templates)
  AC_CACHE_VAL(lat_cv_check_template_rescope,
   [AC_TRY_COMPILE(
      [template<class T>struct A{typedef int N;]
      [struct I{typedef typename A::N N;};};], [],
      lat_cv_check_template_rescope=yes,
      lat_cv_check_template_rescope=no)])
  AC_MSG_RESULT($lat_cv_check_template_rescope)
  if test $lat_cv_check_template_rescope = no ; then
    AC_DEFINE(HAVE_BROKEN_TEMPLATE_RESCOPE)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TEMPLATE_NULL_ARGS,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements null template arguments)
  AC_CACHE_VAL(lat_cv_check_template_null_args,
   [AC_TRY_COMPILE(
      [template<class T>class Y;template<class T>int operator+(Y<T>&,int);]
      [template<class T>class Y{friend int operator+<>(Y<T>&,int);};], [Y<int> y
;],
      lat_cv_check_template_null_args=yes,
      lat_cv_check_template_null_args=no)])
  AC_MSG_RESULT($lat_cv_check_template_null_args)
  if test $lat_cv_check_template_null_args = yes ; then
    AC_DEFINE(HAVE_TEMPLATE_NULL_ARGS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TEMPLATE_NULL_SPEC,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements null template specialisation)
  AC_CACHE_VAL(lat_cv_check_template_null_spec,
   [AC_TRY_COMPILE(
      [template<class T>class Y;template<>class Y<int>{};], [Y<int> y;],
      lat_cv_check_template_null_spec=yes,
      lat_cv_check_template_null_spec=no)])
  AC_MSG_RESULT($lat_cv_check_template_null_spec)
  if test $lat_cv_check_template_null_spec = yes ; then
    AC_DEFINE(HAVE_TEMPLATE_NULL_SPEC)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TEMPLATE_PARTIAL_SPEC,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements partial template specialisation)
  AC_CACHE_VAL(lat_cv_check_template_partial_spec,
   [AC_TRY_COMPILE(
      [template<class T>class Y{};template<class T>class Y<T *>{};], [Y<int *> y
;],
      lat_cv_check_template_partial_spec=yes,
      lat_cv_check_template_partial_spec=no)])
  AC_MSG_RESULT($lat_cv_check_template_partial_spec)
  if test $lat_cv_check_template_partial_spec = yes ; then
    AC_DEFINE(HAVE_TEMPLATE_PARTIAL_SPEC)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_NAMESPACES,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements namespaces)
  AC_CACHE_VAL(lat_cv_check_namespaces,
   [AC_TRY_COMPILE([namespace foo{int bar;}namespace goo{using foo::bar;}],
     [using namespace goo;int n=bar*foo::bar;],
     lat_cv_check_namespaces=yes,
     lat_cv_check_namespaces=no)])
  AC_MSG_RESULT($lat_cv_check_namespaces)
  if test $lat_cv_check_namespaces = yes ; then
    AC_DEFINE(HAVE_NAMESPACES)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_NAMESPACE_STD,
 [AC_REQUIRE([LAT_CHECK_NAMESPACES])
  if test $lat_cv_check_namespaces = yes ; then
    AC_MSG_CHECKING(whether ${CXX-CC} implements namespace std)
    AC_CACHE_VAL(lat_cv_check_namespace_std,
     [AC_TRY_COMPILE([#include <vector>], [std::vector<int> a;],
       lat_cv_check_namespace_std=yes,
       lat_cv_check_namespace_std=no)])
    AC_MSG_RESULT($lat_cv_check_namespace_std)
    if test $lat_cv_check_namespace_std = yes ; then
      AC_DEFINE(HAVE_NAMESPACE_STD)
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_NAMESPACE_STD_REL_OPS,
 [AC_REQUIRE([LAT_CHECK_NAMESPACE_STD])
  if test ${lat_cv_check_namespace_std-no} = yes ; then
    AC_MSG_CHECKING(whether ${CXX-CC} implements namespace std::rel_ops)
    AC_CACHE_VAL(lat_cv_check_namespace_std_rel_ops,
     [AC_TRY_COMPILE([#include <utility>
	struct I{int operator==(const I&) const{return true;}};
	using std::rel_ops::operator!=;], [I i;return i!=i;],
       lat_cv_check_namespace_std_rel_ops=yes,
       lat_cv_check_namespace_std_rel_ops=no)])
    AC_MSG_RESULT($lat_cv_check_namespace_std_rel_ops)
    if test $lat_cv_check_namespace_std_rel_ops = yes ; then
      AC_DEFINE(HAVE_NAMESPACE_STD_REL_OPS)
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_REL_OPS_IN_STD,
 [AC_REQUIRE([LAT_CHECK_NAMESPACE_STD_REL_OPS])
  if test ${lat_cv_check_namespace_std_rel_ops-no} = no &&
     test ${lat_cv_check_namespace_std-no} = yes ; then
    AC_MSG_CHECKING(whether ${CXX-CC} has relational operators in namespace std)
    AC_CACHE_VAL(lat_cv_check_rel_ops_in_std,
     [AC_TRY_COMPILE([#include <utility>
	struct I{int operator==(const I&) const{return true;}};],
	[using std::operator!=;I i,j;return i!=j;],
       lat_cv_check_rel_ops_in_std=yes,
       lat_cv_check_rel_ops_in_std=no)])
    AC_MSG_RESULT($lat_cv_check_rel_ops_in_std)
    if test $lat_cv_check_rel_ops_in_std = yes ; then
      AC_DEFINE(HAVE_REL_OPS_IN_STD)
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_STD_NEW_HANDLER,
 [AC_REQUIRE([LAT_CHECK_NAMESPACE_STD])
  if test ${lat_cv_check_namespace_std-no} = yes ; then
    AC_CHECK_HEADERS(new new.h)
    AC_MSG_CHECKING(whether ${CXX-CC} new_handler is in namespace std)
    AC_CACHE_VAL(lat_cv_check_std_new_handler,
     [AC_TRY_COMPILE([
#if HAVE_NEW
# include <new>
#else
# include <new.h>
#endif],
	[std::new_handler x = std::set_new_handler (0)],
       lat_cv_check_std_new_handler=yes,
       lat_cv_check_std_new_handler=no)])
    AC_MSG_RESULT($lat_cv_check_std_new_handler)
    if test $lat_cv_check_std_new_handler = yes ; then
      AC_DEFINE(HAVE_STD_NEW_HANDLER)
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_NEW_IOSTREAMS,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements new-style iostreams)
  AC_CACHE_VAL(lat_cv_check_new_iostreams,
   [AC_TRY_COMPILE([
#include <iosfwd>
#include <iostream>
#include <streambuf>
#ifdef HAVE_NAMESPACE_STD
# define STD std::
#else
# define STD
#endif],
     [STD streambuf::int_type n=STD streambuf::traits_type::eof();],
     lat_cv_check_new_iostreams=yes,
     lat_cv_check_new_iostreams=no)])
  AC_MSG_RESULT($lat_cv_check_new_iostreams)
  if test $lat_cv_check_new_iostreams = yes ; then
    AC_DEFINE(HAVE_NEW_IOSTREAMS)

    AC_MSG_CHECKING(whether ${CXX-CC} declares streambuf in iosfwd)
    AC_CACHE_VAL(lat_cv_check_streambuf_in_iosfwd,
     [AC_TRY_COMPILE([
#include <iosfwd>
#ifdef HAVE_NAMESPACE_STD
# define STD std::
#else
# define STD
#endif],
     [STD streambuf *buf;],
     lat_cv_check_streambuf_in_iosfwd=yes,
     lat_cv_check_streambuf_in_iosfwd=no)])
    AC_MSG_RESULT($lat_cv_check_streambuf_in_iosfwd)
    if test $lat_cv_check_streambuf_in_iosfwd = yes ; then
      AC_DEFINE(HAVE_STREAMBUF_IN_IOSFWD)
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_OSTREAM_CHAR_OVERLOAD,
 [AC_MSG_CHECKING(whether ${CXX-CC} overloads ostream with signed char operators)
  AC_CACHE_VAL(lat_cv_check_ostream_char_overload,
   [AC_TRY_COMPILE([
#if HAVE_NEW_IOSTREAMS
# include <iostream>
#else
# include <iostream.h>
#endif
#if HAVE_NAMESPACE_STD
# define STD std::
#endif],
	[char c='a';unsigned char uc='a';signed char sc='a';STD cout<<c<<sc<<uc;],
      lat_cv_check_ostream_char_overload=yes,
      lat_cv_check_ostream_char_overload=no)])
  AC_MSG_RESULT($lat_cv_check_ostream_char_overload)
  if test $lat_cv_check_ostream_char_overload = yes ; then
    AC_DEFINE(HAVE_OSTREAM_CHAR_OVERLOAD)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_OSTREAM_LONG_DOUBLE_OVERLOAD,
 [AC_MSG_CHECKING(whether ${CXX-CC} overloads ostream with long double operators)
  AC_CACHE_VAL(lat_cv_check_ostream_long_double_overload,
   [AC_TRY_COMPILE([
#if HAVE_NEW_IOSTREAMS
# include <iostream>
#else
# include <iostream.h>
#endif
#if HAVE_NAMESPACE_STD
# define STD std::
#endif],
	[long double ld = 0.0L; STD cout<<ld;],
      lat_cv_check_ostream_long_double_overload=yes,
      lat_cv_check_ostream_long_double_overload=no)])
  AC_MSG_RESULT($lat_cv_check_ostream_long_double_overload)
  if test $lat_cv_check_ostream_long_double_overload = yes ; then
    AC_DEFINE(HAVE_OSTREAM_LONG_DOUBLE_OVERLOAD)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_OSTREAM_LONG_LONG_OVERLOAD,
 [AC_MSG_CHECKING(whether ${CXX-CC} overloads ostream with long long operators)
  AC_CACHE_VAL(lat_cv_check_ostream_long_long_overload,
   [AC_TRY_COMPILE([
#if HAVE_NEW_IOSTREAMS
# include <iostream>
#else
# include <iostream.h>
#endif
#if HAVE_NAMESPACE_STD
# define STD std::
#endif],
	[long long l = 0ll; STD cout<<l;],
      lat_cv_check_ostream_long_long_overload=yes,
      lat_cv_check_ostream_long_long_overload=no)])
  AC_MSG_RESULT($lat_cv_check_ostream_long_long_overload)
  if test $lat_cv_check_ostream_long_long_overload = yes ; then
    AC_DEFINE(HAVE_OSTREAM_LONG_LONG_OVERLOAD)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_ITERATOR_TRAITS,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements iterator_traits)
  AC_CACHE_VAL(lat_cv_check_iterator_traits,
   [AC_TRY_COMPILE([
#include <iterator>
#ifdef HAVE_NAMESPACE_STD
# define STD std::
#else
# define STD
#endif
struct X {
  struct I {
    typedef int value_type;
    typedef int difference_type;
    typedef int *pointer;
    typedef int &reference;
    typedef STD random_access_iterator_tag iterator_category;
  };
};],
[ STD iterator_traits<X::I>::value_type v = 10;
  STD iterator_traits<X::I>::pointer p = &v;
  STD iterator_traits<X::I>::reference r = *p;
  STD iterator_traits<X::I>::difference_type d = 1;

  p += d; r = 20;],
     lat_cv_check_iterator_traits=yes,
     lat_cv_check_iterator_traits=no)])
  AC_MSG_RESULT($lat_cv_check_iterator_traits)
  if test $lat_cv_check_iterator_traits = yes ; then
    AC_DEFINE(HAVE_ITERATOR_TRAITS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_ITERATOR,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements iterator)
  AC_CACHE_VAL(lat_cv_check_iterator,
   [AC_TRY_COMPILE([
#include <iterator>
#ifdef HAVE_NAMESPACE_STD
# define STD std::
#else
# define STD
#endif
struct I : STD iterator<STD input_iterator_tag,int,int>
{
  typedef int *pointer;
  typedef int &reference;
};],[],
     lat_cv_check_iterator=yes,
     lat_cv_check_iterator=no)])dnl
  AC_MSG_RESULT($lat_cv_check_iterator)
  if test $lat_cv_check_iterator = yes ; then
    AC_DEFINE(HAVE_ITERATOR)
  fi])dnl
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_REVERSE_ITERATOR,
 [AC_REQUIRE([LAT_CHECK_ITERATOR_TRAITS])
  AC_MSG_CHECKING(what arguments ${CXX-CC} wants for reverse_iterator)
  AC_CACHE_VAL(lat_cv_check_reverse_iterator,
   [lat_prefix="
#include <iterator>
#ifdef HAVE_NAMESPACE_STD
# define STD std::
#else
# define STD
#endif
struct I {
  typedef int value_type;
  typedef int difference_type;
  typedef int *pointer;
  typedef int &reference;
  typedef STD random_access_iterator_tag iterator_category;
};
I operator+ (I, int); I operator+ (int,I); I operator- (I,int);
int operator- (I, I); I &operator+= (I,int); I &operator-= (I,int);"
    AC_TRY_COMPILE([$lat_prefix], [STD reverse_iterator<I> rev;],
     lat_cv_check_reverse_iterator=iterator,
     [AC_TRY_COMPILE([$lat_prefix],
       [STD reverse_iterator<I,STD random_access_iterator_tag,int,
			    int&,int *,int> rev;],
       lat_cv_check_reverse_iterator=tag,
       [AC_TRY_COMPILE([$lat_prefix],
	  [STD reverse_iterator<I,int,int&,int *,int> rev;],
          lat_cv_check_reverse_iterator=notag,
          [AC_TRY_COMPILE([$lat_prefix],
	    [STD os_reverse_iterator<I,int,int&,int *,int> rev;],
            lat_cv_check_reverse_iterator=osnotag,
            lat_cv_check_reverse_iterator=unknown)])])])])
  case "${lat_cv_check_reverse_iterator-unknown}" in
    iterator)
      AC_MSG_RESULT(iterator only)
      AC_DEFINE(HAVE_REVERSE_ITERATOR_STYLE,1)
      ;;
    tag)
      AC_MSG_RESULT(all including iterator category)
      AC_DEFINE(HAVE_REVERSE_ITERATOR_STYLE,2)
      ;;
    notag)
      AC_MSG_RESULT(all without iterator category)
      AC_DEFINE(HAVE_REVERSE_ITERATOR_STYLE,3)
      ;;
    osnotag)
      AC_MSG_RESULT(all without iterator category (ObjectSpace style))
      AC_DEFINE(HAVE_REVERSE_ITERATOR_STYLE,4)
      ;;
    *)
      AC_MSG_RESULT(no idea -- good luck!)
      ;;
  esac])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_USING,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements a working using directive)
  AC_CACHE_VAL(lat_cv_check_using,
   [AC_TRY_COMPILE(,
     [
; return 0; }
class X{public:virtual int f(int a)=0;virtual int f(char a);};
class Y:public X{public:using X::f; virtual int f(int a);
virtual int f(double a);};
int z(Y *y) { return y->f('a'); }
int g(void) { Y y; ],
     lat_cv_check_using=yes,
     lat_cv_check_using=no)])
   AC_MSG_RESULT($lat_cv_check_using)
   if test $lat_cv_check_using = yes ; then
     AC_DEFINE(HAVE_USING)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_FOR_SCOPE,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements the new for scope rules)
  AC_CACHE_VAL(lat_cv_check_for_scope,
   [AC_TRY_COMPILE(,[for(int i=0;i<10;++i);for(int i=0;i<10;++i);],
     lat_cv_check_for_scope=yes,
     lat_cv_check_for_scope=no)])
  AC_MSG_RESULT($lat_cv_check_for_scope)
  if test $lat_cv_check_for_scope = no ; then
    AC_DEFINE_UNQUOTED(for,[if(0);else for])
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TYPENAME_IN_BASE,
 [AC_MSG_CHECKING(whether ${CXX-CC} requires typename in nested template base)
  AC_CACHE_VAL(lat_cv_check_typename_in_base,
   [AC_TRY_COMPILE([template<class T>class X{public:class A{};};
	template<class T>class Y:public X<T>::A{};],
     [],
     lat_cv_check_typename_in_base=no,
     AC_TRY_COMPILE([template<class T>class X{public:class A{};};
	template<class T>class Y:public typename X<T>::A{};],
       [],
       lat_cv_check_typename_in_base=yes,
       lat_cv_check_typename_in_base=unknown))])
  AC_MSG_RESULT($lat_cv_check_typename_in_base)
  case $lat_cv_check_typename_in_base in
    yes) AC_DEFINE_UNQUOTED(TYPENAME_IN_BASEDECL,[typename]);;
    *)   AC_DEFINE_UNQUOTED(TYPENAME_IN_BASEDECL,);;
  esac])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_BOOL,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements bool)
  AC_CACHE_VAL(lat_cv_check_bool,
   [AC_TRY_COMPILE(,[bool a=true; bool b=false; a=b; b=a;],
     lat_cv_check_bool=yes,
     lat_cv_check_bool=no)])
  AC_MSG_RESULT($lat_cv_check_bool)
  if test $lat_cv_check_bool = yes ; then
    AC_DEFINE(HAVE_BOOL)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_MEMBER_TEMPLATES,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements member templates)
  AC_CACHE_VAL(lat_cv_check_member_templates,
   [AC_TRY_COMPILE([template<class T>class Y{public:Y(const T&t):m(t){}T m;};
	template<class T>class X{public:template<class U>X(const Y<U>&y):m(y.m){}T m;};],
     [Y<int> yi(0); X<long> xl(yi);],
     lat_cv_check_member_templates=yes,
     lat_cv_check_member_templates=no)])
  AC_MSG_RESULT($lat_cv_check_member_templates)
  if test $lat_cv_check_member_templates = yes ; then
    AC_DEFINE(HAVE_MEMBER_TEMPLATES)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TEMPLATE_DEFAULT_ARGS,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements template default arguments)
  AC_CACHE_VAL(lat_cv_check_template_default_arg,
   [AC_TRY_COMPILE([template<class T>class X{};template<class T,class U=X<T> >class Y{};],
     [Y<int> y;],
     lat_cv_check_template_default_arg=yes,
     lat_cv_check_template_default_arg=no)])
  AC_MSG_RESULT($lat_cv_check_template_default_arg)
  if test $lat_cv_check_template_default_arg = yes ; then
    AC_DEFINE(HAVE_TEMPLATE_DEFAULT_ARGS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_OPERATOR_ARROW,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements correct \`operator->' semantics)
  AC_CACHE_VAL(lat_cv_check_operator_arrow,
   [AC_TRY_COMPILE([template <class T> class X{public:T*operator->(void)const{return(T*)0;}};],
     [X<int> x;],
     lat_cv_check_operator_arrow=yes,
     lat_cv_check_operator_arrow=no)])
  AC_MSG_RESULT($lat_cv_check_operator_arrow)
  if test $lat_cv_check_operator_arrow = yes ; then
    AC_DEFINE(HAVE_ANSI_OPERATOR_ARROW)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_CLASS_ACCESS,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements correct access rights semantics)
  AC_CACHE_VAL(lat_cv_check_class_access,
   [AC_TRY_COMPILE([class X{private:class Y{}; public:Y f();};]
		   [X::Y X::f(){return Y();}], [],
     lat_cv_check_class_access=yes,
     lat_cv_check_class_access=no)])
  AC_MSG_RESULT($lat_cv_check_class_access)
  if test $lat_cv_check_class_access = no ; then
    AC_DEFINE(HAVE_BROKEN_CLASS_ACCESS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_DYNAMIC_CAST,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements dynamic_cast)
  AC_CACHE_VAL(lat_cv_check_dynamic_cast,
   [AC_TRY_COMPILE([struct X{virtual~X(){}}; struct Y:X{};],
     [X *x=new Y; Y *y=dynamic_cast<Y*>(x);],
     lat_cv_check_dynamic_cast=yes,
     lat_cv_check_dynamic_cast=no)])
  AC_MSG_RESULT($lat_cv_check_dynamic_cast)
  if test $lat_cv_check_dynamic_cast = yes ; then
    AC_DEFINE(HAVE_DYNAMIC_CAST)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_NEW_STYLE_CASTS,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements new style casts)
  AC_CACHE_VAL(lat_cv_check_new_style_casts,
   [AC_TRY_COMPILE(,
     [int *p1=0; void *p2=static_cast<void *>(p1); const int *p3=0;
      int *p4=const_cast<int*>(p3); long *p5=reinterpret_cast<long *>(p4);],
     lat_cv_check_new_style_casts=yes,
     lat_cv_check_new_style_casts=no)])
  AC_MSG_RESULT($lat_cv_check_new_style_casts)
  if test $lat_cv_check_new_style_casts = yes ; then
    AC_DEFINE(HAVE_NEW_STYLE_CASTS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TYPEID,
 [AC_CHECK_HEADERS(typeinfo typeinfo.h)
  AC_MSG_CHECKING(whether ${CXX-CC} implements typeid and type_info)
  AC_CACHE_VAL(lat_cv_check_typeid,
   [AC_TRY_COMPILE([
#if HAVE_TYPEINFO
# include <typeinfo>
#elif HAVE_TYPEINFO_H
# include <typeinfo.h>
#endif
#if HAVE_NAMESPACE_STD
# define STD std::
#else
# define STD
#endif
struct X{virtual~X(){}};],
     [const STD type_info &info=typeid(X);],
     lat_cv_check_typeid=yes,
     lat_cv_check_typeid=no)])
  AC_MSG_RESULT($lat_cv_check_typeid)
  if test $lat_cv_check_typeid = yes ; then
    AC_DEFINE(HAVE_TYPEID)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TYPENAME,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements typename keyword)
  AC_CACHE_VAL(lat_cv_check_typename,
   [AC_TRY_COMPILE([template<class T>struct X{typedef int I;};
	template<class T>struct Y{typename X<T>::I f(void);};
	template<class T>typename X<T>::I Y<T>::f(void){}],
     [],
     lat_cv_check_typename=yes,
     lat_cv_check_typename=no)])
  AC_MSG_RESULT($lat_cv_check_typename)
  if test $lat_cv_check_typename = yes ; then
    AC_DEFINE(HAVE_TYPENAME)
  fi])
dnl ---######################################################################
dnl +++######################################################################
dnl Stolen from DDD and added test for __FUNCTION__ and __func__.
AC_DEFUN(LAT_CHECK_PRETTY_FUNCTION,
 [AC_MSG_CHECKING(whether ${CXX-CC} implements __PRETTY_FUNCTION__)
  AC_CACHE_VAL(lat_cv_have_pretty_function,
   [AC_TRY_LINK([#include <stdio.h>],[puts(__PRETTY_FUNCTION__);],
     lat_cv_have_pretty_function=yes,
     lat_cv_have_pretty_function=no)])
  AC_MSG_RESULT($lat_cv_have_pretty_function)
  if test "$lat_cv_have_pretty_function" = yes; then
    AC_DEFINE(HAVE_PRETTY_FUNCTION)
  else
    AC_MSG_CHECKING(whether ${CXX-CC} implements __FUNCTION__)
    AC_CACHE_VAL(lat_cv_have_function,
     [AC_TRY_LINK([#include <stdio.h>],[puts(__FUNCTION__);],
       lat_cv_have_function=yes,
       lat_cv_have_function=no)])
    AC_MSG_RESULT($lat_cv_have_function)
    if test "$lat_cv_have_function" = yes; then
      AC_DEFINE(HAVE_FUNCTION)
    else
      AC_MSG_CHECKING(whether ${CXX-CC} implements __func__)
      AC_CACHE_VAL(lat_cv_have_func,
       [AC_TRY_LINK([#include <stdio.h>],[puts(__func__);],
	lat_cv_have_func=yes,
	lat_cv_have_func=no)])
      AC_MSG_RESULT($lat_cv_have_func)
      if test "$lat_cv_have_func" = yes; then
	AC_DEFINE(HAVE_FUNC)
      fi
    fi
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_CXX_STDC_HEADERS,
 [AC_MSG_CHECKING(whether ${CXX-CC} has standard C++ C headers)
  AC_CACHE_VAL(lat_cv_check_cxx_stdc_headers,
   [AC_TRY_COMPILE([
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
//#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
//#include <cwchar>
//#include <cwctype>
#include <vector> // include to resolve possible conflicts
],[std::size_t s = 0;],
      lat_cv_check_cxx_stdc_headers=yes,
      lat_cv_check_cxx_stdc_headers=no)])
  AC_MSG_RESULT($lat_cv_check_cxx_stdc_headers)
  if test $lat_cv_check_cxx_stdc_headers = yes ; then
    AC_DEFINE(HAVE_CXX_STDC_HEADERS)
  fi])
dnl ---######################################################################
