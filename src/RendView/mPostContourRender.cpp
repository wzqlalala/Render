#include "mPostContourRender.h"
#include "mPostOneFrameRendData.h"

#include <renderpch.h>

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshNodeData1.h"

//MDatabase
#include "mPostMeshData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostMeshPartData1.h"

#include <algorithm>
#include <math.h>
#include <QtConcurrent/QtConcurrent>

#define refinement true

using namespace MDataPost;
using namespace std;
namespace MPostRend
{
	mPostContourRender::mPostContourRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData)
	{
		_app = app;
		_parent = parent;
		_oneFrameData = oneFrameData;
		_currentFrameRendData = currentFrameRendData;

		_lineDrawable = nullptr;
		_faceDrawable = nullptr;

		_geode = MakeAsset<mxr::Geode>();
		_parent->addChild(_geode);
	}

	mPostContourRender::~mPostContourRender()
	{
		
	}

	void mPostContourRender::setLineStateSet(std::shared_ptr<mxr::StateSet> stateSet)
	{
		_lineStateSet = stateSet;
	}

	void mPostContourRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> stateSet)
	{
		_faceStateSet = stateSet;
	}

	void mPostContourRender::updateData(float minValue, float maxValue, int num)
	{
		//resetDrawable();
		_isShow = true;

		float step = (maxValue - minValue) / (num + 1);
		_contourValues.append(minValue + step);
		for (int i = 1; i < num; i++)
		{
			_contourValues.append(_contourValues.last() + step);
		}
		QFuture<void> future;
		future = QtConcurrent::run(this, &mPostContourRender::createPostContourData);
		QObject::connect(&w, &QFutureWatcher<void>::finished, this, &mPostContourRender::finishedWork);
		w.setFuture(future);

	}

	void mPostContourRender::updateData(float minValue, float maxValue, QVector<float> values)
	{
		_contourValues = values;

		if (values.size() == 1)
		{
			if (values.first() > maxValue || values.first() < minValue)
			{
				//_isUpdateBuffer = true;
				return;
			}
		}

		QFuture<void> future;
		future = QtConcurrent::run(this, &mPostContourRender::createPostContourData);
		QObject::connect(&w, &QFutureWatcher<void>::finished, this, &mPostContourRender::finishedWork);
		w.setFuture(future);
	}

	void mPostContourRender::setVisiable(bool isshow)
	{
		_isShow = isshow;
		if (_isShow)
		{
			_geode->setNodeMask(0);
		}
		else
		{
			_geode->setNodeMask(1);
		}
	}

	void mPostContourRender::createPostContourData()
	{
		if (_currentFrameRendData == nullptr)
		{
			return;
		}
		int frameID = _currentFrameRendData->getRendID();
		_values = _currentFrameRendData->getRendValue();
		_dis = _currentFrameRendData->getNodeDisplacementData();
		_deformationScale = _currentFrameRendData->getDeformationScale();
		//qDebug() << QThreadPool::globalInstance()->maxThreadCount();
		//qDebug() << QThreadPool::globalInstance()->activeThreadCount();
		//QThreadPool::globalInstance()->setMaxThreadCount(20);
		QVector<MDataPost::mPostMeshData1*> meshDatas = _oneFrameData->getAll3DMeshIDs();
		int num = meshDatas.size();
		int n = 10;
		int step = num / (n + 1);
		//QVector<VertexData> ress3(n + 1);
		QVector<QFuture<VertexData2>> futures;
		if (num != 0)
		{
			for (int i = 0; i < n; i++)
			{
				futures.append(QtConcurrent::run(this, &mPostContourRender::calculateContour3D, meshDatas.mid(step * i, step)));
			}
			futures.append(QtConcurrent::run(this, &mPostContourRender::calculateContour3D, meshDatas.mid(step * n, num - step * n)));
		}
		while (!futures.empty())
		{
			futures.back().waitForFinished();
			_facevertexs.append(futures.back().result().vertexs);
			_facevertexvalues.append(futures.back().result().values);
			futures.takeLast();
		}
		meshDatas = _oneFrameData->getAll2DMeshIDs();
		num = meshDatas.size();
		step = num / (n + 1);
		QVector<QFuture<VertexData2>> futures1;
		if (num != 0)
		{
			for (int i = 0; i < n; i++)
			{
				futures1.append(QtConcurrent::run(this, &mPostContourRender::calculateContour2D, meshDatas.mid(step * i, step)));
			}
			futures1.append(QtConcurrent::run(this, &mPostContourRender::calculateContour2D, meshDatas.mid(step * n, num - step * n)));
		}
		while (!futures1.empty())
		{
			futures1.back().waitForFinished();
			_linevertexs.append(futures1.back().result().vertexs);
			_linevertexvalues.append(futures1.back().result().values);
			futures1.takeLast();
		}
	}

	void mPostContourRender::resetDrawable()
	{
		if (_lineDrawable)
		{
			_geode->removeChild(_lineDrawable);
		}

		if (_faceDrawable)
		{
			_geode->removeChild(_faceDrawable);
		}

		_lineDrawable = MakeAsset<mxr::Drawable>();
		_faceDrawable = MakeAsset<mxr::Drawable>();
		_lineDrawable->setStateSet(_lineStateSet);
		_faceDrawable->setStateSet(_faceStateSet);
		_geode->addChild(_lineDrawable);
		_geode->addChild(_faceDrawable);
	}

	void mPostContourRender::finishedWork()
	{
		resetDrawable();

		_app->GLContext()->makeCurrent(_app->GLContext()->surface());

		_lineDrawable->setVertexAttribArray(0, MakeAsset<mxr::Vec3Array>(_linevertexs));
		_lineDrawable->setVertexAttribArray(1, MakeAsset<mxr::FloatArray>(_linevertexvalues));

		_faceDrawable->setVertexAttribArray(0, MakeAsset<mxr::Vec3Array>(_facevertexs));
		_faceDrawable->setVertexAttribArray(1, MakeAsset<mxr::FloatArray>(_facevertexvalues));
	}

	VertexData2 mPostContourRender::calculateContour3D(QVector<MDataPost::mPostMeshData1*> meshDatas)
	{
		//QVector<QVector3D> faces;
		//QVector<float> values;
		//QVector<QVector3D> normals;
		VertexData2 vertexData;
		for (MDataPost::mPostMeshData1* meshData : meshDatas)
		{
			if (meshData == nullptr)
			{
				continue;
			}
			QVector<int> index = meshData->getNodeIndex();
			this->calculateface(index, vertexData.vertexs, vertexData.values);
		}
		//vertexDatas = vertexData;
		return vertexData;
	}


	VertexData2 mPostContourRender::calculateContour2D(QVector<MDataPost::mPostMeshData1*> meshDatas)
	{
		VertexData2 vertexData;
		for (MDataPost::mPostMeshData1* meshData : meshDatas)
		{
			if (meshData == nullptr)
			{
				continue;
			}
			QVector<int> index = meshData->getNodeIndex();
			this->calculateline(index, vertexData.vertexs, vertexData.values);
		}

		return vertexData;
	}
	void mPostContourRender::calculateline(QVector<int> index, QVector<QVector3D>& lines, QVector<float>& values)
	{
		if (index.size() == 3)//三角形
		{
			QVector<float> nodevalues(3);
			nodevalues[0] = _values.value(index.at(0));
			nodevalues[1] = _values.value(index.at(1));
			nodevalues[2] = _values.value(index.at(2));

			QVector<QVector3D> vertexs(3);
			vertexs[0] = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + _deformationScale * _dis.value(index.at(0));
			vertexs[1] = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + _deformationScale * _dis.value(index.at(1));
			vertexs[2] = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + _deformationScale * _dis.value(index.at(2));

			for (float value : _contourValues)
			{
				//lines.append((vertexs[0] + vertexs[1]) / 2.0); lines.append((vertexs[1] + vertexs[2]) / 2.0);
				float t = (value - nodevalues[0]) / (nodevalues[1] - nodevalues[0]);//0 1 1 2 2 0
				if (t < 1 && t > 0)//第一条边存在等值点
				{
					lines.append(vertexs[0] + t * (vertexs[1] - vertexs[0]));
					t = (value - nodevalues[1]) / (nodevalues[2] - nodevalues[1]);
					if (t < 1 && t > 0)//第二条边存在等值点
					{
						lines.append(vertexs[1] + t * (vertexs[2] - vertexs[1]));
					}
					else
					{
						t = (value - nodevalues[2]) / (nodevalues[0] - nodevalues[2]);
						//if (t < 1 && t > 0)//第三条边存在等值点
						{
							lines.append(vertexs[2] + t * (vertexs[0] - vertexs[2]));
						}
					}
				}
				else//第一条边不存在等值点
				{
					t = (value - nodevalues[1]) / (nodevalues[2] - nodevalues[1]);
					if (t < 1 && t > 0)//第二条边存在等值点
					{
						lines.append(vertexs[1] + t * (vertexs[2] - vertexs[1]));
						t = (value - nodevalues[2]) / (nodevalues[0] - nodevalues[2]);
						//if (t < 1 && t > 0)//第三条边存在等值点
						{
							lines.append(vertexs[2] + t * (vertexs[0] - vertexs[2]));
						}
					}
					else
					{
						continue;
					}
				}
				values.append(value);
				values.append(value);
			}

		}
		else
		{
			QVector<float> nodevalues1(4);
			nodevalues1[0] = _values.value(index.at(0));
			nodevalues1[1] = _values.value(index.at(1));
			nodevalues1[2] = _values.value(index.at(2));
			nodevalues1[3] = _values.value(index.at(3));

			//判断是否存在等值点
			pair<float*, float*> minmaxindex = minmax_element(nodevalues1.begin(), nodevalues1.end());
			float minvalue = *minmaxindex.first; float maxvalue = *minmaxindex.second;
			bool hasContour{ false };
			for (float value : _contourValues)
			{
				if (value < maxvalue && value > minvalue)
				{
					hasContour = true;
					break;
				}
			}

			if (!hasContour)//不存在等值点
			{
				return;
			}

			QVector<QVector3D> vertexs1(4);
			vertexs1[0] = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + _deformationScale * _dis.value(index.at(0));
			vertexs1[1] = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + _deformationScale * _dis.value(index.at(1));
			vertexs1[2] = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + _deformationScale * _dis.value(index.at(2));
			vertexs1[3] = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + _deformationScale * _dis.value(index.at(3));

			QVector3D center = (vertexs1[0] + vertexs1[1] + vertexs1[2] + vertexs1[3]) / 4.0;
			float centervalue = (nodevalues1[0] + nodevalues1[1] + nodevalues1[2] + nodevalues1[3]) / 4.0;
			QVector<QVector<QVector3D>> allvertexs; QVector<QVector<float>> allnodevalues;
			//将一个四边形分割成四个三角形，中心为新生成的点
			allvertexs.append(QVector<QVector3D>{vertexs1[0], vertexs1[1], center});
			allvertexs.append(QVector<QVector3D>{vertexs1[1], vertexs1[2], center});
			allvertexs.append(QVector<QVector3D>{vertexs1[2], vertexs1[3], center});
			allvertexs.append(QVector<QVector3D>{vertexs1[3], vertexs1[0], center});

			allnodevalues.append(QVector<float>{nodevalues1[0], nodevalues1[1], centervalue});
			allnodevalues.append(QVector<float>{nodevalues1[1], nodevalues1[2], centervalue});
			allnodevalues.append(QVector<float>{nodevalues1[2], nodevalues1[3], centervalue});
			allnodevalues.append(QVector<float>{nodevalues1[3], nodevalues1[0], centervalue});

			for (float value : _contourValues)
			{
				if (value < maxvalue && value > minvalue)
				{
					for (int i = 0; i < 4; ++i)
					{
						QVector<QVector3D> vertexs = allvertexs.at(i);
						QVector<float> nodevalues = allnodevalues.at(i);
						float t = (value - nodevalues[0]) / (nodevalues[1] - nodevalues[0]);//0 1 1 2 2 0
						if (t < 1 && t > 0)//第一条边存在等值点
						{
							lines.append(vertexs[0] + t * (vertexs[1] - vertexs[0]));
							t = (value - nodevalues[1]) / (nodevalues[2] - nodevalues[1]);
							if (t < 1 && t > 0)//第二条边存在等值点
							{
								lines.append(vertexs[1] + t * (vertexs[2] - vertexs[1]));
							}
							else
							{
								t = (value - nodevalues[2]) / (nodevalues[0] - nodevalues[2]);
								//if (t < 1 && t > 0)//第三条边存在等值点
								{
									lines.append(vertexs[2] + t * (vertexs[0] - vertexs[2]));
								}
							}
						}
						else//第一条边不存在等值点
						{
							t = (value - nodevalues[1]) / (nodevalues[2] - nodevalues[1]);
							if (t < 1 && t > 0)//第二条边存在等值点
							{
								lines.append(vertexs[1] + t * (vertexs[2] - vertexs[1]));
								t = (value - nodevalues[2]) / (nodevalues[0] - nodevalues[2]);
								//if (t < 1 && t > 0)//第三条边存在等值点
								{
									lines.append(vertexs[2] + t * (vertexs[0] - vertexs[2]));
								}
							}
							else
							{
								continue;
							}
						}
						values.append(value);
						values.append(value);
					}
				}
			}
		}

	}
	void mPostContourRender::calculateface(QVector<int> index, QVector<QVector3D>& faces, QVector<float>& values)
	{
		int n = index.size();
		if (n == 4)//四面体
		{
			QVector<float> nodevalues(4);
			nodevalues[0] = _values.value(index.at(0));
			nodevalues[1] = _values.value(index.at(1));
			nodevalues[2] = _values.value(index.at(2));
			nodevalues[3] = _values.value(index.at(3));

			//判断是否存在等值点
			pair<float*, float*> minmaxindex = minmax_element(nodevalues.begin(), nodevalues.end());
			float minvalue = *minmaxindex.first; float maxvalue = *minmaxindex.second;
			bool hasContour{ false };
			for (float value : _contourValues)
			{
				if (value < maxvalue && value > minvalue)
				{
					hasContour = true;
					break;
				}
			}

			if (!hasContour)//不存在等值点
			{
				return;
			}

			QVector<QVector3D> vertexs(4);
			vertexs[0] = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + _deformationScale * _dis.value(index.at(0));
			vertexs[1] = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + _deformationScale * _dis.value(index.at(1));
			vertexs[2] = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + _deformationScale * _dis.value(index.at(2));
			vertexs[3] = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + _deformationScale * _dis.value(index.at(3));
			calculatefaceByTet(vertexs, nodevalues, minvalue, maxvalue, faces, values);
		}
		else 
		{
			QVector<float> nodevalues(n);
			for (int i = 0; i < n; i++)
			{
				nodevalues[i] = _values.value(index.at(i));
			}

			//判断是否存在等值点
			pair<float*, float*> minmaxindex = minmax_element(nodevalues.begin(), nodevalues.end());
			float minvalue = *minmaxindex.first; float maxvalue = *minmaxindex.second;
			bool hasContour{ false };
			for (float value : _contourValues)
			{
				if (value < maxvalue && value > minvalue)
				{
					hasContour = true;
					break;
				}
			}

			if (!hasContour)//不存在等值点
			{
				return;
			}
			QVector<QVector3D> vertexs(n);
			for (int i = 0; i < n; i++)
			{
				vertexs[i] = _oneFrameData->getNodeDataByID(index.at(i))->getNodeVertex() + _deformationScale * _dis.value(index.at(i));
			}
			if (n == 8)//六面体
			{

#if refinement

				QVector3D center0 = (vertexs.at(0) + vertexs.at(1) + vertexs.at(2) + vertexs.at(3) + vertexs.at(4) + vertexs.at(5) + vertexs.at(6) + vertexs.at(7)) / 8.0;
				QVector3D center1 = (vertexs.at(0) + vertexs.at(1) + vertexs.at(2) + vertexs.at(3)) / 4.0;
				QVector3D center2 = (vertexs.at(4) + vertexs.at(5) + vertexs.at(6) + vertexs.at(7)) / 4.0;
				QVector3D center3 = (vertexs.at(0) + vertexs.at(1) + vertexs.at(5) + vertexs.at(4)) / 4.0;
				QVector3D center4 = (vertexs.at(1) + vertexs.at(2) + vertexs.at(6) + vertexs.at(5)) / 4.0;
				QVector3D center5 = (vertexs.at(2) + vertexs.at(3) + vertexs.at(7) + vertexs.at(6)) / 4.0;
				QVector3D center6 = (vertexs.at(3) + vertexs.at(0) + vertexs.at(4) + vertexs.at(7)) / 4.0;

				float value0 = (nodevalues.at(0) + nodevalues.at(1) + nodevalues.at(2) + nodevalues.at(3) + nodevalues.at(4) + nodevalues.at(5) + nodevalues.at(6) + nodevalues.at(7)) / 8.0;
				float value1 = (nodevalues.at(0) + nodevalues.at(1) + nodevalues.at(2) + nodevalues.at(3)) / 4.0;
				float value2 = (nodevalues.at(4) + nodevalues.at(5) + nodevalues.at(6) + nodevalues.at(7)) / 4.0;
				float value3 = (nodevalues.at(0) + nodevalues.at(1) + nodevalues.at(5) + nodevalues.at(4)) / 4.0;
				float value4 = (nodevalues.at(1) + nodevalues.at(2) + nodevalues.at(6) + nodevalues.at(5)) / 4.0;
				float value5 = (nodevalues.at(2) + nodevalues.at(3) + nodevalues.at(7) + nodevalues.at(6)) / 4.0;
				float value6 = (nodevalues.at(3) + nodevalues.at(0) + nodevalues.at(4) + nodevalues.at(7)) / 4.0;

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(1), center1, center0}, QVector<float>{nodevalues.at(0), nodevalues.at(1), value1, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(2), center1, center0}, QVector<float>{nodevalues.at(1), nodevalues.at(2), value1, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(3), center1, center0}, QVector<float>{nodevalues.at(2), nodevalues.at(3), value1, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(0), center1, center0}, QVector<float>{nodevalues.at(3), nodevalues.at(0), value1, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(7), center2, center0}, QVector<float>{nodevalues.at(4), nodevalues.at(7), value2, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(7), vertexs.at(6), center2, center0}, QVector<float>{nodevalues.at(7), nodevalues.at(6), value2, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(6), vertexs.at(5), center2, center0}, QVector<float>{nodevalues.at(6), nodevalues.at(5), value2, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(5), vertexs.at(4), center2, center0}, QVector<float>{nodevalues.at(5), nodevalues.at(4), value2, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(4), center3, center0}, QVector<float>{nodevalues.at(0), nodevalues.at(4), value3, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(5), center3, center0}, QVector<float>{nodevalues.at(4), nodevalues.at(5), value3, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(5), vertexs.at(1), center3, center0}, QVector<float>{nodevalues.at(5), nodevalues.at(1), value3, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(0), center3, center0}, QVector<float>{nodevalues.at(1), nodevalues.at(0), value3, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(5), center4, center0}, QVector<float>{nodevalues.at(1), nodevalues.at(5), value4, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(5), vertexs.at(6), center4, center0}, QVector<float>{nodevalues.at(5), nodevalues.at(6), value4, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(6), vertexs.at(2), center4, center0}, QVector<float>{nodevalues.at(6), nodevalues.at(2), value4, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(1), center4, center0}, QVector<float>{nodevalues.at(2), nodevalues.at(1), value4, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(6), center5, center0}, QVector<float>{nodevalues.at(2), nodevalues.at(6), value5, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(6), vertexs.at(7), center5, center0}, QVector<float>{nodevalues.at(6), nodevalues.at(7), value5, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(7), vertexs.at(3), center5, center0}, QVector<float>{nodevalues.at(7), nodevalues.at(3), value5, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(2), center5, center0}, QVector<float>{nodevalues.at(3), nodevalues.at(2), value5, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(7), center6, center0}, QVector<float>{nodevalues.at(3), nodevalues.at(7), value6, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(7), vertexs.at(4), center6, center0}, QVector<float>{nodevalues.at(7), nodevalues.at(4), value6, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(0), center6, center0}, QVector<float>{nodevalues.at(4), nodevalues.at(0), value6, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(3), center6, center0}, QVector<float>{nodevalues.at(0), nodevalues.at(3), value6, value0}, minvalue, maxvalue, faces, values);
#else
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(5), vertexs.at(7), vertexs.at(4)}, QVector<float>{nodevalues.at(0), nodevalues.at(5), nodevalues.at(7), nodevalues.at(4)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(1), vertexs.at(3), vertexs.at(7)}, QVector<float>{nodevalues.at(0), nodevalues.at(1), nodevalues.at(3), nodevalues.at(7)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(1), vertexs.at(7), vertexs.at(5)}, QVector<float>{nodevalues.at(0), nodevalues.at(1), nodevalues.at(7), nodevalues.at(5)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(1), vertexs.at(6), vertexs.at(7)}, QVector<float>{nodevalues.at(3), nodevalues.at(1), nodevalues.at(6), nodevalues.at(7)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(6), vertexs.at(7), vertexs.at(5)}, QVector<float>{nodevalues.at(1), nodevalues.at(6), nodevalues.at(7), nodevalues.at(5)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(2), vertexs.at(3), vertexs.at(6)}, QVector<float>{nodevalues.at(1), nodevalues.at(2), nodevalues.at(3), nodevalues.at(6)}, minvalue, maxvalue, faces, values);
			
				//calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(1), vertexs.at(3), vertexs.at(4)}, QVector<float>{nodevalues.at(0), nodevalues.at(1), nodevalues.at(3), nodevalues.at(4)}, minvalue, maxvalue, faces, values);
				//calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(1), vertexs.at(3), vertexs.at(7)}, QVector<float>{nodevalues.at(4), nodevalues.at(1), nodevalues.at(3), nodevalues.at(7)}, minvalue, maxvalue, faces, values);
				//calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(7), vertexs.at(5), vertexs.at(1)}, QVector<float>{nodevalues.at(4), nodevalues.at(7), nodevalues.at(5), nodevalues.at(1)}, minvalue, maxvalue, faces, values);
				//calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(2), vertexs.at(3), vertexs.at(7)}, QVector<float>{nodevalues.at(1), nodevalues.at(2), nodevalues.at(3), nodevalues.at(7)}, minvalue, maxvalue, faces, values);
				//calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(2), vertexs.at(7), vertexs.at(5)}, QVector<float>{nodevalues.at(1), nodevalues.at(2), nodevalues.at(7), nodevalues.at(5)}, minvalue, maxvalue, faces, values);
				//calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(7), vertexs.at(5), vertexs.at(6)}, QVector<float>{nodevalues.at(2), nodevalues.at(7), nodevalues.at(5), nodevalues.at(6)}, minvalue, maxvalue, faces, values);
			
