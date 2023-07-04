#-----------------------------------------------------------------------------
# 头文件搜索路径
#-----------------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendData)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendBasic)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendBase)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendLabel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RendLabelData)

#-----------------------------------------------------------------------------
# 自动添加include目录
#-----------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#-----------------------------------------------------------------------------
# 添加资源文件
#-----------------------------------------------------------------------------
set(_qrc "${CMAKE_CURRENT_SOURCE_DIR}/../../resource/Render.qrc")
qt5_add_resources(_resource ${_qrc})

#-----------------------------------------------------------------------------
# 源码扫描
#-----------------------------------------------------------------------------
file(GLOB _header "*.h")
file(GLOB _source "*.cpp")
file(GLOB _cc "*.cc")

#-----------------------------------------------------------------------------
# 添加动态库目标
#-----------------------------------------------------------------------------
add_library(RendView
	${_resource}
	${_header}
	${_source}
	${_cc}
)

#-----------------------------------------------------------------------------
# 添加接口声明宏
#-----------------------------------------------------------------------------
target_compile_definitions(RendView PRIVATE "RENDVIEW_LIB")

list(APPEND _depend_library RendData RendBase RendBasic RendLabel RendLabelData)

list(APPEND _runtimes_libraries
	Qt5::Core Qt5::Gui Qt5::Widgets MxFunctions MeshGeoData MeshData Freetype Qwt avcodec avformat avdevice avfilter avutil postproc swresample swscale
)

#-----------------------------------------------------------------------------
# 链接依赖库
#-----------------------------------------------------------------------------
target_link_libraries(RendView PRIVATE
	${_runtimes_libraries}
	${_depend_library}
)

#-----------------------------------------------------------------------------
# 添加依赖关系
#-----------------------------------------------------------------------------
add_dependencies(RendView ${_depend_library})

#-----------------------------------------------------------------------------
# 添加运行时依赖关系
#-----------------------------------------------------------------------------
set(MxRender_RendView_Runtimes_Libraries ${_runtimes_libraries})

#-----------------------------------------------------------------------------
# 安装头文件
#-----------------------------------------------------------------------------
install(
	FILES ${_header}
	DESTINATION ${INSTALL_INCDIR}			#将头文件安装到install/include目录
)