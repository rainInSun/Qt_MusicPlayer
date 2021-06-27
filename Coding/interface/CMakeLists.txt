find_package(Qt5 COMPONENTS Core REQUIRED)

set(LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/build/lib)
set(FUNCSERVICE_SRC 
	${PROJECT_SOURCE_DIR}/interface/gmusicplayservice.h
)

include_directories(
	${PROJECT_SOURCE_DIR}/interface
)

add_library(interface STATIC ${FUNCSERVICE_SRC})

target_link_libraries(interface 
    Qt5::Core
)