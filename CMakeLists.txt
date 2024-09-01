cmake_minimum_required(VERSION 3.29.6)

project(libmacchiato VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WARN_DEPRECATED ON)
set(CMAKE_POLICY_WARNING_CMP0077 ON)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.s)

add_library(libmacchiato STATIC ${SOURCES})

set_target_properties(libmacchiato PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "libmacchiato"
)

target_include_directories(libmacchiato PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)
target_include_directories(libmacchiato PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_include_directories(libmacchiato PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/sdl-utils/Include
)

if (NOT TARGET sdl-utils)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/sdl-utils)
endif()

#target_link_libraries(libmacchiato wups libnotifications glm::glm sdl-utils imgui CURL::libcurl cpr)
target_link_libraries(libmacchiato PRIVATE sdl-utils)
