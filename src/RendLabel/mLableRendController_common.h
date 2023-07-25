#pragma once
#include <QObject>
#include <memory>
#include "mLableRendEnum.h"
#include "rendlabel_global.h"
#include "mLableDataStruct.h"
#include "mLableDataController_post.h"
#include "mLableDataController_common.h"
#include "mLableRendIntegrate_post.h"
#include "mLableRendIntegrate_common.h"
#include "mViewToolClass.h"
#include "mLableRendBase.h"
#include "mLableRendFont.h"

//#include "mGeoModelData1.h"
//#include "mMeshModelData1.h"
#define PI 3.1415926
using namespace std;
using namespace MDataLable;
//using namespace MViewBasic;
namespace MLableRend
{
	class RENDLABEL_EXPORT mLableRendController_common : public QObject
	{
		Q_OBJECT

	public:
		mLableRendController_common(shared_ptr<mLableRendIntegrate_common> lableRend_common)
		{
			_lableRend = lableRend_common;
		};
		~mLableRendController_common() {};
		////////////////////////全局通用单个数据操作////////////////////////
		/*
		* 添加单个渲染(（1）固定文字 pos范围为（-1,1）  （2）固定文字)
		*/
		template<class T>
		void appendFontRendData(const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_txt.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//文字
			t.fontData.setLableID(_ID);
			t.fontData.setParameter(pos_txt);
			t.fontData.setLableShowState(showState);
			t.fontData.setLableHighlightState(false);
			t.fontData.setLableDepthState(depthState);
			t.fontData.setLableSize(size);
			t.fontData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();

			}
			_ID++;
		}

