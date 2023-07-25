#pragma once
#include <QObject>
#include<QHash>
#include <memory>
#include"mLableData.h"
#include<type_traits>

#include"rendlabeldata_global.h"
#include"mLableDataEnum.h"

using namespace std;
namespace MDataLable
{
	class RENDLABELDATA_EXPORT mLableDataController_pre : public QObject
	{
		Q_OBJECT

	public:
		mLableDataController_pre()
		{
			_lableData_implicit = make_shared<mLableData_implicit>();
			_lableData_explicit = make_shared<mLableData_explicit>();
			_lableData_fluid = make_shared<mLableData_fluid>();
			_lableData_magnet = make_shared<mLableData_magnet>();
		}
		~mLableDataController_pre() {};

		/*
		* 获取数据库（特化在cpp中实现）
		*/

		template<class T>
		shared_ptr<T> getLableDataBase();
		/*
		* 获取显隐和高亮数据  (只能获取含有图标的)
		*/
		template<class T>
		void getLableDataInfo(bool &state, const QString& name, const LableDataType type)
		{
			auto &lableData = getLableData<T>();
			if (lableData.find(name) == lableData.end())
			{
				return;
			}
			if (type == LableDataType_ShowState)
			{
				state = lableData[name].lableData.getLableShowState();
			}
			else if (type == LableDataType_HighLightState)
			{
				state = lableData[name].lableData.getLableHighlightState();
			}
		}
		/*
		* 判断数据是否存在
		*/
		template<class T>
		bool isLableDataExist(const QString& name)
		{
			auto &lableData = getLableData<T>();
			return lableData.find(name) != lableData.end();
		}
		////////////////////////单个数据操作////////////////////////
		/*
		* 添加单个数据
		*/
		template<class T>
		void appendLableData(const QString& name, T &data)
		{
			auto &lableData = getLableData<T>();
			lableData[name] = data;
		}
		/*
		* 删除单个数据
		*/
		template<class T>
		void deleteLableData(const QString& name)
		{
			auto &lableData = getLableData<T>();
			if (lableData.find(name) != lableData.end())
			{
				lableData.remove(name);
			}		
		}

		/*
		* 修改单个显隐数据  (第三个参数为实现重载)
		*/
		template<class T>
		void updateLableData_ShowState(const QString& name, const bool state, Font f)
		{
			auto &lableData = getLableData<T>();
			//文字		
			lableData[name].fontData.setLableShowState(state);
		}
		template<class T>
		void updateLableData_ShowState(const QString& name, const bool state, Lable l)
		{
			auto &lableData = getLableData<T>();
			//图标		
			lableData[name].lableData.setLableShowState(state);
		}
		template<class T>
		void updateLableData_ShowState(const QString& name, const bool state, Lable_Font lf)
		{
			auto &lableData = getLableData<T>();
			//图标+文字		
			lableData[name].lableData.setLableShowState(state);
			lableData[name].fontData.setLableShowState(state);
		}
		template<class T>
		void updateLableData_ShowState(const QString& name, const bool state, Lable_Lable ll)
		{
			auto &lableData = getLableData<T>();
			//图标+图标	
			lableData[name].lableData.setLableShowState(state);
			lableData[name].lableData2.setLableShowState(state);
		}
		template<class T>
		void updateLableData_ShowState(const QString& name, const bool state, Lable_Lable_Lable_Font lllf)
		{
			auto &lableData = getLableData<T>();
			//图标+图标+图标+文字
			lableData[name].lableData.setLableShowState(state);
			lableData[name].lableData2.setLableShowState(state);
			lableData[name].lableData3.setLableShowState(state);
			lableData[name].fontData.setLableShowState(state);

		}

		/*
		* 修改单个高亮数据 (第三个参数为实现重载)
		*/
		template<class T>
		void updateLableData_HighlightState(const QString& name, const bool state, Font f)
		{
			auto &lableData = getLableData<T>();
			//文字
			lableData[name].fontData.setLableHighlightState(state);

		}
		template<class T>
		void updateLableData_HighlightState(const QString& name, const bool state, Lable l)
		{
			auto &lableData = getLableData<T>();
			//图标
			lableData[name].lableData.setLableHighlightState(state);
		}
		template<class T>
		void updateLableData_HighlightState(const QString& name, const bool state, Lable_Font lf)
		{
			auto &lableData = getLableData<T>();
			//图标+文字
			lableData[name].lableData.setLableHighlightState(state);
			lableData[name].fontData.setLableHighlightState(state);
		}
		template<class T>
		void updateLableData_HighlightState(const QString& name, const bool state, Lable_Lable ll)
		{
			auto &lableData = getLableData<T>();
			//图标+图标
			lableData[name].lableData.setLableHighlightState(state);
			lableData[name].lableData2.setLableHighlightState(state);
		}
		template<class T>
		void updateLableData_HighlightState(const QString& name, const bool state, Lable_Lable_Lable_Font lllf)
		{
			auto &lableData = getLableData<T>();
			//图标+图标+图标+文字
			lableData[name].lableData.setLableHighlightState(state);
			lableData[name].lableData2.setLableHighlightState(state);
			lableData[name].lableData3.setLableHighlightState(state);
			lableData[name].fontData.setLableHighlightState(state);
		}

