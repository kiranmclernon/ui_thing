cmake_minimum_required(VERSION 3.0)
project(vis)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

macro(add_library_macro libName fileName) 
        add_library(${libName}
                ${CMAKE_CURRENT_SOURCE_DIR}/include/${fileName}.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/${fileName}.cpp)
        target_include_directories(${libName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endmacro()

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(sfml_interface INTERFACE)
target_link_libraries(sfml_interface INTERFACE sfml-graphics)
target_include_directories(sfml_interface INTERFACE ${SFML_INCLUDE_DIR})

add_library_macro(geometery geometery)
target_link_libraries(geometery PUBLIC sfml_interface)

add_library_macro(element element)
target_link_libraries(element PUBLIC sfml_interface geometery)

add_library_macro(window window)
target_link_libraries(window PUBLIC sfml_interface geometery element)


add_executable(${PROJECT_NAME}_bin main.cpp)
target_link_libraries(${PROJECT_NAME}_bin geometery element window)