#endif // refinement
			}
			else if (n == 6)//三棱柱
			{
#if refinement
				QVector3D center0 = (vertexs.at(0) + vertexs.at(1) + vertexs.at(2) + vertexs.at(3) + vertexs.at(4) + vertexs.at(5)) / 6.0;
				QVector3D center1 = (vertexs.at(0) + vertexs.at(1) + vertexs.at(4) + vertexs.at(3)) / 4.0;
				QVector3D center2 = (vertexs.at(1) + vertexs.at(2) + vertexs.at(5) + vertexs.at(4)) / 4.0;
				QVector3D center3 = (vertexs.at(2) + vertexs.at(0) + vertexs.at(3) + vertexs.at(5)) / 4.0;

				float value0 = (nodevalues.at(0) + nodevalues.at(1) + nodevalues.at(2) + nodevalues.at(3) + nodevalues.at(4) + nodevalues.at(5)) / 6.0;
				float value1 = (nodevalues.at(0) + nodevalues.at(1) + nodevalues.at(4) + nodevalues.at(3)) / 4.0;
				float value2 = (nodevalues.at(1) + nodevalues.at(2) + nodevalues.at(5) + nodevalues.at(4)) / 4.0;
				float value3 = (nodevalues.at(2) + nodevalues.at(0) + nodevalues.at(3) + nodevalues.at(5)) / 4.0;

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(3), center1, center0}, QVector<float>{nodevalues.at(0), nodevalues.at(3), value1, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(4), center1, center0}, QVector<float>{nodevalues.at(3), nodevalues.at(4), value1, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(1), center1, center0}, QVector<float>{nodevalues.at(4), nodevalues.at(1), value1, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(0), center1, center0}, QVector<float>{nodevalues.at(1), nodevalues.at(0), value1, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(4), center2, center0}, QVector<float>{nodevalues.at(1), nodevalues.at(4), value2, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(4), vertexs.at(5), center2, center0}, QVector<float>{nodevalues.at(4), nodevalues.at(5), value2, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(5), vertexs.at(2), center2, center0}, QVector<float>{nodevalues.at(5), nodevalues.at(2), value2, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(1), center2, center0}, QVector<float>{nodevalues.at(2), nodevalues.at(1), value2, value0}, minvalue, maxvalue, faces, values);

				calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(5), center3, center0}, QVector<float>{nodevalues.at(2), nodevalues.at(5), value3, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(5), vertexs.at(3), center3, center0}, QVector<float>{nodevalues.at(5), nodevalues.at(3), value3, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(0), center3, center0}, QVector<float>{nodevalues.at(3), nodevalues.at(0), value3, value0}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(2), center3, center0}, QVector<float>{nodevalues.at(0), nodevalues.at(2), value3, value0}, minvalue, maxvalue, faces, values);
