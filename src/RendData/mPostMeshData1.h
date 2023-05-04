#pragma once
#include "renddata_global.h"

#include <QVector>
#include <QObject>
#include <set>

#include "mBasicEnum.h"

namespace MDataPost
{
	class mPostMeshFaceData1;
	class mPostMeshNodeData1;
	class RENDDATA_EXPORT mPostMeshData1
	{
	public:
		

		mPostMeshData1(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType eleType, QString partName);

		~mPostMeshData1();

		//��������Ľڵ�
		void setNodeIndex(QVector<int> index);

		//��ȡ����Ľڵ�
		QVector<int> getNodeIndex();
	

		//��ȡ�������������
		MBasicFunction::MeshType getMeshType();

		//��������ĵ�Ԫ����
		void setElementType(MBasicFunction::ElementType eleType);

		//��ȡ����ĵ�Ԫ����
		MBasicFunction::ElementType getElementType();

		//��������Ӧ��������
		void appendMeshFace(mPostMeshFaceData1* meshFace);

		//���������Ӧ��������
		void setMeshFace(uint8_t location, mPostMeshFaceData1* meshFace);

		//���������Ӧ��������
		//void setMeshFace(const QVector<int>& ids);

		//��ȡ�����Ӧ��������
		QVector<int> getMeshFaces();
		QVector<mPostMeshFaceData1*> getFace();
		//��������Ӧ��������
		void appendMeshLine(int meshLineID);

		//��ȡ�����Ӧ��������
		QVector<int> getMeshLines();

		//���������Ӧ��������
		void setMeshLine(const QVector<int>& ids);

		//������������
		void setMeshVisual(bool isshow);

		//������������
		void reverseMeshVisual();

		//��ȡ�����Ƿ�����
		bool getMeshVisual();

		//��ȡ�����ά��
		int getMeshDimension();

		//��ȡ�����ID
		int getMeshID();

		//��ȡ���ڵĲ�������
		QString getPartName();

	private:
		QVector<int> _index;
		mPostMeshFaceData1* faces[6] = { nullptr };	
		//�����µ����е�Ԫ��ID
		//QVector<int> _meshLineIDs;
		//�������ڵĲ�������
		QString _partName;
		//��������
		MBasicFunction::MeshType _meshType;
		//��Ԫ����
		MBasicFunction::ElementType _eleType;
		//����ID
		int _meshID;
		//�����Ƿ���ʾ
		bool _isShow;
		//��Ĵ�С
		uint8_t _location = 0;


	};
}
