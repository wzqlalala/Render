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
#include "mBasicEnum.h"
//#include "mGeoModelData1.h"
//#include "mMeshModelData1.h"
#define PI 3.1415926
using namespace std;
using namespace MDataLable;
//using namespace MBasicFunction;
namespace MLableRend
{
	class RENDLABEL_EXPORT mLableRendController_post : public QObject
	{
		Q_OBJECT

	public:
		mLableRendController_post(shared_ptr<mLableRendIntegrate_post> lableRend_post)
		{
			_lableRend_post = lableRend_post;
		};
		~mLableRendController_post() {};
		////////////////////////全局通用单个数据操作////////////////////////
		/*
		* 添加某一帧单个渲染(固定文字  pos范围为（-1,1）)
		*/
		template<class T>
		void appendFontRendData(const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr) return;
			if (pos_txt.empty()) return;
			//判断数据是否存在
			if (_lableRend_post->_lableDataController->isLableDataExist<T>(name))
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
			_lableRend_post->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateOneRend_AllFrame_Global<T>(name, Append);
				emit updateView();

			}
			_ID++;
		}



		////////////////////////某一帧单个数据操作////////////////////////
		/*
		* 添加某一帧单个渲染(后处理矢量箭头)   自定义颜色  size<=0 表示矢量大小
		*/
		template<class T>
		void appendLableRendData(const int frameID, const QString &name, const QVector<POS_DIR_COLOR> &pdc, const float size = 0.0, const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr) return;
			if (pdc.empty()) return;
			//判断数据是否存在
			if (_lableRend_post->_lableDataController->isLableDataExist<T>(frameID, name))
			{
				deleteLableRendData<T>(frameID, name);
			}
			QVector<POS_DIR_SIZE_COLOR> pdsc;
			//自定义大小
			if (size > 0)
			{
				for (auto v : pdc)
				{
					pdsc.push_back({ v.pos,v.dir,size,v.color });
				}
			}
			//自适应大小
			else
			{
				//size范围固定在 0.5-2之间
				float min = pdc.front().dir.length();
				float max = min;
				for (auto v : pdc)
				{
					float cur = v.dir.length();
					if (cur < min)
					{
						min = cur;
					}
					else if (cur > max)
					{
						max = cur;
					}
				}
				for (auto v : pdc)
				{
					float prop = (v.dir.length() - min) / (max - min);
					float size = 0.5 + prop * 0.5;
					pdsc.push_back({ v.pos,v.dir,size,v.color });
				}
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pdsc);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);

			//添加数据库
			_lableRend_post->_lableDataController->appendLableData<T>(frameID, name, t);

			//渲染更新
			if (isUpdate)
			{
				updateOneRend_OneFrame<T>(frameID, name, Append);
				emit updateView();
			}
			_ID++;
		}
	
		/*
		* 添加某一帧单个渲染(固定文字  pos范围为（-1,1）)
		*/
		template<class T>
		void appendFontRendData(const int frameID, const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr) return;
			if (pos_txt.empty()) return;
			//判断数据是否存在
			if (_lableRend_post->_lableDataController->isLableDataExist<T>(frameID,name))
			{
				deleteLableRendData<T>(frameID,name);
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
			_lableRend_post->_lableDataController->appendLableData<T>(frameID,name, t);

			//渲染更新
			if (isUpdate)
			{
				updateOneRend_OneFrame<T>(frameID,name, Append);
				emit updateView();

			}
			_ID++;
		}

		/*
		* 删除某一帧单个渲染（通用）
		*/
		template<class T>
		void deleteLableRendData(const int frameID, const QString &name, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			if( !_lableRend_post->_lableDataController->isLableDataExist<T>(frameID,name))
			{
				return;
			}
			//更新渲染
			if (isUpdate)
			{
				updateOneRend_OneFrame<T>(frameID, name, Delete);
				emit updateView();
			}
			//删除数据库中数据		
			_lableRend_post->_lableDataController->deleteLableData<T>(frameID, name);

		}

		///////////////////////某一帧某一类型数据操作/////////////////////
		/*
		* 删除某一帧某一类型渲染操作 （删除数据库中数据）
		*/
		void deleteLableRendData_OneFrame_SameType(const int frameID, const LableOperateType type)
		{
			if (_lableRend_post == nullptr) return;
			//删除当前帧的渲染
			updateAllRend_OneFrame_SameType(frameID, type, Delete);
			//清空当前帧的数据库
			_lableRend_post->getLableDataController()->deleteAllLableData_OneFrame_SameType(frameID, type);
		}

	
		////////////////////////某一帧所有数据操作////////////////////////
		/*
		* 某一帧更新渲染操作  (添加)
		*/
		void appendLableRendData_OneFrame(const int frameID)
		{
			if (_lableRend_post == nullptr) return;			
			//先清空所有帧中渲染
			clearLableRendData_AllFrame();		
			//更新当前帧的所有渲染
			updateAllRend_OneFrame(frameID, Append);
		}	
		/*
		* 清空某一帧渲染操作 （不删除数据库中数据）
		*/
		void clearLableRendData_OneFrame(const int frameID)
		{
			if (_lableRend_post == nullptr) return;
			//清空当前帧的所有渲染
			updateAllRend_OneFrame(frameID, Delete);
		}
		/*
		* 删除某一帧渲染操作 （删除数据库中数据）
		*/
		void deleteLableRendData_OneFrame(const int frameID)
		{
			if (_lableRend_post == nullptr) return;
			//清空当前帧的所有非全局渲染
			clearLableRendData_OneFrame(frameID);
			//清空当前帧的数据库
			_lableRend_post->getLableDataController()->deleteAllLableData_OneFrame(frameID);
		}


		////////////////////////所有帧所有数据操作////////////////////////
		/*
		* 清空所有帧所有渲染（不删除数据库中数据）
		*/
		void clearLableRendData_AllFrame()
		{
			if (_lableRend_post == nullptr) return;
			//清空所有帧渲染数据
			updateAllRend_AllFrame(Delete);
			
		}
		/*
		* 删除所有帧所有渲染（删除数据库中数据）
		*/
		void deleteLableRendData_AllFrame()
		{
			if (_lableRend_post == nullptr) return;
			//删除所有帧所有渲染
			updateAllRend_AllFrame(Delete);
			//删除数据库中所有帧的数据
			_lableRend_post->getLableDataController()->deleteAllLableData_AllFrame();
		}
	private:
		/*
		* 添加文字数据
		*/
		void appendFontData(LableData1<QVector<POS_TXT_DIR>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QString& txt, const QVector<POS_DIR> &para)
		{
			if (_lableRend_post == nullptr) return;
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
			if (_lableRend_post == nullptr) return;
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
			if (_lableRend_post == nullptr) return;
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
		* 更新某一帧单个渲染
		*/
		template<class T>
		void updateOneRend_OneFrame(const int frameID, const QString &name, const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			//矢量箭头
			if (is_same<T, MDataLable::VectorArrow_Post>::value)
			{
				_lableRend_post->_vecArrow_post->updateSingleRend_OneFrame(frameID, mode, name);
			}
			//固定文字
			else if (is_same<T, MDataLable::Fixed_Font_Post>::value)
			{
				_lableRend_post->_fontRend_fixed_post->updateSingleRend_OneFrame(frameID, mode, name);
			}
			emit updateView();
		}
		/*
		* 更新某一帧某一类型渲染
		*/
		void updateAllRend_OneFrame_SameType(const int frameID, const LableOperateType type, const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			//矢量箭头
			switch (type)
			{
			case LableOperateType_VecArrow:	_lableRend_post->_vecArrow_post->updateAllRend_OneFrame(frameID, mode);
				break;
			case LableOperateType_FixedFont_Post:_lableRend_post->_fontRend_fixed_post->updateAllRend_OneFrame(frameID, mode);
				break;
			default:
				break;
			}
					
			emit updateView();
		}
		/*
		* 更新某一帧所有渲染
		*/
		void updateAllRend_OneFrame(const int frameID, const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			updateAllRend_OneFrame_SameType(frameID,LableOperateType_VecArrow, mode);//矢量箭头
			updateAllRend_OneFrame_SameType(frameID, LableOperateType_FixedFont_Post, mode);//固定文字
			emit updateView();
		}
		
		/*
		* 更新所有帧所有渲染()
		*/
		void updateAllRend_AllFrame(const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}

			_lableRend_post->_vecArrow_post->updateAllRend(mode);//矢量箭头
			_lableRend_post->_fontRend_fixed_post->updateAllRend(mode);//固定文字
	
			emit updateView();
		}

	private:
		//渲染对象ID
		int _ID = 0;

	signals:
		void updateView();


	private:
		//后处理渲染对象
		shared_ptr<mLableRendIntegrate_post> _lableRend_post = nullptr;
	};
}
