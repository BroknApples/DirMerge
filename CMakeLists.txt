cmake_minimum_required(VERSION 3.20)

project("fmerge" LANGUAGES CXX)

set(SOURCES
  # Main
  src/main.cpp
  
  # Core
  src/core/filesystem.cpp
  src/core/merge_config.cpp
)

set(INCLUDES
  
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})