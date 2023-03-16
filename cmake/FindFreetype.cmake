#-----------------------------------------------------------------------------
# 引入Freetype
#-----------------------------------------------------------------------------

#防止重复引入
if(FreeType_ALREADY_INCLUDED)
	return()
endif()
set(FreeType_ALREADY_INCLUDED 1)

#设置路径
set(Freetype_Dir "${CMAKE_SOURCE_DIR}/../../ThirdLib/freetype_2.10.4")

set(Freetype_INC_DIR "${Freetype_Dir}/include")
set(Freetype_BIN_DIR "${Freetype_Dir}/bin")
set(Freetype_LIB_DIR "${Freetype_Dir}/lib")

add_library(Freetype SHARED IMPORTED)
set_property(TARGET Freetype PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Freetype_INC_DIR})
set_property(TARGET Freetype APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(Freetype PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${Freetype_LIB_DIR}/freetype.lib"
		IMPORTED_LOCATION_RELEASE "${Freetype_BIN_DIR}/freetype.dll"
		IMPORTED_IMPLIB_DEBUG "${Freetype_LIB_DIR}/freetype.lib"
		IMPORTED_LOCATION_DEBUG "${Freetype_BIN_DIR}/freetype.dll"
	)