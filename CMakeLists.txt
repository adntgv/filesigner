# CMakeList.txt: проект CMake для FileSigner; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_BUILD_TYPE Debug)
# Добавьте источник в исполняемый файл этого проекта.
add_executable (filesigner "main.cpp" "signature.cpp" "signature.h")

include_directories("/mnt/d/workspace/boost/boost_1_71_0" ".")

set(BOOST_ROOT "/mnt/d/workspace/boost/boost_1_71_0")
set (Boost_USE_STATIC_LIBS OFF) #enable dynamic linking

#search for unit_test_framework
find_package (Boost REQUIRED COMPONENTS filesystem)

#link Boost libraries to the new target
target_link_libraries (filesigner ${Boost_LIBRARIES} pthread)