		/*
		* 修改单个颜色数据 (第三个参数为实现重载)
		*/
		template<class T>
		void updateLableData_Color(const QString& name, const QVector3D color, Font f)
		{
			auto &lableData = getLableData<T>();
			//文字	
			lableData[name].fontData.setLableColor(color);
		}
		template<class T>
		void updateLableData_Color(const QString& name, const QVector3D color, Lable l)
		{
			auto &lableData = getLableData<T>();
			//图标
			lableData[name].lableData.setLableColor(color);
		}
		template<class T>
		void updateLableData_Color(const QString& name, const QVector3D color, Lable_Font lf)
		{
			auto &lableData = getLableData<T>();
			//图标+文字
			lableData[name].lableData.setLableColor(color);
			lableData[name].fontData.setLableColor(color);
		}
		template<class T>
		void updateLableData_Color(const QString& name, const QVector3D color, Lable_Lable ll)
		{
			auto &lableData = getLableData<T>();
			//图标+图标
			lableData[name].lableData.setLableColor(color);
			lableData[name].lableData2.setLableColor(color);
		}
		template<class T>
		void updateLableData_Color(const QString& name, const QVector3D color, Lable_Lable_Lable_Font lllf)
		{
			auto &lableData = getLableData<T>();
			//图标+图标+图标+文字
			lableData[name].lableData.setLableColor(color);
			lableData[name].lableData2.setLableColor(color);
			lableData[name].lableData3.setLableColor(color);
			lableData[name].fontData.setLableColor(color);
		}
		/*
		* 修改单个大小数据（第三个参数为实现重载 不支持图标和文字一起改大小）
		*/
		template<class T>
		void updateLableData_Size(const QString& name, const float size, Font f)
		{
			auto &lableData = getLableData<T>();
			//文字
			lableData[name].fontData.setLableSize(size);
		}
		template<class T>
		void updateLableData_Size(const QString& name, const float size, Lable l) //
		{
			auto &lableData = getLableData<T>();
			//图标
			lableData[name].fontData.setLableSize(size);
		}
		template<class T>
		void updateLableData_Size(const QString& name, const float size, Lable_Lable l) //
		{
			auto &lableData = getLableData<T>();
			//图标+图标
			lableData[name].lableData.setLableSize(size);
			lableData[name].lableData2.setLableSize(size);
		}
		template<class T>
		void updateLableData_Size(const QString& name, const float size, Lable_Lable_Lable lll) //
		{
			auto &lableData = getLableData<T>();
			//图标+图标
			lableData[name].lableData.setLableSize(size);
			lableData[name].lableData2.setLableSize(size);
			lableData[name].lableData3.setLableSize(size);
		}


		/*
		* 获取单个数据（特化在cpp中实现）
		*/
		template<class T>
		QHash<QString, T>& getLableData();

		/*
		*  重命名
		*/
		template<class T>
		void resetName(const QString& oldname, const QString& newname)
		{
			auto &data = getLableData<T>();
			if (data.find(oldname) == data.end())
			{
				return;
			}
			T tmp = data.value(oldname);
			data.remove(oldname);
			data.insert(newname, tmp);
		}

		////////////////////////某一分析类型同一类型数据操作////////////////////////
		/*
		* 删除某一分析类型同一类型所有数据
		*/
		void deleteLableData_SameType(const AnalysisModule module, const LableOperateType type)
		{
			if (module == ImplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					_lableData_implicit->_arrow_Font_Axis_Implicit_Data.clear();
				}
				else if (type == LableOperateType_Load)
				{
					_lableData_implicit->_arrow_Font_Load_Implicit_Data.clear();
					_lableData_implicit->_triangle_Font_Load_Implicit_Data.clear();
					_lableData_implicit->_rhombus_Font_Load_Implicit_Data.clear();
					_lableData_implicit->_X_Font_Load_Implicit_Data.clear();
					_lableData_implicit->_lightning_Font_Load_Implicit_Data.clear();
				}
				else if (type == LableOperateType_BC)
				{
					_lableData_implicit->_df_BC_Implicit_Data.clear();
					_lableData_implicit->_home_BC_Implicit_Data.clear();
				}
				else if (type == LableOperateType_Connect)
				{
					_lableData_implicit->_line_Connect_Implicit_Data.clear();
					_lableData_implicit->_faceToFace_Connect_Implicit_Data.clear();
				}
				else if (type == LableOperateType_Contact)
				{			
					_lableData_implicit->_faceToFace_Contact_Implicit_Data.clear();				
				}			
				else if (type == LableOperateType_RigidConnect)
				{
					_lableData_implicit->_Type1_RC_Implicit_Data.clear();
					_lableData_implicit->_Type2_RC_Implicit_Data.clear();
					_lableData_implicit->_sphere_RC_Implicit_Data.clear();
					_lableData_implicit->_line_RC_Implicit_Data.clear();
				}
				else if (type == LableOperateType_RigidConstraint)
				{
					_lableData_implicit->_df_RBC_Implicit_Data.clear();
				}
				else if (type == LableOperateType_PlyCoordinate)
				{
					_lableData_implicit->_arrow_PlyCoordinate_Implicit_Data.clear();
				}
	
			}
			else if (module == ExplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					_lableData_explicit->_arrow_Font_Axis_Explicit_Data.clear();
				}
				else if (type == LableOperateType_Load)
				{
					_lableData_explicit->_arrow_Font_Load_Explicit_Data.clear();
					_lableData_explicit->_df_Load_Explicit_Data.clear();

				}
				else if (type == LableOperateType_Connect)
				{
					_lableData_explicit->_line_Connect_Explicit_Data.clear();
					_lableData_explicit->_faceToFace_Connect_Explicit_Data.clear();
					_lableData_explicit->_df_Connect_Explicit_Data.clear();
				}
				else if (type == LableOperateType_Contact)
				{
					_lableData_explicit->_faceToFace_Contact_Explicit_Data.clear();
				}
				else if (type == LableOperateType_BC)
				{
					_lableData_explicit->_df_BC_Explicit_Data.clear();
					_lableData_explicit->_arrow_BC_Explicit_Data.clear();
				}
				else if (type == LableOperateType_RigidWall)
				{
					_lableData_explicit->_line_RW_Explicit_Data.clear();
					_lableData_explicit->_face_RW_Explicit_Data.clear();
					_lableData_explicit->_axis_RW_Explicit_Data.clear();
				}
				else if (type == LableOperateType_InitCondition)
				{
					_lableData_explicit->_arrow_Font_InitCondition_Explicit_Data.clear();
					_lableData_explicit->_triangle_Font_InitCondition_Explicit_Data.clear();
				}
				else if (type == LableOperateType_Box)
				{
					_lableData_explicit->_face_Box_Explicit_Data.clear();
					_lableData_explicit->_line_Box_Explicit_Data.clear();
				}
				else if (type == LableOperateType_Vector)
				{
					_lableData_explicit->_arrow_Vector_Explicit_Data.clear();
				}
			}
			else if (module == MagnetAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					_lableData_magnet->_arrow_Font_Axis_Magnet_Data.clear();
				}

