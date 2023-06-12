# 防止重复引入
if(MxSim_ffmpeg_ALREADY_INCLUDED)
	return()
endif()
set(MxSim_ffmpeg_ALREADY_INCLUDED 1)

#设置路径
set(ffmpeg_Dir "${CMAKE_SOURCE_DIR}/../../ThirdLib/ffmpeg")

set(ffmpeg_INC_DIR "${ffmpeg_Dir}/include")
set(ffmpeg_BIN_DIR "${ffmpeg_Dir}/bin")
set(ffmpeg_LIB_DIR "${ffmpeg_Dir}/lib")

add_library(avcodec SHARED IMPORTED)
set_property(TARGET avcodec PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET avcodec APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(avcodec PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/avcodec.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/avcodec-60.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/avcodec.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/avcodec-60.dll"
	)

add_library(avdevice SHARED IMPORTED)
set_property(TARGET avdevice PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET avdevice APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(avdevice PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/avdevice.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/avdevice-60.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/avdevice.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/avdevice-60.dll"
	)

add_library(avfilter SHARED IMPORTED)
set_property(TARGET avfilter PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET avfilter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(avfilter PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/avfilter.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/avfilter-9.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/avfilter.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/avfilter-9.dll"
	)

add_library(avformat SHARED IMPORTED)
set_property(TARGET avformat PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET avformat APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(avformat PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/avformat.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/avformat-60.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/avformat.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/avformat-60.dll"
	)

add_library(avutil SHARED IMPORTED)
set_property(TARGET avutil PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET avutil APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(avutil PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/avutil.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/avutil-58.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/avutil.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/avutil-58.dll"
	)

add_library(postproc SHARED IMPORTED)
set_property(TARGET postproc PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET postproc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(postproc PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/postproc.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/postproc-57.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/postproc.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/postproc-57.dll"
	)

add_library(swresample SHARED IMPORTED)
set_property(TARGET swresample PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET swresample APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(swresample PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/swresample.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/swresample-4.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/swresample.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/swresample-4.dll"
	)

add_library(swscale SHARED IMPORTED)
set_property(TARGET swscale PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffmpeg_INC_DIR})
set_property(TARGET swscale APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(swscale PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/swscale.lib"
		IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/swscale-7.dll"
		IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/swscale.lib"
		IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/swscale-7.dll"
	)

#list(APPEND ffmpegLibs avcodec-60 avdevice-60 avfilter-9 avformat-60 avutil-58 postproc-57 swresample-4 swscale-7)
#
#foreach(_library ${ffmpegLibs})
#	set_target_properties(${_library} PROPERTIES
#	IMPORTED_IMPLIB_RELEASE "${ffmpeg_LIB_DIR}/${_library}.lib"
#	IMPORTED_LOCATION_RELEASE "${ffmpeg_BIN_DIR}/${_library}.dll"
#	IMPORTED_IMPLIB_DEBUG "${ffmpeg_LIB_DIR}/${_library}.lib"
#	IMPORTED_LOCATION_DEBUG "${ffmpeg_BIN_DIR}/${_library}.dll"
#)
#endforeach()