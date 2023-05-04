#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include<qvector3d.h>
#include<qmatrix4x4.h>
#include<qvector2d.h>

namespace MViewBasic
{
	
	class RENDBASIC_EXPORT mViewToolClass : public QObject
	{
		Q_OBJECT

	public:
		mViewToolClass();
		~mViewToolClass();

		//���ߺ���
		static QVector3D GetGeoCenterPosition(QVector<QVector3D> vertex);				//��ȡ��ת����
		static float GetMaxRadius(QVector<QVector3D> vertex, QVector3D center);		//��������ת�뾶
		static float GetMaxRadius(float left, float right, float bottom, float top, float back, float front, QVector3D center);//��֪�µ���ת���ĺ�ģ�͵����ߴ磬����ת�뾶
		static float GetMaxDistance(QVector<QVector3D> vertex);						//�Ҿ�����������֮��ľ���
		static QVector3D NormToModelPosition(QVector3D normPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);	//��׼����תģ������
		static QVector3D ModelToNormPosition(QVector3D modelPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);	//ģ������ת��׼����
		static QVector3D ModelToViewPosition(QVector3D modelPos, QMatrix4x4 view, QMatrix4x4 model);	//ģ������ת��׼����
		static QVector3D ModelToModelPosition(QVector3D modelPos1, QMatrix4x4 projection1, QMatrix4x4 view1, QMatrix4x4 model1, QMatrix4x4 projection2, QMatrix4x4 view2, QMatrix4x4 model2);
		static QVector3D PixelToNormPosition(int Posx, int Posy, int scr_width, int scr_height, float depth = 0);	//��������ת��Ϊ��׼����(���ֵĬ��Ϊ0.5)
		static QVector3D PixelToModelPosition(int Posx, int Posy, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model, float scr_width, int scr_height, float depth = 0);//��������ת��Ϊģ�͵�����
		static QVector2D PixelToOrthoPosition(int Posx, int Posy, float Left, float Right, float Bottom, float Top, float scr_width, int scr_height);//��������ת��Ϊ����ͶӰ������
		static QVector2D ModelToPixelPositon(QVector3D modelPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model, float scr_width, int scr_height);//ģ������ת��Ļ��������
		
		//����һ���������һ������������ֱ�ĵ�λ����
        static QVector3D getNormVector(QVector3D vec);
		//����һ���������������ֱ��4����λ������4����λ�������90��
		static QVector<QVector3D> getFourNormVector(QVector3D vec);
	
	};
}
