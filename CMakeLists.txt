cmake_minimum_required(VERSION 3.2)
project(SnmpCollector)

# set version
set(SNMPCOLLECTOR_VERSION "0.1.0")
#
# Debugging Options
#
SET (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#SET(CMAKE_INSTALL_PREFIX /usr/local)
SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/target)

#local cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})


#c++11 __cplusplus > 201103L
#c++14 __cplusplus > 201402L
# Initialize CXXFLAGS.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
  message("compile support c++14, now set cxx flag with: -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")
  message("compile support c++11, now set cxx flag with: -std=c++11")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.
  Please use a different C++ compiler.")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG          "$ENV{CXXFLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "$ENV{CXXFLAGS} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "$ENV{CXXFLAGS} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "$ENV{CXXFLAGS} -O2 -g")

if(MSVC)
  SET(GCC_COVERAGE_LINK_FLAGS "/SAFESEH:NO")
  SET(CMAKE_EXE_LINKER_FLAGS "${GCC_COVERAGE_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
endif()
#
# Project Output Paths
#
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/archive )

# for multi-config builds (e.g. msvc xcode)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/bin )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/plugins )
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/archive )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_BINARY_DIR})

FIND_PACKAGE(NETSNMP)
INCLUDE_DIRECTORIES(${NETSNMP_INCLUDE_DIRS})
LINK_DIRECTORIES(${NETSNMP_LIBRARIES})

file(GLOB HEDER_FILES "./*.h" "device/*.h" "snmp/*.h" "common/*.h")
file(GLOB SOURCE_FILES "./*.cc" "device/*.cc" "snmp/*.cc" "common/*.cc")

set(LINK_LIBS netsnmp)
add_executable(SNMPTEST ${SOURCE_FILES} ${HEDER_FILES})
target_link_libraries(SNMPTEST ${LINK_LIBS})