#else
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(2), vertexs.at(1), vertexs.at(3)}, QVector<float>{nodevalues.at(0), nodevalues.at(2), nodevalues.at(1), nodevalues.at(3)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(4), vertexs.at(3), vertexs.at(2)}, QVector<float>{nodevalues.at(1), nodevalues.at(4), nodevalues.at(3), nodevalues.at(2)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(4), vertexs.at(3), vertexs.at(5)}, QVector<float>{nodevalues.at(2), nodevalues.at(4), nodevalues.at(3), nodevalues.at(5)}, minvalue, maxvalue, faces, values);
#endif // refinement

			}
			else//四棱锥
			{
#if refinement
			QVector3D center1 = (vertexs.at(0) + vertexs.at(1) + vertexs.at(2) + vertexs.at(3)) / 4.0;

			float value1 = (nodevalues.at(0) + nodevalues.at(0) + nodevalues.at(4) + nodevalues.at(3)) / 4.0;

			calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(1), center1, vertexs.at(4)}, QVector<float>{nodevalues.at(0), nodevalues.at(1), value1, nodevalues.at(4)}, minvalue, maxvalue, faces, values);
			calculatefaceByTet(QVector<QVector3D>{vertexs.at(1), vertexs.at(2), center1, vertexs.at(4)}, QVector<float>{nodevalues.at(1), nodevalues.at(2), value1, nodevalues.at(4)}, minvalue, maxvalue, faces, values);
			calculatefaceByTet(QVector<QVector3D>{vertexs.at(2), vertexs.at(3), center1, vertexs.at(4)}, QVector<float>{nodevalues.at(2), nodevalues.at(3), value1, nodevalues.at(4)}, minvalue, maxvalue, faces, values);
			calculatefaceByTet(QVector<QVector3D>{vertexs.at(3), vertexs.at(0), center1, vertexs.at(4)}, QVector<float>{nodevalues.at(3), nodevalues.at(0), value1, nodevalues.at(4)}, minvalue, maxvalue, faces, values);
