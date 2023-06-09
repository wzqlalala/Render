#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QVector2D>
#include <QHash>
#include <QVector>
#include <QVector3D>
#include <QFutureWatcher>

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
namespace MDataPost
{
	class mDataPost1;
	class mOneFrameData1;
	class mPostMeshFaceData1;
	class mPostMeshData1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
using namespace MDataPost;
namespace MPostRend
{
	struct VertexData2
	{
		QVector<QVector3D> vertexs;
		QVector<float> values;
	};
	class RENDVIEW_EXPORT mPostContourRender:public QObject
	{
		Q_OBJECT
	public:

		mPostContourRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData = nullptr);

		~mPostContourRender();

		void setLineStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void updateData(float minValue, float maxValue, int num = 10);

		void updateData(float minValue, float maxValue, QVector<float> values = {});

		void setVisiable(bool isshow);

		void createPostContourData();

		void resetDrawable();

	private slots:

		void finishedWork();

	private:

		VertexData2 calculateContour3D(QVector<MDataPost::mPostMeshData1*> meshDatas);

		VertexData2 calculateContour2D(QVector<MDataPost::mPostMeshData1*> meshDatas);

		void calculateline(QVector<int> index, QVector<QVector3D> &lines, QVector<float> &values);

		void calculateface(QVector<int> index, QVector<QVector3D> &faces, QVector<float> &values);

		void calculatefaceByTet(QVector<QVector3D> vertexs, QVector<float> nodevalues, float minValue, float maxValue, QVector<QVector3D> &faces, QVector<float> &values);

	private:

		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;//���ڵ�
		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		//��ֵ��
		std::shared_ptr<mxr::Drawable> _lineDrawable;
		std::shared_ptr<mxr::StateSet> _lineStateSet;

		//��ֵ��
		std::shared_ptr<mxr::Drawable> _faceDrawable;
		std::shared_ptr<mxr::StateSet> _faceStateSet;

		MDataPost::mOneFrameData1* _oneFrameData;
		mPostOneFrameRendData *_currentFrameRendData;
		QHash<int, float> _values;
		QHash<int, QVector3D> _dis;
		QVector3D _deformationScale;

		//�Ƿ���Ⱦ
		bool _isShow;

		//��ֵ�߲�������(�����СΪ��ֵ�߲���)
		QVector<float> _contourValues;

		//int _linenum;

		QVector<QVector3D> _linevertexs;
		QVector<float> _linevertexvalues;

		//int _facenum;
		QVector<QVector3D> _facevertexs;
		QVector<float> _facevertexvalues;

		QFutureWatcher<void> w;
	};


}
