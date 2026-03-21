cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(QtTest CXX)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Widgets Sql REQUIRED)
add_executable(qt-demo main.cpp mainwindow.cpp contactdialog.cpp logindialog.cpp)
set_target_properties(qt-demo PROPERTIES
	WIN32_EXECUTABLE FALSE
)
target_link_libraries(qt-demo Qt6::Widgets Qt6::Sql)
