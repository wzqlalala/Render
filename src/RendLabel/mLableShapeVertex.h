#pragma once

#include <QObject>
#include<QVector3D>
const static double M_PI = 3.1415926;

namespace MLableRend
{
	class mLableShapeVertex 
	{
	public:
		mLableShapeVertex();
		~mLableShapeVertex();
		/*
		* ��ȡ����ָ��
		*/
		static mLableShapeVertex* getInstance();

	private:
		/*
		* ������������
		*/
		void createVertex();

	public:
		//X�ֿ���ת���ģ�
		QVector<QVector3D> _XVertices2;

		//����ͷ�������ݣ������� ���� ���� �����غ� ��ʼ�ٶȣ�
		QVector<QVector3D> _Arrow_Vertices, _Arrow_Vertices_2; //_2����˫��ͷ����
		//λ�� �ٶ� ���ٶ� ����Լ��
		QVector<QVector3D> _Cone_Vertices_0, _Cylinder_Vertices_0, _Cone_Line_Vertices, _Cylinder_Vertices;
		//�����ο򣨳�ʼӦ���������ܶ�,�¶��غɣ�
		QVector<QVector3D> _TriVertices;
		//���ο򣨶������ȣ�
		QVector<QVector3D> _RhombusVertices;
		//ʮ�ֿ��ڲ���Դ��
		QVector<QVector3D> _XVertices;
		//��������ͼ�꣨���Դ������Դ��
		QVector<QVector3D> _LightningVertices;
		//������ͼ�꣨�¶�Լ����
		QVector<QVector3D> _HomeVertices;
		//������ͼ�꣨��ʾ����󶨡�ʧЧ�����ӣ�
		QVector<QVector3D> _QuadVertices;
		//�������Ӷ�������
		QVector<QVector3D> _Revolute_Vertex; //��ת����������
		QVector<QVector3D> _PlanarRC_Vertex; //ƽ�渱��������
		QVector<QVector3D> _Cylindrical_Vertex; //Բ������������
		QVector<QVector3D> _Prismatic_Vertex; //ƽ�Ƹ���������
		QVector<QVector3D> _Sphere_Vertex; //Բ��¸�

	private:
		static mLableShapeVertex* _instance;

	};
}
