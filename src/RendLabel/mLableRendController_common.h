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
		////////////////////////ȫ��ͨ�õ������ݲ���////////////////////////
		/*
		* ��ӵ�����Ⱦ(��1���̶����� pos��ΧΪ��-1,1��  ��2���̶�����)
		*/
		template<class T>
		void appendFontRendData(const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_txt.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
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
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();

			}
			_ID++;
		}

		/*
		* ��ӵ�����Ⱦ(�̶���״)����ת����,��ʱ�㣨��������,size������Ч��,��ʱ������Ⱦ���Ӵ�����������Ӹ�����);
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);

			//������ݿ�
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* ��ӵ�����Ⱦ(�̶���״+��������) ����ʱ�㣨�����֣�size������Ч��
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QString &txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//���������������
			appendFontData(t.fontData, showState, false, depthState, size, color, txt, pos);
			//������ݿ�
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* ��ӵ�����Ⱦ(��ͷ)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_dir.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos_dir);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//������ݿ�
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* ��ӵ�����Ⱦ(��ͷ+��������) ��ȫ������ϵ��  posΪ��Ļ���½�Ϊԭ�� ����Ϊy�������� ����Ϊx�������� ����������
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector2D pos = QVector2D(75,75), const float size = 0.8,
			const QVector3D color_x = QVector3D(255, 0, 0), const QVector3D color_y = QVector3D(0, 255, 0), const QVector3D color_z = QVector3D(0, 0, 255),
			const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//ͼ��
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
			//���������������	
			QVector<POS_TXT_DIR> ptd = { { {pos,0} ,{"X",QVector3D(1,0,0)} }, { {pos,0} ,{"Y",QVector3D(0,1,0)} } ,{ {pos,0} ,{"Z",QVector3D(0,0,1)} } };
			appendFontData(t.fontData, showState, false, depthState, size, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ*/
			//������ݿ�
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}
		
		/*
		* ɾ��������Ⱦ��ͨ�ã�
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
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<T>(name, Delete);
				emit updateView();
			}
			//ɾ�����ݿ�������		
			_lableRend->_lableDataController->deleteLableData<T>(name);

		}
		
		/*
		* ���õ�������״̬��ͨ�ã�  //T1Ϊ��������  T2Ϊ Font Lable  Lable_Lable Lable_Font  Lable_Lable_Lable_Font ��ֻ�޸�����  ͼ��  ���� ����+ͼ��
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
			//�������ݿ�������
			mode = (state == true ? Show : Hide);
			_lableRend->_lableDataController->updateLableData_ShowState<T1>(name, state, T2());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<T1>(name, mode);
				emit updateView();

			}
		}
		/*
		* ���õ�������״̬��ͨ�ã�  //T1Ϊ��������  T2Ϊ Font Lable Lable_Lable Lable_Font  ��ֻ�޸�����  ͼ��  ���� ����+ͼ��
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
			//�������ݿ�������		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<T1>(name, state, T2());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<T1>(name, mode);
				emit updateView();
			}
		}
	
		/*
		* ���õ�����ɫ��ͨ�ã� //T1Ϊ��������  T2Ϊ Font Lable Lable_Lable Lable_Font  ��ֻ�޸����֡�ͼ�� ������ ����+ͼ��
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

			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<T1>(name, SetColor);
				emit updateView();
			}
		}

		/*
		* ���õ�����С��ͨ�ã�   //T1Ϊ��������  T2Ϊ Font Lable Lable_Lable  ��ֻ�޸����֡�ͼ��  ��֧��ͬʱ�޸�
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

			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<T1>(name, SetSize);
				emit updateView();
			}
		}


		////////////////////////ͬһ�������ݲ���////////////////////////
		/*
		* ���ͬһ����������Ⱦ  ���ڶ��ļ������
		*/
		void appendLableRendData_SameType(const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			updateAllRend_SameType(type, Append);
		}
		/*
		* ɾ��ͬһ����������Ⱦ
		*/
		void deleteLableRendData_SameType(const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			//������Ⱦ
			updateAllRend_SameType(type, Delete);
			//ɾ�����ݿ�������	
			_lableRend->_lableDataController->deleteLableData_SameType(type);
		}
		/*
		* ����һ������������״̬
		*/
		void setLableRend_ShowState_SameType(const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_ShowState_SameType(type, state);
			//������Ⱦ
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
		* ����ͬһ�������и���״̬
		*/
		void setLableRend_Highlight_SameType(const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_HighlightState_SameType(type, state);
			//������Ⱦ
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
		* ����ͬһ����������ɫ
		*/
		void setLableRend_Color_SameType(const LableOperateType type, const QVector3D color)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_Color_SameType(type, color);
			//������Ⱦ		
			updateAllRend_SameType(type, SetColor);

		}
		/*
		* ����ͬһ�������д�С(Ŀǰֻ֧���޸�ͼ���С ��֧������)
		*/
		void setLableRend_Size_SameType(const LableOperateType type, const float size)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_Size_SameType<Lable>(type, size);  //ֻ����ͼ���С
			//������Ⱦ		
			updateAllRend_SameType(type, SetSize);

		}


		////////////////////////������Ⱦ����////////////////////////
		/*
		* ɾ��������Ⱦ  
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
		* �����������
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
			//��ʸ���������

			for (auto pos : para)
			{
				t.push_back({ pos,txt });
			}
			//�������
			fontData.setParameter(t);
		}

		/*
		* ���µ�����Ⱦ
		*/
		template<class T>
		void updateRend_Single(const QString &name, const LableOperateMode type)
		{
			if (_lableRend == nullptr)
			{
				return;
			}
			//��ת����
			if (is_same<T, MDataLable::X_Point_Common>::value)
			{
				_lableRend->_XPointRend_common->updateSingleRend(type, name);
			}
			//�̶�����
			else if (is_same<T, MDataLable::Fixed_Font_Common>::value)
			{
				_lableRend->_fontRend_fixed_common->updateSingleRend(type, name);
			}
			//ͨ������
			else if (is_same<T, MDataLable::Common_Font_Common>::value)
			{
				_lableRend->_fontRend_common_common->updateSingleRend(type, name);
			}
			//��ʱ��
			else if (is_same<T, MDataLable::TempPoint_Common>::value)
			{
				_lableRend->_tempPointRend_common->updateSingleRend(type, name);
				_lableRend->_tempPointFontRend_common->updateSingleRend(type, name);
			}
			//��ʱ����
			else if (is_same<T, MDataLable::TempMesh_Common>::value)
			{
				_lableRend->_tempMeshRend_common->updateSingleRend(type, name);
			}
			//ȫ������ϵ
			else if (is_same<T, MDataLable::Arrow_Font_Axis_Common>::value)
			{
				_lableRend->_globalAxisRend_common->updateSingleRend(type, name);
				_lableRend->_globalAxisFontRend_common->updateSingleRend(type, name);
			}
			//��ͷ
			else if (is_same<T, MDataLable::Arrow_Common>::value)
			{
				_lableRend->_arrowRend_common->updateSingleRend(type, name);
			}
			emit updateView();
		}
		/*
		* ����ͬһ����������Ⱦ
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
		//��Ⱦ����ID
		int _ID = 0;

	signals:
		void updateView();


	private:

		//ͨ����Ⱦ����
		shared_ptr<mLableRendIntegrate_common> _lableRend = nullptr;
	};
}
