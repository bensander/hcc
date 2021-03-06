HCC : An open source C++ compiler for heterogeneous devices

Introduction
--------------------------------------------------------------------------------
This repository hosts HCC compiler implementation project. The goal is to
implement a compiler that takes a program conforming parallel programming
standards such as C++AMP, HC, C++17 ParallelSTL, or OpenMP and transforms it
into the following targets:

1. OpenCL SPIR
2. OpenCL C
3. HSA BRIG

The project is based on LLVM+CLANG.

HSA-specific features are also gradually being added to the project.  For more
HSA-related information please visit:

https://bitbucket.org/multicoreware/hcc/wiki/HSA%20Support%20Status


Repository Structure
-------------------------------------------------------------------------------
The current repository is composed of as following:
1. hcc
  - This repository. It contains all the related implementations, tests,
    scripts and build system.
2. hcc-clang
  - Modified Clang. It will be downloaded as part of the build procedure which
    will be described later. This repository is basically a clone of Clang
    repository.


Build Instruction
--------------------------------------------------------------------------------
* Prerequisite

Please install the following packges beforehand:

cmake
git
subversion
g++
libstdc++-4.8-dev
libdwarf-dev
libelf-dev
libtinfo-dev
libc6-dev-i386
gcc-multilib
llvm
llvm-dev
llvm-runtime
libc++1
libc++-dev
re2c
libncurses5-dev

* Prerequisite/OpenCL

1. Linux distribution: Ubuntu 14.04 or higher

2. OpenCL Driver: OpenCL 1.1 or above
  - AMD:
    - OpenCL 1.2 : http://support.amd.com/en-us/download
    - (NOT thoroughly tested yet) OpenCL 2.0 : http://support.amd.com/en-us/kb-articles/Pages/OpenCL2-Driver.aspx
  - Intel:
    - https://software.intel.com/en-us/articles/opencl-drivers
  - NVidia:
    - http://www.nvidia.com/Download/index.aspx?lang=en-us

3. OpencL SDK: OpenCL 1.1 or above
  - AMD:
    - http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/
  - Intel:
    - https://software.intel.com/en-us/intel-opencl
  - NVidia:
    - https://developer.nvidia.com/cuda-downloads

* Prerequisite/HSA

1. Linux distribution: Ubuntu 14.04 or higher

2. HSA Linux Driver:
  - https://github.com/HSAFoundation/HSA-Drivers-Linux-AMD
  - make sure kfd_check_installation.sh shows "Can run HSA YES"

3. HSA Runtime:
  - https://github.com/HSAFoundation/HSA-Runtime-AMD

4. (optional) HSA HOF:
   This is an optional tool to help you finalize HSA kernels at compile-time
   rather than runtime.  It can be found at:
   - https://github.com/HSAFoundation/HSA-HOF-AMD

   Please notice the tools only supports Kaveri APU as of now. If you use dGPU
   such as Fiji please do NOT install this tool.

* Prerequisite/Repository

For any development work, you should fork from them and create your own
repository for development. This build procedure assumes that you will
work on your own repository. As such, the following repositories should
be available:

  https://${username}@bitbucket.org/${username}/hcc
  https://${username}@bitbucket.org/${username}/hcc-clang

, where ${username} is a placeholder for your ID for bitbucket.org.

Also developers need to register their public key to bitbucket. Instructions
are:

1. Create public-private key pair
  # ssh-keygen

2. Upload .ssh/id_rsa.pub to bitbucket, under Admin of your account.


* Now, here goes actual build instructions.

1. Prepare a directory for work space.
   Please make sure there is no special characters like '+' or space in the full path.
  # mkdir cppamp

2. Pull your cppamp-driver-ng repository from bitbucket.
  # cd cppamp
  # git clone git@bitbucket.org:${username}/cppamp-driver-ng-35.git src

