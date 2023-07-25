#pragma once
#include <QObject>
#include<QHash>
#include <memory>
#include"mLableData.h"
#include<type_traits>

#include"rendlabeldata_global.h"
#include"mLableDataEnum.h"

using namespace std;
//using namespace MViewBasic;
namespace MDataLable
{
	class RENDLABELDATA_EXPORT mLableDataController_common : public QObject
	{
		Q_OBJECT

	public:
		mLableDataController_common()
		{
			_lableData_common = make_shared<mLableData_common>();

		}
		~mLableDataController_common() {};

		/*
		* 获取数据库
		*/
		shared_ptr<mLableData_common> getLableDataBase()
		{
			return _lableData_common;
		};
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

		////////////////////////同一类型数据操作////////////////////////
		/*
		* 删除同一类型所有数据
		*/
		void deleteLableData_SameType(const LableOperateType type)
		{
			if (type == LableOperateType_XPoint)
			{
				_lableData_common->_XPoint_Common_Data.clear();
			}
			else if (type == LableOperateType_FixedFont)
			{
				_lableData_common->_font_Fixed_Common_Data.clear();
			}
			else if (type == LableOperateType_CommonFont)
			{
				_lableData_common->_font_Common_Common_Data.clear();
			}
			else if (type == LableOperateType_TempPoint)
			{
				_lableData_common->_tempPoint_Common_Data.clear();
			}
			else if (type == LableOperateType_TempMesh)
			{
				_lableData_common->_tempMesh_Common_Data.clear();
			}
			else if (type == LableOperateType_GlobalAxis)
			{
				_lableData_common->_globalAxis_Common_Data.clear();
			}
			else if (type == LableOperateType_Arrow)
			{
				_lableData_common->_arrow_Common_Data.clear();
			}
		}

		////////////////////////同一类型数据操作////////////////////////
	
		/*
		* 修改同一类型所有显隐数据
		*/
		void updateLableData_ShowState_SameType(const LableOperateType type, const bool state)
		{
			if (type == LableOperateType_XPoint)
			{
				setAllShowState<X_Point_Common>(_lableData_common->_XPoint_Common_Data, state, Lable());
			}
			else if (type == LableOperateType_FixedFont)
			{
				setAllShowState<Fixed_Font_Common>(_lableData_common->_font_Fixed_Common_Data, state, Font());
			}
			else if (type == LableOperateType_CommonFont)
			{
				setAllShowState<Common_Font_Common>(_lableData_common->_font_Common_Common_Data, state, Font());
			}
			else if (type == LableOperateType_TempPoint)
			{
				setAllShowState<TempPoint_Common>(_lableData_common->_tempPoint_Common_Data, state, Lable_Font());
			}
			else if (type == LableOperateType_TempMesh)
			{
				setAllShowState<TempMesh_Common>(_lableData_common->_tempMesh_Common_Data, state, Lable());
			}
			else if (type == LableOperateType_Arrow)
			{
				setAllShowState<Arrow_Common>(_lableData_common->_arrow_Common_Data, state, Lable());
			}
		}

		/*
		* 修改同一类型所有高亮数据
		*/
		void updateLableData_HighlightState_SameType(const LableOperateType type, const bool state)
		{
			if (type == LableOperateType_XPoint)
			{
				setAllHighlightState<X_Point_Common>(_lableData_common->_XPoint_Common_Data, state, Lable());
			}
			else if (type == LableOperateType_FixedFont)
			{
				setAllHighlightState<Fixed_Font_Common>(_lableData_common->_font_Fixed_Common_Data, state, Font());
			}
			else if (type == LableOperateType_CommonFont)
			{
				setAllHighlightState<Common_Font_Common>(_lableData_common->_font_Common_Common_Data, state, Font());
			}
			else if (type == LableOperateType_TempPoint)
			{
				setAllHighlightState<TempPoint_Common>(_lableData_common->_tempPoint_Common_Data, state, Lable_Font());
			}
			else if (type == LableOperateType_TempMesh)
			{
				setAllHighlightState<TempMesh_Common>(_lableData_common->_tempMesh_Common_Data, state, Lable());
			}
			else if (type == LableOperateType_Arrow)
			{
				setAllHighlightState<Arrow_Common>(_lableData_common->_arrow_Common_Data, state, Lable());
			}
		}

