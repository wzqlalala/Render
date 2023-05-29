﻿# 防止重复引入
if(MxSim_Qwt_ALREADY_INCLUDED)
	return()
endif()
set(MxSim_Qwt_ALREADY_INCLUDED 1)

#设置路径
set(Qwt_Dir "${CMAKE_SOURCE_DIR}/../../ThirdLib/Qwt")

set(Qwt_INC_DIR "${MxFunctions_Dir}/include")
set(Qwt_BIN_DIR "${MxFunctions_Dir}/bin")
set(Qwt_LIB_DIR "${MxFunctions_Dir}/lib")

add_library(Qwt SHARED IMPORTED)
set_property(TARGET Qwt PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Qwt_INC_DIR})
set_property(TARGET Qwt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(MxFunctions PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${Qwt_LIB_DIR}/qwt.lib"
		IMPORTED_LOCATION_RELEASE "${Qwt_BIN_DIR}/qwt.dll"
		IMPORTED_IMPLIB_DEBUG "${Qwt_LIB_DIR}/qwtd.lib"
		IMPORTED_LOCATION_DEBUG "${Qwt_BIN_DIR}/qwtd.dll"
	)