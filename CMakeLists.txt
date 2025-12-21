cmake_minimum_required(VERSION 3.20)
project(MolarTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if (WIN32)
  find_package(unofficial-sqlite3 CONFIG REQUIRED)
  target_link_libraries(MolarTracker PRIVATE unofficial::sqlite3::sqlite3)
else()
  find_package(SQLite3 REQUIRED)
endif()

qt_standard_project_setup()

add_subdirectory(src)
