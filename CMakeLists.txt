cmake_minimum_required (VERSION 3.19)
project("${CMAKE_PROJECT_NAME}")
include("$ENV{TOOLCHAIN_PATH}/CMakeLists.txt.in")

add_lib("libref" "source/lib")
add_unittest(libref ${LIB_SHM} ${LIB_LOG} ${LIB_COM})

add_bin(loadref "source/bin/loadref" libref ${LIB_CFG} ${LIB_SHM} ${LIB_LOG} ${LIB_COM})
