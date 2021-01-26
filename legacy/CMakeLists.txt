project(legacy)
cmake_minimum_required(VERSION 3.16
	)
include(../cmake/readMakeList.txt)

readVariable(MakeList LEGACY_SRC SRCS)
readVariable(MakeList HDR HDR)

find_package(
  Qt5
  REQUIRED
  Widgets
  Gui
  Core
  )

# TODO: try to remove all these external sources!
add_library(legacy SHARED ${SRCS} components/component.cpp components/componentdialog.cpp components/gatecomponent.cpp)
# TODO: get rid of all of this dependecies
target_include_directories(legacy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/components
    ${PROJECT_SOURCE_DIR}/../src
 ${PROJECT_SOURCE_DIR}/../include
 ${qucs_BINARY_DIR}/main # config.h
 )
target_link_libraries(legacy Qt5::Core)
target_link_libraries(legacy Qt5::Gui)
target_link_libraries(legacy Qt5::Widgets)

install(TARGETS legacy DESTINATION lib/qucs)

