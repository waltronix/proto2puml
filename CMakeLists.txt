cmake_minimum_required(VERSION 3.10)
project(proto2puml)

set (CMAKE_CXX_STANDARD 17)

include(ExternalProject)

set(PEGTL_DOWNLOAD_LOCATION ${CMAKE_CURRENT_LIST_DIR}/3rdParty/PEGTL)

ExternalProject_Add(PEGTL
    GIT_REPOSITORY https://github.com/taocpp/PEGTL
    GIT_TAG master
#    GIT_TAG 2.8.0
    PREFIX "3rdParty"
    SOURCE_DIR ${PEGTL_DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
 )

#add_subdirectory(${PEGTL_DOWNLOAD_LOCATION})

include_directories(${PEGTL_DOWNLOAD_LOCATION}/include)

#add_executable(testit src/test.cpp)


add_executable(proto2puml src/main.cpp)
#target_link_libraries(proto2puml PRIVATE taocpp::pegtl)
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
