project(mobilitygenerator)

add_executable(mobilitygenerator main.cpp node.cc obstacle.cc)

install(TARGETS mobilitygenerator RUNTIME DESTINATION bin)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    -O2
    # Other flags
)
