#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QHash>
#include <QVector3D>
#include "Array.h"

#include "SpaceTree.h"

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "mGroupRender.h"

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace MDataPost
{
	class mOneFrameData1;
	class mDataPost1;
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshFaceData1;
	class mPostMeshLineData1;
}
using namespace MViewBasic;
using namespace MBaseRend;
using namespace mxr;
namespace MPostRend
{
	////������
	//class RENDVIEW_EXPORT mPostPartFaceRender
	//{
	//public:
	//	//0 - ԭʼ���ꣻ1 - ������ɫ��2 - ������ɫ������ֵ0, 1��3 - ����ֵ��
	//	//4 - λ��ֵ��
	//	//QVector3D, GLubyte, GLubyte, float, QVector3D, GLubyte, QVector3D, float
	//	std::shared_ptr<mxr::Vec3Array>  _vertex0;
	//	std::shared_ptr<mxr::Vec3Array>  _vertex1;
	//	std::shared_ptr<mxr::FloatArray> _vertex2;
	//	std::shared_ptr<mxr::FloatArray> _vertex3;
	//	std::shared_ptr<mxr::Vec3Array>  _vertex4;
	//	//std::shared_ptr<mxr::FloatArray> _vertex5;
	//	//std::shared_ptr<mxr::Vec3Array>  _vertex6;
	//	//std::shared_ptr<mxr::FloatArray> _vertex8;
	//	mPostPartFaceRender(std::shared_ptr<mxr::Group> parent);
	//	~mPostPartFaceRender();
	//	std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
	//	void setStateSet(std::shared_ptr<mxr::StateSet> state);
	//private:
	//	std::shared_ptr<mxr::Drawable> _drawable;
	//	std::shared_ptr<mxr::Group> _parent;

	//};

	////͸�������ޱ��ε���
	//class mPostPartFaceTransparentNodeformationRender
	//{
	//public:
	//	//meshline stateset
	//	std::shared_ptr<mxr::Vec3Array>  _vertex0;
	//	//std::shared_ptr<mxr::Vec3Array>  _vertex1;
	//	mPostPartFaceTransparentNodeformationRender(std::shared_ptr<mxr::Group> parent);
	//	~mPostPartFaceTransparentNodeformationRender();
	//	std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
	//	void setStateSet(std::shared_ptr<mxr::StateSet> state);
	//private:
	//	std::shared_ptr<mxr::Drawable> _drawable;
	//	std::shared_ptr<mxr::Group> _parent;

	//};

	////��ʾ��������
	//class mPostPartFaceLineRender
	//{
	//public:
	//	//meshline stateset
	//	std::shared_ptr<mxr::Vec3Array>   _vertex0;
	//	std::shared_ptr<mxr::Vec3Array>   _vertex1;
	//	std::shared_ptr<mxr::FloatArray>  _vertex2;
	//	mPostPartFaceLineRender(std::shared_ptr<mxr::Group> parent);
	//	~mPostPartFaceLineRender();
	//	std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
	//	void setStateSet(std::shared_ptr<mxr::StateSet> state);
	//private:
	//	std::shared_ptr<mxr::Drawable> _drawable;
	//	std::shared_ptr<mxr::Group> _parent;

	//};

	////������
	//class RENDVIEW_EXPORT mPostPartLineRender
	//{
	//public:
	//	//0 - ԭʼ���ꣻ1 - ������ɫ��2 - ������ɫ������ֵ0, 1��3 - ����ֵ��
	//	//4 - λ��ֵ�� 5 - �Ƿ���ֵ��û��ֵ�Ļ���ʾ��ɫ����ֵ�Ļ�����IsColor�ж�
	//	//QVector3D, GLubyte, GLubyte, float, QVector3D, GLubyte, QVector3D, float
	//	std::shared_ptr<mxr::Vec3Array>  _vertex0;
	//	std::shared_ptr<mxr::Vec3Array>  _vertex1;
	//	std::shared_ptr<mxr::FloatArray> _vertex2;
	//	std::shared_ptr<mxr::FloatArray> _vertex3;
	//	std::shared_ptr<mxr::Vec3Array>  _vertex4;
	//	//std::shared_ptr<mxr::FloatArray> _vertex5;
	//	mPostPartLineRender(std::shared_ptr<mxr::Group> parent);
	//	~mPostPartLineRender();
	//	std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
	//	void setStateSet(std::shared_ptr<mxr::StateSet> state);
	//private:
	//	std::shared_ptr<mxr::Drawable> _drawable;
	//	std::shared_ptr<mxr::Group> _parent;

