project(Diagrams CXX)
include(../../cmake/readMakeList.txt)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

readVariable(MakeList DIAGRAMS DIAGRAMS)
message(STATUS "Available diagrams: ${DIAGRAMS}")

include(configure.cmake)
set(CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/diagrams.cpp)
configureDiagramLibrary(${CONFIG_FILE} "${DIAGRAMS}")

# for each diagram its own source file must exist
set(SRC "")
foreach(DIAGRAM ${DIAGRAMS})
	set(SRC ${SRC} "${DIAGRAM}.cpp")
endforeach()

add_library(diagrams SHARED ${SRC} ${NOTYET} ${CONFIG_FILE}
	${CMAKE_CURRENT_SOURCE_DIR}/../../src/diagram.cpp # interface class
	)

target_link_libraries(diagrams Qt5::Widgets)
target_include_directories(diagrams
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include # element.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../legacy # obsolete_paintings.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../src # schematic_dialog.h, diagram.h
	${qucs_BINARY_DIR}/main # config.h
	)

install(TARGETS diagrams DESTINATION lib/qucs/plugins)
