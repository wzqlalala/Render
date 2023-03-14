#-----------------------------------------------------------------------------
# 引入MxFunctions基础模块
#-----------------------------------------------------------------------------

#防止重复引入
if(MXFUNCTIONS_ALREADY_INCLUDED)
	return()
endif()
set(MXFUNCTIONS_ALREADY_INCLUDED 1)

#设置路径
set(MxFunctions_Dir "${CMAKE_SOURCE_DIR}/../Functions/install")

set(MxFunctions_INC_DIR "${MxFunctions_Dir}/include")
set(MxFunctions_BIN_DIR "${MxFunctions_Dir}/bin")
set(MxFunctions_LIB_DIR "${MxFunctions_Dir}/lib")

add_library(MxFunctions SHARED IMPORTED)
set_property(TARGET MxFunctions PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${MxFunctions_INC_DIR})
set_property(TARGET MxFunctions APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(MxFunctions PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${MxFunctions_LIB_DIR}/MxFunctions.lib"
		IMPORTED_LOCATION_RELEASE "${MxFunctions_BIN_DIR}/MxFunctions.dll"
	)