				else if (type == LableOperateType_Load)
				{
					_lableData_magnet->_arrow_Font_Load_Magnet_Data.clear();
					_lableData_magnet->_triangle_Font_Load_Magnet_Data.clear();
					_lableData_magnet->_lightning_Font_Load_Magnet_Data.clear();
				}
				else if (type == LableOperateType_BC)
				{
					_lableData_magnet->_arrow_BC_Magnet_Data.clear();
					_lableData_magnet->_home_BC_Magnet_Data.clear();
				}
			}

		}

		/*
		* 修改某一分析类型同一类型所有显隐数据
		*/
		void updateLableData_ShowState_SameType(const AnalysisModule module,const LableOperateType type, const bool state)
		{
			if (module == ImplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllShowState<Arrow_Font_Axis_Implicit>(_lableData_implicit->_arrow_Font_Axis_Implicit_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllShowState<Arrow_Font_Load_Implicit>(_lableData_implicit->_arrow_Font_Load_Implicit_Data, state, Lable_Font());
					setAllShowState<Triangle_Font_Load_Implicit>(_lableData_implicit->_triangle_Font_Load_Implicit_Data, state, Lable_Font());
					setAllShowState<Rhombus_Font_Load_Implicit>(_lableData_implicit->_rhombus_Font_Load_Implicit_Data, state, Lable_Font());
					setAllShowState<X_Font_Load_Implicit>(_lableData_implicit->_X_Font_Load_Implicit_Data, state, Lable_Font());
					setAllShowState<Lightning_Font_Load_Implicit>(_lableData_implicit->_lightning_Font_Load_Implicit_Data, state, Lable_Font());
				}
				else if (type == LableOperateType_BC)
				{
					setAllShowState<DF_BC_Implicit>(_lableData_implicit->_df_BC_Implicit_Data, state, Lable());
					setAllShowState<Home_BC_Implicit>(_lableData_implicit->_home_BC_Implicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllShowState<Line_Connect_Implicit>(_lableData_implicit->_line_Connect_Implicit_Data, state, Lable_Font());
					setAllShowState<FaceToFace_Connect_Implicit>(_lableData_implicit->_faceToFace_Connect_Implicit_Data, state, Lable_Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllShowState<FaceToFace_Contact_Implicit>(_lableData_implicit->_faceToFace_Contact_Implicit_Data, state, Lable_Lable());
				}
				else if (type == LableOperateType_RigidConnect)
				{
					setAllShowState<Type1_RC_Implicit>(_lableData_implicit->_Type1_RC_Implicit_Data, state, Lable());
					setAllShowState<Type2_RC_Implicit>(_lableData_implicit->_Type2_RC_Implicit_Data, state, Lable());
					setAllShowState<Line_RC_Implicit>(_lableData_implicit->_line_RC_Implicit_Data, state, Lable());
					setAllShowState<Sphere_RC_Implicit>(_lableData_implicit->_sphere_RC_Implicit_Data, state, Lable());
				}
				else if (type == LableOperateType_RigidConstraint)
				{
					setAllShowState<DF_RBC_Implicit>(_lableData_implicit->_df_RBC_Implicit_Data, state, Lable());
				}
				else if (type == LableOperateType_PlyCoordinate)
				{
					setAllShowState<Arrow_PlyCoordinate_Implicit>(_lableData_implicit->_arrow_PlyCoordinate_Implicit_Data, state, Lable());
				}
			}
			else if (module == ExplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllShowState<Arrow_Font_Axis_Explicit>(_lableData_explicit->_arrow_Font_Axis_Explicit_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllShowState<Arrow_Font_Load_Explicit>(_lableData_explicit->_arrow_Font_Load_Explicit_Data, state, Lable_Font());
					setAllShowState<DF_Load_Explicit>(_lableData_explicit->_df_Load_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_BC)
				{
					setAllShowState<Arrow_BC_Explicit>(_lableData_explicit->_arrow_BC_Explicit_Data, state, Lable());
					setAllShowState<DF_BC_Explicit>(_lableData_explicit->_df_BC_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllShowState<Line_Connect_Explicit>(_lableData_explicit->_line_Connect_Explicit_Data, state, Lable_Font());
					setAllShowState<FaceToFace_Connect_Explicit>(_lableData_explicit->_faceToFace_Connect_Explicit_Data, state, Lable_Lable());
					setAllShowState<DF_Connect_Explicit>(_lableData_explicit->_df_Connect_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllShowState<FaceToFace_Contact_Explicit>(_lableData_explicit->_faceToFace_Contact_Explicit_Data, state, Lable_Lable());
				}
				else if (type == LableOperateType_RigidWall)
				{
					setAllShowState<Line_RW_Explicit>(_lableData_explicit->_line_RW_Explicit_Data, state, Lable());
					setAllShowState<Face_RW_Explicit>(_lableData_explicit->_face_RW_Explicit_Data, state, Lable());
					setAllShowState<Axis_RW_Explicit>(_lableData_explicit->_axis_RW_Explicit_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_InitCondition)
				{
					setAllShowState<Arrow_Font_InitCondition_Explicit>(_lableData_explicit->_arrow_Font_InitCondition_Explicit_Data, state, Lable_Font());
					setAllShowState<Triangle_Font_InitCondition_Explicit>(_lableData_explicit->_triangle_Font_InitCondition_Explicit_Data, state, Lable_Font());
				}
				else if (type == LableOperateType_Box)
				{
					setAllShowState<Line_Box_Explicit>(_lableData_explicit->_line_Box_Explicit_Data, state, Lable());
					setAllShowState<Face_Box_Explicit>(_lableData_explicit->_face_Box_Explicit_Data, state, Lable());			
				}
				else if (type == LableOperateType_Vector)
				{
					setAllShowState<Arrow_Vector_Explicit>(_lableData_explicit->_arrow_Vector_Explicit_Data, state, Lable());
				}
			}
			else if (module == MagnetAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllShowState<Arrow_Font_Axis_Magnet>(_lableData_magnet->_arrow_Font_Axis_Magnet_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllShowState<Arrow_Font_Load_Magnet>(_lableData_magnet->_arrow_Font_Load_Magnet_Data, state, Lable_Font());
					setAllShowState<Triangle_Font_Load_Magnet>(_lableData_magnet->_triangle_Font_Load_Magnet_Data, state, Lable_Font());
					setAllShowState<Lightning_Font_Load_Magnet>(_lableData_magnet->_lightning_Font_Load_Magnet_Data, state, Lable_Font());
				}
				else if (type == LableOperateType_BC)
				{
					setAllShowState<Arrow_BC_Magnet>(_lableData_magnet->_arrow_BC_Magnet_Data, state, Lable());
					setAllShowState<Home_BC_Magnet>(_lableData_magnet->_home_BC_Magnet_Data, state, Lable());
				}
			}
		}

		/*
		* 修改某一分析类型同一类型所有高亮数据
		*/
		void updateLableData_HighlightState_SameType(const AnalysisModule module, const LableOperateType type, const bool state)
		{
			if (module == ImplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllHighlightState<Arrow_Font_Axis_Implicit>(_lableData_implicit->_arrow_Font_Axis_Implicit_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllHighlightState<Arrow_Font_Load_Implicit>(_lableData_implicit->_arrow_Font_Load_Implicit_Data, state, Lable_Font());
					setAllHighlightState<Triangle_Font_Load_Implicit>(_lableData_implicit->_triangle_Font_Load_Implicit_Data, state, Lable_Font());
					setAllHighlightState<Rhombus_Font_Load_Implicit>(_lableData_implicit->_rhombus_Font_Load_Implicit_Data, state, Lable_Font());
					setAllHighlightState<X_Font_Load_Implicit>(_lableData_implicit->_X_Font_Load_Implicit_Data, state, Lable_Font());
					setAllHighlightState<Lightning_Font_Load_Implicit>(_lableData_implicit->_lightning_Font_Load_Implicit_Data, state, Lable_Font());
				}
				else if (type == LableOperateType_BC)
				{
					setAllHighlightState<DF_BC_Implicit>(_lableData_implicit->_df_BC_Implicit_Data, state, Lable());
					setAllHighlightState<Home_BC_Implicit>(_lableData_implicit->_home_BC_Implicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllHighlightState<Line_Connect_Implicit>(_lableData_implicit->_line_Connect_Implicit_Data, state, Lable_Font());
					setAllHighlightState<FaceToFace_Connect_Implicit>(_lableData_implicit->_faceToFace_Connect_Implicit_Data, state, Lable_Lable());

				}
				else if (type == LableOperateType_Contact)
				{
					setAllHighlightState<FaceToFace_Contact_Implicit>(_lableData_implicit->_faceToFace_Contact_Implicit_Data, state, Lable_Lable());
				}
				else if (type == LableOperateType_RigidConnect)
				{
					setAllHighlightState<Type1_RC_Implicit>(_lableData_implicit->_Type1_RC_Implicit_Data, state, Lable());
					setAllHighlightState<Type2_RC_Implicit>(_lableData_implicit->_Type2_RC_Implicit_Data, state, Lable());
					setAllHighlightState<Line_RC_Implicit>(_lableData_implicit->_line_RC_Implicit_Data, state, Lable());
					setAllHighlightState<Sphere_RC_Implicit>(_lableData_implicit->_sphere_RC_Implicit_Data, state, Lable());
				}
				else if (type == LableOperateType_RigidConstraint)
				{
					setAllHighlightState<DF_RBC_Implicit>(_lableData_implicit->_df_RBC_Implicit_Data, state, Lable());
				}
				else if (type == LableOperateType_PlyCoordinate)
				{
					setAllHighlightState<Arrow_PlyCoordinate_Implicit>(_lableData_implicit->_arrow_PlyCoordinate_Implicit_Data, state, Lable());
				}
			}
			else if (module == ExplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllHighlightState<Arrow_Font_Axis_Explicit>(_lableData_explicit->_arrow_Font_Axis_Explicit_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllHighlightState<Arrow_Font_Load_Explicit>(_lableData_explicit->_arrow_Font_Load_Explicit_Data, state, Lable_Font());
					setAllHighlightState<DF_Load_Explicit>(_lableData_explicit->_df_Load_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_BC)
				{
					setAllHighlightState<Arrow_BC_Explicit>(_lableData_explicit->_arrow_BC_Explicit_Data, state, Lable());
					setAllHighlightState<DF_BC_Explicit>(_lableData_explicit->_df_BC_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllHighlightState<Line_Connect_Explicit>(_lableData_explicit->_line_Connect_Explicit_Data, state, Lable_Font());
					setAllHighlightState<FaceToFace_Connect_Explicit>(_lableData_explicit->_faceToFace_Connect_Explicit_Data, state, Lable_Lable());
					setAllHighlightState<DF_Connect_Explicit>(_lableData_explicit->_df_Connect_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllHighlightState<FaceToFace_Contact_Explicit>(_lableData_explicit->_faceToFace_Contact_Explicit_Data, state, Lable_Lable());
				}
				else if (type == LableOperateType_RigidWall)
				{
					setAllHighlightState<Line_RW_Explicit>(_lableData_explicit->_line_RW_Explicit_Data, state, Lable());
					setAllHighlightState<Face_RW_Explicit>(_lableData_explicit->_face_RW_Explicit_Data, state, Lable());
					setAllHighlightState<Axis_RW_Explicit>(_lableData_explicit->_axis_RW_Explicit_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_InitCondition)
				{
					setAllHighlightState<Arrow_Font_InitCondition_Explicit>(_lableData_explicit->_arrow_Font_InitCondition_Explicit_Data, state, Lable_Font());
					setAllHighlightState<Triangle_Font_InitCondition_Explicit>(_lableData_explicit->_triangle_Font_InitCondition_Explicit_Data, state, Lable_Font());
				}
				else if (type == LableOperateType_Box)
				{
					setAllHighlightState<Face_Box_Explicit>(_lableData_explicit->_face_Box_Explicit_Data, state, Lable());
					setAllHighlightState<Line_Box_Explicit>(_lableData_explicit->_line_Box_Explicit_Data, state, Lable());
				}
				else if (type == LableOperateType_Vector)
				{
					setAllHighlightState<Arrow_Vector_Explicit>(_lableData_explicit->_arrow_Vector_Explicit_Data, state, Lable());
				}
			}
			else if (module == MagnetAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllHighlightState<Arrow_Font_Axis_Magnet>(_lableData_magnet->_arrow_Font_Axis_Magnet_Data, state, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllHighlightState<Arrow_Font_Load_Magnet>(_lableData_magnet->_arrow_Font_Load_Magnet_Data, state, Lable_Font());
					setAllHighlightState<Triangle_Font_Load_Magnet>(_lableData_magnet->_triangle_Font_Load_Magnet_Data, state, Lable_Font());
					setAllHighlightState<Lightning_Font_Load_Magnet>(_lableData_magnet->_lightning_Font_Load_Magnet_Data, state, Lable_Font());
				}
				else if (type == LableOperateType_BC)
				{
					setAllHighlightState<Arrow_BC_Magnet>(_lableData_magnet->_arrow_BC_Magnet_Data, state, Lable());
					setAllHighlightState<Home_BC_Magnet>(_lableData_magnet->_home_BC_Magnet_Data, state, Lable());
				}

			}

			
		}

		/*
		* 修改某一分析类型同一类型所有颜色数据
		*/
		void updateLableData_Color_SameType(const AnalysisModule module, const LableOperateType type, const QVector3D color)
		{
			if (module == ImplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllColor<Arrow_Font_Axis_Implicit>(_lableData_implicit->_arrow_Font_Axis_Implicit_Data, color, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllColor<Arrow_Font_Load_Implicit>(_lableData_implicit->_arrow_Font_Load_Implicit_Data, color, Lable_Font());
					setAllColor<Triangle_Font_Load_Implicit>(_lableData_implicit->_triangle_Font_Load_Implicit_Data, color, Lable_Font());
					setAllColor<Rhombus_Font_Load_Implicit>(_lableData_implicit->_rhombus_Font_Load_Implicit_Data, color, Lable_Font());
					setAllColor<X_Font_Load_Implicit>(_lableData_implicit->_X_Font_Load_Implicit_Data, color, Lable_Font());
					setAllColor<Lightning_Font_Load_Implicit>(_lableData_implicit->_lightning_Font_Load_Implicit_Data, color, Lable_Font());
				}
				else if (type == LableOperateType_BC)
				{
					setAllColor<DF_BC_Implicit>(_lableData_implicit->_df_BC_Implicit_Data, color, Lable());
					setAllColor<Home_BC_Implicit>(_lableData_implicit->_home_BC_Implicit_Data, color, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllColor<Line_Connect_Implicit>(_lableData_implicit->_line_Connect_Implicit_Data, color, Lable_Font());
					setAllColor<FaceToFace_Connect_Implicit>(_lableData_implicit->_faceToFace_Connect_Implicit_Data, color, Lable_Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllColor<FaceToFace_Contact_Implicit>(_lableData_implicit->_faceToFace_Contact_Implicit_Data, color, Lable_Lable());
				}
				else if (type == LableOperateType_RigidConnect)
				{
					setAllColor<Type1_RC_Implicit>(_lableData_implicit->_Type1_RC_Implicit_Data, color, Lable());
					setAllColor<Type2_RC_Implicit>(_lableData_implicit->_Type2_RC_Implicit_Data, color, Lable());
					setAllColor<Line_RC_Implicit>(_lableData_implicit->_line_RC_Implicit_Data, color, Lable());
					setAllColor<Sphere_RC_Implicit>(_lableData_implicit->_sphere_RC_Implicit_Data, color, Lable());
				}
				else if (type == LableOperateType_RigidConstraint)
				{
					setAllColor<DF_RBC_Implicit>(_lableData_implicit->_df_RBC_Implicit_Data, color, Lable());
				}
				else if (type == LableOperateType_PlyCoordinate)
				{
					setAllColor<Arrow_PlyCoordinate_Implicit>(_lableData_implicit->_arrow_PlyCoordinate_Implicit_Data, color, Lable());
				}
				
			}
			else if (module == ExplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllColor<Arrow_Font_Axis_Explicit>(_lableData_explicit->_arrow_Font_Axis_Explicit_Data, color, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllColor<Arrow_Font_Load_Explicit>(_lableData_explicit->_arrow_Font_Load_Explicit_Data, color, Lable_Font());
					setAllColor<DF_Load_Explicit>(_lableData_explicit->_df_Load_Explicit_Data, color, Lable());
				}
				else if (type == LableOperateType_BC)
				{	
					setAllColor<Arrow_BC_Explicit>(_lableData_explicit->_arrow_BC_Explicit_Data, color, Lable());
					setAllColor<DF_BC_Explicit>(_lableData_explicit->_df_BC_Explicit_Data, color, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllColor<Line_Connect_Explicit>(_lableData_explicit->_line_Connect_Explicit_Data, color, Lable_Font());		
					setAllColor<FaceToFace_Connect_Explicit>(_lableData_explicit->_faceToFace_Connect_Explicit_Data, color, Lable_Lable());
					setAllColor<DF_Connect_Explicit>(_lableData_explicit->_df_Connect_Explicit_Data, color, Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllColor<FaceToFace_Contact_Explicit>(_lableData_explicit->_faceToFace_Contact_Explicit_Data, color, Lable_Lable());
				}
				else if (type == LableOperateType_RigidWall)
				{
					setAllColor<Line_RW_Explicit>(_lableData_explicit->_line_RW_Explicit_Data, color, Lable());
					setAllColor<Face_RW_Explicit>(_lableData_explicit->_face_RW_Explicit_Data, color, Lable());
					setAllColor<Axis_RW_Explicit>(_lableData_explicit->_axis_RW_Explicit_Data, color, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_InitCondition)
				{
					setAllColor<Arrow_Font_InitCondition_Explicit>(_lableData_explicit->_arrow_Font_InitCondition_Explicit_Data, color, Lable_Font());
					setAllColor<Triangle_Font_InitCondition_Explicit>(_lableData_explicit->_triangle_Font_InitCondition_Explicit_Data, color, Lable_Font());
				}
				else if (type == LableOperateType_Box)
				{
					setAllColor<Face_Box_Explicit>(_lableData_explicit->_face_Box_Explicit_Data, color, Lable());
					setAllColor<Line_Box_Explicit>(_lableData_explicit->_line_Box_Explicit_Data, color, Lable());
				}
				else if (type == LableOperateType_Vector)
				{
					setAllColor<Arrow_Vector_Explicit>(_lableData_explicit->_arrow_Vector_Explicit_Data, color, Lable());
				}
			}
			else if (module == MagnetAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllColor<Arrow_Font_Axis_Magnet>(_lableData_magnet->_arrow_Font_Axis_Magnet_Data, color, Lable_Lable_Lable_Font());
				}
				else if (type == LableOperateType_Load)
				{
					setAllColor<Arrow_Font_Load_Magnet>(_lableData_magnet->_arrow_Font_Load_Magnet_Data, color, Lable_Font());
					setAllColor<Triangle_Font_Load_Magnet>(_lableData_magnet->_triangle_Font_Load_Magnet_Data, color, Lable_Font());
					setAllColor<Lightning_Font_Load_Magnet>(_lableData_magnet->_lightning_Font_Load_Magnet_Data, color, Lable_Font());
				}
				else if (type == LableOperateType_BC)
				{
					setAllColor<Arrow_BC_Magnet>(_lableData_magnet->_arrow_BC_Magnet_Data, color, Lable());
					setAllColor<Home_BC_Magnet>(_lableData_magnet->_home_BC_Magnet_Data, color, Lable());
				}

			}
		}

		/*
		* 修改某一分析类型同一类型所有大小数据  T为 Font 、 Lable、Lable_Lable 、Lable_Lable_Lable 不支持文字图标一起修改
		*/
		template<class T>
		void updateLableData_Size_SameType(const AnalysisModule module, const LableOperateType type, const float size)
		{
			if (module == ImplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllSize<Arrow_Font_Axis_Implicit>(_lableData_implicit->_arrow_Font_Axis_Implicit_Data, size, T());
				}
				else if (type == LableOperateType_Load)
				{
					setAllSize<Arrow_Font_Load_Implicit>(_lableData_implicit->_arrow_Font_Load_Implicit_Data, size, T());
					setAllSize<Triangle_Font_Load_Implicit>(_lableData_implicit->_triangle_Font_Load_Implicit_Data, size, T());
					setAllSize<Rhombus_Font_Load_Implicit>(_lableData_implicit->_rhombus_Font_Load_Implicit_Data, size, T());
					setAllSize<X_Font_Load_Implicit>(_lableData_implicit->_X_Font_Load_Implicit_Data, size, T());
					setAllSize<Lightning_Font_Load_Implicit>(_lableData_implicit->_lightning_Font_Load_Implicit_Data, size, T());
				}
				else if (type == LableOperateType_BC)
				{
					setAllSize<DF_BC_Implicit>(_lableData_implicit->_df_BC_Implicit_Data, size, Lable());
					setAllSize<Home_BC_Implicit>(_lableData_implicit->_home_BC_Implicit_Data, size, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllSize<Line_Connect_Implicit>(_lableData_implicit->_line_Connect_Implicit_Data, size, T());
					setAllSize<FaceToFace_Connect_Implicit>(_lableData_implicit->_faceToFace_Connect_Implicit_Data, size, Lable_Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllSize<FaceToFace_Contact_Implicit>(_lableData_implicit->_faceToFace_Contact_Implicit_Data, size, Lable_Lable());
				}
				else if (type == LableOperateType_RigidConnect)
				{
					setAllSize<Type1_RC_Implicit>(_lableData_implicit->_Type1_RC_Implicit_Data, size, Lable());
					setAllSize<Type2_RC_Implicit>(_lableData_implicit->_Type2_RC_Implicit_Data, size, Lable());
					setAllSize<Sphere_RC_Implicit>(_lableData_implicit->_sphere_RC_Implicit_Data, size, Lable());
					setAllSize<Line_RC_Implicit>(_lableData_implicit->_line_RC_Implicit_Data, size, Lable());
				}
				else if (type == LableOperateType_RigidConstraint)
				{
					setAllSize<DF_RBC_Implicit>(_lableData_implicit->_df_RBC_Implicit_Data, size, Lable());
				}
				else if (type == LableOperateType_PlyCoordinate)
				{
					setAllSize<Arrow_PlyCoordinate_Implicit>(_lableData_implicit->_arrow_PlyCoordinate_Implicit_Data, size, Lable());
				}
	
			}	
			else if (module == ExplicitAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllSize<Arrow_Font_Axis_Explicit>(_lableData_explicit->_arrow_Font_Axis_Explicit_Data, size, T());
				}
				else if (type == LableOperateType_Load)
				{
					setAllSize<Arrow_Font_Load_Explicit>(_lableData_explicit->_arrow_Font_Load_Explicit_Data, size, T());
					setAllSize<DF_Load_Explicit>(_lableData_explicit->_df_Load_Explicit_Data, size, Lable());
				}
				else if (type == LableOperateType_BC)
				{
					setAllSize<Arrow_BC_Explicit>(_lableData_explicit->_arrow_BC_Explicit_Data, size, Lable());
					setAllSize<DF_BC_Explicit>(_lableData_explicit->_df_BC_Explicit_Data, size, Lable());
				}
				else if (type == LableOperateType_Connect)
				{
					setAllSize<Line_Connect_Explicit>(_lableData_explicit->_line_Connect_Explicit_Data, size, T());
					setAllSize<FaceToFace_Connect_Explicit>(_lableData_explicit->_faceToFace_Connect_Explicit_Data, size, Lable_Lable());
					setAllSize<DF_Connect_Explicit>(_lableData_explicit->_df_Connect_Explicit_Data, size, Lable());
				}
				else if (type == LableOperateType_Contact)
				{
					setAllSize<FaceToFace_Contact_Explicit>(_lableData_explicit->_faceToFace_Contact_Explicit_Data, size, Lable_Lable());
				}
				else if (type == LableOperateType_RigidWall)
				{
					setAllSize<Line_RW_Explicit>(_lableData_explicit->_line_RW_Explicit_Data, size, Lable());
					setAllSize<Face_RW_Explicit>(_lableData_explicit->_face_RW_Explicit_Data, size, Lable());
					setAllSize<Axis_RW_Explicit>(_lableData_explicit->_axis_RW_Explicit_Data, size, T());
				}
				else if (type == LableOperateType_InitCondition)
				{
					setAllSize<Arrow_Font_InitCondition_Explicit>(_lableData_explicit->_arrow_Font_InitCondition_Explicit_Data, size, T());
					setAllSize<Triangle_Font_InitCondition_Explicit>(_lableData_explicit->_triangle_Font_InitCondition_Explicit_Data, size, T());
				}
				else if (type == LableOperateType_Box)
				{
					setAllSize<Face_Box_Explicit>(_lableData_explicit->_face_Box_Explicit_Data, size, Lable());
					setAllSize<Line_Box_Explicit>(_lableData_explicit->_line_Box_Explicit_Data, size, Lable());
				}
				else if (type == LableOperateType_Vector)
				{
					setAllSize<Arrow_Vector_Explicit>(_lableData_explicit->_arrow_Vector_Explicit_Data, size, Lable());
				}
			}
			else if (module == MagnetAnalysis)
			{
				if (type == LableOperateType_LocalAxis)
				{
					setAllSize<Arrow_Font_Axis_Magnet>(_lableData_magnet->_arrow_Font_Axis_Magnet_Data, size, T());
				}
				else if (type == LableOperateType_Load)
				{
					setAllSize<Arrow_Font_Load_Magnet>(_lableData_magnet->_arrow_Font_Load_Magnet_Data, size, T());
					setAllSize<Triangle_Font_Load_Magnet>(_lableData_magnet->_triangle_Font_Load_Magnet_Data, size, T());
					setAllSize<Lightning_Font_Load_Magnet>(_lableData_magnet->_lightning_Font_Load_Magnet_Data, size, T());
				}
				else if (type == LableOperateType_BC)
				{
					setAllSize<Arrow_BC_Magnet>(_lableData_magnet->_arrow_BC_Magnet_Data, size, Lable());
					setAllSize<Home_BC_Magnet>(_lableData_magnet->_home_BC_Magnet_Data, size, Lable());
				}

			}

		}

		////////////////////////所有某一分析类型数据操作////////////////////////////
		/*
		* 删除某一分析类型所有数据
		*/
		void deleteAllLableData(const AnalysisModule module)
		{
			deleteLableData_SameType(module, LableOperateType_LocalAxis);
			deleteLableData_SameType(module,LableOperateType_Load);
			deleteLableData_SameType(module,LableOperateType_BC);
			deleteLableData_SameType(module,LableOperateType_Connect);
			deleteLableData_SameType(module,LableOperateType_Contact);
			deleteLableData_SameType(module, LableOperateType_RigidConnect);
			deleteLableData_SameType(module, LableOperateType_RigidConstraint);
			deleteLableData_SameType(module, LableOperateType_RigidWall);
			deleteLableData_SameType(module, LableOperateType_InitCondition);
			deleteLableData_SameType(module, LableOperateType_Box);
			deleteLableData_SameType(module, LableOperateType_Vector);
			deleteLableData_SameType(module, LableOperateType_PlyCoordinate);
		}

		/*
		* 修改某一分析类型所有显隐数据
		*/
		void updateAllLableData_ShowState(const AnalysisModule module,const bool state)
		{
			updateLableData_ShowState_SameType(module, LableOperateType_LocalAxis, state);
			updateLableData_ShowState_SameType(module, LableOperateType_Load, state);
			updateLableData_ShowState_SameType(module, LableOperateType_BC, state);
			updateLableData_ShowState_SameType(module, LableOperateType_Connect, state);
			updateLableData_ShowState_SameType(module, LableOperateType_Contact, state);
			updateLableData_ShowState_SameType(module, LableOperateType_RigidConnect, state);
			updateLableData_ShowState_SameType(module, LableOperateType_RigidConstraint, state);
			updateLableData_ShowState_SameType(module, LableOperateType_RigidWall, state);
			updateLableData_ShowState_SameType(module, LableOperateType_InitCondition, state);
			updateLableData_ShowState_SameType(module, LableOperateType_Box, state);
			updateLableData_ShowState_SameType(module, LableOperateType_Vector, state);
			updateLableData_ShowState_SameType(module, LableOperateType_PlyCoordinate, state);
		}

		/*
		* 修改某一分析类型所有高亮数据
		*/
		void updateAllLableData_HighlightState(const AnalysisModule module, const bool state)
		{
			updateLableData_HighlightState_SameType(module, LableOperateType_LocalAxis, state);
			updateLableData_HighlightState_SameType(module,LableOperateType_Load, state);
			updateLableData_HighlightState_SameType(module,LableOperateType_BC, state);
			updateLableData_HighlightState_SameType(module,LableOperateType_Connect, state);
			updateLableData_HighlightState_SameType(module,LableOperateType_Contact, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_RigidConnect, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_RigidConstraint, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_RigidWall, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_InitCondition, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_Box, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_Vector, state);
			updateLableData_HighlightState_SameType(module, LableOperateType_PlyCoordinate, state);
		}

		/*
		* 修改某一分析类型所有颜色数据
		*/
		void updateAllLableData_Color(const AnalysisModule module, const QVector3D color)
		{
			updateLableData_Color_SameType(module, LableOperateType_LocalAxis, color);
			updateLableData_Color_SameType(module, LableOperateType_Load, color);
			updateLableData_Color_SameType(module, LableOperateType_BC, color);
			updateLableData_Color_SameType(module, LableOperateType_Connect, color);
			updateLableData_Color_SameType(module, LableOperateType_Contact, color);
			updateLableData_Color_SameType(module, LableOperateType_RigidConnect, color);
			updateLableData_Color_SameType(module, LableOperateType_RigidConstraint, color);
			updateLableData_Color_SameType(module, LableOperateType_RigidWall, color);
			updateLableData_Color_SameType(module, LableOperateType_InitCondition, color);
			updateLableData_Color_SameType(module, LableOperateType_Box, color);
			updateLableData_Color_SameType(module, LableOperateType_Vector, color);
			updateLableData_Color_SameType(module, LableOperateType_PlyCoordinate, color);
		}

		/*
		* 修改某一分析类型所有大小数据
		*/
		template<class T>
		void updateAllLableData_Size(const AnalysisModule module,const float size)
		{		
			updateLableData_Size_SameType(module, LableOperateType_LocalAxis, size, T());
			updateLableData_Size_SameType(module, LableOperateType_Load, size, T());
			updateLableData_Size_SameType(module, LableOperateType_BC, size, T());
			updateLableData_Size_SameType(module, LableOperateType_Connect, size, T());
			updateLableData_Size_SameType(module, LableOperateType_Contact, size, T());
			updateLableData_Size_SameType(module, LableOperateType_RigidConnect, size, T());
			updateLableData_Size_SameType(module, LableOperateType_RigidConstraint, size, T());
			updateLableData_Size_SameType(module, LableOperateType_RigidWall, size, T());
			updateLableData_Size_SameType(module, LableOperateType_InitCondition, size, T());
			updateLableData_Size_SameType(module, LableOperateType_Box, size, T());
			updateLableData_Size_SameType(module, LableOperateType_Vector, size, T());
			updateLableData_Size_SameType(module, LableOperateType_PlyCoordinate, size, T());
		}

		////////////////////////所有分析类型数据操作////////////////////////////
		/*
		* 删除所有分析类型所有数据
		*/
		void deleteAllLableData()
		{
			deleteAllLableData(ImplicitAnalysis);
			deleteAllLableData(ExplicitAnalysis);
			deleteAllLableData(MagnetAnalysis);
		}

		/*
		* 修改所有分析类型所有显隐数据
		*/
		void updateAllLableData_ShowState(const bool state)
		{
			updateAllLableData_ShowState(ImplicitAnalysis, state);
			updateAllLableData_ShowState(ExplicitAnalysis, state);
			updateAllLableData_ShowState(MagnetAnalysis, state);
		}

		/*
		* 修改所有分析类型所有高亮数据
		*/
		void updateAllLableData_HighlightState(const bool state)
		{
			updateAllLableData_HighlightState(ImplicitAnalysis, state);
			updateAllLableData_HighlightState(ExplicitAnalysis, state);
			updateAllLableData_HighlightState(MagnetAnalysis, state);
		}

		/*
		* 修改所有分析类型所有颜色数据
		*/
		void updateAllLableData_Color(const QVector3D color)
		{
			updateAllLableData_Color(ImplicitAnalysis, color);
			updateAllLableData_Color(ExplicitAnalysis, color);
			updateAllLableData_Color(MagnetAnalysis, color);
		}

		/*
		* 修改所有分析类型所有大小数据
		*/
		template<class T>
		void updateAllLableData_Size(const float size)
		{
			updateAllLableData_Size(ImplicitAnalysis, size, T());
			updateAllLableData_Size(ExplicitAnalysis, size, T());
			updateAllLableData_Size(MagnetAnalysis, size, T());
		}
		/*
		* 获取额外的顶点数据（用于更新视角）
		*/
		QVector<QVector3D> getExtraVertex()
		{
			QVector<QVector3D> extraPos;
			//刚性墙
			for (auto it : _lableData_explicit->_face_RW_Explicit_Data)
			{
				extraPos += it.lableData.getParameter();
			}
			//盒子
			for (auto it : _lableData_explicit->_face_Box_Explicit_Data)
			{
				extraPos += it.lableData.getParameter();
			}
			//坐标系
			for (auto it : _lableData_explicit->_arrow_Font_Axis_Explicit_Data)
			{
				for (auto pos : it.lableData.getParameter())
				{
					extraPos += pos.first;
				}
			}
			//坐标系
			for (auto it : _lableData_implicit->_arrow_Font_Axis_Implicit_Data)
			{
				for (auto pos : it.lableData.getParameter())
				{
					extraPos += pos.first;
				}
			}
			//坐标系
			for (auto it : _lableData_magnet->_arrow_Font_Axis_Magnet_Data)
			{
				for (auto pos : it.lableData.getParameter())
				{
					extraPos += pos.first;
				}
			}
			return extraPos;
		}

	private:
		/*
		* 修改所有显隐数据(第三个参数为实现重载)
		*/
		template<class T>
		void setAllShowState(QHash<QString, T> &data, const bool state, Font f)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().fontData.setLableShowState(state);
			}
		}
		template<class T>
		void setAllShowState(QHash<QString, T> &data, const bool state, Lable l)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableShowState(state);
			}
		}
		template<class T>
		void setAllShowState(QHash<QString, T> &data, const bool state, Lable_Font lf)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableShowState(state);
				it.value().fontData.setLableShowState(state);
			}
		}
		template<class T>
		void setAllShowState(QHash<QString, T> &data, const bool state, Lable_Lable ll)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableShowState(state);
				it.value().lableData2.setLableShowState(state);
			}
		}
		template<class T>
		void setAllShowState(QHash<QString, T> &data, const bool state, Lable_Lable_Lable_Font ll)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableShowState(state);
				it.value().lableData2.setLableShowState(state);
				it.value().lableData3.setLableShowState(state);
				it.value().fontData.setLableShowState(state);
			}
		}
		/*
		* 修改所有高亮数据(第三个参数为实现重载)
		*/
		template<class T>
		void setAllHighlightState(QHash<QString, T> &data, const bool state, Font f)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().fontData.setLableHighlightState(state);
			}
		}
		template<class T>
		void setAllHighlightState(QHash<QString, T> &data, const bool state, Lable l)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableHighlightState(state);
			}
		}
		template<class T>
		void setAllHighlightState(QHash<QString, T> &data, const bool state, Lable_Font lf)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableHighlightState(state);
				it.value().fontData.setLableHighlightState(state);

			}
		}
		template<class T>
		void setAllHighlightState(QHash<QString, T> &data, const bool state, Lable_Lable ll)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableHighlightState(state);
				it.value().lableData2.setLableHighlightState(state);

			}
		}
		template<class T>
		void setAllHighlightState(QHash<QString, T> &data, const bool state, Lable_Lable_Lable_Font lllf)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableHighlightState(state);
				it.value().lableData2.setLableHighlightState(state);
				it.value().lableData3.setLableHighlightState(state);
				it.value().fontData.setLableHighlightState(state);

			}
		}
		/*
		* 修改所有颜色数据(第三个参数为实现重载)
		*/
		template<class T>
		void setAllColor(QHash<QString, T> &data, const QVector3D color, Font f)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().fontData.setLableColor(color);
			}
		}
		template<class T>
		void setAllColor(QHash<QString, T> &data, const QVector3D color, Lable l)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableColor(color);
			}
		}
		template<class T>
		void setAllColor(QHash<QString, T> &data, const QVector3D color, Lable_Font lf)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableColor(color);
				it.value().fontData.setLableColor(color);
			}
		}
		template<class T>
		void setAllColor(QHash<QString, T> &data, const QVector3D color, Lable_Lable ll)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableColor(color);
				it.value().lableData2.setLableColor(color);
			}
		}
		template<class T>
		void setAllColor(QHash<QString, T> &data, const QVector3D color, Lable_Lable_Lable_Font lllf)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableColor(color);
				it.value().lableData2.setLableColor(color);
				it.value().lableData3.setLableColor(color);
				it.value().fontData.setLableColor(color);
			}
		}
		/*
		* 修改所有大小数据(第三个参数为实现重载)
		*/
		template<class T>
		void setAllSize(QHash<QString, T> &data, const float size, Font f)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().fontData.setLableSize(size);
			}
		}
		template<class T>
		void setAllSize(QHash<QString, T> &data, const float size, Lable l)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableSize(size);
			}
		}
		template<class T>
		void setAllSize(QHash<QString, T> &data, const float size, Lable_Lable ll)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableSize(size);
				it.value().lableData2.setLableSize(size);
			}
		}
		template<class T>
		void setAllSize(QHash<QString, T> &data, const float size, Lable_Lable_Lable lll)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				it.value().lableData.setLableSize(size);
				it.value().lableData2.setLableSize(size);
				it.value().lableData3.setLableSize(size);
			}
		}



	public:

		//隐式数据
		shared_ptr<mLableData_implicit> _lableData_implicit = nullptr;
		//显式数据
		shared_ptr<mLableData_explicit> _lableData_explicit = nullptr;
		//流体数据
		shared_ptr<mLableData_fluid> _lableData_fluid = nullptr;
		//电磁数据
		shared_ptr<mLableData_magnet> _lableData_magnet = nullptr;
	};
}
