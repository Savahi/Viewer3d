project(viewer3d)

cmake_minimum_required(VERSION 2.6)

find_package(GLUT)
find_package(OpenGL)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} include)

#file( GLOB CPPS "src/*.cpp" )

set(VIEWER3D_SOURCES src/display.cpp src/displayaxis.cpp src/displayfacet.cpp src/displayselectedmodelinfo.cpp src/displaytimescale.cpp src/displaytools.cpp src/helpers.cpp src/loadmodels.cpp src/loadoperations.cpp src/loadoptypes.cpp src/models.cpp src/operations.cpp src/viewer3d.cpp)

set(TOWEB_SOURCES src/helpers.cpp src/loadoperations.cpp src/loadoplinks.cpp src/loadresources.cpp src/loadopresassignments src/toweb.cpp)

add_executable( ${PROJECT_NAME} ${VIEWER3D_SOURCES} )
target_link_libraries( ${PROJECT_NAME} ${GLUT_glut_LIBRARY} ${OPENGL_LIBRARY} )

add_executable( toweb ${TOWEB_SOURCES} )
target_link_libraries( ${PROJECT_NAME} )

#set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/ )
#install( TARGETS ${PROJECT_NAME} DESTINATION bin )