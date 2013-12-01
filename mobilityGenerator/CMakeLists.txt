project(mobilitygenerator)

add_executable(mobilitygenerator main.cpp)

install(TARGETS mobilitygenerator RUNTIME DESTINATION bin)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)
