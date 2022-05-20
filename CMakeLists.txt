cmake_minimum_required(VERSION 3.19)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(world-clock)

find_package(vulkify CONFIG)

if(NOT vulkify_FOUND)
  message(STATUS "Downloading vulkify")
  include(FetchContent)
  FetchContent_Declare(vf_src
    GIT_REPOSITORY https://github.com/vulkify/vulkify
    GIT_TAG f3dea66
  )
  FetchContent_MakeAvailable(vf_src)
endif()

# exe
add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE vulkify::vulkify vulkify::options)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_sources(${PROJECT_NAME} PRIVATE
  src/app/app.cpp
  src/app/app.hpp
  src/app/input.hpp

  src/gui/gui.cpp
  src/gui/gui.hpp

  src/io/io.cpp
  src/io/io.hpp

  src/clock/world_clock.hpp
  src/clock/world_hour.hpp

  src/main.cpp
)