	//};

	////������
	//class RENDVIEW_EXPORT mPostPartPointRender
	//{
	//public:
	//	//0 - ԭʼ���ꣻ1 - ������ɫ��2 - ������ɫ������ֵ���Ƿ��ɫ0, 1, 2, 3��3 - ����ֵ��
	//	//4 - λ��ֵ
	//	//QVector3D, GLubyte, GLubyte, float, QVector3D, GLubyte, QVector3D, float
	//	std::shared_ptr<mxr::Vec3Array>  _vertex0;
	//	std::shared_ptr<mxr::Vec3Array>  _vertex1;
	//	std::shared_ptr<mxr::FloatArray> _vertex2;
	//	std::shared_ptr<mxr::FloatArray> _vertex3;
	//	std::shared_ptr<mxr::Vec3Array>  _vertex4;
	//	//std::shared_ptr<mxr::FloatArray> _vertex5;
	//	mPostPartPointRender(std::shared_ptr<mxr::Group> parent);
	//	~mPostPartPointRender();
	//	std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
	//	void setStateSet(std::shared_ptr<mxr::StateSet> state);
	//private:
	//	std::shared_ptr<mxr::Drawable> _drawable;
	//	std::shared_ptr<mxr::Group> _parent;

	//};

	////��ʾ�ı߽���
	//class mPostPartEdgeLineRender
	//{
	//public:
	//	//meshline stateset
	//	std::shared_ptr<mxr::Vec3Array>   _vertex0;
	//	std::shared_ptr<mxr::Vec3Array>   _vertex1;
	//	//std::shared_ptr<mxr::FloatArray>   _vertex2;
	//	mPostPartEdgeLineRender(std::shared_ptr<mxr::Group> parent);
	//	~mPostPartEdgeLineRender();
	//	std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
	//	void setStateSet(std::shared_ptr<mxr::StateSet> state);
	//private:
	//	std::shared_ptr<mxr::Drawable> _drawable;
	//	std::shared_ptr<mxr::Group> _parent;

	//};

	class RENDVIEW_EXPORT mPostPartRender
	{
	public:

		float _minValue, _maxValue;

		mPostPartRender(std::shared_ptr<mxr::Group> parent, MDataPost::mOneFrameData1 *oneFrameData, MDataPost::mPostMeshPartData1 *part);

		~mPostPartRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		void calculateSpaceTreeThread(const QHash<int, QVector3D> &dis);
		Space::SpaceTree* getPartSpaceTree();
		MDataPost::mPostMeshPartData1 *getpartData() {return _partData; };

		void ImportPart(QVector3D deformationScale, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void UpdatePartVariableValue(QHash<int, float> &values, MViewBasic::NodeOrElement type);

		void setShowFuntion(ShowFuntion showFuntion);
		void setIsShowInitialShape(bool isShowInitialShape);//�����Ƿ���ʾ��ʼ���͵�͸��ͼ��
		std::shared_ptr<mxr::Drawable> getFaceDrawable() { return _facerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getLineDrawable() { return _linerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getPointDrawable() { return _pointrend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getFaceTransparentNodeformationDrawable() { return _facetransparentnodeformationrend->getDrawable(); };
		std::shared_ptr<mxr::Geode> getGeode() { return _geode; }

	private:
		void AppendMesh0(MDataPost::mPostMeshData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMesh1(MDataPost::mPostMeshData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMesh2(MDataPost::mPostMeshData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMesh3(MDataPost::mPostMeshFaceData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMeshLine(MDataPost::mPostMeshLineData1 * mesh, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);

		void GetVariableValue_Mesh0(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
		void GetVariableValue_Mesh1(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
		void GetVariableValue_Mesh2(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
		void GetVariableValue_Mesh3(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshFaceData1 *mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
	private:
		shared_ptr<mxr::Group> _parent;//���ڵ�
		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _facerend;//��
		std::shared_ptr<mGroupRender1<Vec3Array>> _facetransparentnodeformationrend;//����ǰ͸������
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _edgelinerend;//�߽���
		std::shared_ptr<mGroupRender3<Vec3Array, Vec3Array, FloatArray>>  _facelinerend;//����
		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _linerend;//��
		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _pointrend;//��

		QVector3D deformationScale;
		MDataPost::mOneFrameData1 *_oneFrameData;
		MDataPost::mPostMeshPartData1 *_partData;

		Space::SpaceTree *_spaceTree;
	};
}
