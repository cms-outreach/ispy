#!/bin/sh

## Usage: install.sh [options] [packages]
## Builds binary and source rpms for the packages (default: all)
## into RPMS/SRPMS.  As a side effect installs all the packages
## as well -- by far the most useful function at the moment.
##
## Options:
##  -spec, -no-spec	do/skip creating specs (default: do)
##  -src, -no-src	do/skip source download (default: do)
##  -rpm, -no-rpm	do/skip rpm build (default: do)
##  -check, -no-check	do/skip check step (default: skip)
##  -force, -no-force	do/don't rebuilt existing rpms (default: no)
##
##  -version P V	override package P version as V
##
##  -pass ARG		pass ARG to "rpm" commands
##  -define ARG		define rpm macro string ARG
##  -b[abpcils]		pass option to "rpmbuild"
##  -fast		pass --short-circuit to "rpmbuild"
##
## The package name arguments are the name of packages for which
## spec fragments are defined.

# FIXME: Support CC= CXX= CFLAGS= CXXFLAGS= on command line?
# FIXME: Handle rpm defines (use_system_gcc, use_distcc, use_ccache,
#   use_system_zlib, tgspath, ...)
# FIXME: Some RPMS still pick up system libs (zlib, iconv, ...)

######################################################################
here=$PWD; me=$0; case $me in /* ) ;; * ) me=$PWD/$me ;; esac
home=$(dirname $me); [ X"$home" = X ] && home=$PWD

rpmall=false
dospecs=true dosrcs=true dorpms=true dochecks=false force=false
rpmargs= rpmbuild=-ba rpmbuildsc=
version_overrides=

while [ $# -gt 0 ]; do
  case $1 in
    -help )
      grep '^##' $me | grep -v '^###'; exit 1;;

    -no-spec | -no-src | -no-rpm | -no-check )
      eval "do$(echo $1 | sed 's/^-no//; s/^-//;')s=false"; shift ;;
    -spec | -src | -rpm | -check )
      eval "do$(echo $1 | sed 's/^-no//; s/^-//;')s=true"; shift ;;

    -no-force )
      force=false; shift ;;
    -force )
      force=true; shift ;;

    -version )
      [ $# -gt 2 ] || { echo "Option \`$1' requires two arguments" 1>&2; exit 1; }
      version_overrides="$version_overrides $2:$3"; shift; shift; shift ;;

    -pass )
      [ $# -gt 1 ] || { echo "Option \`$1' requires an argument" 1>&2; exit 1; }
      rpmargs="$rpmargs '$2'"; shift; shift ;;

    -define )
      [ $# -gt 1 ] || { echo "Option \`$1' requires an argument" 1>&2; exit 1; }
      rpmargs="$rpmargs --define \"$2\""; shift; shift ;;

    -b[abpcils] )
      rpmbuild="$1"; shift ;;

    -fast )
      rpmbuildsc="--short-circuit"; shift ;;

    -* )
      echo "Unknown option \`$1'" 1>&2
      exit 1;;

    * )
      break ;;
  esac
done

######################################################################
# Tools

hostsof () { dig "$1" | perl -ne 'next if 1 .. /;; ANSWER SECT/; next if /^;;/ .. eof; next if /^$/; print' | awk '{print $5}'; }

# Download a file
download_method=
download_curl () { curl -f -q -s "$1" -o "$2.tmp" && mv "$2.tmp" "$2"; }
download_wget () { wget -q -O "$2.tmp" "$1" && mv "$2.tmp" "$2"; }
download_none () { echo "no curl or wget, cannot fetch $1" 1>&2; exit 1; }
download_getit () {
  url="$1" to="$2"

  # Fake cvs:* as download by checking stuff out into a tar ball
  case $url in cvs:* )
    # Extract the different parts of the url, assuming format:
    # cvs://:type:user@server:port/repo?passwd=x&tag=-[rD]x&module=x&output=/tar-ball.tar.gz
    cvsrepo=$(echo "$url" | sed 's|^cvs://||; s|?.*||')
    cvspasswd= cvstag= cvsmodule= cvsoutput= cvsexport=
    for arg in $(echo "$url" | sed 's|.*?||; s|&| |g'); do
      case $arg in
        passwd=* | tag=* | module=* | output=* | export=* )
	  var="$(echo "$arg" | sed 's/=.*//')"
	  value="$(echo "$arg" | sed 's/^[^=]*=//')"
	  eval "cvs$var=\"\$value\"" ;;
	* )
	  echo "unknown cvs argument <$arg>" 1>&2; exit 1 ;;
      esac
    done

    [ -z "$cvsrepo" ] && { echo "cvs: no repository ($url)" 1>&2; exit 1; }
    [ -z "$cvspasswd" ] && { echo "cvs: no 'passwd' ($url)" 1>&2; exit 1; }
    [ -z "$cvstag" ] && { echo "cvs: no 'tag' ($url)" 1>&2; exit 1; }
    [ -z "$cvsmodule" ] && { echo "cvs: no 'module' ($url)" 1>&2; exit 1; }
    [ -z "$cvsoutput" ] && { echo "cvs: no 'output' ($url)" 1>&2; exit 1; }
    case $cvsrepo in :*:*@*:[0-9]*/* ) ;; * )
      echo "cvs: warning: repository definition unlikely to work ($cvsrepo)" 1>&2;;
    esac
    case $cvsoutput in /* ) ;; * )
      echo "cvs: argument to 'output' must start with / ($url)" 1>&2; exit 1;;
    esac

     # FIXME: if the output is a single file (no cvsoutput), copy as
     # such; this is needed for patches and other individual files.

    (tmpdir=$(mktemp -d ${TMPDIR-/tmp}/tmp.XXXXXX)
     # trap 'rm -fr $tmpdir' 0 1 2 15
     export CVS_PASSFILE=$tmpdir/cvspass
     [ -z "$cvsexport" ] && cvsexport="$cvsmodule"
     for repo in $cvsrepo $(echo $cvsrepo | sed 's|:2401/|:/|'); do
       case $repo in
	 *:2401/* ) echo "/1 $repo $cvspasswd" > $CVS_PASSFILE ;;
	 * )        echo "$repo $cvspasswd" > $CVS_PASSFILE ;;
       esac
       mkdir -p "$tmpdir/checkout/$cvsexport"
       echo "  Checkout to $cvsexport: $cvsmodule $cvstag from $repo"
       cd $tmpdir/checkout
       cvs -z6 -Q -d"$repo" export -d"$cvsexport" "$cvstag" "$cvsmodule" &&
         tar -zcf "$to" "$cvsexport" &&
	 break
       cd $here
       rm -fr $tmpdir/checkout
     done
     cd $here
     rm -fr $tmpdir)
    return
    ;;
  esac

  # Figure out how to download stuff
  if [ -z "$download_method" ]; then
    if [ $(curl --version 2>&1 | wc -l) != 0 ]; then
      download_method=curl
    elif [ $(wget --version 2>&1 | wc -l) != 0 ]; then
      download_method=wget
    else
      download_method=none
    fi
  fi

  # there's a lot of broken sourceforge download mirrors out there,
  # so force a rotation through the different mirrors until we find
  # one that works; also sometimes hosts bounce connections
  method="$(echo "$url" | sed 's|:/.*||')"
  host="$(echo "$url" | sed 's|^[a-z]*://||; s|/.*||')"
  rest="$(echo "$url" | sed 's|^[a-z]*://[^/]*/||')"

  # first try raw download
  echo "Downloading $url"
  download_${download_method} "$url" "$to"
  [ -f "$to" ] && return

  # ouch, failed, try again rotating through hosts
  for i in 2 3 4 5; do
    for h in $(hostsof $host); do
      # (FIXME: Handle more parts of url -- port, user, ...?)
      newurl="$method://$h/$rest"
      echo "Downloading $url (trying host $h, attempt $i)"
      download_${download_method} "$newurl" "$to"
      [ -f "$to" ] && return
    done
  done
}

