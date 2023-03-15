#-----------------------------------------------------------------------------
# 引入MxFunctions基础模块
#-----------------------------------------------------------------------------

#防止重复引入
if(MESHDATA_ALREADY_INCLUDED)
	return()
endif()
set(MESHDATA_ALREADY_INCLUDED 1)

#设置路径
set(MeshData_Dir "${CMAKE_SOURCE_DIR}/../Mesh/install")

set(MeshData_INC_DIR "${MeshData_Dir}/include")
set(MeshData_BIN_DIR "${MeshData_Dir}/bin")
set(MeshData_LIB_DIR "${MeshData_Dir}/lib")

add_library(MeshData SHARED IMPORTED)
set_property(TARGET MeshData PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${MeshData_INC_DIR})
set_property(TARGET MeshData APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(MeshData PROPERTIES
		IMPORTED_IMPLIB_RELEASE "${MeshData_LIB_DIR}/MeshData.lib"
		IMPORTED_LOCATION_RELEASE "${MeshData_BIN_DIR}/MeshData.dll"
	)