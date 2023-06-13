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
		////////////////////////ȫ��ͨ�õ������ݲ���////////////////////////
		/*
		* ���ĳһ֡������Ⱦ(�̶�����  pos��ΧΪ��-1,1��)
		*/
		template<class T>
		void appendFontRendData(const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr) return;
			if (pos_txt.empty()) return;
			//�ж������Ƿ����
			if (_lableRend_post->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//����
			t.fontData.setLableID(_ID);
			t.fontData.setParameter(pos_txt);
			t.fontData.setLableShowState(showState);
			t.fontData.setLableHighlightState(false);
			t.fontData.setLableDepthState(depthState);
			t.fontData.setLableSize(size);
			t.fontData.setLableColor(color);
			//������ݿ�
			_lableRend_post->_lableDataController->appendLableData<T>(name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateOneRend_AllFrame_Global<T>(name, Append);
				emit updateView();

			}
			_ID++;
		}



		////////////////////////ĳһ֡�������ݲ���////////////////////////
		/*
		* ���ĳһ֡������Ⱦ(����ʸ����ͷ)   �Զ�����ɫ  size<=0 ��ʾʸ����С
		*/
		template<class T>
		void appendLableRendData(const int frameID, const QString &name, const QVector<POS_DIR_COLOR> &pdc, const float size = 0.0, const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr) return;
			if (pdc.empty()) return;
			//�ж������Ƿ����
			if (_lableRend_post->_lableDataController->isLableDataExist<T>(frameID, name))
			{
				deleteLableRendData<T>(frameID, name);
			}
			QVector<POS_DIR_SIZE_COLOR> pdsc;
			//�Զ����С
			if (size > 0)
			{
				for (auto v : pdc)
				{
					pdsc.push_back({ v.pos,v.dir,size,v.color });
				}
			}
			//����Ӧ��С
			else
			{
				//size��Χ�̶��� 0.5-2֮��
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
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pdsc);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);

			//������ݿ�
			_lableRend_post->_lableDataController->appendLableData<T>(frameID, name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateOneRend_OneFrame<T>(frameID, name, Append);
				emit updateView();
			}
			_ID++;
		}
	
		/*
		* ���ĳһ֡������Ⱦ(�̶�����  pos��ΧΪ��-1,1��)
		*/
		template<class T>
		void appendFontRendData(const int frameID, const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend_post == nullptr) return;
			if (pos_txt.empty()) return;
			//�ж������Ƿ����
			if (_lableRend_post->_lableDataController->isLableDataExist<T>(frameID,name))
			{
				deleteLableRendData<T>(frameID,name);
			}
			T t;
			//����
			t.fontData.setLableID(_ID);
			t.fontData.setParameter(pos_txt);
			t.fontData.setLableShowState(showState);
			t.fontData.setLableHighlightState(false);
			t.fontData.setLableDepthState(depthState);
			t.fontData.setLableSize(size);
			t.fontData.setLableColor(color);
			//������ݿ�
			_lableRend_post->_lableDataController->appendLableData<T>(frameID,name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateOneRend_OneFrame<T>(frameID,name, Append);
				emit updateView();

			}
			_ID++;
		}

		/*
		* ɾ��ĳһ֡������Ⱦ��ͨ�ã�
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
			//������Ⱦ
			if (isUpdate)
			{
				updateOneRend_OneFrame<T>(frameID, name, Delete);
				emit updateView();
			}
			//ɾ�����ݿ�������		
			_lableRend_post->_lableDataController->deleteLableData<T>(frameID, name);

		}

		///////////////////////ĳһ֡ĳһ�������ݲ���/////////////////////
		/*
		* ɾ��ĳһ֡ĳһ������Ⱦ���� ��ɾ�����ݿ������ݣ�
		*/
		void deleteLableRendData_OneFrame_SameType(const int frameID, const LableOperateType type)
		{
			if (_lableRend_post == nullptr) return;
			//ɾ����ǰ֡����Ⱦ
			updateAllRend_OneFrame_SameType(frameID, type, Delete);
			//��յ�ǰ֡�����ݿ�
			_lableRend_post->getLableDataController()->deleteAllLableData_OneFrame_SameType(frameID, type);
		}

	
		////////////////////////ĳһ֡�������ݲ���////////////////////////
		/*
		* ĳһ֡������Ⱦ����  (���)
		*/
		void appendLableRendData_OneFrame(const int frameID)
		{
			if (_lableRend_post == nullptr) return;			
			//���������֡����Ⱦ
			clearLableRendData_AllFrame();		
			//���µ�ǰ֡��������Ⱦ
			updateAllRend_OneFrame(frameID, Append);
		}	
		/*
		* ���ĳһ֡��Ⱦ���� ����ɾ�����ݿ������ݣ�
		*/
		void clearLableRendData_OneFrame(const int frameID)
		{
			if (_lableRend_post == nullptr) return;
			//��յ�ǰ֡��������Ⱦ
			updateAllRend_OneFrame(frameID, Delete);
		}
		/*
		* ɾ��ĳһ֡��Ⱦ���� ��ɾ�����ݿ������ݣ�
		*/
		void deleteLableRendData_OneFrame(const int frameID)
		{
			if (_lableRend_post == nullptr) return;
			//��յ�ǰ֡�����з�ȫ����Ⱦ
			clearLableRendData_OneFrame(frameID);
			//��յ�ǰ֡�����ݿ�
			_lableRend_post->getLableDataController()->deleteAllLableData_OneFrame(frameID);
		}


		////////////////////////����֡�������ݲ���////////////////////////
		/*
		* �������֡������Ⱦ����ɾ�����ݿ������ݣ�
		*/
		void clearLableRendData_AllFrame()
		{
			if (_lableRend_post == nullptr) return;
			//�������֡��Ⱦ����
			updateAllRend_AllFrame(Delete);
			
		}
		/*
		* ɾ������֡������Ⱦ��ɾ�����ݿ������ݣ�
		*/
		void deleteLableRendData_AllFrame()
		{
			if (_lableRend_post == nullptr) return;
			//ɾ������֡������Ⱦ
			updateAllRend_AllFrame(Delete);
			//ɾ�����ݿ�������֡������
			_lableRend_post->getLableDataController()->deleteAllLableData_AllFrame();
		}
	private:
		/*
		* �����������
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
			//��ʸ���������

			for (auto val : para)
			{
				QVector3D pos = val.first;
				QVector3D dir = val.second;
				t.push_back({ pos,{txt,dir} });
			}
			//�������
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
			//��ʸ���������

			for (auto pos : para)
			{
				t.push_back({ pos,txt });
			}
			//�������
			fontData.setParameter(t);
		}

		/*
		* ����ĳһ֡������Ⱦ
		*/
		template<class T>
		void updateOneRend_OneFrame(const int frameID, const QString &name, const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			//ʸ����ͷ
			if (is_same<T, MDataLable::VectorArrow_Post>::value)
			{
				_lableRend_post->_vecArrow_post->updateSingleRend_OneFrame(frameID, mode, name);
			}
			//�̶�����
			else if (is_same<T, MDataLable::Fixed_Font_Post>::value)
			{
				_lableRend_post->_fontRend_fixed_post->updateSingleRend_OneFrame(frameID, mode, name);
			}
			emit updateView();
		}
		/*
		* ����ĳһ֡ĳһ������Ⱦ
		*/
		void updateAllRend_OneFrame_SameType(const int frameID, const LableOperateType type, const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			//ʸ����ͷ
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
		* ����ĳһ֡������Ⱦ
		*/
		void updateAllRend_OneFrame(const int frameID, const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}
			updateAllRend_OneFrame_SameType(frameID,LableOperateType_VecArrow, mode);//ʸ����ͷ
			updateAllRend_OneFrame_SameType(frameID, LableOperateType_FixedFont_Post, mode);//�̶�����
			emit updateView();
		}
		
		/*
		* ��������֡������Ⱦ()
		*/
		void updateAllRend_AllFrame(const LableOperateMode mode)
		{
			if (_lableRend_post == nullptr)
			{
				return;
			}

			_lableRend_post->_vecArrow_post->updateAllRend(mode);//ʸ����ͷ
			_lableRend_post->_fontRend_fixed_post->updateAllRend(mode);//�̶�����
	
			emit updateView();
		}

	private:
		//��Ⱦ����ID
		int _ID = 0;

	signals:
		void updateView();


	private:
		//������Ⱦ����
		shared_ptr<mLableRendIntegrate_post> _lableRend_post = nullptr;
	};
}