# Download a file into some directory
download () {
  dir=$1 url=$2 filepart=${3-"`echo $2 | sed 's|.*/||'`"}
  [ -d $dir ] || mkdir -p $dir
  if [ ! -f $dir/$filepart ]; then
    if [ -z "$local_downloads" ]; then
      download_getit $url $dir/$filepart
    else
      echo Downloading from local repository $filepart
      cp $local_downloads/$filepart $dir/$filepart
    fi
  fi
}

# Convert string to a safe regular expression
rxname () {
  echo "$1" | sed 's/\([^A-Za-z0-9_]\)/[\1]/g'
}

# Extract a named installation file
getfile () {
  suffix=${2-".file"}
  if [ -f "$home/inst-files/$1$suffix" ]; then
    cat "$home/inst-files/$1$suffix"
  else
    echo "no '$home/inst-files/$1$suffix'" 1>&2
    cat /dev/null
  fi
}

# Extract a named patch
getpatch () {
  getfile "$1" .patch
}

# Extract a named spec file
getspec () {
  getfile "$1" .spec
}

# Get the RPM package version number from the spec
rpmversion () {
  local v=$(getspec "$1" | egrep '^### RPM ' | sed 's/.*RPM *[^ ]* *[^ ]* *//; s/ *$//')

  # Check for overrides
  for o in $version_overrides; do
    case $o in $1:* ) v=$(echo $o | sed 's/[^:]*://') ;; esac
  done

  echo $v
}

