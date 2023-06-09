#pragma once
#include "rendview_global.h"

#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include <QHash>

#include <memory>

namespace MDataPost
{
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshNodeData1;
	class mPostMeshData1;
	class mPostMeshLineData1;
	class mPostMeshFaceData1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
namespace mxr
{
	class Application;
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
using namespace MDataPost;
namespace MPostRend
{
	class mPostRendStatus;
	class RENDVIEW_EXPORT mPostStreamLineData1
	{
	public:
		mPostStreamLineData1(mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData = nullptr, int num = 500, float ratio = 0.1);

		~mPostStreamLineData1();

		void setSphereParameter(QVector3D center, float radius);

		void setStreamLineNum(int num = 500);

		void OrganizeModeldata();

		QVector<QVector3D> getForwardStreamLineVertex();

		QVector<float> getForwardStreamLineTexture();

		QVector<QVector3D> getBackwardStreamLineVertex();

		QVector<float> getBackwardStreamLineTexture();

	private:

		//������ָ������������
		void splitHexahedron(QVector<int> index, MDataPost::mPostMeshData1 *meshData, MDataPost::mPostMeshPartData1 *partData);

		//�������ָ������������
		void splitWedge(QVector<int> index, MDataPost::mPostMeshData1 *meshData, MDataPost::mPostMeshPartData1 *partData);

		//����׶�ָ������������
		void splitPyramid(QVector<int> index, MDataPost::mPostMeshData1 *meshData, MDataPost::mPostMeshPartData1 *partData);

		//����������
		void createTetrahedron(int meshID, QVector<int> index, MDataPost::mPostMeshPartData1 *partData);

		//����һ�����ߣ�trueorfalse����������������߻��֣�pmesID�������ߵ���ʼ����ID,lineVertex����Ҫ��Ӷ���������飬lineTexture����Ҫ��������������
		void caculateStreamLineData(bool trueorfalse,int pmeshID,QVector<QVector3D> &lineVertex, QVector<float> &lineTexture);

		//�ж�һ�����Ƿ���һ����������
		bool jdugePointIsInTetrahedron(QVector<QVector3D> vertexs,QVector3D point);

		//ͨ����ֵ����һ�������������е�ʸ��ֵ
		QVector3D caculatePointValueInTetrahedron(QVector3D point, QVector<QVector3D> vertexs, QVector<QVector3D> vectorValues);

		//ͨ����ֵ����һ�������������еı���ֵ
		float caculatePointValueInTetrahedron(QVector3D point, QVector<QVector3D> vertexs, QVector<float> values);

		//ͨ����ֵ����һ�������������еı���ֵ
		QPair<QVector3D,float> caculatePointValueInTetrahedron(QVector3D point, QVector<QVector3D>vertexs, QVector<QVector3D> vectorValues, QVector<float> values);
	private:
		mPostOneFrameRendData *_currentFrameRendData;

		//�����ʸ��ֵ
		QHash<int, QVector3D> _vectorValue;

		//����ı���ֵ
		QHash<int, float> _value;

		//��ֵ
		float _maxval;
		float _minval;
		float _fenmu;

		//���򴫳����ߵ�ֵ
		QVector<QVector3D> _forwardLineVertex;
		QVector<float> _forwardlineTexture;

		//���򴫳����ߵ�ֵ
		QVector<QVector3D> _backwardLineVertex;
		QVector<float> _backwardLineTexture;

		//���������ģ������
		QHash<int, MDataPost::mPostMeshNodeData1*> _nodeData1;
		QHash<int, MDataPost::mPostMeshData1*> _meshData1;
		QHash<QString, MDataPost::mPostMeshPartData1*> _partData1;
		//��Ԫ������
		QHash<QSet<int>, QVector<int>> _gongxianwangge;//���ߵ�����

		//���ֲ���
		int _integrationSteps;

		//ȷ����ʼ�����
		QVector3D _sphereCenter;
		float _sphereRadius;

		//�����ܶȣ�����������
		int _streamLineNum;

		//���ֱ���
		float _integrateRatio;

		//��������
		mOneFrameData1 *_oneFrameData;
	};


	class RENDVIEW_EXPORT mPostStreamLineRender
	{

	public:
		mPostStreamLineRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mOneFrameData1 *oneFrameData,std::shared_ptr<mPostRendStatus> rendStatus, mPostOneFrameRendData *currentFrameRendData = nullptr, int num = 500, float ratio = 0.1);

		~mPostStreamLineRender();

		void setLineStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void setPointStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void setSphereParameter(QVector3D center, float radius);

		void setStreamLineNum(int num = 500);

		void setStreamLineShowForm(int streamLineShowForm = 0);//0����������1�����

		void setIntergrateDirection(int intergrateDirection = 0);//0����˫��1��������2������

		void OrganizeModeldata();

	private:

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mPostStreamLineData1 *_postStreamLineData;

		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;//���ڵ�
		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		//��ֵ��
		std::shared_ptr<mxr::Drawable> _lineDrawable;
		std::shared_ptr<mxr::StateSet> _lineStateSet;
		std::shared_ptr<mxr::StateSet> _pointStateSet;
	};
}