		/*
		* 添加单个渲染(固定形状)（旋转中心,临时点（不带文字,size功能无效）,临时网格渲染（接触、面面绑定连接高亮）);
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);

			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(固定形状+文字类型) （临时点（带文字，size功能无效）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QString &txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加文字数据文字
			appendFontData(t.fontData, showState, false, depthState, size, color, txt, pos);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(箭头)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_dir.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos_dir);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(箭头+文字类型) （全局坐标系）  pos为屏幕左下角为原点 向上为y轴正方形 向右为x轴正方形 的像素坐标
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector2D pos = QVector2D(75,75), const float size = 0.8,
			const QVector3D color_x = QVector3D(255, 0, 0), const QVector3D color_y = QVector3D(0, 255, 0), const QVector3D color_z = QVector3D(0, 0, 255),
			const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter({ { {pos,0} ,QVector3D(1,0,0) } });
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color_x);

			t.lableData2.setLableID(_ID);
			t.lableData2.setParameter({ { {pos,0}  ,QVector3D(0,1,0) } });
			t.lableData2.setLableShowState(showState);
			t.lableData2.setLableHighlightState(false);
			t.lableData2.setLableDepthState(depthState);
			t.lableData2.setLableSize(size);
			t.lableData2.setLableColor(color_y);

			t.lableData3.setLableID(_ID);
			t.lableData3.setParameter({ { {pos,0}  ,QVector3D(0,0,1) } });
			t.lableData3.setLableShowState(showState);
			t.lableData3.setLableHighlightState(false);
			t.lableData3.setLableDepthState(depthState);
			t.lableData3.setLableSize(size);
			t.lableData3.setLableColor(color_z);
			//添加文字数据文字	
			QVector<POS_TXT_DIR> ptd = { { {pos,0} ,{"X",QVector3D(1,0,0)} }, { {pos,0} ,{"Y",QVector3D(0,1,0)} } ,{ {pos,0} ,{"Z",QVector3D(0,0,1)} } };
			appendFontData(t.fontData, showState, false, depthState, size, QVector3D(255, 255, 255), ptd); //文字默认为白色*/
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}
		
		/*
		* 删除单个渲染（通用）
		*/
		template<class T>
		void deleteLableRendData(const QString &name, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T> &data = _lableRend->_lableDataController->getLableData<T>();
			if (data.find(name) == data.end())
			{
				return;
			}
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T>(name, Delete);
				emit updateView();
			}
			//删除数据库中数据		
			_lableRend->_lableDataController->deleteLableData<T>(name);

		}
		
		/*
		* 设置单个显隐状态（通用）  //T1为数据类型  T2为 Font Lable  Lable_Lable Lable_Font  Lable_Lable_Lable_Font 看只修改文字  图标  或者 文字+图标
		*/
		template<class T1, class T2>
		void setLableRendShowState(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			LableOperateMode mode;
			//更新数据库中数据
			mode = (state == true ? Show : Hide);
			_lableRend->_lableDataController->updateLableData_ShowState<T1>(name, state, T2());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, mode);
				emit updateView();

			}
		}
		/*
		* 设置单个高亮状态（通用）  //T1为数据类型  T2为 Font Lable Lable_Lable Lable_Font  看只修改文字  图标  或者 文字+图标
		*/
		template<class T1, class T2>
		void setLableRendHightlightState(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			LableOperateMode mode;
			//更新数据库中数据		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<T1>(name, state, T2());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, mode);
				emit updateView();
			}
		}
	
		/*
		* 设置单个颜色（通用） //T1为数据类型  T2为 Font Lable Lable_Lable Lable_Font  看只修改文字、图标 、或者 文字+图标
		*/
		template<class T1, class T2>
		void setLableRendColor(const QString &name, const QVector3D color, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			_lableRend->_lableDataController->updateLableData_Color<T1>(name, color, T2());

			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, SetColor);
				emit updateView();
			}
		}

		/*
		* 设置单个大小（通用）   //T1为数据类型  T2为 Font Lable Lable_Lable  看只修改文字、图标  不支持同时修改
		*/
		template<class T1, class T2>
		void setLableRendSize(const QString &name, const float size, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			_lableRend->_lableDataController->updateLableData_Size<T1>(name, size, T2());

			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, SetSize);
				emit updateView();
			}
		}


		////////////////////////同一类型数据操作////////////////////////
		/*
		* 添加同一类型所有渲染  用于读文件后添加
		*/
		void appendLableRendData_SameType(const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			updateAllRend_SameType(type, Append);
		}
		/*
		* 删除同一类型所有渲染
		*/
		void deleteLableRendData_SameType(const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			//更新渲染
			updateAllRend_SameType(type, Delete);
			//删除数据库中数据	
			_lableRend->_lableDataController->deleteLableData_SameType(type);
		}
		/*
		* 设置一类型所有显隐状态
		*/
		void setLableRend_ShowState_SameType(const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_ShowState_SameType(type, state);
			//更新渲染
			if (state)
			{
				updateAllRend_SameType(type, Show);
			}
			else
			{
				updateAllRend_SameType(type, Hide);
			}
		}
		/*
		* 设置同一类型所有高亮状态
		*/
		void setLableRend_Highlight_SameType(const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_HighlightState_SameType(type, state);
			//更新渲染
			if (state)
			{
				updateAllRend_SameType(type, Highlight);
			}
			else
			{
				updateAllRend_SameType(type, NoHighlight);
			}
		}

		/*
		* 设置同一类型所有颜色
		*/
		void setLableRend_Color_SameType(const LableOperateType type, const QVector3D color)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_Color_SameType(type, color);
			//更新渲染		
			updateAllRend_SameType(type, SetColor);

		}
		/*
		* 设置同一类型所有大小(目前只支持修改图标大小 不支持文字)
		*/
		void setLableRend_Size_SameType(const LableOperateType type, const float size)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_Size_SameType<Lable>(type, size);  //只更新图标大小
			//更新渲染		
			updateAllRend_SameType(type, SetSize);

		}


		////////////////////////所有渲染操作////////////////////////
		/*
		* 删除所有渲染  
		*/
		void deleteAllLableRendData()
		{
			deleteLableRendData_SameType(LableOperateType_TempPoint);
			deleteLableRendData_SameType(LableOperateType_TempMesh);
			deleteLableRendData_SameType(LableOperateType_XPoint);
			deleteLableRendData_SameType(LableOperateType_FixedFont);
			deleteLableRendData_SameType(LableOperateType_CommonFont);
			deleteLableRendData_SameType(LableOperateType_GlobalAxis);
			deleteLableRendData_SameType(LableOperateType_Arrow);
		}

	private:
		/*
		* 添加文字数据
		*/
		void appendFontData(LableData1<QVector<POS_TXT_DIR>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QString& txt, const QVector<POS_DIR> &para)
		{
			fontData.setLableID(_ID);
			fontData.setLableShowState(showState);
			fontData.setLableHighlightState(highlightState);
			fontData.setLableDepthState(depthState);
			fontData.setLableSize(size);
			fontData.setLableColor(color);
			QVector<POS_TXT_DIR> t;
			//带矢量方向参数

			for (auto val : para)
			{
				QVector3D pos = val.first;
				QVector3D dir = val.second;
				t.push_back({ pos,{txt,dir} });
			}
			//添加数据
			fontData.setParameter(t);
		}
		void appendFontData(LableData1<QVector<POS_TXT_DIR>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QVector<POS_TXT_DIR> &para)
		{
			fontData.setLableID(_ID);
			fontData.setLableShowState(showState);
			fontData.setLableHighlightState(highlightState);
			fontData.setLableDepthState(depthState);
			fontData.setLableSize(size);
			fontData.setLableColor(color);
			fontData.setParameter(para);
		}
		void appendFontData(LableData1<QVector<POS_TXT>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QString& txt, const QVector<QVector3D> &para)
		{
			fontData.setLableID(_ID);
			fontData.setLableShowState(showState);
			fontData.setLableHighlightState(highlightState);
			fontData.setLableDepthState(depthState);
			fontData.setLableSize(size);
			fontData.setLableColor(color);
			QVector<POS_TXT> t;
			//带矢量方向参数

			for (auto pos : para)
			{
				t.push_back({ pos,txt });
			}
			//添加数据
			fontData.setParameter(t);
		}

		/*
		* 更新单个渲染
		*/
		template<class T>
		void updateRend_Single(const QString &name, const LableOperateMode type)
		{
			if (_lableRend == nullptr)
			{
				return;
			}
			//旋转中心
			if (is_same<T, MDataLable::X_Point_Common>::value)
			{
				_lableRend->_XPointRend_common->updateSingleRend(type, name);
			}
			//固定文字
			else if (is_same<T, MDataLable::Fixed_Font_Common>::value)
			{
				_lableRend->_fontRend_fixed_common->updateSingleRend(type, name);
			}
			//通用文字
			else if (is_same<T, MDataLable::Common_Font_Common>::value)
			{
				_lableRend->_fontRend_common_common->updateSingleRend(type, name);
			}
			//临时点
			else if (is_same<T, MDataLable::TempPoint_Common>::value)
			{
				_lableRend->_tempPointRend_common->updateSingleRend(type, name);
				_lableRend->_tempPointFontRend_common->updateSingleRend(type, name);
			}
			//临时网格
			else if (is_same<T, MDataLable::TempMesh_Common>::value)
			{
				_lableRend->_tempMeshRend_common->updateSingleRend(type, name);
			}
			//全局坐标系
			else if (is_same<T, MDataLable::Arrow_Font_Axis_Common>::value)
			{
				_lableRend->_globalAxisRend_common->updateSingleRend(type, name);
				_lableRend->_globalAxisFontRend_common->updateSingleRend(type, name);
			}
			//箭头
			else if (is_same<T, MDataLable::Arrow_Common>::value)
			{
				_lableRend->_arrowRend_common->updateSingleRend(type, name);
			}
			emit updateView();
		}
		/*
		* 更新同一类型所有渲染
		*/
		void updateAllRend_SameType(const LableOperateType type, const LableOperateMode mode)
		{
			if (_lableRend == nullptr)
			{
				return;
			}
			switch (type)
			{
			case LableOperateType_FixedFont:
				_lableRend->_fontRend_fixed_common->updateAllRend(mode);
				break;
			case LableOperateType_CommonFont:
				_lableRend->_fontRend_common_common->updateAllRend(mode);
				break;
			case LableOperateType_XPoint:
				_lableRend->_XPointRend_common->updateAllRend(mode);
				break;
			case LableOperateType_TempPoint:
				_lableRend->_tempPointRend_common->updateAllRend(mode);
				_lableRend->_tempPointFontRend_common->updateAllRend(mode);
				break;
			case LableOperateType_TempMesh:
				_lableRend->_tempMeshRend_common->updateAllRend(mode);
				break;
			case LableOperateType_GlobalAxis:
				_lableRend->_globalAxisRend_common->updateAllRend(mode);
				_lableRend->_globalAxisFontRend_common->updateAllRend(mode);
				break;
			case LableOperateType_Arrow:
				_lableRend->_arrowRend_common->updateAllRend(mode);
				break;
			default:
				break;
			}		
			emit updateView();
		}
	private:
		//渲染对象ID
		int _ID = 0;

	signals:
		void updateView();


	private:

		//通用渲染对象
		shared_ptr<mLableRendIntegrate_common> _lableRend = nullptr;
	};
}
