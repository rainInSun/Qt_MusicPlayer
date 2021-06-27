cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME musicplayer)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(PROJECT_BINARY_DIR ${PROJC_SOURCE_DIR}/build)
set(Qt5_DIR D:/Qt5.12/5.12.9/msvc2017/lib/cmake/Qt5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(
	${PROJECT_SOURCE_DIR}/shell/uiwindows/include
	${PROJECT_SOURCE_DIR}/interface
	${PROJECT_SOURCE_DIR}/funcservice
)

add_subdirectory(${PROJECT_SOURCE_DIR}/interface)
add_subdirectory(${PROJECT_SOURCE_DIR}/shell)
add_subdirectory(${PROJECT_SOURCE_DIR}/funcservice)

add_executable(musicplayer
	main.cpp
)

target_link_libraries(musicplayer
	shell
	funcservice
 )
