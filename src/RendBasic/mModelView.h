#pragma once
#include "rendbasic_global.h"

#include "mViewBase.h"

#include <QObject>
#include<qmatrix4x4.h>
#include <QVector3D>
#include <QVector2D>
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mModelView : public mViewBase
	{
	public:
		mModelView();
		~mModelView();
	

	public:					

		//������Ļ�������
		float _Ratio_Old;						//��һ�ε���Ļ�����
		float _Ratio_New;						// Ŀǰ����Ļ�����

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
		void SetOrthoByRatio(float scr_width, float scr_height) override;	
		//ͨ����Ļ����Ȼ�ԭ����ͶӰ
		void ReturnOrthoByRatio();		

		//��ת ƽ�� ����
		void Translate(float xoffset, float yoffset);//ƽ��
		void Rotate(float xoffset, float yoffset, RotateType rotateType = Rotate_XY, float sensitivity = 0.25);//��ת
		void Rotate_ByBotton(float angle);//���Ŵ�ֱ��Ļ������ת�̶��Ƕ�	
		void ZoomAtMouse_Bywheel(int posX, int posY, int rollervalue, ScalePerspectice scalePerspectice, float sensitivity = 1.2) override;//�����������ţ��������ţ�
		void ZoomAtFrameCenter(int CenterposX, int CenterposY, float sensitivity = 2.0);//���ݿ�ѡ��������
		void ZoomAtViewCenter_ByBotton(ScaleDirection scaleDirection, float sensitivity = 2);//�����ӽ���������(�����һ������һ�� ������)
		void ZoomAtViewCenter_ByPercent(float percent);//�����ӽ���������(���ٷֱ����� ��������0.1)
		void ZoomAtViewCenter_ByMove(int FirstPosx, int FirstPosy, int nowPosx, int nowPosy,float sensitivity = 1.1);	//�����ӽ��������ţ�����ƶ����ţ�������
		//�����ӽ�																													
		void SetPerspective(Perspective type);	//���ù۲��ӽ�
		void FitView(QVector3D center);		//�����ӽ�ʹģ�ͳ���������ͼ
		void SaveCurrentView() override;//���浱ǰ�ӽ�
		void CallSavedView() override;	//���ñ�����ӽ�
		
		//������ת����
		void SetRotateCenterToPoint(QVector3D center, float maxRadius);	//������ת����Ϊѡ��ĵ�
		void SetRotateCenterToViewCenter(QVector3D center,float maxRadius);	//������ת����Ϊ��ͼ����				
		void SetRotateCenterToModelCenter(QVector3D center,float maxRadius);	//������ת����Ϊ��������
		
		//ģ�ͷ����仯������ӽǣ�������һ�ε���ת���ԣ�
		void ResetOrthoAndCamera(QVector3D center,float maxRadius) override;//������ת���ĺ������ת�뾶
		//���ģ�Ͷ��������ӽǣ��ӽ�����Ӧ��
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex,QVector<QVector3D> AddVertex) override;
		void UpdateOrthoAndCamera(float maxRadius,QVector<QVector3D> AddVertex) override;
		//���/ɾ��ģ�Ͳ��ֶ��������ӽǣ��ӽ�������Ӧ��
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex) override;
		void UpdateOrthoAndCamera(float maxRadius_modelcenter, float maxRadius_nowcenter) override;

		
	};
}

