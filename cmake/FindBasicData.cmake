#-----------------------------------------------------------------------------
# 引入BasicData基础模块
#-----------------------------------------------------------------------------

#防止重复引入
if(BASICDATA_ALREADY_INCLUDED)
	return()
endif()
set(BASICDATA_ALREADY_INCLUDED 1)

#设置路径
set(BasicData_Dir "${CMAKE_SOURCE_DIR}/../BasicData/install")

set(BasicData_INC_DIR "${BasicData_Dir}/include")
set(BasicData_BIN_DIR "${BasicData_Dir}/bin")
set(BasicData_LIB_DIR "${BasicData_Dir}/lib")

add_library(MeshData SHARED IMPORTED)
set_property(TARGET MeshData PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BasicData_INC_DIR})
set_property(TARGET MeshData APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(MeshData PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${BasicData_LIB_DIR}/MeshData.lib"
		IMPORTED_LOCATION_RELEASE "${BasicData_BIN_DIR}/MeshData.dll"
		IMPORTED_IMPLIB_DEBUG "${BasicData_LIB_DIR}/MeshDatad.lib"
		IMPORTED_LOCATION_DEBUG "${BasicData_BIN_DIR}/MeshDatad.dll"
	)
	
add_library(MeshGeoData SHARED IMPORTED)
set_property(TARGET MeshGeoData PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BasicData_INC_DIR})
set_property(TARGET MeshGeoData APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(MeshGeoData PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${BasicData_LIB_DIR}/MeshGeoData.lib"
		IMPORTED_LOCATION_RELEASE "${BasicData_BIN_DIR}/MeshGeoData.dll"
		IMPORTED_IMPLIB_DEBUG "${BasicData_LIB_DIR}/MeshGeoDatad.lib"
		IMPORTED_LOCATION_DEBUG "${BasicData_BIN_DIR}/MeshGeoDatad.dll"
	)