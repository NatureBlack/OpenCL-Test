# marker detection library
cmake_minimum_required(VERSION 2.8)

execute_process(COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/cmake/parser.py -i ${CMAKE_CURRENT_SOURCE_DIR} -o ${CMAKE_CURRENT_BINARY_DIR})

project(OpenCLComputePi)

find_package(OpenCL)
if(OpenCL_FOUND)
	mark_as_advanced(CLEAR OpenCL_INCLUDE_DIR OpenCL_LIBRARY)
else()
	set(OpenCL_INCLUDE_DIRS "")
	set(OpenCL_LIBRARIES "")
endif()

file(GLOB SOURCE "*.c" "*.cpp" "*.cl")
file(GLOB HEADER "*.h" "*.hpp")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER})
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARIES})