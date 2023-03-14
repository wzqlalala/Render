if(MxRender_WIN)
	#获取qmake.exe的绝对路径，存储到变量_qmake_executable
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	#获取qmake.exe所在文件夹的绝对路径，存储到变量_qt_bin_dir
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

	#将QT和第三方的动态库复制到目标文件夹
	foreach(_lib ${MxRender_Main_Runtimes_Libraries})
		add_custom_command(TARGET Main POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy 	#执行CMake命令，CMAKE_COMMAND表示CMake的绝对路径
				$<TARGET_FILE:${_lib}>
			$<TARGET_FILE_DIR:Main>
		)
	endforeach()

	# 在windows环境需要拷贝所有依赖库的dll到构建目录
	#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#	#[[COMMAND ${CMAKE_COMMAND} -E copy 
	#	$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> 
	#	$<TARGET_FILE_DIR:${PROJECT_NAME}>
  	#	COMMAND_EXPAND_LISTS]]
	#
	#	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	#		# 拷贝Qt图片格式支持文件
	#		${_qt_bin_dir}/../plugins/imageformats
	#	$<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
	#
	#	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	#		# 拷贝Qt平台支持文件
	#		${_qt_bin_dir}/../plugins/platforms
	#	$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
	#)
else()
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
	
	foreach(_lib ${MxSimStart_Runtimes_Libraries})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy 
				$<TARGET_LINKER_FILE:${_lib}>
				$<TARGET_SONAME_FILE:${_lib}>
			$<TARGET_FILE_DIR:PythonModule>
		)
	endforeach()

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD

		COMMAND ${CMAKE_COMMAND} -E copy_directory 
			# 拷贝Qt图片格式支持文件
			${_qt_bin_dir}/../plugins/imageformats
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats

		COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${_qt_bin_dir}/../plugins/platforms
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms

		COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${_qt_bin_dir}/../plugins/platforminputcontexts	  
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforminputcontexts

		COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${_qt_bin_dir}/../plugins/xcbglintegrations	
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/xcbglintegrations

		COMMAND ${CMAKE_COMMAND} -E copy 
			$<TARGET_LINKER_FILE:Qt5::XcbQpa>
			$<TARGET_SONAME_FILE:Qt5::XcbQpa>
		$<TARGET_FILE_DIR:PythonModule>
	)

	file(
		COPY 
			# 拷贝Qt多国语言支持文件
			${_qt_bin_dir}/../lib
		DESTINATION ${DEVRUNTIME_LIBDIR}/..
		USE_SOURCE_PERMISSIONS
		FILES_MATCHING
		PATTERN "cmake*" EXCLUDE
		PATTERN "pkgconfig*" EXCLUDE
		PATTERN "libicu*.so*"
	)
endif()