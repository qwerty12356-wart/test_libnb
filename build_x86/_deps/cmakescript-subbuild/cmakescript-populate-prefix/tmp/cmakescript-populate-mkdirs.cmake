# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-src"
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-build"
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix"
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix/tmp"
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix/src/cmakescript-populate-stamp"
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix/src"
  "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix/src/cmakescript-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix/src/cmakescript-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/hein/Downloads/test_libnb/build_x86/_deps/cmakescript-subbuild/cmakescript-populate-prefix/src/cmakescript-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
