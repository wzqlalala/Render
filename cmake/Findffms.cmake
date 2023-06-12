# 防止重复引入
if(MxSim_ffms_ALREADY_INCLUDED)
	return()
endif()
set(MxSim_ffms_ALREADY_INCLUDED 1)

#设置路径
set(ffms_Dir "${CMAKE_SOURCE_DIR}/../../ThirdLib/ffms")

set(ffms_INC_DIR "${ffms_Dir}/include")
set(ffms_BIN_DIR "${ffms_Dir}/bin")
set(ffms_LIB_DIR "${ffms_Dir}/lib")

add_library(ffms SHARED IMPORTED)
set_property(TARGET ffms PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ffms_INC_DIR})
set_property(TARGET ffms APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(ffms PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${ffms_LIB_DIR}/ffms2.lib"
		IMPORTED_LOCATION_RELEASE "${ffms_BIN_DIR}/ffms2.dll"
		IMPORTED_IMPLIB_DEBUG "${ffms_LIB_DIR}/ffms2.lib"
		IMPORTED_LOCATION_DEBUG "${ffms_BIN_DIR}/ffms2.dll"
	)