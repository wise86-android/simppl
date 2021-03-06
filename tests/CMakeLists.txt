cmake_minimum_required(VERSION 2.8)

include_directories(. ../include)


add_subdirectory(${GTEST_ROOT} gtest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(unittests
   timeout.cpp
   simple.cpp
   errors.cpp
   async_server.cpp
   properties.cpp
   variant.cpp
)

target_link_libraries(unittests gtest gtest_main simppl rt dbus-1)


# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(name "simpple unit tests" command unittests)


# not a test, just an introspection example  
add_executable(introserver
   introserver.cpp
)


target_link_libraries(introserver simppl rt dbus-1)
