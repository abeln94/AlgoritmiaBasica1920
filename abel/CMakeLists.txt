cmake_minimum_required(VERSION 3.10)
project(algoritmia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D USING_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3")
# -march=native: It makes the binary possible not compatible on a lot of hardware (basically any CPU that is not a
# direct descendent of the one used for compilation). IMPORTANT: Optimize the program using all the cpu instructions.

file(GLOB files "*.hpp" "*.cpp")

add_executable(algoritmia ${files})

# for threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(algoritmia Threads::Threads)