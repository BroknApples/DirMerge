# Setup project
cmake_minimum_required(VERSION 3.20)
project("fmerge" LANGUAGES CXX)


# Set some options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Add source code
set(SOURCES
  # Main
  src/main.cpp
  
  # Core
  src/core/filesystem.cpp
  src/core/merge_config.cpp
  src/core/merge_manager.cpp

  # UI
  src/ui/app.cpp
)


# Set include directories
set(INCLUDES
  
)


# Find Qt6 on the system
#set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64/lib/cmake") # NOTE: Replace with your local path to Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


# Create executable and link libraries/includes
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)