#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"
#include <QObject>
#include <set>
#include <QHash>

//MBasicFunction
#include "mBasicStructor.h"


namespace MDataGeo
{
	using namespace std;
	using namespace MBasicFunction;
	class RENDDATA_EXPORT mGeoPickData1
	{

	public:

		//�������ʰȡ���Ǽ���ʰȡ
		void setGeoPickFunction(int pickfunction);//0��1��

		//ʰȡʱ����
		void setSoloPickGeoPointData(int Pointid, float depth);
		void setSoloPickGeoLineData(int Lineid, float depth);
		void setSoloPickGeoFaceData(int Faceid, float depth);
		void setSoloPickGeoSolidData(int Solidid, float depth);
		void setSoloPickVertexOnGeoLineData(int geoLineID, QVector3D vertex, float depth);
		void setSoloPickVertexOnGeoFaceData(int geoFaceID, QVector3D vertex, float depth);
		void setSoloPickVertexOnScreenData(QVector3D vertex);
		void setSoloPickGeoPointByPartData(set<int> Pointids, float depth);
		void setSoloPickGeoLineByPartData(set<int> Lineids, float depth);
		void setSoloPickGeoFaceByPartData(set<int> Faceids, float depth);
		void setSoloPickGeoSolidByPartData(set<int> Solidids, float depth);
		void setSoloPickGeoPartData(QString partName, float depth);


		void setSoloPickGeoSolidByPartData(QString PartName, float depth);

		void setMutiplyPickGeoPointData(set<int> Pointids);
		void setMutiplyPickGeoLineData(set<int> Lineids);
		void setMutiplyPickGeoFaceData(set<int> Faceids);
		void setMutiplyPickGeoSolidData(set<int> Solidids);
		void setMutiplyPickGeoPartData(QString partName);

		//һ���Ը����������һ�θ���
		void setAllPickGeoPointData(set<int> Pointids);
		void setAllPickGeoLineData(set<int> Lineids);
		void setAllPickGeoFaceData(set<int> Faceids);
		void setAllPickGeoSolidData(set<int> Solidids);
		void setAllPickGeoPartData(set<QString> partNames);

		//�ۻ�����
		void setAddPickGeoPointData(set<int> Pointids);
		void setAddPickGeoLineData(set<int> Lineids);
		void setAddPickGeoFaceData(set<int> Faceids);
		void setAddPickGeoSolidData(set<int> Solidids);
		void setAddPickGeoPartData(set<QString> partNames);

		//���ٸ���
		void setReducePickGeoPointData(set<int> Pointids);
		void setReducePickGeoLineData(set<int> Lineids);
		void setReducePickGeoFaceData(set<int> Faceids);
		void setReducePickGeoSolidData(set<int> Solidids);
		void setReducePickGeoPartData(set<QString> partNames);

		//��ѡʰȡ��ɺ����
		void setSoloPickData();

		//��ȡʰȡ��ļ��ε���
		set<int> getPickPointIDs();

		//��ȡʰȡ��ļ����߱��
		set<int> getPickLineIDs();

		//��ȡʰȡ��ļ�������
		set<int> getPickFaceIDs();

		//��ȡʰȡ��ļ�������
		set<int> getPickSolidIDs();

		//��ȡʰȡ��ļ��β�������
		set<QString> getPickGeoPartNames();

		//��ȡʰȡ��ļ������ϵ����������
		QHash<int, QVector<QVector3D>> getPickVertexOnGeoLineDatas();

		//��ȡʰȡ��ļ������ϵ����������
		QHash<int, QVector<QVector3D>> getPickVertexOnGeoFaceDatas();

		//��ȡʰȡ�����Ļ�ϵ����������
		QVector<QVector3D> getPickVertexOnScreenDatas();

		//������м��εĸ���
		void clearAllPickData();

		//Ĭ�Ϲ��캯��
		mGeoPickData1();

		//��������
		~mGeoPickData1();


	private:

		int _pickFunction;//0��1��

		IDDepthBuffer _solidBuffer;//��ǰ��������

		IDDepthBuffer _faceBuffer;//��ǰ��������

		IDDepthBuffer _lineBuffer;//��ǰ�����߱��

		IDDepthBuffer _pointBuffer;//��ǰ���ε���

		NameDepthBuffer _partBuffer;//��ǰ�������

		VertexOnGeoLineDepthBuffer _vertexOnLineBuffer;//��ǰ���ڼ������ϵ�����

		VertexOnGeoFaceDepthBuffer _vertexOnFaceBuffer;//��ǰ���ڼ������ϵ�����

		IDSetDepthBuffer _solidBuffers;//������ʰȡ������ı��

		IDSetDepthBuffer _faceBuffers;//������ʰȡ������ı��

		IDSetDepthBuffer _lineBuffers;//������ʰȡ�����ߵı��

		IDSetDepthBuffer _pointBuffers;//������ʰȡ���ε�ı��

		NameDepthBuffer _partSolidBuffer;//��ǰ������������

		NameDepthBuffer _partFaceBuffer;//��ǰ������������

		NameDepthBuffer _partLineBuffer;//��ǰ���������߱��

		NameDepthBuffer _partPointBuffer;//��ǰ�������ε���

		set<int> _pickPoints;//����ʰȡ���ļ��ε�
		set<int> _pickLines;//����ʰȡ���ļ�����
		set<int> _pickFaces;//����ʰȡ���ļ�����
		set<int> _pickSolids;//����ʰȡ���ļ�����
		set<QString> _pickParts;//����ʰȡ���ļ��β���
		QHash<int, QVector<QVector3D>> _pickVertexsOnLines;//����ʰȡ���ļ��α��ϵ������
		QHash<int, QVector<QVector3D>> _pickVertexsOnFaces;//����ʰȡ���ļ������ϵ������
		QVector<QVector3D> _pickVertexsOnScreen;//����ʰȡ������Ļ�ϵĵ�
	};
}
