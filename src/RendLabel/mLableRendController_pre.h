#pragma once
#include <QObject>
#include <memory>
#include "mLableRendEnum.h"
#include "rendlabel_global.h"
#include "mLableDataStruct.h"
#include "mLableDataController_pre.h"
#include "mLableRendIntegrate_pre.h"
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
	class RENDLABEL_EXPORT mLableRendController_pre : public QObject
	{
		Q_OBJECT

	public:
		mLableRendController_pre(shared_ptr<mLableRendIntegrateBase> lableRend) 
		{
			_lableRend = dynamic_pointer_cast<mLableRendIntegrate_pre>(lableRend);
		};
		~mLableRendController_pre() {};

		friend class mLableRendController_pre;

		////////////////////////�������ݲ���////////////////////////

		/*
		* ���������Ⱦ(�̶�����  pos��ΧΪ��-1,1��)
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
		* ��ӵ�����Ⱦ(��ͷ+��������) ���ֲ�����ϵ��
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector3D point_origin, const QVector3D vec_x, const QVector3D vec_xy, const  LocalAxisType type, const float size = 0.8,
			const QVector3D color_x = QVector3D(255, 0, 0), const QVector3D color_y = QVector3D(0, 255, 0), const QVector3D color_z = QVector3D(0, 0, 255),
			const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			vec_x.normalized();
			QVector3D vec_z = QVector3D::crossProduct(vec_x, vec_xy).normalized();//Z������
			QVector3D vec_y = QVector3D::crossProduct(vec_z, vec_x).normalized();//Y������

			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter({ { point_origin ,vec_x } });
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color_x);

			t.lableData2.setLableID(_ID);
			t.lableData2.setParameter({ { point_origin ,vec_y } });
			t.lableData2.setLableShowState(showState);
			t.lableData2.setLableHighlightState(false);
			t.lableData2.setLableDepthState(depthState);
			t.lableData2.setLableSize(size);
			t.lableData2.setLableColor(color_y);

			t.lableData3.setLableID(_ID);
			t.lableData3.setParameter({ { point_origin ,vec_z } });
			t.lableData3.setLableShowState(showState);
			t.lableData3.setLableHighlightState(false);
			t.lableData3.setLableDepthState(depthState);
			t.lableData3.setLableSize(size);
			t.lableData3.setLableColor(color_z);

			//���������������
			QString x, y, z;
			switch (type)
			{
			case LocalAxisType_Rectangular:x = "X"; y = "Y"; z = "Z";
				break;
			case LocalAxisType_Cylinder:x = "R"; y = "T"; z = "Z";
				break;
			case LocalAxisType_Sphere:x = "R"; y = "T"; z = "P";
				break;
			default:
				break;
			}
			QVector<POS_TXT_DIR> ptd = { { point_origin ,{x,vec_x} }, { point_origin ,{y,vec_y} } ,{ point_origin ,{z,vec_z} } };
			appendFontData(t.fontData, showState, false, depthState, size, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ
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
		* ��ӵ�����Ⱦ(��ͷ+�������� ����ֻ��һ��) �������������������ء����غɡ�ѹǿ��Ԥ������������ �����غɡ����غɣ� 
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const QString &txt, const float size = 1.0,
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
			//���������������
			appendFontData(t.fontData, showState, false, depthState, size, color, txt, pos_dir);
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
		* ��ӵ�����Ⱦ(��ͷ+�������� �����ж���) ����ʼ�ٶȣ�
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_TXT_DIR> &pos_txt_dir,const float size = 1.0,const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, 
			const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_txt_dir.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			QVector<POS_DIR> pos_dir;
			for (auto v : pos_txt_dir)
			{
				POS_DIR pd;
				pd.first = v.first;
				pd.second = v.second.second;
				pos_dir.push_back(pd);
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
			//���������������
			appendFontData(t.fontData, showState, false, depthState, size, color, pos_txt_dir);
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
		* ��ӵ�����Ⱦ(�̶���״)����ʼ�¶ȣ��¶ȱ߽磬����, Բ��¸�,);
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
		* ��ӵ�����Ⱦ(�̶���״+��������) �������ܶȣ�����ܶȡ��������ȣ��ȷ��䡢�ڲ���Դ���¶��غɡ���ɡ��ڵ�/����/�� ���   �ڵ�/���� �������������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ�ȣ�
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
		* ��ӵ�����Ⱦ(Լ�����ɶ����� ��ͷ���) ��λ��ת�ǡ� �̶�/�Գ�/���Գ� ����ʼ�ٶ�  ����ʼλ�� ������Լ��������Լ���������˶���
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QMap<unsigned int, float> &value, const float size = 1.0,
			const QVector3D color = QVector3D(0, 0, 255), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty() || value.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter1(pos);
			t.lableData.setParameter2(value);
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
		* ��ӵ�����Ⱦ(ֱ��+����)  �����ԡ����ԡ�mpc���ӣ�1��1  1�Զ�
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector3D &pos, const QVector<QVector3D> &pos1, const QString &txt, const QVector3D color = QVector3D(255, 255, 0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos1.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			//���϶�������
			QVector3D fontPos = pos; //����λ��
			QVector<QVector3D> vertex{};
			for (auto p : pos1)
			{
				vertex.push_back(pos);
				vertex.push_back(p);
				fontPos += p;
			}
			fontPos /= (pos1.size() + 1);

			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(vertex);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //����ͼ��û��size��С
			t.lableData.setLableColor(color);

			//���������������
			appendFontData(t.fontData, showState, false, depthState, 1.0, color, txt, { fontPos });
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
		* ��ӵ�����Ⱦ(ֱ��+����)  ���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ���������-�����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ�
									//�ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�նȣ� 1��1
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QPair<QVector3D,QVector3D>> &pos, const QString &txt, const QVector3D color = QVector3D(255, 255, 0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			//���϶�������
			QVector<QVector3D> fontPos ; //����λ��
			QVector<QVector3D> vertex;
			for (auto p : pos)
			{
				vertex.push_back(p.first);
				vertex.push_back(p.second);
				fontPos.push_back((p.first+ p.second)/2.0);
			}
			T t;
			//ͼ��
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(vertex);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //����ͼ��û��size��С
			t.lableData.setLableColor(color);

			//���������������
			appendFontData(t.fontData, showState, false, depthState, 1.0, color, txt, fontPos);
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
		* ��ӵ�����Ⱦ(ֱ��)  ������ ���ᣩ
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector3D &pos1, const QVector3D &pos2, const QVector3D color = QVector3D(255, 255, 0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
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
			t.lableData.setParameter({ pos1 ,pos2 });
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //����ͼ��û��size��С
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
		* ��ӵ�����Ⱦ(������)  //�������е������ ���Ӵ���
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos1, const QVector<QVector3D> &pos2, const QVector3D color1 = QVector3D(255, 255, 0),
			const QVector3D color2 = QVector3D(160, 32, 240), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos1.empty() || pos2.empty()) return;
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}

			T t;
			//ͼ��
			//����
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos1);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //����ͼ��û��size��С
			t.lableData.setLableColor(color1);

			//����
			t.lableData2.setLableID(_ID);
			t.lableData2.setParameter(pos2);
			t.lableData2.setLableShowState(showState);
			t.lableData2.setLableHighlightState(false);
			t.lableData2.setLableDepthState(depthState);
			t.lableData2.setLableSize(1.0); //����ͼ��û��size��С
			t.lableData2.setLableColor(color2);

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
		* ��Ӹ�������(��ת����ƽ�渱��Բ���θ�)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const FixShapeRotateType type, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = false, const bool isUpdate = true)//����ռλ����
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
			t.lableData.setParameter1(pos_dir);
			t.lableData.setParameter2(type);
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
		* ��Ӹ�������(ƽ��)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QVector3D vec1, const QVector3D vec2, const FixShapeRotateType type,
			const float size = 1.0, const QVector3D color = QVector3D(0, 255, 0), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
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
			t.lableData.setParameter1(pos);
			t.lableData.setParameter2({ vec1 ,vec2 });
			t.lableData.setParameter3(type);
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
		* ��Ӹ���ǽ��Ⱦ
		*/
		//���޴�ƽ�����
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, 
			QVector3D minEdge = QVector3D(0,0,0), QVector3D maxEdge = QVector3D(0,0,0),
			const QVector3D color = QVector3D(188.0, 188.0, 188.0),const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name) && _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			//ģ�ͼ�����������
			//��������
			//ģ������
			QVector3D ModelCenterPos = (minEdge + maxEdge / 2);
			//��ȡ��ģ�ͼ�������Ϊ���ĵ������ת�뾶
			float max_x = std::max(std::abs(minEdge.x() - ModelCenterPos.x()), std::abs(maxEdge.x() - ModelCenterPos.x()));
			float max_y = std::max(std::abs(minEdge.y() - ModelCenterPos.y()), std::abs(maxEdge.y() - ModelCenterPos.y()));
			float max_z = std::max(std::abs(minEdge.z() - ModelCenterPos.z()), std::abs(maxEdge.z() - ModelCenterPos.z()));
			float MaxRadius = sqrt(pow(max_x, 2) + pow(max_y, 2) + pow(max_z, 2));
			//ģ�ͼ������ĵ�ƽ����̾���
			float distance = ModelCenterPos.distanceToPlane(pos, normal);

			//ģ�ͼ������ĵ�ƽ���ӳ��㣬Ҳ����ƽ������ĵ����꣨�����������ϣ�
			QVector3D PlanCenterPos;

			if (std::abs((ModelCenterPos + normal * distance).distanceToPlane(pos, normal)) < 0.1)
			{
				PlanCenterPos = ModelCenterPos + normal * distance;
			}
			else if (std::abs((ModelCenterPos - normal * distance).distanceToPlane(pos, normal)) < 0.1)
			{
				PlanCenterPos = ModelCenterPos - normal * distance;
			}
			else
			{
				return;
			}
			//�������޴�ƽ��ĳ��Ϳ� ��1.2��ģ�����ߴ磩
			float Size = MaxRadius == 0 ? 10 : 2.2 * MaxRadius;

			//��֤ƽ���һ������X��Zƽ����
			QVector3D L_vec, W_vec;
			if (normal == QVector3D(0, 1, 0))
			{
				L_vec = QVector3D(1, 0, 0);
				W_vec = QVector3D(0, 0, 1);
			}
			else
			{
				L_vec = QVector3D::crossProduct(normal, QVector3D(0, 1, 0)).normalized(); //ȷ�����ȷ��������
				W_vec = QVector3D::crossProduct(normal, L_vec).normalized();//ȷ����ȷ��������
			}
			//��ȡƽ����ĸ���������
			QVector3D PlanPointPos_1 = PlanCenterPos + Size / 2 * (-L_vec - W_vec);
			QVector3D PlanPointPos_2 = PlanCenterPos + Size / 2 * (-L_vec + W_vec);
			QVector3D PlanPointPos_3 = PlanCenterPos + Size / 2 * (L_vec + W_vec);
			QVector3D PlanPointPos_4 = PlanCenterPos + Size / 2 * (L_vec - W_vec);
			//������
			QVector<QVector3D> PlanPointPos = { PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4 };


			//��
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //����ͼ��û��size��С
			f.lableData.setLableColor(color);

			//����ϵ
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,normal } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(0, 0, 255));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,normal } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 0, 255));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//����ϵ����
			QVector<POS_TXT_DIR> ptd = { { pos ,{"N",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ

			//������ݿ�
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}


			_ID++;
		}
		//���޴�ƽ�����ǽ
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, QVector3D L_vec, const float L, const float W, const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name) 
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			normal = normal.normalized();
			L_vec = L_vec.normalized();
			QVector3D W_vec = QVector3D::crossProduct(normal, L_vec);
			//��һ������ ��ʼ����
			QVector3D PlanPointPos_1 = pos;
			//�ڶ������� 
			QVector3D PlanPointPos_2 = pos + L_vec * L;
			//����������
			QVector3D PlanPointPos_3 = pos + L_vec * L + W_vec * W;
			//���ĸ�����
			QVector3D PlanPointPos_4 = pos + W_vec * W;
			//������
			QVector<QVector3D> LinePointPos = { PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_2,PlanPointPos_3 ,PlanPointPos_3,PlanPointPos_4,PlanPointPos_4,PlanPointPos_1 };
			//������
			QVector<QVector3D> PlanPointPos = { PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4 };
			//��
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //����ͼ��û��size��С
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //�߽��߹̶�Ϊ��ɫ
			//��
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //����ͼ��û��size��С
			f.lableData.setLableColor(color);

			//����ϵ
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,L_vec } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(255, 0, 0));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,W_vec } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 255, 0));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//����ϵ����
			QVector<POS_TXT_DIR> ptd = { { pos ,{"L",L_vec}},{ pos ,{"W",W_vec}}, { pos ,{"N",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ

			//������ݿ�
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}
		//���������ǽ
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D P_normal, QVector3D L_vec, QVector3D M_vec, const float P, const float L, const float M, const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			P_normal = P_normal.normalized();
			L_vec = L_vec.normalized();
			M_vec = M_vec.normalized();
			//��һ������ ��ʼ����
			QVector3D PlanPointPos_1 = pos;
			//�ڶ�������L
			QVector3D PlanPointPos_2 = pos + L_vec * L;
			//����������
			QVector3D PlanPointPos_3 = pos + L_vec * L + M_vec * M;
			//���ĸ�����
			QVector3D PlanPointPos_4 = pos + M_vec * M;
			//��5������
			QVector3D PlanPointPos_5 = pos - P_normal * P;
			//��6����
			QVector3D PlanPointPos_6 = pos - P_normal * P + L_vec * L;
			//��7����
			QVector3D PlanPointPos_7 = pos - P_normal * P + L_vec * L + M_vec * M;
			//��8����
			QVector3D PlanPointPos_8 = pos - P_normal * P + M_vec * M;

			//������
			QVector<QVector3D> LinePointPos = {
				PlanPointPos_1 ,PlanPointPos_2 ,
				PlanPointPos_2,PlanPointPos_3 ,
				PlanPointPos_3,PlanPointPos_4,
				PlanPointPos_4,PlanPointPos_1 ,
				PlanPointPos_5 ,PlanPointPos_6 ,
				PlanPointPos_6,PlanPointPos_7 ,
				PlanPointPos_7,PlanPointPos_8,
				PlanPointPos_8,PlanPointPos_5,
				PlanPointPos_1,PlanPointPos_5,
				PlanPointPos_2,PlanPointPos_6,
				PlanPointPos_3,PlanPointPos_7,
				PlanPointPos_4,PlanPointPos_8,
			};
			//������
			QVector<QVector3D> PlanPointPos = { 
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4,
				PlanPointPos_5 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_8,
				PlanPointPos_1 ,PlanPointPos_5 ,PlanPointPos_8 ,PlanPointPos_4,
				PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_3,
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_5,
				PlanPointPos_4 ,PlanPointPos_3 ,PlanPointPos_7 ,PlanPointPos_8,
			};
			//��
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //����ͼ��û��size��С
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //�߽��߹̶�Ϊ��ɫ
			//��
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //����ͼ��û��size��С
			f.lableData.setLableColor(color);

			//����ϵ
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,M_vec } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(255, 0, 0));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,L_vec } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 255, 0));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,P_normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//����ϵ����
			QVector<POS_TXT_DIR> ptd = { { pos ,{"L",L_vec}},{ pos ,{"M",M_vec}}, { pos ,{"P",P_normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ

			//������ݿ�
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}
		//Բ�������ǽ
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, const float radius, const float L, const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			QVector3D rVec = mViewToolClass::getNormVector(normal);
			QVector<QVector3D> ShapePointPos, LinePointPos;
			getCylinderVertex(ShapePointPos, LinePointPos, pos, normal, rVec, radius, L, 5);

		
			//��
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //����ͼ��û��size��С
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //�߽��߹̶�Ϊ��ɫ
			//��
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(ShapePointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //����ͼ��û��size��С
			f.lableData.setLableColor(color);

			//����ϵ
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,normal } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(0, 0, 255));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,normal } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 0, 255));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//����ϵ����
			QVector<POS_TXT_DIR> ptd = { { pos ,{"P",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ

			//������ݿ�
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}
		//�������ǽ
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, const float radius,  const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			QVector<QVector3D> SpherePointPos, LinePointPos;
			getSphereVertex(SpherePointPos, LinePointPos, pos, radius, 5);


			//��
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //����ͼ��û��size��С
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //�߽��߹̶�Ϊ��ɫ
			//��
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(SpherePointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //����ͼ��û��size��С
			f.lableData.setLableColor(color);

			//����ϵ
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,normal } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(0, 0, 255));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,normal } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 0, 255));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//����ϵ����
			QVector<POS_TXT_DIR> ptd = { { pos ,{"N",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //����Ĭ��Ϊ��ɫ

			//������ݿ�
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}

		/*
		* ��Ӻ���
		*/
		void appendLableRendData_Box(const QString &name, const QVector3D pos, QVector3D normalx, QVector3D normaly, QVector3D normalz, const float l_x, const float l_y, const float l_z, const QVector3D color = QVector3D(74.0, 181.0, 217.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			//�ж������Ƿ����
			if (_lableRend->_lableDataController->isLableDataExist<Face_Box_Explicit>(name) && _lableRend->_lableDataController->isLableDataExist<Line_Box_Explicit>(name))
			{
				deleteLableRendData_Box(name);
			}
			normalx = normalx.normalized();
			normaly = normaly.normalized();
			normalz = normalz.normalized();
			//��һ������ ��ʼ����
			QVector3D PlanPointPos_1 = pos + normaly * l_y + normalz * l_z;
			//�ڶ�������L
			QVector3D PlanPointPos_2 = pos + normalx * l_x + normaly * l_y + normalz * l_z;
			//����������
			QVector3D PlanPointPos_3 = pos + normalx * l_x + normaly * l_y;
			//���ĸ�����
			QVector3D PlanPointPos_4 = pos + normaly * l_y;
			//��5������
			QVector3D PlanPointPos_5 = pos + normalz * l_z;
			//��6����
			QVector3D PlanPointPos_6 = pos + normalx * l_x + normalz * l_z;
			//��7����
			QVector3D PlanPointPos_7 = pos + normalx * l_x;
			//��8����
			QVector3D PlanPointPos_8 = pos;
			//������
			QVector<QVector3D> LinePointPos = {
				PlanPointPos_1 ,PlanPointPos_2 ,
				PlanPointPos_2,PlanPointPos_3 ,
				PlanPointPos_3,PlanPointPos_4,
				PlanPointPos_4,PlanPointPos_1 ,
				PlanPointPos_5 ,PlanPointPos_6 ,
				PlanPointPos_6,PlanPointPos_7 ,
				PlanPointPos_7,PlanPointPos_8,
				PlanPointPos_8,PlanPointPos_5,
				PlanPointPos_1,PlanPointPos_5,
				PlanPointPos_2,PlanPointPos_6,
				PlanPointPos_3,PlanPointPos_7,
				PlanPointPos_4,PlanPointPos_8,
			};
			//������
			QVector<QVector3D> PlanPointPos = {
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4,
				PlanPointPos_5 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_8,
				PlanPointPos_1 ,PlanPointPos_5 ,PlanPointPos_8 ,PlanPointPos_4,
				PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_3,
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_5,
				PlanPointPos_4 ,PlanPointPos_3 ,PlanPointPos_7 ,PlanPointPos_8,
			};
			//��
			Line_Box_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //����ͼ��û��size��С
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //�߽��߹̶�Ϊ��ɫ

			//��
			Face_Box_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //����ͼ��û��size��С
			f.lableData.setLableColor(color);
			//������ݿ�
			_lableRend->_lableDataController->appendLableData<Line_Box_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_Box_Explicit>(name, f);

			//��Ⱦ����
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, Append);
				updateRend_Single<Face_Box_Explicit>(name, Append);
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
		//����ǽ
		void deleteLableRendData_RigidWall(const QString &name, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//������Ⱦ
			if (isUpdate)
			{		
				updateRend_Single<Line_RW_Explicit>(name, Delete);
				updateRend_Single<Face_RW_Explicit>(name, Delete);
				updateRend_Single<Axis_RW_Explicit>(name, Delete);	
				emit updateView();
			}
			//ɾ�����ݿ�������	
			_lableRend->_lableDataController->deleteLableData<Line_RW_Explicit>(name);
			_lableRend->_lableDataController->deleteLableData<Face_RW_Explicit>(name);
			_lableRend->_lableDataController->deleteLableData<Axis_RW_Explicit>(name);

		}
		//����
		void deleteLableRendData_Box(const QString &name, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, Delete);
				updateRend_Single<Face_Box_Explicit>(name, Delete);

				emit updateView();
			}
			//ɾ�����ݿ�������	
			_lableRend->_lableDataController->deleteLableData<Line_Box_Explicit>(name);
			_lableRend->_lableDataController->deleteLableData<Face_Box_Explicit>(name);


		}

		/*
		* ���õ�������״̬��ͨ�ã�  //T1Ϊ��������  T2Ϊ Font Lable  Lable_Lable Lable_Font  Lable_Lable_Lable_Font ��ֻ�޸�����  ͼ��  ���� ����+ͼ��
		*/
		template<class T1, class T2>
		void setLableRendShowState(const QString &name, const bool state, const bool isUpdate = true)
		{
			if(_lableRend==nullptr) return;
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
		//����ǽ
		void setLableRendShowState_RigidWall(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//�������ݿ�������
			mode = (state == true ? Show : Hide);

			_lableRend->_lableDataController->updateLableData_ShowState<Line_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_ShowState<Face_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_ShowState<Axis_RW_Explicit>(name, state, Lable_Lable_Lable_Font());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, mode);
				updateRend_Single<Face_RW_Explicit>(name, mode);
				updateRend_Single<Axis_RW_Explicit>(name, mode);
				emit updateView();
			}
			
		}
		//����
		void setLableRendShowState_Box(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//�������ݿ�������
			mode = (state == true ? Show : Hide);

			_lableRend->_lableDataController->updateLableData_ShowState<Line_Box_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_ShowState<Face_Box_Explicit>(name, state, Lable());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, mode);
				updateRend_Single<Face_Box_Explicit>(name, mode);
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
		//����ǽ
		void setLableRendHightlightState_RigidWall(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//�������ݿ�������		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<Line_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_HighlightState<Face_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_HighlightState<Axis_RW_Explicit>(name, state, Lable_Lable_Lable_Font());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, mode);
				updateRend_Single<Face_RW_Explicit>(name, mode);
				updateRend_Single<Axis_RW_Explicit>(name, mode);
				emit updateView();
			}
			
		}
		//����
		void setLableRendHightlightState_Box(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//�������ݿ�������		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<Line_Box_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_HighlightState<Face_Box_Explicit>(name, state, Lable());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, mode);
				updateRend_Single<Face_Box_Explicit>(name, mode);	
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
		//����ǽ(ֻ�����������ɫ)
		void setLableRendColor_RigidWall(const QString &name, const QVector3D color, const AnalysisModule module, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			_lableRend->_lableDataController->updateLableData_Color<Face_RW_Explicit>(name, color, Lable());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Face_RW_Explicit>(name, SetColor);
				emit updateView();
			}
		}
		//����(ֻ�����������ɫ)
		void setLableRendColor_Box(const QString &name, const QVector3D color, const AnalysisModule module, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			_lableRend->_lableDataController->updateLableData_Color<Face_Box_Explicit>(name, color, Lable());
			//������Ⱦ
			if (isUpdate)
			{
				updateRend_Single<Face_Box_Explicit>(name, SetColor);
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

		////////////////////////ĳһ��������ͬһ�������ݲ���////////////////////////
		/*
		* ���ĳһ��������ͬһ����������Ⱦ  ���ڶ��ļ������
		*/
		void appendLableRendData_SameType(const AnalysisModule module, const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			updateAllRend_SameType(module, type, Append);
		}
		/*
		* ɾ��ĳһ��������ͬһ����������Ⱦ 
		*/
		void deleteLableRendData_SameType(const AnalysisModule module, const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			//������Ⱦ
			updateAllRend_SameType(module, type, Delete);
			//ɾ�����ݿ�������	
			_lableRend->_lableDataController->deleteLableData_SameType(module, type);		
		}
		/*
		* ����ĳһ��������ͬһ������������״̬
		*/
		void setLableRend_ShowState_SameType(const AnalysisModule module, const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_ShowState_SameType(module, type, state);
			//������Ⱦ
			if (state)
			{
				updateAllRend_SameType(module, type, Show);
			}
			else
			{
				updateAllRend_SameType(module, type, Hide);
			}
		}
		/*
		* ����ĳһ��������ͬһ�������и���״̬
		*/
		void setLableRend_Highlight_SameType(const AnalysisModule module, const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_HighlightState_SameType(module, type, state);
			//������Ⱦ
			if (state)
			{
				updateAllRend_SameType(module, type, Highlight);
			}
			else
			{
				updateAllRend_SameType(module, type, NoHighlight);
			}
		}

		/*
		* ����ĳһ��������ͬһ����������ɫ
		*/
		void setLableRend_Color_SameType(const AnalysisModule module, const LableOperateType type, const QVector3D color)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_Color_SameType(module, type, color);
			//������Ⱦ		
			updateAllRend_SameType(module, type, SetColor);

		}
		/*
		* ����ĳһ��������ͬһ�������д�С(Ŀǰֻ֧���޸�ͼ���С ��֧������)
		*/
		void setLableRend_Size_SameType(const AnalysisModule module, const LableOperateType type, const float size)
		{
			if (_lableRend == nullptr) return;
			//�������ݿ�������
			_lableRend->_lableDataController->updateLableData_Size_SameType<Lable>(module, type, size);  //ֻ����ͼ���С
			//������Ⱦ		
			updateAllRend_SameType(module, type, SetSize);

		}


		////////////////////////ĳһ��������������Ⱦ����////////////////////////
		/*
		* ɾ��ĳһ��������������Ⱦ  
		*/
		void deleteAllLableRendData(const AnalysisModule module)
		{
			deleteLableRendData_SameType(module, LableOperateType_LocalAxis);
			deleteLableRendData_SameType(module, LableOperateType_Load);
			deleteLableRendData_SameType(module, LableOperateType_BC);
			deleteLableRendData_SameType(module, LableOperateType_Connect);
			deleteLableRendData_SameType(module, LableOperateType_Contact);
			deleteLableRendData_SameType(module, LableOperateType_RigidConnect);
			deleteLableRendData_SameType(module, LableOperateType_RigidConstraint);
			deleteLableRendData_SameType(module, LableOperateType_RigidWall);
			deleteLableRendData_SameType(module, LableOperateType_InitCondition);
			deleteLableRendData_SameType(module, LableOperateType_Box);
			deleteLableRendData_SameType(module, LableOperateType_Vector);
			deleteLableRendData_SameType(module, LableOperateType_PlyCoordinate);
		}
		/*
		* ����ĳһ����������������״̬
		*/
		void setAllLableRend_ShowState(const AnalysisModule module, const bool state)
		{
			setLableRend_ShowState_SameType(module, LableOperateType_LocalAxis, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Load, state);
			setLableRend_ShowState_SameType(module, LableOperateType_BC, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Connect, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Contact, state);
			setLableRend_ShowState_SameType(module, LableOperateType_RigidConnect, state);
			setLableRend_ShowState_SameType(module, LableOperateType_RigidConstraint, state);
			setLableRend_ShowState_SameType(module, LableOperateType_RigidWall, state);
			setLableRend_ShowState_SameType(module, LableOperateType_InitCondition, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Box, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Vector, state);
			setLableRend_ShowState_SameType(module, LableOperateType_PlyCoordinate, state);
		}
		/*
		* ����ĳһ�����������и���״̬
		*/
		void setAllLableRend_Highlight(const AnalysisModule module, const bool state)
		{
			setLableRend_Highlight_SameType(module, LableOperateType_LocalAxis, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Load, state);
			setLableRend_Highlight_SameType(module, LableOperateType_BC, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Connect, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Contact, state);
			setLableRend_Highlight_SameType(module, LableOperateType_RigidConnect, state);
			setLableRend_Highlight_SameType(module, LableOperateType_RigidConstraint, state);
			setLableRend_Highlight_SameType(module, LableOperateType_RigidWall, state);
			setLableRend_Highlight_SameType(module, LableOperateType_InitCondition, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Box, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Vector, state);
			setLableRend_Highlight_SameType(module, LableOperateType_PlyCoordinate, state);
		}
		/*
		* ����ĳһ��������������ɫ(һ�㲻�ô˽ӿ� ������ͼ����ɫ��ͬ)
		*/
		void setAllLableRendColor(const AnalysisModule module, const QVector3D color)
		{
			setLableRend_Color_SameType(module, LableOperateType_LocalAxis, color);
			setLableRend_Color_SameType(module, LableOperateType_Load, color);
			setLableRend_Color_SameType(module, LableOperateType_BC, color);
			setLableRend_Color_SameType(module, LableOperateType_Connect, color);
			setLableRend_Color_SameType(module, LableOperateType_Contact, color);
			setLableRend_Color_SameType(module, LableOperateType_RigidConnect, color);
			setLableRend_Color_SameType(module, LableOperateType_RigidConstraint, color);
			setLableRend_Color_SameType(module, LableOperateType_RigidWall, color);
			setLableRend_Color_SameType(module, LableOperateType_InitCondition, color);
			setLableRend_Color_SameType(module, LableOperateType_Box, color);
			setLableRend_Color_SameType(module, LableOperateType_Vector, color);
			setLableRend_Color_SameType(module, LableOperateType_PlyCoordinate, color);
		}
		/*
		* ����ĳһ�����������д�С
		*/
		void setAllLableRendSize(const AnalysisModule module, const float size)
		{

			setLableRend_Size_SameType(module, LableOperateType_LocalAxis, size);
			setLableRend_Size_SameType(module, LableOperateType_Load, size);
			setLableRend_Size_SameType(module, LableOperateType_BC, size);
			setLableRend_Size_SameType(module, LableOperateType_Connect, size);
			setLableRend_Size_SameType(module, LableOperateType_Contact, size);
			setLableRend_Size_SameType(module, LableOperateType_RigidConnect, size);
			setLableRend_Size_SameType(module, LableOperateType_RigidConstraint, size);
			setLableRend_Size_SameType(module, LableOperateType_RigidWall, size);
			setLableRend_Size_SameType(module, LableOperateType_InitCondition, size);
			setLableRend_Size_SameType(module, LableOperateType_Box, size);
			setLableRend_Size_SameType(module, LableOperateType_Vector, size);
			setLableRend_Size_SameType(module, LableOperateType_PlyCoordinate, size);
		}

		////////////////////////���з�������������Ⱦ����////////////////////////
		/*
		* ɾ�����з�������������Ⱦ
		*/
		void deleteAllLableRendData()
		{
			deleteAllLableRendData(ImplicitAnalysis);
			deleteAllLableRendData(ExplicitAnalysis);
			deleteAllLableRendData(MagnetAnalysis);
		}
		/*
		* �������з���������������״̬
		*/
		void setAllLableRend_ShowState(const bool state)
		{
			setAllLableRend_ShowState(ImplicitAnalysis, state);
			setAllLableRend_ShowState(ExplicitAnalysis, state);
			setAllLableRend_ShowState(MagnetAnalysis, state);
		}
		/*
		* �������з����������и���״̬
		*/
		void setAllLableRend_Highlight(const bool state)
		{
			setAllLableRend_Highlight(ImplicitAnalysis, state);
			setAllLableRend_Highlight(ExplicitAnalysis, state);
			setAllLableRend_Highlight(MagnetAnalysis, state);
		}
		/*
		* �������з�������������ɫ(һ�㲻�ô˽ӿ� ������ͼ����ɫ��ͬ)
		*/
		void setAllLableRendColor(const QVector3D color)
		{
			setAllLableRendColor(ImplicitAnalysis, color);
			setAllLableRendColor(ExplicitAnalysis, color);
			setAllLableRendColor(MagnetAnalysis, color);
		}
		/*
		* �������з����������д�С
		*/
		void setAllLableRendSize(const float size)
		{
			setAllLableRendSize(ImplicitAnalysis, size);
			setAllLableRendSize(ExplicitAnalysis, size);
			setAllLableRendSize(MagnetAnalysis, size);
		}
		////////////////////////////////////////////////////////////
		/*
		*  ������
		*/
		template<class T>
		void resetName(const QString& oldname, const QString& newname)
		{
			if (_lableRend == nullptr) return;
			_lableRend->_lableDataController->resetName<T>(oldname, newname);
		}
		//����ǽ
		void resetName_RigidWall(const QString& oldname, const QString& newname)
		{
			if (_lableRend == nullptr) return;
			_lableRend->_lableDataController->resetName<Line_RW_Explicit>(oldname, newname);
			_lableRend->_lableDataController->resetName<Face_RW_Explicit>(oldname, newname);
			_lableRend->_lableDataController->resetName<Axis_RW_Explicit>(oldname, newname);
		}		
		//����
		void resetName_Box(const QString& oldname, const QString& newname)
		{
			if (_lableRend == nullptr) return;
			_lableRend->_lableDataController->resetName<Line_Box_Explicit>(oldname, newname);
			_lableRend->_lableDataController->resetName<Face_Box_Explicit>(oldname, newname);

		}
		
	private:																		
		/*
		* �����������
		*/
		void appendFontData(LableData1<QVector<POS_TXT_DIR>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QString& txt, const QVector<POS_DIR> &para)
		{
			if (_lableRend == nullptr) return;
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
			if (_lableRend == nullptr) return;
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
			if (_lableRend == nullptr) return;
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
			//��ͷ + �����������ͼ����Ⱦ���� ���ֲ�����ϵ��
			if (is_same<T, MDataLable::Arrow_Font_Axis_Implicit>::value)
			{
				_lableRend->_arrowRend_axis_i->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_axis_i->updateSingleRend(type, name);
			}
			//��ͷ + �����������ͼ����Ⱦ���� ���ֲ�����ϵ��
			else if (is_same<T, MDataLable::Arrow_Font_Axis_Explicit>::value)
			{
				_lableRend->_arrowRend_axis_e->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_axis_e->updateSingleRend(type, name);
			}
			//��ͷ + �����������ͼ����Ⱦ���� ���ֲ�����ϵ��
			else if (is_same<T, MDataLable::Arrow_Font_Axis_Magnet>::value)
			{
				_lableRend->_arrowRend_axis_mag->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_axis_mag->updateSingleRend(type, name);
			}
			//��ͷ + �����������ͼ����Ⱦ���� �������������������ء����غɡ�ѹǿ��Ԥ��������������
			else if (is_same<T, MDataLable::Arrow_Font_Load_Implicit>::value)
			{
				_lableRend->_arrowRend_load_i->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_load_i->updateSingleRend(type, name);
			}
			//��ͷ + �����������ͼ����Ⱦ���� ����ʼ�ٶȣ�
			else if (is_same<T, MDataLable::Arrow_Font_InitCondition_Explicit>::value)
			{
				_lableRend->_arrowRend_initCondition_e->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_initCondition_e->updateSingleRend(type, name);
			}
			//��ͷ + �����������ͼ����Ⱦ���� �����غɡ����غɡ�������
			else if (is_same<T, MDataLable::Arrow_Font_Load_Explicit>::value)
			{
				_lableRend->_arrowRend_load_e->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_load_e->updateSingleRend(type, name);
			}
			//��ͷ + �����������ͼ����Ⱦ���� ���� ���/����   �Ż�ǿ�ȣ�
			else if (is_same<T, MDataLable::Arrow_Font_Load_Magnet>::value)
			{
				_lableRend->_arrowRend_load_mag->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_load_mag->updateSingleRend(type, name);
			}
			//��ͷ����ͼ����Ⱦ���� �����ƣ�
			else if (is_same<T, MDataLable::Arrow_BC_Explicit>::value)
			{
				_lableRend->_arrowRend_BC_e->updateSingleRend(type, name);		
			}
			//��ͷ����ͼ����Ⱦ���� ��������
			else if (is_same<T, MDataLable::Arrow_Vector_Explicit>::value)
			{
				_lableRend->_arrowRend_vector_e->updateSingleRend(type, name);
			}
			//��ͷ����ͼ����Ⱦ���� ����ʸ��λ��
			else if (is_same<T, MDataLable::Arrow_BC_Magnet>::value)
			{
				_lableRend->_arrowRend_BC_mag->updateSingleRend(type, name);
			}
			//��ͷ����ͼ����Ⱦ���� �������̲�����ϵ��
			else if (is_same<T, MDataLable::Arrow_PlyCoordinate_Implicit>::value)
			{
				_lableRend->_arrowRend_plyCoordinate_i->updateSingleRend(type, name);
			}
			//������+���� �������ͼ����Ⱦ���� �������ܶȣ�����ܶȣ�
			else if (is_same<T, MDataLable::Triangle_Font_Load_Implicit>::value)
			{
				_lableRend->_triangleRend_load_i->updateSingleRend(type, name);
				_lableRend->_triangleFontRend_load_i->updateSingleRend(type, name);
			}
			//������+���� �������ͼ����Ⱦ���� ����ʼӦ����SPH��ʼӦ������ʼը�㣩
			else if (is_same<T, MDataLable::Triangle_Font_InitCondition_Explicit>::value)
			{
				_lableRend->_triangleRend_initCondition_e->updateSingleRend(type, name);
				_lableRend->_triangleFontRend_initCondition_e->updateSingleRend(type, name);
			}
			//������+���� �������ͼ����Ⱦ���� ���������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ�ȣ�
			else if (is_same<T, MDataLable::Triangle_Font_Load_Magnet>::value)
			{
				_lableRend->_triangleRend_load_mag->updateSingleRend(type, name);
				_lableRend->_triangleFontRend_load_mag->updateSingleRend(type, name);
			}
			//����ͼ��+���� �������ͼ����Ⱦ���󣨶������ȣ��ȷ��䣩
			else if (is_same<T, MDataLable::Rhombus_Font_Load_Implicit>::value)
			{
				_lableRend->_rhombusRend_load_i->updateSingleRend(type, name);
				_lableRend->_rhombusFontRend_load_i->updateSingleRend(type, name);
			}
			//ʮ��ͼ��+���� �������ͼ����Ⱦ����(�ڲ���Դ,�¶��غ�)
			else if (is_same<T, MDataLable::X_Font_Load_Implicit>::value)
			{
				_lableRend->_XRend_load_i->updateSingleRend(type, name);
				_lableRend->_XFontRend_load_i->updateSingleRend(type, name);
			}
			//������ͼ��+���� �������ͼ����Ⱦ����(���)
			else if (is_same<T, MDataLable::Lightning_Font_Load_Implicit>::value)
			{
				_lableRend->_lightningRend_load_i->updateSingleRend(type, name);
				_lableRend->_lightningFontRend_load_i->updateSingleRend(type, name);
			}
			//������ͼ��+���� �������ͼ����Ⱦ����(�ڵ�/����/�� ���   �ڵ�/���� ����)
			else if (is_same<T, MDataLable::Lightning_Font_Load_Magnet>::value)
			{
				_lableRend->_lightningRend_load_mag->updateSingleRend(type, name);
				_lableRend->_lightningFontRend_load_mag->updateSingleRend(type, name);
			}
			//���ɶ�Լ������ (�̶� �Գ� λ��ת�� ��ʼ���� ����Լ��)
			else if (is_same<T, MDataLable::DF_BC_Implicit>::value)
			{
				_lableRend->_dfRend_BC_i->updateSingleRend(type, name);
			}
			//���ɶ�Լ������ (�����غ�)
			else if (is_same<T, MDataLable::DF_Load_Explicit>::value)
			{
				_lableRend->_dfRend_load_e->updateSingleRend(type, name);
			}
			//���ɶ�Լ������ (λ�� �ٶ� ���ٶ�)
			else if (is_same<T, MDataLable::DF_BC_Explicit>::value)
			{
				_lableRend->_dfRend_BC_e->updateSingleRend(type, name);
			}
			//���ɶ�Լ������ (�����˶�)
			else if (is_same<T, MDataLable::DF_Connect_Explicit>::value)
			{
				_lableRend->_dfRend_connect_e->updateSingleRend(type, name);
			}
			//������ ���ͣ���ʼ�߽�����(��ʼ�¶ȣ����¶ȱ߽磬���ƣ���
			else if (is_same<T, MDataLable::Home_BC_Implicit>::value)
			{
				_lableRend->_homeRend_BC_i->updateSingleRend(type, name);
			}
			//������ ���ͣ�������λ��
			else if (is_same<T, MDataLable::Home_BC_Magnet>::value)
			{
				_lableRend->_homeRend_BC_mag->updateSingleRend(type, name);
			}

			//ֱ��+���� ���ͣ����ԡ����ԡ�mpc���ӣ�
			else if (is_same<T, MDataLable::Line_Connect_Implicit>::value)
			{
				_lableRend->_lineRend_connect_i->updateSingleRend(type, name);
				_lableRend->_lineFontRend_connect_i->updateSingleRend(type, name);
			}
			//ֱ��+���� ���ͣ��纸���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ���������-�����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ�
			//�ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�նȣ�
			else if (is_same<T, MDataLable::Line_Connect_Explicit>::value)
			{
				_lableRend->_lineRend_connect_e->updateSingleRend(type, name);
				_lableRend->_lineFontRend_connect_e->updateSingleRend(type, name);
			}
			//������ ���ͣ�����������󶨣�
			else if (is_same<T, MDataLable::FaceToFace_Connect_Implicit>::value)
			{
				_lableRend->_faceTofaceRend_connect_i->updateSingleRend(type, name);
			}
			//������ ���ͣ�����������󶨡�ʧЧ�󶨣�
			else if (is_same<T, MDataLable::FaceToFace_Connect_Explicit>::value)
			{
				_lableRend->_faceTofaceRend_connect_e->updateSingleRend(type, name);
			}
			//������ ���ͣ��Ӵ��� ����� ����棩
			else if (is_same<T, MDataLable::FaceToFace_Contact_Explicit>::value)
			{
				_lableRend->_faceTofaceRend_contact_e->updateSingleRend(type, name);
			}
			//������ ���ͣ��Ӵ���
			else if (is_same<T, MDataLable::FaceToFace_Contact_Implicit>::value)
			{
				_lableRend->_faceTofaceRend_contact_i->updateSingleRend(type, name);
			}
			//��������
			else if (is_same<T, MDataLable::Type1_RC_Implicit>::value)
			{
				_lableRend->_RCRend_type1_i->updateSingleRend(type, name);
			}
			else if (is_same<T, MDataLable::Type2_RC_Implicit>::value)
			{
				_lableRend->_RCRend_type2_i->updateSingleRend(type, name);
			}
			//ֱ�����ͣ����� ���ᣩ
			else if (is_same<T, MDataLable::Line_RC_Implicit>::value)
			{
				_lableRend->_lineRend_RC_i->updateSingleRend(type, name);
			}
			//Բ��¸���Բ��¸���
			else if (is_same<T, MDataLable::Sphere_RC_Implicit>::value)
			{
				_lableRend->_sphereRend_RC_i->updateSingleRend(type, name);
			}
			//����Լ��
			else if (is_same<T, MDataLable::DF_RBC_Implicit>::value)
			{
				_lableRend->_dfRend_RBC_i->updateSingleRend(type, name);
			}
			//����ǽ���ߣ�
			else if (is_same<T, MDataLable::Line_RW_Explicit>::value)
			{
				_lableRend->_lineRend_RW_e->updateSingleRend(type, name);
			}
			//����ǽ���棩
			else if (is_same<T, MDataLable::Face_RW_Explicit>::value)
			{
				_lableRend->_faceRend_RW_e->updateSingleRend(type, name);
			}
			//����ǽ������ϵ��
			else if (is_same<T, MDataLable::Axis_RW_Explicit>::value)
			{
				_lableRend->_axisRend_RW_e->updateSingleRend(type, name);
				_lableRend->_axisFontRend_RW_e->updateSingleRend(type, name);
			}
			//���ӣ��棩
			else if (is_same<T, MDataLable::Face_Box_Explicit>::value)
			{
				_lableRend->_faceRend_box_e->updateSingleRend(type, name);	
			}
			//���ӣ��ߣ�
			else if (is_same<T, MDataLable::Line_Box_Explicit>::value)
			{
				_lableRend->_lineRend_box_e->updateSingleRend(type, name);
			}
			emit updateView();
		}
		/*
		* ����ͬһ����������Ⱦ
		*/
		void updateAllRend_SameType(const AnalysisModule module, const LableOperateType type, const LableOperateMode mode)
		{
			if (_lableRend == nullptr)
			{
				return;
			}
			if (module == ImplicitAnalysis)
			{
				switch (type)
				{
				case LableOperateType_LocalAxis:
					_lableRend->_arrowRend_axis_i->updateAllRend(mode);
					_lableRend->_arrowFontRend_axis_i->updateAllRend(mode);
					break;
				case LableOperateType_Load:
					_lableRend->_arrowRend_load_i->updateAllRend(mode);
					_lableRend->_arrowFontRend_load_i->updateAllRend(mode);
					_lableRend->_triangleRend_load_i->updateAllRend(mode);
					_lableRend->_triangleFontRend_load_i->updateAllRend(mode);
					_lableRend->_rhombusRend_load_i->updateAllRend(mode);
					_lableRend->_rhombusFontRend_load_i->updateAllRend(mode);
					_lableRend->_XRend_load_i->updateAllRend(mode);
					_lableRend->_XFontRend_load_i->updateAllRend(mode);
					_lableRend->_lightningRend_load_i->updateAllRend(mode);
					_lableRend->_lightningFontRend_load_i->updateAllRend(mode);
					break;
				case LableOperateType_BC:
					_lableRend->_dfRend_BC_i->updateAllRend(mode);
					_lableRend->_homeRend_BC_i->updateAllRend(mode);
					break;
				case LableOperateType_Connect:
					_lableRend->_lineRend_connect_i->updateAllRend(mode);
					_lableRend->_lineFontRend_connect_i->updateAllRend(mode);
					_lableRend->_faceTofaceRend_connect_i->updateAllRend(mode);
					break;
				case LableOperateType_Contact:
					_lableRend->_faceTofaceRend_contact_i->updateAllRend(mode);
					break;
				case LableOperateType_RigidConnect:
					_lableRend->_RCRend_type1_i->updateAllRend(mode);
					_lableRend->_RCRend_type2_i->updateAllRend(mode);
					_lableRend->_lineRend_RC_i->updateAllRend(mode);
					_lableRend->_sphereRend_RC_i->updateAllRend(mode);
					break;
				case LableOperateType_RigidConstraint:
					_lableRend->_dfRend_RBC_i->updateAllRend(mode);
					break;
				case LableOperateType_PlyCoordinate:
					_lableRend->_arrowRend_plyCoordinate_i->updateAllRend(mode);
				default:
					break;
				}
			}
			else if (module == ExplicitAnalysis)
			{
				switch (type)
				{
				case LableOperateType_LocalAxis:
					_lableRend->_arrowRend_axis_e->updateAllRend(mode);
					_lableRend->_arrowFontRend_axis_e->updateAllRend(mode);
					break;
				case LableOperateType_Load:
					_lableRend->_arrowRend_load_e->updateAllRend(mode);
					_lableRend->_arrowFontRend_load_e->updateAllRend(mode);
					_lableRend->_dfRend_load_e->updateAllRend(mode);
					break;
				case LableOperateType_BC:
					_lableRend->_dfRend_BC_e->updateAllRend(mode);
					_lableRend->_arrowRend_BC_e->updateAllRend(mode);
					break;
				case LableOperateType_Connect:
					_lableRend->_lineRend_connect_e->updateAllRend(mode);
					_lableRend->_lineFontRend_connect_e->updateAllRend(mode);
					_lableRend->_faceTofaceRend_connect_e->updateAllRend(mode);
					_lableRend->_dfRend_connect_e->updateAllRend(mode);
					break;
				case LableOperateType_Contact:
					_lableRend->_faceTofaceRend_contact_e->updateAllRend(mode);
					break;
				case LableOperateType_InitCondition:
					_lableRend->_arrowRend_initCondition_e->updateAllRend(mode);
					_lableRend->_arrowFontRend_initCondition_e->updateAllRend(mode);
					_lableRend->_triangleRend_initCondition_e->updateAllRend(mode);
					_lableRend->_triangleFontRend_initCondition_e->updateAllRend(mode);
					break;
				case LableOperateType_RigidWall:
					_lableRend->_lineRend_RW_e->updateAllRend(mode);
					_lableRend->_faceRend_RW_e->updateAllRend(mode);
					_lableRend->_axisRend_RW_e->updateAllRend(mode);
					_lableRend->_axisFontRend_RW_e->updateAllRend(mode);
					break;
				case LableOperateType_Box:
					_lableRend->_faceRend_box_e->updateAllRend(mode);
					_lableRend->_lineRend_box_e->updateAllRend(mode);
					break;
				case LableOperateType_Vector:
					_lableRend->_arrowRend_vector_e->updateAllRend(mode);

					break;
				default:
					break;
				}
			}
			else if (module == MagnetAnalysis)
			{
				switch (type)
				{
				case LableOperateType_Load:
					_lableRend->_arrowRend_load_mag->updateAllRend(mode);
					_lableRend->_arrowFontRend_load_mag->updateAllRend(mode);
					_lableRend->_triangleRend_load_mag->updateAllRend(mode);
					_lableRend->_triangleFontRend_load_mag->updateAllRend(mode);
					_lableRend->_lightningRend_load_mag->updateAllRend(mode);
					_lableRend->_lightningFontRend_load_mag->updateAllRend(mode);
					break;
				case LableOperateType_BC:
					_lableRend->_arrowRend_BC_mag->updateAllRend(mode);
					_lableRend->_homeRend_BC_mag->updateAllRend(mode);
					break;
				case LableOperateType_LocalAxis:
					_lableRend->_arrowRend_axis_mag->updateAllRend(mode);
					_lableRend->_arrowFontRend_axis_mag->updateAllRend(mode);
					break;
				default:
					break;
				}
			}
			
			emit updateView();
		}


	private:
		//��ȡԲ���嶥������
		void getCylinderVertex(QVector<QVector3D> &ShapePointPos, QVector<QVector3D> &linePointPos, QVector3D pos, QVector3D normVec, QVector3D rVec, float r, float L, float angle)
		{
			rVec.normalized();
			normVec.normalized();
			QVector<QVector3D> face1, face2, face3;
			float cur_Angle = 0;
			while (cur_Angle < 360)
			{
				cur_Angle += angle;

				QMatrix4x4 matrixRotate;
				matrixRotate.setToIdentity();
				matrixRotate.rotate(cur_Angle, normVec);
				QVector4D tmp = QVector4D(rVec, 1.0) * matrixRotate;
				QVector3D next_rVec = QVector3D(tmp.x(), tmp.y(), tmp.z());
				next_rVec.normalized();

				//�������������
				QVector3D pos1 = pos;
				QVector3D pos2;
				if (face1.empty())
				{
					pos2 = pos + rVec * r;
				}
				else
				{
					pos2 = face1.back();
				}
				QVector3D pos3 = pos + next_rVec * r;
				//����
				face1.push_back(pos1);
				face1.push_back(pos1);
				face1.push_back(pos2);
				face1.push_back(pos3);
				linePointPos.push_back(pos2);
				linePointPos.push_back(pos3);
				//����
				face2.push_back(pos1 - normVec * L);
				face2.push_back(pos1 - normVec * L);
				face2.push_back(pos2 - normVec * L);
				face2.push_back(pos3 - normVec * L);
				linePointPos.push_back(pos2 - normVec * L);
				linePointPos.push_back(pos3 - normVec * L);
				//������
				if (cur_Angle - angle == 0 || cur_Angle - angle == 180)
				{
					linePointPos.push_back(pos2);
					linePointPos.push_back(pos2 - normVec * L);

				}

				//����
				int id = face1.size() - 1;
				face3.push_back(face1[id - 1]);		
				face3.push_back(face2[id - 1]);		
				face3.push_back(face2[id]);
				face3.push_back(face1[id]);



			}
			ShapePointPos = face1+face2+face3;
		}
		//��ȡ���嶥������
		void getSphereVertex(QVector<QVector3D>& shperePointPos, QVector<QVector3D>& linePointPos, QVector3D pos, double r, double angle)
		{
			QVector<QVector3D> curPos;
			QVector<QVector3D> lastPos;
			for (double i = 0; i <= 180; i += angle)
			{
				curPos.clear();
				double curR = r * sin(i / 180 * PI); //��ʱԲ�İ뾶
				double z = pos.z() + r * cos(i / 180 * PI);  //z����
				for (double j = 0; j <= 360; j += angle)
				{
					double x = pos.x() + curR * cos(j / 180 * PI);
					double y = pos.y() + curR * sin(j / 180 * PI);
					QVector3D onePoint = QVector3D(x, y, z);
					curPos.push_back(onePoint);
				}

				if (!lastPos.isEmpty())
				{
					for (int j = 0; j < lastPos.size() - 1; ++j)
					{
						//�������1
						if (i == 90)
						{
							linePointPos.push_back(curPos[j]);
							linePointPos.push_back(curPos[j + 1]);
						}
						//�������2
						if (j*angle == 0 || j * angle == 180)
						{
							linePointPos.push_back(lastPos[j]);
							linePointPos.push_back(curPos[j]);
						}
						//���ζ�������
						shperePointPos.push_back(lastPos[j]);
						shperePointPos.push_back(lastPos[j + 1]);
						shperePointPos.push_back(curPos[j + 1]);
						shperePointPos.push_back(curPos[j]);
					}
				}
				lastPos = curPos;
			}


		}
		//��Ⱦ����ID
		int _ID = 0;

	signals:
		void updateView();


	private:
		//����Ⱦ����
		shared_ptr<mLableRendIntegrate_pre> _lableRend = nullptr;
	};
}
