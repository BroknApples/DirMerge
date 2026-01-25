# ==================> Setup project
cmake_minimum_required(VERSION 3.20)
project("VSFM" LANGUAGES CXX)


# ==================> Set options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# ==================> Add source code
set(SOURCES
  # Main
  src/main.cpp
  
  # Core
  src/core/filesystem.cpp
  src/core/merge_config.cpp
  src/core/merge_manager.cpp

  # UI
  src/ui/application.cpp
  src/ui/app_backend.cpp
)


# ==================> Set include directories
set(INCLUDES
  
)


# ==================> Find Qt6 on the system
#set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64/lib/cmake") # NOTE: Replace with your local path to Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick QuickControls2)


# ==================> Qt Policies
qt_policy(SET QTP0004 NEW)


# ==================> Create executable
add_executable(${PROJECT_NAME} ${SOURCES})


# ==================> Qt module
qt_add_qml_module(${PROJECT_NAME}
  URI vsfm
  VERSION 1.0
  RESOURCE_PREFIX /qt/qml
  QML_FILES
    src/ui/app_interface.qml  # Ensure the path to your QML file is correct
)


# ==================> Add dirs
target_include_directories(${PROJECT_NAME} PUBLIC
  ${INCLUDES}
)


# ==================> Add new define when building for release
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Release>:APP_RELEASE_BUILD>
  $<$<NOT:$<CONFIG:Release>>:APP_DEV_BUILD>
)


# ==================> Link Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt6::Core 
  Qt6::Gui 
  Qt6::Widgets 
  Qt6::Qml 
  Qt6::Quick
  Qt6::QuickControls2
)
