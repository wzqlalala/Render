#-----------------------------------------------------------------------------
# 头文件搜索路径
#-----------------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendBasic)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendData)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendBase)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendLabel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendView)

#-----------------------------------------------------------------------------
# 自动添加include目录
#-----------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#-----------------------------------------------------------------------------
# 源码扫描
#-----------------------------------------------------------------------------
file(GLOB _source "*.cpp")

#-----------------------------------------------------------------------------
# 添加动态库目标
#-----------------------------------------------------------------------------
add_executable(Main
	WIN32	#关闭控制台窗口
	${_source}
)

#-----------------------------------------------------------------------------
# 设置Visual Studio的启动项
#-----------------------------------------------------------------------------
set_property(DIRECTORY ${CMAKE_SOURCE_DIR}
	PROPERTY
		VS_STARTUP_PROJECT Main
)

#-----------------------------------------------------------------------------
# 设置Visual Studio的启动项
#-----------------------------------------------------------------------------
set_property(DIRECTORY ${CMAKE_SOURCE_DIR}
	PROPERTY
		VS_STARTUP_PROJECT Main
)

list(APPEND _depend_library RendBasic RendData RendBase RendLabel RendView)
list(APPEND _runtimes_libraries
	Qt5::Core Qt5::Gui Qt5::Widgets MxFunctions Freetype
)

#-----------------------------------------------------------------------------
# 链接依赖库
#-----------------------------------------------------------------------------
target_link_libraries(Main PRIVATE
	${_runtimes_libraries}
	${_depend_library}
)

#-----------------------------------------------------------------------------
# 添加依赖关系
#-----------------------------------------------------------------------------
add_dependencies(Main ${_depend_library})

#-----------------------------------------------------------------------------
# 添加运行时依赖关系
#-----------------------------------------------------------------------------
set(MxRender_Main_Runtimes_Libraries ${_runtimes_libraries})

#-----------------------------------------------------------------------------
# 安装Qt的依赖文件
#-----------------------------------------------------------------------------
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

#-----------------------------------------------------------------------------
# 拷贝依赖文件到构建目录
#-----------------------------------------------------------------------------
include(InitRuntime)

#-----------------------------------------------------------------------------
# 安装Qt的imageformats、platforms
#-----------------------------------------------------------------------------
install(
	DIRECTORY
		${_qt_bin_dir}/../plugins/imageformats
		${_qt_bin_dir}/../plugins/platforms
	DESTINATION ${INSTALL_BINDIR}
)