# Get the RPM package category from the end of this installation script
rpmcategory () {
  getspec "$1" | egrep '^### RPM ' | sed 's/.*RPM *//; s/ .*//'
}

# Add a default tag value to a spec if no value was specified
rpmdefspec () {
  grep "^$1:" < $p_rpmdir/spec.in.head > /dev/null 2>&1 ||
    echo "$1: $2" >> $p_rpmdir/spec.def.head
}

# Initialise for working with RPM.  Creates local RPM database.
rpminit () {
  # FIXME: Copy system RPM database here?  We should know what is
  # installed on the system so that RPM can record correct package
  # dependencies into the RPMs we build.  That is, if pick up a system
  # library or a program, we want RPM to notice that -- the dependency
  # is real and should be managed, whether we should have had it in
  # the first place.  On the other hand, we don't want to touch the
  # system RPM database, so we have to have our own separate one.
  #
  # FIXME: Note that we need to ask RPM for %_dbpath -- the database
  # location is system-setup dependent.

  rpmdb=$here/RPMDB
  if [ ! -d $rpmdb -o ! -f $rpmdb/Packages ]; then
    mkdir -p $rpmdb && rpm --dbpath $rpmdb --initdb
  fi

  mkdir -p $here/tmp
}

# Execute a RPM command with custom arguments
rpmcmd () {
  rpmcmd=$1; shift
  eval "$rpmcmd --dbpath $rpmdb $rpmargs $*"
}

######################################################################
# Setup directory variables needed by a RPM package specified in the
# spec section below in this script.
rpmvars () {
  cd $here

  p_name=$1
  p_rxname=$(rxname $p_name)
  p_version=$(rpmversion $p_name)
  p_category=$(rpmcategory $p_name)
  
  # Set directories
  p_rpmdir=$here/SPECS/$p_category/$p_name/$p_version
  p_src=$here/SOURCES/$p_category/$p_name/$p_version
  p_uname="$(echo $p_name | tr '[a-z]' '[A-Z]' | sed 's/[^A-Z0-9]/_/g')"

  mkdir -p $p_rpmdir $p_src $here/BUILD $here/SRPMS $here/RPMS

  # FIXME! SUPER HYPER UGLY KLUDGE WARNING: CERNLIB can only be
  # built in a path with all lower-case characters.  Make a link
  # to the build directory so that it can be found.  If the path
  # here contains capitals and the file system is case-sensitive,
  # refuse to build.
  if [ $p_name = cernlib ]; then
    [ -e build ] || ln -s BUILD build
    if [ $(echo $here | tr -dc '[A-Z]' | wc -c) != 0 -a \
	 ! -d $(echo $here | tr '[A-Z]' '[a-z]') ]; then
      (echo "The current directory $(pwd) contains capital letters and the"
       echo "file system is case-sensitive, and there is no lower-case-only"
       echo "alternative."
       echo
       echo "You are trying to build CERNLIB, which cannot be built in a"
       echo "directory with capital letters in it.  Please do the builds"
       echo "somewhere else where the entire path is accessible without"
       echo "capital letters."
      ) 1>&2
      exit 1
    fi
  fi
}

