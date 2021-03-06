cmake_minimum_required (VERSION 2.6)

project (simppl)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "-DSIMPPL_HAVE_INTROSPECTION=1 -std=c++1y -g -pthread")

include_directories(include)

# DBus includes
find_path(DBUS_INCLUDE_DIR
    dbus/dbus-bus.h
    PATHS /usr/include /usr/local/include
    PATH_SUFFIXES dbus-1.0
    ONLY_CMAKE_FIND_ROOT_PATH
    NO_DEFAULT_PATH
)

find_path(DBUS_ARCHDEPS_INCLUDE_DIR
    dbus/dbus-arch-deps.h
    PATHS /usr/lib /usr/lib/* /usr/local/lib /usr/local/lib/*
    PATH_SUFFIXES dbus-1.0/include
    ONLY_CMAKE_FIND_ROOT_PATH
    NO_DEFAULT_PATH
)

if(DBUS_INCLUDE_DIR)
    message("Found DBus include dir under '${DBUS_INCLUDE_DIR}'")
    include_directories(${DBUS_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Didn't find dbus directory")
endif()

if(DBUS_ARCHDEPS_INCLUDE_DIR)
    message("Found DBus arch-deps include dir under '${DBUS_ARCHDEPS_INCLUDE_DIR}'")
    include_directories(${DBUS_ARCHDEPS_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Didn't find dbus arch-deps directory")
endif()

add_subdirectory(src)


# gtest environment 
if(NOT GTEST_ROOT)
   message("Searching for gtest installation...")
   
   find_path(GTEST_ROOT
       include/gtest/gtest.h
       PATHS $ENV{HOME}/*
       PATHS /usr/share/*
       ONLY_CMAKE_FIND_ROOT_PATH
       NO_DEFAULT_PATH
   )
endif()

# having tests?
if(GTEST_ROOT)
    message("Building tests against '${GTEST_ROOT}'")
    add_subdirectory(tests)
else()
    message("Omitting tests!")
endif()


install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/simppl DESTINATION include)