#else
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(1), vertexs.at(2), vertexs.at(4)}, QVector<float>{nodevalues.at(0), nodevalues.at(1), nodevalues.at(2), nodevalues.at(4)}, minvalue, maxvalue, faces, values);
				calculatefaceByTet(QVector<QVector3D>{vertexs.at(0), vertexs.at(2), vertexs.at(3), vertexs.at(4)}, QVector<float>{nodevalues.at(0), nodevalues.at(2), nodevalues.at(3), nodevalues.at(4)}, minvalue, maxvalue, faces, values);
#endif // refinement	
			}
		}
	}
	void mPostContourRender::calculatefaceByTet(QVector<QVector3D> vertexs, QVector<float> nodevalues, float minvalue, float maxvalue, QVector<QVector3D>& faces, QVector<float>& values)
	{
		for (float value : _contourValues)
		{
			if (value < maxvalue && value > minvalue)
			{
				QVector<QVector3D> res;
				float t = (value - nodevalues[0]) / (nodevalues[1] - nodevalues[0]);//01 12 20 30 13 23
				if (t < 1 && t > 0)//第一条边存在等值点
				{
					res.append(vertexs[0] + t * (vertexs[1] - vertexs[0]));
				}
				t = (value - nodevalues[1]) / (nodevalues[2] - nodevalues[1]);
				if (t < 1 && t > 0)//第二条边存在等值点
				{
					res.append(vertexs[1] + t * (vertexs[2] - vertexs[1]));
				}
				t = (value - nodevalues[2]) / (nodevalues[0] - nodevalues[2]);
				if (t < 1 && t > 0)//第三条边存在等值点
				{
					res.append(vertexs[2] + t * (vertexs[0] - vertexs[2]));
				}
				t = (value - nodevalues[3]) / (nodevalues[0] - nodevalues[3]);
				if (t < 1 && t > 0)//第四条边存在等值点
				{
					res.append(vertexs[3] + t * (vertexs[0] - vertexs[3]));
				}
				t = (value - nodevalues[1]) / (nodevalues[3] - nodevalues[1]);
				if (t < 1 && t > 0)//第五条边存在等值点
				{
					res.append(vertexs[1] + t * (vertexs[3] - vertexs[1]));
				}
				t = (value - nodevalues[2]) / (nodevalues[3] - nodevalues[2]);
				if (t < 1 && t > 0)//第六条边存在等值点
				{
					res.append(vertexs[2] + t * (vertexs[3] - vertexs[2]));
				}
				if (res.size() == 0)
				{
					continue;
				}
				QVector3D normal = QVector3D::crossProduct((res.at(1) - res.at(0)).normalized(), (res.at(2) - res.at(1)).normalized()).normalized();
				if (res.size() == 3)
				{
					faces.append(res);
					values.append(QVector<float>(3, value));
				}
				else if (res.size() == 4)
				{
					QVector3D center;
					for (int i = 0; i < 4; ++i)
					{
						center += res.at(i);
					}
					center = center / 4;

					QVector3D OO = (res.first() - center).normalized();

					sort(res.begin(), res.end(), [this, center, normal, OO](QVector3D a, QVector3D b)
					{
						QVector3D OA = a - center;
						QVector3D OB = b - center;

						//向量OA和向量OB的叉积
						QVector3D voa = QVector3D::crossProduct(OA, OO);
						QVector3D vob = QVector3D::crossProduct(OB, OO);

						float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
						float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

						if (QVector3D::dotProduct(voa, normal) < 0)
						{
							cosA = -(cosA + 2);
						}
						if (QVector3D::dotProduct(vob, normal) < 0)
						{
							cosB = -(cosB + 2);
						}
						return cosA < cosB;
					});

					//三角化
					for (int i = 1; i < 3; ++i)
					{
						faces.append(res.at(0));
						faces.append(res.at(i));
						faces.append(res.at(i + 1));
					}
					values.append(QVector<float>(6, value));
				}
			}
		}
	}


}