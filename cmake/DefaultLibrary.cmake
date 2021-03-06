cmake_minimum_required(VERSION 2.8)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

#---'Target "x" links to itself.' meldung unterdrücken---
cmake_policy(SET CMP0038 OLD)
cmake_policy(SET CMP0022 OLD)

include_directories(
	${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
	${ASSIMP_INCLUDE_PATH}
	#${CVK_INCLUDE_PATH}
	#${CUDA_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${CMAKE_SOURCE_DIR}/src/libraries/
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLFW_INCLUDE_GLCOREARB)

add_library(${ProjectId} ${SOURCES} ${HEADER})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_link_libraries(
    ${ProjectId}
    ${ALL_LIBRARIES}
	${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OpenGL3_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    #${CUDA_LIBRARIES}
)
