project(Viewer3d)

cmake_minimum_required(VERSION 2.6)

find_package(GLUT)
find_package(OpenGL)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} include)

file( GLOB CPPS "src/*.cpp" )

#add_definitions(-Wall -O2)

add_executable( ${PROJECT_NAME} ${CPPS} )

target_link_libraries( ${PROJECT_NAME} ${GLUT_glut_LIBRARY} ${OPENGL_LIBRARY} )

#set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/ )
#install( TARGETS ${PROJECT_NAME} DESTINATION bin )