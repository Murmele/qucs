# everything related to install the qucs application
# any dependencies are installed here
if(UNIX AND NOT APPLE)
	set(DESKTOP qucs.desktop)
	install(FILES ${DESKTOP}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

	install(FILES bitmaps/hicolor/16x16/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/16x16/apps)
	install(FILES bitmaps/hicolor/22x22/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/22x22/apps)
	install(FILES bitmaps/hicolor/32x32/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps)
	install(FILES bitmaps/hicolor/48x48/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/48x48/apps)
	install(FILES bitmaps/hicolor/64x64/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps)
	install(FILES bitmaps/hicolor/128x128/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps)
	install(FILES bitmaps/hicolor/256x256/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/256x256/apps)
	install(FILES bitmaps/hicolor/512x512/apps/qucs.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/512x512/apps)
	install(FILES bitmaps/hicolor/scalable/apps/qucs.svg
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps)
endif()



##
## Prepare the installation
##
#set(plugin_dest_dir bin)
#set(qtconf_dest_dir bin)
#set(APPS "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}")
#if(APPLE)
#  set(plugin_dest_dir ${PROJECT_NAME}.app/Contents/MacOS)
#  set(qtconf_dest_dir ${PROJECT_NAME}.app/Contents/Resources)
#  set(APPS "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.app")
#endif(APPLE)

#if(WIN32)
#  set(APPS "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.exe")
#endif(WIN32)

##
## Install the Qucs application, on Apple, the bundle is installed as on other
## platforms it'll go into the bin directory.
##
#install(
#  TARGETS qucs
#  BUNDLE DESTINATION bin COMPONENT Runtime
#  RUNTIME DESTINATION bin COMPONENT Runtime)

## set Windows runtime location for libqucschematic See:
## http://www.cmake.org/pipermail/cmake/2010-June/037461.html
#install(
#  TARGETS qucsschematic
#  RUNTIME DESTINATION bin COMPONENT runtime
#  ARCHIVE DESTINATION lib COMPONENT devel
#  LIBRARY DESTINATION lib COMPONENT library)

##
## Install needed Qt plugins by copying directories from the qt installation One
## can cull what gets copied by using 'REGEX "..." EXCLUDE'
##
#if(APPLE)
#  install(
#    DIRECTORY "${QT_PLUGINS_DIR}/imageformats"
#    DESTINATION bin/${plugin_dest_dir}/plugins
#    COMPONENT Runtime)
#endif()
##
## install a qt.conf file this inserts some cmake code into the install script to
## write the file
##
#if(APPLE)
#  install(CODE "
#    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/bin/${qtconf_dest_dir}/qt.conf\" \"\")
#    " COMPONENT Runtime)
#endif()

## ------------------------------------------------------------------------------
## --
## Use BundleUtilities to get all other dependencies for the application to work.
## It takes a bundle or executable along with possible plugins and inspects it
## for dependencies.  If they are not system dependencies, they are copied.

## directories to look for dependencies
#if(APPLE)
#  set(DIRS ${QT_LIBRARY_DIRS})
#endif()

## Now the work of copying dependencies into the bundle/package The quotes are
## escaped and variables to use at install time have their $ escaped An
## alternative is the do a configure_file() on a script and use install(SCRIPT
## ...). Note that the image plugins depend on QtSvg and QtXml, and it got those
## copied over.
#if(APPLE)
#  install(
#    CODE "
#    file(GLOB_RECURSE QTPLUGINS
#      \"\${CMAKE_INSTALL_PREFIX}/bin/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#    set(BU_CHMOD_BUNDLE_ITEMS ON)
#    include(BundleUtilities)
#    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#    "
#    COMPONENT Runtime)
#endif()

## Install wrapper scripts
#if(WIN32)
#  set(SCRIPTS qucsdigi.bat qucsveri.bat qucsdigilib.bat)
#else()
#  set(SCRIPTS qucsdigi qucsveri qucsdigilib)
#endif()
#install(FILES ${SCRIPTS} DESTINATION bin/)

## To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac
## OS X where CPackConfig.cmake is created by including CPack And then there's
## ways to customize this as well set(CPACK_BINARY_DRAGNDROP ON) include(CPack)
