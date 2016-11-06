cmake_minimum_required(VERSION 3.6)

# Used with documentation
set(MAIN_PROJECT_NAME "Kabanero")
set(MAIN_PROJECT_VERSION "0.1.0")

set(EXECUTABLE_NAME "kabanero")
project("${EXECUTABLE_NAME}" C CXX)

set(IMGUI_SFML_BUILD_EXAMPLES 0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14")
set(IMGUI_ROOT "${CMAKE_SOURCE_DIR}/lib/imgui")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES      src/*.cpp include/*.hpp)
file(GLOB_RECURSE SOURCE_TEST_FILES test/*.cpp)
file(GLOB_RECURSE RESOURCE_FILES resources/*)

include_directories(/usr/bin/include)
include_directories(include)
include_directories(lib/sol2/single/sol)
include_directories(lib/glm)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/imgui-sfml")

add_subdirectory(test)

# Documentation
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API
documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
  message("Doxygen found. Building documentation woth doxygen.")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()

# Add executable
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

# Lua
find_package(Lua REQUIRED)
if (LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARIES})
endif()

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
