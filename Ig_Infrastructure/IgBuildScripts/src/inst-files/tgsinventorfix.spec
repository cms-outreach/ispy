### RPM external tgsinventorfix 3.1
# Ugly workaround to create a library that resolves symbols
# that are unresolved in libInventor.so and would be defined
# in libInventorXt.so -- which we don't want to use.  This
# library must be mentioned on the link line to get the tests
# to succeed, otherwise they will die because of unsatisfied
# symbols.  The only way we can sneak it in is to use $LIBS,
# which gets used in every test; the library luckily does not
# make any of them fail.
#
# All this magic can be blamed on Lassi A. Tuura <lat@iki.fi>

# [ -z "$use_system_tgsinventor" ] && return

Source: tgsinventorfix.cc

%prep
%build
case $(uname) in
  Darwin ) pic= so=dylib shared=-dynamiclib ;;
  * )      pic=-fPIC so=so shared=-shared ;;
esac

c++ $pic $shared -o libTGSInventorFix.$so %_sourcedir/tgsinventorfix.cc

%install
case $(uname) in Darwin ) so=dylib ;; * ) so=so ;; esac
mkdir -p %i/lib
cp libTGSInventorFix.$so %i/lib
