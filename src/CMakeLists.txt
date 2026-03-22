cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(ContactMgr CXX)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Widgets Sql REQUIRED)
add_executable(contactmgr main.cpp mainwindow.cpp contactdialog.cpp logindialog.cpp)
set_target_properties(contactmgr PROPERTIES
	WIN32_EXECUTABLE FALSE
)
target_link_libraries(contactmgr Qt6::Widgets Qt6::Sql)
