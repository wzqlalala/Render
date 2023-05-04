#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

//MBasicFunction
#include "mPostEnum.h"
#include "mBasicStructor.h"

//Qt
#include <QVector3D>
#include <QVector>

using namespace MBasicFunction;
namespace MDataPost
{
	class RENDDATA_EXPORT mPostRigidWallDataBase
	{
	public:

		/*
		* ���캯��
		*/
		mPostRigidWallDataBase(int ID, PostRigidWallType rigidWallType, double *normalStartEndPos);

		/*
		* ��������
		*/
		~mPostRigidWallDataBase() {};

		/*
		* ��ȡ����ǽID
		*/
		int getRigidWallID();

		/*
		* ��ȡ����ǽ����
		*/
		PostRigidWallType getRigidWallType();

		/*
		* ��ȡ����ǽ������ʼ����
		*/
		QVector3D getRigidWallStartPos();

		/*
		* ��ȡ����ǽ������ֹ����
		*/
		QVector3D getRigidWallEndPos();

		/*
		* ���ø���ǽ��������������
		*/
		void setFaceVertex(QVector<QVector3D> vertexs);

		/*
		* ��ȡ����ǽ��������������
		*/
		virtual QVector<QVector3D> getFaceVertex() { return QVector<QVector3D>(); };

		/*
		* ��ȡ����ǽ�ı߽�������
		*/
		virtual QVector<QVector3D> getLineVertex() { return QVector<QVector3D>(); };

		/*
		* ��ȡ����ǽ�ĳߴ�
		*/
		virtual ModelSize getRigidWallSize() { return _modelSize; };

	protected:

		//�������ǽ��ԭʼ��Χ��
		void caculateRigidWallSize();

	protected:

		//����ǽID
		int _rigidWallID;

		//����ǽ����
		PostRigidWallType _rigidWallType;

		//������ʼ����
		QVector3D _normalStartPos;

		//������ֹ����
		QVector3D _normalEndPos;

		//�߽�������
		QVector<QVector3D> _lineVertexs;

		//��������������
		QVector<QVector3D> _faceVertexs;

		//����ǽ�ߴ�
		ModelSize _modelSize;
	};

	class RENDDATA_EXPORT mPostInfinitelyLargePlaneRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* ���캯��
		*/
		mPostInfinitelyLargePlaneRigidWall(int ID, double *normalStartEndPos);

		/*
		* ��������
		*/
		~mPostInfinitelyLargePlaneRigidWall() {};

		//��ȡ������
		QVector<QVector3D> getFaceVertex();

		//��ȡ������
		QVector<QVector3D> getLineVertex();

	};

	class RENDDATA_EXPORT mPostLimitedLargePlaneRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* ���캯��
		*/
		mPostLimitedLargePlaneRigidWall(int ID, double *normalStartEndPos);

		/*
		* ��������
		*/
		~mPostLimitedLargePlaneRigidWall() {};

		//��ȡ������
		QVector<QVector3D> getFaceVertex();

		//��ȡ������
		QVector<QVector3D> getLineVertex();

		//��ȡ����ǽ�ĳߴ�
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

	public:
		//����ǽ��������ֹ����
		double _lineEndVector[3];

		//�߳�L
		double _L;

		//�߳�M
		double _M;

	};

	class RENDDATA_EXPORT mPostRectangularRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* ���캯��
		*/
		mPostRectangularRigidWall(int ID, double *normalStartEndPos);

		/*
		* ��������
		*/
		~mPostRectangularRigidWall() {};

		//��ȡ������
		QVector<QVector3D> getFaceVertex();

		//��ȡ������
		QVector<QVector3D> getLineVertex();

		//��ȡ����ǽ�ĳߴ�
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

	public:
		//����ǽ��������ֹ����
		double _lineEndVector[3];

		//�߳�L
		double _L;

		//�߳�M
		double _M;

		//�߳�P
		double _P;

	};

	class RENDDATA_EXPORT mPostCylindricalRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* ���캯��
		*/
		mPostCylindricalRigidWall(int ID, double *normalStartEndPos);

		/*
		* ��������
		*/
		~mPostCylindricalRigidWall() {};

		//��ȡ������
		QVector<QVector3D> getFaceVertex();

		//��ȡ������
		QVector<QVector3D> getLineVertex();

		//��ȡ����ǽ�ĳߴ�
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

	public:
		//�뾶
		double _radius;

		//���򳤶�
		double _length;

	};

	class RENDDATA_EXPORT mPostSphericalRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* ���캯��
		*/
		mPostSphericalRigidWall(int ID, double *normalStartEndPos);

		/*
		* ��������
		*/
		~mPostSphericalRigidWall() {};

		//��ȡ������
		QVector<QVector3D> getFaceVertex();

		//��ȡ������
		QVector<QVector3D> getLineVertex();

		//��ȡ����ǽ�ĳߴ�
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

		//
		float distSquare(QVector3D a, QVector3D b);

		//�����ƽ���ߺ�����Ľ���
		QVector3D midArcPoint(QVector3D a,QVector3D b);

	public:
		//�뾶
		double _radius;

	};

}
