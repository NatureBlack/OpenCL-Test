# marker detection library
cmake_minimum_required(VERSION 3.8)

execute_process(COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/cmake/parser.py -i ${CMAKE_CURRENT_SOURCE_DIR} -o ${CMAKE_CURRENT_BINARY_DIR})

project(OpenCLComputePi)

find_package(OpenCL)

file(GLOB SOURCE "*.c" "*.cpp" "*.cl")
file(GLOB HEADER "*.h" "*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
target_compile_definitions(${PROJECT_NAME} PRIVATE CL_PATH="${CMAKE_CURRENT_SOURCE_DIR}")