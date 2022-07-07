cmake_minimum_required(VERSION 3.5)
project(Weather)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT /usr/include/boost)
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_FLAGS -O3)

find_package(
        Boost 1.78 COMPONENTS
        system
        thread
        json
        program_options)

include_directories(${Boost_INCLUDE_DIR})
file(GLOB SOURCES "src/sources/*.cpp")
file(GLOB HEADERS "src/headers/*.hpp")
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
