#!/bin/bash


LLVM_CLANG_REVISION="146372"
JOBS="4"
DEBUG=0

function die() {
  echo "Something went wrong!"; exit 1;
}

function usage() {
  echo "USAGE:"
  echo "$0 <path> [ -j N | --jobs N ] [ -d | --debug ]"
  echo "$0 -h|--help"
  echo ""
  echo "This script fetches KLEE-CL and its dependencies and compiles everything."
  echo "<path> is the destination directory for KLEE-CL files."
  echo ""
  echo "OPTIONS"
  echo "-d , --debug - Debug script. Default $DEBUG"
  echo "-j N , --jobs N - Run Make with N jobs. Default $JOBS"
}

# Parse command line args
if [ "$#" -lt 1 ]; then 
  echo "Error: Insufficient arguments"
  usage
  exit 1
fi
while [ -n "$1" ]; do
  if [ $# -gt 1 ] || [ $( echo "$1" | grep -Ec '^(-h|--help)$' ) -eq 1 ] ; then
    #Handle optional arguments
    case "$1" in
      -h | --help )
        usage
        exit 0
        ;;
      -j | --jobs )
        shift
        JOBS=$1
        ;;
      -d | --debug )
        DEBUG=1
        ;;
      *)
        echo "Unrecognsied option '$1'"
        exit 1
    esac
  else
    #Handle mandatory argument
    [ ! -d "$1" ] && { echo "Directory '${1}' does not exist!"; exit 1; }
    ROOT="$( cd "$1"; pwd)"
  fi
  shift
done

#Show what we're up to if in debug mode
[ $DEBUG -eq 1 ] &&  set -x

#Check job number
if [ $( echo "$JOBS" | grep -Ec '^[0-9]+$') -ne 1 ]; then
  echo "Error: '$JOBS' is an invalid number of jobs to run"
  exit 1
fi

cd "${ROOT}" || die
LLVM_ROOT="${ROOT}/llvm_and_clang"
KLEE_CL_ROOT="${ROOT}/klee-cl"
UCLIBC_ROOT="${ROOT}/uclibc"
STP_ROOT="${ROOT}/stp"

#get klee-cl
[ ! -d "${KLEE_CL_ROOT}" ] && { mkdir "${KLEE_CL_ROOT}" || die ; }
cd "${KLEE_CL_ROOT}" || die
(cd src; git status) || { git clone git@github.com:delcypher/klee-cl.git src || die; }

#Setup llvm and clang
[ ! -d "${LLVM_ROOT}" ] && { mkdir "${LLVM_ROOT}" || die; }
cd "${LLVM_ROOT}" || die

#get llvm
svn co -r ${LLVM_CLANG_REVISION} http://llvm.org/svn/llvm-project/llvm/trunk src || die
cd src || die
# Revert any patches that may have been applied previously
svn revert -R . || die
cd "${LLVM_ROOT}/"

#get clang
cd src/tools || die
svn co -r ${LLVM_CLANG_REVISION} http://llvm.org/svn/llvm-project/cfe/trunk clang || die
cd clang || die
# Revert any patches that may have been applied previously
svn revert -R . || die


#Apply llvm/clang patches
cd "${LLVM_ROOT}/src" || die
patch -p1 -i "${KLEE_CL_ROOT}/src/patches/llvm-Define-the-KLEE-OpenCL-target.patch"  || die
patch -p1 -i "${KLEE_CL_ROOT}/src/patches/llvm-build-python.patch"  || die
cd "${LLVM_ROOT}/src/tools/clang" || die
patch -p1 -i "${KLEE_CL_ROOT}/src/patches/clang-Define-the-KLEE-OpenCL-target.patch" || die

#Get compilers-rt
cd "${LLVM_ROOT}/src/projects" || die
svn co -r ${LLVM_CLANG_REVISION} http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt || die

echo "Building LLVM/Clang"
cd "${LLVM_ROOT}" || die
if [ -d "bin" ]; then
  echo "Warning llvm/clang build directory already exists!"
else
  mkdir bin || die
fi

cd bin || die

#Configure
#../src/configure --enable-optimized
# Clang compilation fails
CXX=g++ CC=gcc ../src/configure --enable-debug-symbols --enable-assertions || die
make -j${JOBS} || die

#Get ucLibc
cd "${ROOT}"
[ ! -d "${UCLIBC_ROOT}" ] && { mkdir "${UCLIBC_ROOT}" || die;}
cd "$UCLIBC_ROOT" || die
(cd klee-uclibc; git status) || { git clone git://git.pcc.me.uk/~peter/klee-uclibc.git || die; }

cd klee-uclibc || die
# Patch for llvm build type
#sed -i 's/Debug+Asserts/Release+Debug+Asserts/g' Rules.mak.llvm
python2.7 configure --with-llvm="${LLVM_ROOT}/bin" || die

#build failure with crtn.o excepted
make
touch lib/crtn.o
make || die

# STP
cd "${ROOT}" || die
[ ! -d "${STP_ROOT}" ] && { mkdir "${STP_ROOT}" || die; }
cd "${STP_ROOT}" || die
svn co https://stp-fast-prover.svn.sourceforge.net/svnroot/stp-fast-prover/trunk/stp src || die
[ ! -d "install" ] && { mkdir install || die; }
cd src || die
./scripts/configure --with-prefix=${STP_ROOT}/install
make OPTIMIZE=-O2 CFLAGS_M32=  -j${JOBS} install

#Build KLEE-CL
cd "${KLEE_CL_ROOT}" || die
[ ! -d "bin" ] && { mkdir bin || die ; }
cd bin || die
../src/configure --enable-posix-runtime --enable-opencl \
  --with-uclibc="${UCLIBC_ROOT}/klee-uclibc" \
  --with-stp="${STP_ROOT}/install" \
  --with-llvmsrc="${LLVM_ROOT}/src" \
  --with-llvmobj="${LLVM_ROOT}/bin" || die

make -j${JOBS} || die

