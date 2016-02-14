PROJECT(filesync)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Just comment or uncomment the FindPackage line to use it or not
FIND_PACKAGE(Boost 1.60.0 COMPONENTS filesystem system)
IF(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	ADD_EXECUTABLE(filesync main.cpp)
	TARGET_LINK_LIBRARIES(filesync ${Boost_LIBRARIES})
ELSE()
	ADD_EXECUTABLE(filesync main.cpp)
	TARGET_LINK_LIBRARIES(filesync ${CONAN_LIBS})
ENDIF()