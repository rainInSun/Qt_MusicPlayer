find_package(Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

set(LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/build/lib)
set(FUNCSERVICE_SRC 
	${PROJECT_SOURCE_DIR}/funcservice/gfuncprovider.h
	${PROJECT_SOURCE_DIR}/funcservice/gfuncprovider.cpp
	${PROJECT_SOURCE_DIR}/funcservice/glyricparser.h
	${PROJECT_SOURCE_DIR}/funcservice/glyricparser.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}/interface
)

add_library(funcservice STATIC ${FUNCSERVICE_SRC})

target_link_libraries(funcservice 
    Qt5::Multimedia
    Qt5::Core
	interface
)