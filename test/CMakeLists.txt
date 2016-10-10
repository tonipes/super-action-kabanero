include_directories("${CMAKE_SOURCE_DIR}/lib/catch/single_include")

file(GLOB_RECURSE SOURCE_TEST_FILES *.cpp)

add_executable("kabanero-test" ${SOURCE_TEST_FILES})