		/*
		* 修改同一类型所有颜色数据
		*/
		void updateLableData_Color_SameType(const LableOperateType type, const QVector3D color)
		{
			if (type == LableOperateType_XPoint)
			{
				setAllColor<X_Point_Common>(_lableData_common->_XPoint_Common_Data, color, Lable());
			}
			else if (type == LableOperateType_FixedFont)
			{
				setAllColor<Fixed_Font_Common>(_lableData_common->_font_Fixed_Common_Data, color, Font());
			}
			else if (type == LableOperateType_CommonFont)
			{
				setAllColor<Common_Font_Common>(_lableData_common->_font_Common_Common_Data, color, Font());
			}
			else if (type == LableOperateType_TempPoint)
			{
				setAllColor<TempPoint_Common>(_lableData_common->_tempPoint_Common_Data, color, Lable_Font());
			}
			else if (type == LableOperateType_TempMesh)
			{
				setAllColor<TempMesh_Common>(_lableData_common->_tempMesh_Common_Data, color, Lable());
			}
			else if (type == LableOperateType_Arrow)
			{
				setAllColor<Arrow_Common>(_lableData_common->_arrow_Common_Data, color, Lable());
			}
		}

		/*
		* 修改同一类型所有大小数据  T为 Font 、 Lable、Lable_Lable 、Lable_Lable_Lable 不支持文字图标一起修改
		*/
		template<class T>
		void updateLableData_Size_SameType(const LableOperateType type, const float size)
		{
			if (type == LableOperateType_XPoint)
			{
				setAllSize<X_Point_Common>(_lableData_common->_XPoint_Common_Data, size, T());
			}
			else if (type == LableOperateType_CommonFont)
			{
				setAllSize<Common_Font_Common>(_lableData_common->_font_Common_Common_Data, size, Font());
			}
			else if (type == LableOperateType_FixedFont)
			{
				setAllSize<Fixed_Font_Common>(_lableData_common->_font_Fixed_Common_Data, size, Font());
			}
			else if (type == LableOperateType_TempPoint)
			{
				setAllSize<TempPoint_Common>(_lableData_common->_tempPoint_Common_Data, size, T());
			}
			else if (type == LableOperateType_TempMesh)
			{
				setAllSize<TempMesh_Common>(_lableData_common->_tempMesh_Common_Data, size, T());
			}
			else if (type == LableOperateType_Arrow)
			{
				setAllSize<Arrow_Common>(_lableData_common->_arrow_Common_Data, size, T());
			}


		}

		/*
		* 删除所有数据
		*/
		void deleteAllLableData()
		{			
			_lableData_common->_font_Fixed_Common_Data.clear();
			_lableData_common->_font_Common_Common_Data.clear();
			_lableData_common->_tempPoint_Common_Data.clear();
			_lableData_common->_tempMesh_Common_Data.clear();
			_lableData_common->_XPoint_Common_Data.clear();	
			_lableData_common->_globalAxis_Common_Data.clear();
			_lableData_common->_arrow_Common_Data.clear();
		}
		/*
		* 获取额外的顶点数据（用于更新视角）
		*/
		QVector<QVector3D> getExtraVertex()
		{
			QVector<QVector3D> extraPos;
			//临时点
			for (auto it = _lableData_common->_tempPoint_Common_Data.begin(); it != _lableData_common->_tempPoint_Common_Data.end(); ++it)
			{
				extraPos += it.value().lableData.getParameter();
			}
			return extraPos;
		}
	private:
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
		//通用数据
		shared_ptr<mLableData_common> _lableData_common = nullptr;

	};
}
