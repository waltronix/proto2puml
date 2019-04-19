cmake_minimum_required(VERSION 3.10)
project(proto2puml)

set(CMAKE_CXX_STANDARD 17)

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

include_directories("./3rdParty/PEGTL/include")
include_directories("./include")

add_executable(proto2puml "./src/main.cpp" "./src/proto2puml.cpp" "./src/puml_printer.cpp")


add_subdirectory("./3rdParty/googletest")
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_executable(unittests "./test/test.cpp" "./src/proto2puml.cpp" "./src/puml_printer.cpp")
target_link_libraries(unittests gtest gtest_main)

add_test(unit-tests unittests)
