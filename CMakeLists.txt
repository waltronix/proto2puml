cmake_minimum_required(VERSION 3.10)
project(proto2puml)

set (CMAKE_CXX_STANDARD 17)

include(ExternalProject)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/3rdParty/PEGTL)

#add_executable(testit src/test.cpp)

add_executable(proto2puml src/main.cpp)
set_property(TARGET proto2puml PROPERTY CXX_STANDARD 17)
target_link_libraries(proto2puml PRIVATE taocpp::pegtl)
#target_link_libraries(proto2puml PRIVATE stdc++fs)

set(SAMPLE_FILE ${CMAKE_CURRENT_LIST_DIR}/samples/adressbook)

ADD_CUSTOM_COMMAND(
  OUTPUT ${SAMPLE_FILE}.puml
  COMMAND ./proto2puml ${SAMPLE_FILE}.proto > ${SAMPLE_FILE}.puml
  DEPENDS proto2puml
)

# ADD_CUSTOM_COMMAND(
#   OUTPUT ${SAMPLE_FILE}.png
#   COMMAND dot -Tpng -o${SAMPLE_FILE}.png ${SAMPLE_FILE}.dot
#   DEPENDS ${SAMPLE_FILE}.puml
# )

add_custom_target(sample ALL DEPENDS ${SAMPLE_FILE}.puml)
