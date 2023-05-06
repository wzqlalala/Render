set(SOVERSION ${PROJECT_VERSION} ${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR} ${PROJECT_VERSION_PATCH})

#-----------------------------------------------------------------------------
# 所有项目
#-----------------------------------------------------------------------------
list(APPEND _libraries RendBasic RendData RendBase RendLabel RendLabelData RendView Main)

#-----------------------------------------------------------------------------
# 处理程序的RPATH
#-----------------------------------------------------------------------------
file(RELATIVE_PATH _rel ${CMAKE_INSTALL_PREFIX}/${INSTALL_BINDIR} ${CMAKE_INSTALL_PREFIX})

if(_APPLE_)
	set(_rpath "@loader_path/${_rel}")
else()
	set(_rpath "\$ORIGIN:\$ORIGIN/${_rel}")
endif()

file(TO_NATIVE_PATH "${_rpath}/${INSTALL_LIBDIR}" _lib_rpath)


foreach(_library ${_libraries})
	
	add_subdirectory(${_library})
	
	set_target_properties(${_library}
		PROPERTIES 
			MACOSX_RPATH ON
			SKIP_BUILD_RPATH OFF
			BUILD_WITH_INSTALL_RPATH ON
			INSTALL_RPATH "${_lib_rpath}"
			INSTALL_RPATH_USE_LINK_PATH ON
			#POSITION_INDEPENDENT_CODE 1
			#OUTPUT_NAME $<LOWER_CASE:${_library}>
			#DEBUG_POSTFIX "_d"
			VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}
			FOLDER Modules
	)

	#list(APPEND MxGeo_Runtimes_Libraries ${MxGeo_${_library}_Runtimes_Libraries})

endforeach()


#-----------------------------------------------------------------------------
# 安装MxRender，定义项目的安装目录
#-----------------------------------------------------------------------------
install(
	TARGETS ${_libraries}
	ARCHIVE DESTINATION ${INSTALL_LIBDIR} COMPONENT lib
	RUNTIME DESTINATION ${INSTALL_BINDIR} COMPONENT bin
)