# Save and restore rpm variables for recursion
# Push a variable value on stack
pushvar () {
  eval level=\${push_level_$1-0}
  eval saved_${level}_$1=\$$1
  eval push_level_$1=`expr $level + 1`
}

# Pop a variable value from a stack
popvar () {
  eval level=\${push_level_$1}
  level=`expr $level - 1`
  eval $1=\$saved_${level}_$1
  eval push_level_$1=$level
}

rpmsave () {
  where=`pwd`
  pushvar p_name
  pushvar p_rxname
  pushvar p_version
  pushvar p_category
  pushvar p_rpmdir
  pushvar p_src
  pushvar p_uname
  pushvar where
  cd $here
}

rpmrestore () {
  popvar p_name
  popvar p_rxname
  popvar p_version
  popvar p_category
  popvar p_rpmdir
  popvar p_src
  popvar p_uname
  popvar where
  cd $where
}

######################################################################
# Do something about RPM recursively
rpmrdepth=0 rpmrdeps=
rpmrpush () { rpmrdepth=$(expr $rpmrdepth + 1); }
rpmrpop  () { rpmrdepth=$(expr $rpmrdepth - 1); }
rpmrecho () { [ $rpmrdepth -gt 1 ] && echo -n "[$rpmrdepth] "; echo "$@"; }
rpmrecurse () {
  done=$1 what=$2
  shift; shift;

  # Check pre-requisites are already built, then say we're building here
  builddir=$(rpmeval "%_builddir")
  mkdir -p $builddir
  case $rpmrdeps in
    *:$p_name:* ) ;;
    * )
      rpmrdeps=$rpmrdeps:$p_name:
      eval rpm $rpmargs -qR --specfile spec | sed '/(none)/d' |	# FIXME: rpmcmd
        sed 's/\([^+]*\)+\(.*\)+\([A-Z0-9].*\)/\1 \2 \3/' |
        awk '{print $2}' |
        while read nm; do
	  case $done in *:$nm:* ) ;; * ) echo $nm ;; esac
        done > $builddir/depcache
      ;;
  esac

  rpmrpush
  todo="$(cat $builddir/depcache | tr "\n" ' ')"
  [ $(wc -l < $builddir/depcache ) != 0 -a $# -gt 0 ] &&
    rpmrecho "Required RPMs for $p_name: $todo"
  for nm in $todo; do
    rpmsave
    $what $nm
    rpmrestore
  done
  [ $# -gt 0 ] && rpmrecho ${1+"$@"}
  rpmrpop
}

######################################################################
# Produce a spec file for a package with a "### RPM" spec skeleton
# below in this script.  The argument is the package name from the
# section title.  Adds standard preamble and adds all the default
# parts, plus massages to insert required or convenience content.
# The output will be in SPECS/*/*/*/{spec,cmsos}.
rpmspecdone=
rpmspec () {
  # Remember what've already done, if we've already been here, quit quick
  case "$rpmspecdone" in *:$1:* ) return;; esac
  rpmspecdone=$rpmspecdone:$1:

  # Start working here
  rpmvars $1

  # Get started
  echo "Preparing spec for $p_name..."

  # Create package directories
  rm -fr $p_rpmdir
  mkdir -p $p_rpmdir

  # Extract RPM spec body (FIXME: check for mandatory components?)
  getspec $1 > $p_rpmdir/spec.in

  # Process imports
  grep '^## IMPORT ' < $p_rpmdir/spec.in | sed 's/.*IMPORT *//; s/ *$//' |
    while read file; do getfile $file; done > $p_rpmdir/spec.in2
  cat $p_rpmdir/spec.in2 >> $p_rpmdir/spec.in
  rm $p_rpmdir/spec.in2

  # Check it looks sane
  if grep -i '^source[0-9]*:' $p_rpmdir/spec.in >/dev/null 2>&1; then :; else
    echo "RPM spec body for $1 does not look valid" 1>&2
    return
  fi

  # Figure out build condition
  buildif="$(grep '^## BUILDIF ' < $p_rpmdir/spec.in | sed 's/.*BUILDIF *//; s/ *$//')"
  [ -z "$buildif" ] && buildif=true

  # Generate environment init list
  (echo "## INITENV +PATH PATH %i/bin";
   echo "## INITENV +PATH LD_LIBRARY_PATH %i/lib";
   echo "## INITENV +PATH DYLD_LIBRARY_PATH %i/lib";
   echo "## INITENV SET ${p_uname}_ROOT %i";
   echo "## INITENV SET ${p_uname}_VERSION %v";
   grep '^## INITENV ' < $p_rpmdir/spec.in) > $p_rpmdir/env

  # Separate the spec into parts.  Make sure not to create a fragment unless
  # there is such a section in the original spec.  (These get reassembled at
  # the end into one big spec file after we've massaged them in the middle.)
  sections="description prep build install clean files pre post preun postun changelog"
  sectionrx="($(echo $sections | sed 's/ /|/g'))"
  perl -ne "next if /^%$sectionrx *\$/ .. eof; print" < $p_rpmdir/spec.in > $p_rpmdir/spec.in.head
  for s in $sections; do
    if grep "^%$s *\$" < $p_rpmdir/spec.in > /dev/null 2>&1; then
      perl -ne "next if 1 .. /^%$s *\$/; next if /^%$sectionrx *\$/ .. eof; print" \
        < $p_rpmdir/spec.in > $p_rpmdir/spec.in.$s
    fi
  done

  # Prepare default files: preamble of common macros, default fields etc.
  # FIXME: Ugly hack to get GCC version in.  Think of something nicer...
  getfile cmsos > $p_rpmdir/cmsos
  (echo "# DO NOT EDIT!  This file was automatically generated with IGUANA's"
   echo "# install.sh.  To change this file, edit the appropriate part of"
   echo "# Ig_Infrastructure/IgBuildScripts/src/install.sh, not this spec."
   echo
   echo "%define pkgname        $p_name";
   echo "%define pkgversion     $p_version";
   echo "%define pkgcategory    $p_category";
   echo "%define instroot       $here";
   echo "%define gccver         $(rpmversion gcc)";
   getfile rpm-preamble) > $p_rpmdir/spec.head
  : > $p_rpmdir/spec.def.head

  # - package name, version and release
  rpmdefspec Name		$p_category+$p_name+$p_version
  rpmdefspec Version		1
  rpmdefspec Release		$(expr 1 + $(sed '1,/^%changelog/d' \
      					     < $p_rpmdir/spec.in | \
      					     grep '^\* ' | wc -l))

  # - package summary and copyright info
  rpmdefspec Summary		"CMS Software Package $p_name version $p_version"
  rpmdefspec Copyright		"Unknown"
  # rpmdefspec License		"Unknown"
  rpmdefspec Group		"$p_category"
  rpmdefspec Packager		"CMS <cms-orca-developers@cern.ch>"
  rpmdefspec Distribution	"CMS"
  rpmdefspec Vendor		"CMS"

  # - default build and installation directories
  rpmdefspec Prefix		%i

  # - default package dependency data (FIXME: version numbers!)
  rpmdefspec Obsoletes		$p_category+$p_name+$p_version
  rpmdefspec Provides		$p_category+$p_name+$p_version
  (if [ X"$p_name" != Xgcc ]; then
     echo '%if "%{?use_system_gcc:set}" != "set"'
     echo "Requires: gcc"
     echo '%endif'
     if [ X"$p_name" != Xlibg2cso ]; then
       # echo "Requires: libg2cso"  # FIXME: is this needed?
       if [ X"$p_name" != Xdistcc ]; then
         echo '%if "%{?use_distcc:set}" == "set"'
         echo 'Requires: distcc'
         echo '%endif'
	 if [ X"$p_name" != Xccache ]; then
           echo '%if "%{?use_ccache:set}" == "set"'
           echo 'Requires: ccache'
           echo '%endif'
	 fi
       fi
     fi
   fi) >> $p_rpmdir/spec.def.head

  # Rewrite all the requires to include the version number from
  # the definitions within this script.  (FIXME: transitively?)
  (cat $home/inst-files/*.spec | grep '^### RPM ' |
     awk '{ printf "$versions{'\''%s'\''} = [ '\''%s'\'', '\''%s'\'' ];\n", $4, $3, $5 }'
   for o in $version_overrides; do
     ovp=$(echo $o | sed 's/:.*//')
     ovv=$(echo $o | sed 's/.*://')
     ovc=$(rpmcategory $ovp)
     echo '$versions{'\'"$ovp"\''} = [ '\'"$ovc"\'', '\'"$ovv"\''];'
   done
   echo 'while (<>) {'
   echo '  if (/^(Requires|PreReq|BuildRequires):\s*(.*)/) {'
   echo '    print "$1: ", join(" ", map { "$versions{$_}[0]+$_+$versions{$_}[1]" }'
   echo '				   split(/\s+/, $2)), "\n";'
   echo '  } else {'
   echo '    print;'
   echo '  }'
   echo '}') > $p_rpmdir/spec.edit.pl
  perl -i~ $p_rpmdir/spec.edit.pl $p_rpmdir/spec.*head

  # Generate default preamble into each section
  for s in prep build install clean; do
    echo "%initenv" > $p_rpmdir/spec.pre.$s
  done
  echo "[ -d %i ] && chmod -R u+w %i" >> $p_rpmdir/spec.pre.prep
  echo "rm -fr %i" >> $p_rpmdir/spec.pre.prep
  echo "mkdir -p %i" >> $p_rpmdir/spec.pre.install
  for s in build install; do
    echo "$buildif || exit 0" >> $p_rpmdir/spec.pre.$s
  done

  # Generate default build rules etc. (FIXME: handle multiple sources,
  # patches, etc.)
  (echo "No description") > $p_rpmdir/spec.def.description
  (echo "%setup -n %n-%v") > $p_rpmdir/spec.def.prep
  (echo "./configure --prefix=%i"; echo "make") > $p_rpmdir/spec.def.build
  (echo "make install") > $p_rpmdir/spec.def.install
  # (echo "rm -fr %i") > $p_rpmdir/spec.def.clean
  (echo "%i") > $p_rpmdir/spec.def.files

  # Generate %install post-commands to spit out package initenv
  echo 'mkdir -p %i/etc/profile.d' > $p_rpmdir/spec.post.install
  echo 'cat <<\EOF > %i/etc/profile.d/init.sh' >> $p_rpmdir/spec.post.install
  sed 's/.*INITENV *//' < $p_rpmdir/env |
    while read type var value; do
      case $type in
        SET )	echo "$var=\"$value\"; export $var" ;;
        +PATH )	echo "$var=\"$value\"\${$var+\":\$$var\"}; export $var" ;;
        UNSET )	echo "unset $var || true" ;;
        CMD )	echo "$var $value" ;;
        * )	echo "$p_name: unrecognised INITENV $type" 1>&2 ;;
      esac
    done >> $p_rpmdir/spec.post.install
  echo "EOF" >> $p_rpmdir/spec.post.install

  # Write final RPM spec.  First determine which sections if any are to
  # be output at all: if a section has own content or default content,
  # then output a preamble, content and trailer, otherwise don't output
  # the section at all.

  # Now regenerate the spec with this information.
  cat $p_rpmdir/spec.head \
      $p_rpmdir/spec.in.head \
      $p_rpmdir/spec.def.head \
      > $p_rpmdir/spec

  for s in $sections; do
    files=
    if [ -f $p_rpmdir/spec.in.$s ]; then
      files="$p_rpmdir/spec.pre.$s $p_rpmdir/spec.in.$s"
    elif [ -f $p_rpmdir/spec.def.$s ]; then
      files="$p_rpmdir/spec.pre.$s $p_rpmdir/spec.def.$s"
    fi

    if [ -z "$files" ]; then :; else
      files="$files $p_rpmdir/spec.post.$s"
      
      (echo; echo "%$s") >> $p_rpmdir/spec
      for f in $files; do
        [ -f $f ] && cat $f >> $p_rpmdir/spec
      done
    fi
  done

  # Postprocess, RPM doesn't like variables shorter than 3 characters...
  sed -e 's/%{n}/%{pkgname}/g' \
      -e 's/%n$/%{pkgname}/g' \
      -e 's/%n\([^_A-Za-z0-9]\)/%{pkgname}\1/g' \
      -e 's/%{v}/%{pkgversion}/g' \
      -e 's/%v$/%{pkgversion}/g' \
      -e 's/%v\([^_A-Za-z0-9]\)/%{pkgversion}\1/g' \
      -e 's/%{i}/%{pkginstroot}/g' \
      -e 's/%i$/%{pkginstroot}/g' \
      -e 's/%i\([^_A-Za-z0-9]\)/%{pkginstroot}\1/g' \
      < $p_rpmdir/spec > $p_rpmdir/spec.new
  cp $p_rpmdir/spec.new $p_rpmdir/spec

  rm -f $p_rpmdir/spec.* $p_rpmdir/env

  if $rpmall; then :; else
    cd $p_rpmdir
    rpmrecurse $rpmspecdone rpmspec
    cd $here
  fi
}

