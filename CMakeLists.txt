### YOU CAN'T MODIFY THE CODE BELOW
cmake_minimum_required(VERSION 3.15.2)
project(bookstore_2023)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -O2 -O3")

include_directories(
	${PROJECT_SOURCE_DIR}/src2/
)



file(GLOB_RECURSE main_src src2/*.cpp)
add_executable(code ${main_src}) 
# Add all *.cpp file after src/main.cpp, like src/Evalvisitor.cpp did
