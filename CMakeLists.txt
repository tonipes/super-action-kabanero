cmake_minimum_required(VERSION 3.6)
project(kabanero)

set(IMGUI_SFML_BUILD_EXAMPLES 0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(EXECUTABLE_NAME "kabanero")
set(IMGUI_ROOT "${CMAKE_SOURCE_DIR}/lib/imgui")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES      src/*.cpp include/*.hpp)
file(GLOB_RECURSE SOURCE_TEST_FILES test/*.cpp)
file(GLOB_RECURSE RESOURCE_FILES resources/*)

include_directories(/usr/bin/include)
include_directories(include)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/imgui-sfml")

add_subdirectory(test)
# add_subdirectory(src) # not used
add_executable(${EXECUTABLE_NAME} ${IMGUI_SOURCES} ${IMGUI_SFML_SOURCES} ${SOURCE_FILES})

# Resources
set(RES_SOURCE "${CMAKE_SOURCE_DIR}/resources")
set(RES_DEST "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${RES_SOURCE} ${RES_DEST}
 DEPENDS ${RES_DEST}
 COMMENT "symbolic link resources folder from ${RES_SOURCE} => ${RES_DEST}"
)

# SFML
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR} ${IMGUI_SFML_INCLUDE_DIRS})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${IMGUI_SFML_DEPENDENCIES})
endif()

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
  # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()