######################################################################
# Download all the sources and patches required by a RPM.  Argument is
# the package name, as recorded in the "### RPM" specs below in this
# file.  Assumes the spec is already generated with "rpmspec".  Scans
# the spec file for all "Source" and "Patch" definitions, downloading
# URLs into the package source directory, and extracting bare file
# names from this "### PATCH" or "### FILE" sections in this script.
rpmsrcdone=
rpmsrc () {
  # Remember what've already done, if we've already been here, quit quick
  case "$rpmsrcdone" in *:$1:* ) return;; esac
  rpmsrcdone=$rpmsrcdone:$1:

  # Start working here
  rpmvars $1

  echo "Preparing sources for $p_name..."
  [ -d $p_rpmdir ] || { echo "no spec directory $p_rpmdir" 1>&2; exit 1; }

  # NOTE: RPM knows how to download files with HTTP/FTP, but does not
  # do so for package sources or patches.  Download these files.
  #
  # NOTE: There is no way to query RPM for the sources and patches
  # listed in a spec file, and as the filenames contain RPM variables,
  # and as I have no interest in writing a spec parser, mutilate the
  # original spec so we can use rpm query to grab the file list:
  # remove everything after and including description, keeping just
  # the header part, massage the source plus patch file list into the
  # package description, and get rpm to expand the description and
  # thus produce the list of files needed by the package.

  cd $p_rpmdir
  perl -ne 'next if /^%description/ .. eof; print' < spec | egrep -v -i '^(source|patch)[0-9]*:' > tmpspec
  echo '%description' >> tmpspec
  egrep -i '^(source|patch)[0-9]*:' < spec | sed 's/^/ /' >> tmpspec
  eval rpm $rpmargs -q --specfile tmpspec -i |		# FIXME: rpmcmd?
    perl -ne 'next if 1 .. /^Description/; next if /^[A-Za-z]* *:/ .. eof; print' |
    while read tag url; do
      case $(echo $tag | tr '[A-Z]' '[a-z]'):$url in
	*:*/* )     download $p_src $url ;;
	patch*:* )  getpatch $url > $p_src/$url ;;
	source*:* ) getfile $url > $p_src/$url ;;
      esac
    done
  rm tmpspec

  $rpmall || rpmrecurse $rpmsrcdone rpmsrc

  cd $here
}

######################################################################
# Evaluate the value of a string in a RPM spec file.  The argument is
# whatever string should be expanded by RPM; it should expand to a
# single line.  Expects to find file "spec" in the current directory.
rpmeval () {
  # For description of how this works, see "rpmsrc".
  (perl -ne 'next if /^%description/ .. eof; print' < spec; echo "%description"; echo " X$1") > tmpspec
  (eval rpm $rpmargs -q --specfile tmpspec -i |		# FIXME: rpmcmd?
     perl -ne 'next if 1 .. /^Description/; next if /^[A-Za-z]* *:/ .. eof; s/^ X//; print'
   rm tmpspec)
}

######################################################################
# Build binary RPMs from all SPEC files.  Argument is the package
# name, as recorded in the "### RPM" specs below in this file.
rpmbindone=
rpmbin () {
  # Remember what've already done, if we've already been here, quit quick
  case "$rpmbindone" in *:$1:* ) return;; esac
  rpmbindone=$rpmbindone:$1:

  # Get ready for this package
  rpmvars $1

  [ -d $p_rpmdir ] || { echo "no spec directory $p_rpmdir" 1>&2; exit 1; }
  cd $p_rpmdir

  # Check if the binary RPM already exists; if so, skip this package
  rpmfile=$(rpmeval "%_rpmdir/%_rpmfilename")
  if ! $force && [ -f $rpmfile ]; then
    rpmrecho "RPM for $p_name already exists"
    return
  fi

  # Recurse if necessary
  rpmrecurse $rpmbindone rpmbin -n "Creating RPM for $p_name..."

  # Create build directory
  builddir=$(rpmeval "%_builddir")
  mkdir -p $builddir
  # FIXME: use rpmcmd but need full local copy database first...
  eval rpmbuild $rpmargs $rpmbuildsc $rpmbuild spec > $builddir/log 2>&1
  case $? in 0) echo " ok" ;; *) echo " failed ($?)" ;; esac

  cd $here
}

######################################################################
# Check that our RPMs are consistent and do not have any unintended
# dependencies on system libraries.  Warn about dependencies that are
# likely to cause trouble and should be noticed by spec writers.  This
# warns about stuff like libc.so, but that's in so that we also pick
# up all the tags such as "libc.so.6(GLIBC_NNN)" -- we need to know
# about those.
#
# FIXME: RPM may not correctly record shared library dependencies on
# systems other than Linux, the find-* scripts run at RPM build time
# have hard-coded "ELF" and *.so strings in them.  Certainly doesn't
# work on Mac OS X, all we get is the package dependencies we record
# ourselves.
rpmchecklibs () {
  echo "Checking RPM dependencies..."
  cd $here

  # Grab platform by querying whatever specs we have lying about
  aspec=$(ls SPECS/*/*/*/spec | head -1)
  platf=$(cd $(dirname $aspec) && rpmeval "%cmsplatf")

  # Produce a list of shared libraries the rpms produce
  tmpdir=$(mktemp -d)
  for f in RPMS/$platf/*.rpm; do rpm -qlp $f; done |
    egrep '\.(dylib|so)' > $tmpdir/libs

  # Check that all requirements are provided by some rpm we built or
  # are otherwise acceptable.  (FIXME: use rpmcmd?)
  cd RPMS/$platf
  for rpm in *.rpm; do
    eval rpm $rpmargs -qRp $rpm |
      while read req; do
        case $req in
	  # ignore our rpms, rpmlib internals and unhelpful noise from rpm
	  *+*+[0-9]* | rpmlib* | "(none)" ) ;;
	  *+[0-9]* ) ;; # for old rpms, remove!

	  # report any library we don't provide ourselves
          ld-linux.* | lib* )
            f=$(echo "$req" | sed 's/(.*)//')
            grep "/$(rxname $f)\$" $tmpdir/libs >/dev/null ||
              echo "$req: system library needed by $rpm"
	    ;;

	  # accept a few fixed-path binaries, report everything else
	  /bin/sh | /bin/csh | /bin/ksh | /usr/bin/env ) ;;
	  /* )
	    echo "$req: system binary needed by $rpm"
	    ;;

	  # report anything else as unknown
	  *)
	    echo "$req: unrecognised need by $rpm" ;;

	esac
      done
  done

  rm -fr $tmpdir
  cd $here
}

######################################################################
######################################################################
######################################################################
if [ -z "$*" ]; then
  rpmall=true
  set -- `(cd "$home/inst-files" && ls *.spec | sed 's/\.spec$//') 2>/dev/null`
fi

rpminit

$dospecs && for arg in "$@"; do
  rpmspec $arg
done

$dosrcs && for arg in "$@"; do
  rpmsrc $arg
done

$dorpms && for arg in "$@"; do
  rpmbin $arg
done

$dochecks && rpmchecklibs
exit 0