3. Create a build directory and configure using CMake.
  # mkdir build
  # cd build
  # cmake ../src

  The default options can be overridden on the command line:
  # cmake ../src \
      -DCLANG_URL=https://bitbucket.org/multicoreware/hcc-clang.git \
      -DOPENCL_HEADER_DIR=/opt/AMDAPP/include \
      -DOPENCL_LIBRARY_DIR=/opt/AMDAPP/lib/x86_64 \
      -DHSA_HEADER_DIR=/opt/hsa/include \
      -DHSA_LIBRARY_DIR=/opt/hsa/lib \
      -DHSA_KMT_LIBRARY_DIR=/opt/hsa/lib \
      -DHSA_HOF_DIR=/opt/hsa/bin \
      -DCMAKE_BUILD_TYPE=Release \
      -DCXXAMP_ENABLE_BOLT=OFF
      -DHSAIL_COMPILER_DIR=/opt/amd/bin \
      -DHSAIL_ASSEMBLER_DIR=/opt/amd/bin \

   - CLANG_URL : URL of HCC clang implementation

   - OPENCL_HEADER_DIR : OpenCL header include path
   - OPENCL_LIBRARY_DIR : OpenCL runtime library path

   - HSA_HEADER_DIR : HSA runtime header include path (default is /opt/hsa/include )
   - HSA_LIBRARY_DIR : HSA runtime library path (default is /opt/hsa/lib )

   - HSA_KMT_LIBRARY_DIR : HSA kernel library path (default is /opt/hsa/lib )

   - HSA_HOF_DIR : HSA offline finalizer binary path (default is /opt/hsa/bin )

   - CMAKE_BUILD_TYPE : Release or Debug (default is Release )

   - CXXAMP_ENABLE_BOLT : ON / OFF AMD Bolt API (default is OFF )

   - HSAIL_COMPILER_DIR : Place where HSAIL compiler (HLC) source code is placed.
                          By default HCC compiler will checkout a fresh copy from
                          github if this variable is not set.

   - HSAIL_ASSEMBLER_DIR : Place where HSAIL-Tools (HSAILasm) source code is placed.
                          By default HCC compiler will checkout a fresh copy from
                          github if this variable is not set.

4. Build the whole system. This will build clang and other libraries
   that require one time build.
  # make -j$(getconf _NPROCESSORS_ONLN) world
  # CLAMP_NOTILECHECK=ON make   // this builds llvm utilities

   For recurring builds for llvm and clang:
  # CLAMP_NOTILECHECK=ON make

5. Test. (optional, would be more useful in development cycle)
  # make test

   Please notice HSA-specific tests shall be executed in the following way:
  # HCC_RUNTIME=HSA make test

6. Rebuild build system from CMake. When you bring changes to build system by
   modifying CMakeFiles.txt for example, the build system could be messed up.
   When such happens, you can remove cache of CMake and repopulate the build
   system from scratch.
  # cd build
  # rm -f CMakeCache.txt         // or use rm -rf *
  # (repeat No. 3 in this section)


Dynamic Libaries
-------------------------------------------------------------------------------
After building, please change /etc/ld.so.conf to let dynamic libraries be
locatable at runtime:

# If you install deb files:
# HCC runtime implementations
/opt/hcc/lib

# If you build from source:
# HCC runtime implementations
(path_of_your_build_directory)/build/Release/lib

Remember to use: sudo ldconfig -v to reload ld.so cache.


Environment varialbes
-------------------------------------------------------------------------------
The following environment variables can change the behavior of HCC.

1. HCC_NOSPIR

export HCC_NOSPIR=1 could force HCC emit OpenCL C instead of SPIR at
compile time.

2. HCC_RUNTIME

HCC programs will automatically pick HCC runtime to use in the following
precedence:

- HSA
- OpenCL

Set HCC_RUNTIME to different values to force pick different HCC runtime.
Available options are:

- HSA : HSA
- CL : OpenCL

3. CLAMP_NOTILECHECK

export CLAMP_NOTILECHECK=1 could suppress tile uniformity check in HCC at
compile time.

If you compile AMD Bolt API along with C++AMP, this environment variable must
be set due to internal issues inside Bolt.


How to push your changes to the main repository
-------------------------------------------------------------------------------
Create pull request from bitbucket.


How to make your repository synchronized by pulling from main repository
-------------------------------------------------------------------------------
Synchronization can be done via scripts:

  # hcc/src/scripts/pull_from_trunk.py

It will pull from both cppamp-driver-ng-35 and cppamp-ng-35. Note that you still need to
update repositories. Again, when build system does not seem to work, you will
need to redo step No. 6 from the build instruction.


Directory Structure
-------------------------------------------------------------------------------
$hcc                // top-level directory
  build/               // all outputs
  src/                 // llvm
    compiler/
      tools/
        clang/         // modified Clang frontend
  tests/               // tests including unit tests, TDD issues
  include/             // HCC related headers
  lib/                 // HCC related implementions


Test Driven Development
-------------------------------------------------------------------------------
HCC development project adapts TDD(Test Driven Development). It helps keep
track of development status by working tests intact. Check out src/tests for
actual implementation.


Tips for Developers
-------------------------------------------------------------------------------
1. Debugging Clang

  Clang itself is not debuggable as it is actually 'driver' that calls actual
  compiler. Debugging becomes available once you give '-v' option to the Clang
  execution:

    # clang -v <other arguments>

  It will print out verbose messages that actually it runs. Using this
  information will enable debugging.
