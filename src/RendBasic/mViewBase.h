#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include<qmatrix4x4.h>
#include <QVector3D>
#include <QVector2D>
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		DISABLE,
		ENABLE
	};
	class RENDBASIC_EXPORT mViewBase : public QObject
	{
		Q_OBJECT

	public:
		mViewBase();
		~mViewBase();


	public:

		//��Ļ�����ش�С
		float SCR_WIDTH, SCR_HEIGHT;

		//������ӽǲ���
		QVector3D _Eye = QVector3D(0, 0, 1);
		QVector3D _Eye_0 = QVector3D(0, 0, 1);
		QVector3D _Center = QVector3D(0, 0, 0);
		QVector3D _Up = QVector3D(0, 1, 0);

		//�����ת�뾶
		float _MaxRadius;

		

		//��������ͶӰ
		double _Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane;				//����ͶӰȫ�ֱ���
		double _Left_ini, _Right_ini, _Bottom_ini, _Top_ini;						//����ͶӰ��ʼ��������
		double _Left_res, _Right_res, _Bottom_res, _Top_res;						//���ڴ�С�ı�󱣳�ģ������������		

		//PVM����
		QMatrix4x4 _projection, _view, _model;

		//������ӽǲ���
		QVector3D _Eye_Saved, _Eye_0_Saved, _Center_Saved, _Up_Saved;
		double _MaxRadius_Saved;
		double _Left_Saved, _Right_Saved, _Bottom_Saved, _Top_Saved, _NearPlane_Saved, _FarPlane_Saved;
		double _Left_ini_Saved, _Right_ini_Saved, _Bottom_ini_Saved, _Top_ini_Saved;
		double _Left_res_Saved, _Right_res_Saved, _Bottom_res_Saved, _Top_res_Saved;
		QMatrix4x4 _projection_Saved, _view_Saved, _model_Saved;

		//���ŵı���
		double _ScaleProportion = 1;

		//��ǰ����ͶӰ�Ĳ���(�����ƶ�����)
		float width_firstmouse, height_firstmouse;
		bool IfFirstMouse = false;
		//��ʼ��ģ���ӽ�
		void InitialModelView();
		//��ʼ������ͶӰ
		void InitialOrtho();
		//��ʼ���ӽ�
		void InitialCamera();
		//ͨ����Ļ�������������ͶӰ
		virtual void SetOrthoByRatio(float scr_width, float scr_height);
		//ͨ����Ļ����Ȼ�ԭ����ͶӰ
		void ReturnOrthoByRatio();
		//����PVM�����ֵ
		virtual void SetPVMValue();
		//��ȡPVMֵ
		QMatrix4x4 getPVMValue() { return _projection * _view * _model; };

		virtual void SaveCurrentView() {};//���浱ǰ�ӽ�
		virtual void CallSavedView() {};	//���ñ�����ӽ�

		virtual void ZoomAtMouse_Bywheel(int posX, int posY, int yoffset, ScalePerspectice scalePerspectice, float sensitivity = 1.2);

		virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

		//ģ�ͷ����仯������ӽǣ�������һ�ε���ת���ԣ�
		virtual void ResetOrthoAndCamera(QVector3D center, float maxRadius);//������ת���ĺ������ת�뾶
		//���ģ�Ͷ��������ӽǣ��ӽ�����Ӧ��
		virtual void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex, QVector<QVector3D> AddVertex) {};
		virtual void UpdateOrthoAndCamera(float maxRadius, QVector<QVector3D> AddVertex) {};
		//���/ɾ��ģ�Ͳ��ֶ��������ӽǣ��ӽ�������Ӧ��
		virtual void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex) {};
		virtual void UpdateOrthoAndCamera(float maxRadius_modelcenter, float maxRadius_nowcenter) {};


	};
}

