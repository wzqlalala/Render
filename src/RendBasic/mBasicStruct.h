#pragma once
#include"mMeshViewEnum.h"
#include <QObject>
#include"qmap.h"
#include"qvector3d.h"
#include <QVector2D>
#include"qpair.h"
namespace MViewBasic
{

	/*
	 * wzq���ֽṹ��
	 */
	struct FontText
	{
		QVector2D pos;
		QString value;
		bool visual;
		FontText()
		{
			pos = QVector2D(0, 0);
			value = "";
			visual = true;
		}
	};
	//ͨ�����ֽṹ��
	struct Font_Common
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //ģ������-�ı���
		QVector3D Color;
		float Size;
	};
	//�̶����ֽṹ��
	struct Font_Fixed
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //ģ������-�ı���
		QVector3D Color;
		float Size;
	};
	//�ֲ����������ֽṹ��(��ͷ��������)
	struct Font_LocalAxis
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //ģ������-�ı���
		QVector3D NormVec;//������������ͷʸ���� ��������ת��
	};
	//�غ����ֽṹ��(��ͷ��������)
	struct Font_Load
	{
		int ID;
		QMap<QVector<float>, QPair<QString, QVector3D>> Pos_Txt_Normvec; //ģ������-(�ı���,��������
		QVector3D Color;	//��ɫRGB
	}typedef Font_Vector, Font_RigidWall;

	//��ʼ�������ֽṹ��
	struct Font_InitialCondition
	{
		int ID;
		QVector<QPair<QVector<float>, QPair<QString, QVector3D>>> Pos_Txt_Normvec; //ģ������-(�ı���,��������
		QVector3D Color;	//��ɫRGB
	};
	//�߽���������
	struct Font_BC
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //ģ������-�ı���
		QVector3D Color;	//��ɫRGB
		QVector3D NormVec;//������������ͷʸ���� ��������ת��
	};
	//��������
	struct Font_Connect
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //ģ������-�ı���
		QVector3D Color;	//��ɫRGB
	};

	//����ͷ
	struct Arrow
	{
		int ID;
		QVector<QVector3D> Pos;//����
		QVector3D Direction; //����
		QVector3D Color;//��ɫRGB
		float Size; //��С
		bool isDepth; //�Ƿ���Ȳ���
	};
}
