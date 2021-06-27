find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/build/lib)
set(SHELL_SRC 
	${PROJECT_SOURCE_DIR}/shell/uiwindows/include/gmusicplayermainwidget.h
	${PROJECT_SOURCE_DIR}/shell/uiwindows/src/gmusicplayermainwidget.ui
	${PROJECT_SOURCE_DIR}/shell/uiwindows/src/gmusicplayermainwidget.cpp
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/gpushbutton.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/gpushbutton.cpp
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/gprogressbar.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/gprogressbar.cpp
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/glyricshowview.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/glyricshowview.cpp
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/gcommonwidget.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/gcommonwidget.cpp
	${PROJECT_SOURCE_DIR}/shell/pubheader/gstyledef.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/gfilelistmodel.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/gfilelistmodel.cpp
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/gfilelistview.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/gfilelistview.cpp
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include/gfilelistwidget.h
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/src/gfilelistwidget.cpp
)

qt5_add_resources(SHELL_QRC 
	${PROJECT_SOURCE_DIR}/shell/resource/res.qrc
)

include_directories(
	${PROJECT_SOURCE_DIR}/shell/pubwidgets/include
	${PROJECT_SOURCE_DIR}/interface
	${PROJECT_SOURCE_DIR}/shell/pubheader
)

add_library(shell STATIC ${SHELL_SRC} ${SHELL_QRC})

target_link_libraries(shell
	Qt5::Widgets
	interface
)