project(mobilitygenerator)
cmake_minimum_required(VERSION 2.8)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)


FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HDRS include/*.h)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
)



#add_executable(mobilitygenerator main.cpp node.cc obstacle.cc)
ADD_EXECUTABLE(mobilitygenerator
   ${SRCS}
   ${HDRS}
)


install(TARGETS mobilitygenerator RUNTIME DESTINATION bin)

## Compiler flags
# if(CMAKE_COMPILER_IS_GNUCXX)
#     set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
#     set(CMAKE_CXX_FLAGS "-std=c++11")
#     #set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
# endif()


ADD_DEFINITIONS(
   -std=c++11 # Or -std=c++0x
   -O2
#     # Other flags
)