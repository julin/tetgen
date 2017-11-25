cmake_minimum_required(VERSION 2.8.12)
project(tetgen)

add_library(tetgen STATIC tetgen.h tetgen.cxx predicates.cxx)
if(WINDOWS)
  add_definitions(-D_USE_MATH_DEFINES -DNOMAIN -DNO_TIMER -DNOMINMAX -DMESQUITE_STATIC_LIB -DHAVE_CLOCK -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_DEPRECATE)
  set_target_properties(tetgen PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(tetgen PROPERTIES LINK_FLAGS_RELEASE "/nodefaultlib:LIBCMT")
  foreach(VAR
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${VAR} MATCHES "/RTC1")
        string(REGEX REPLACE "/RTC1" "" ${VAR} "${${VAR}}")
    endif()
  endforeach()  
elseif(UNIX)
  add_definitions(-DLINUX -DHAVE_CLOCK -DNOMAIN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

target_compile_definitions(tetgen PRIVATE -DTETLIBRARY)
