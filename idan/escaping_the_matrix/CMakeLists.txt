cmake_minimum_required(VERSION 3.22)

project(vuln_research_chal_4)

# cpp standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# get sources
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/*.c
)

# add executable
add_executable(vuln_research_chal_4)
target_sources(vuln_research_chal_4 PRIVATE ${SRCS})
target_include_directories(vuln_research_chal_4 PRIVATE ${CMAKE_SOURCE_DIR}/src)
