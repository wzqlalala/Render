#include "mPostStreamLineRender.h"
#include "mPostRendStatus.h"

//MDataPost
#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"

#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshNodeData1.h"

#include "mBasicEnum.h"

#include <renderpch.h>

#include <QDebug>
#include<math.h>

using namespace MxFunctions;
namespace MPostRend
{
	int maxMeshID = 10000000;
	mPostStreamLineData1::mPostStreamLineData1(mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData,int num, float ratio)
	{
		//后处理数据
		_oneFrameData = oneFrameData;

		_integrationSteps = 10000;
		_currentFrameRendData = currentFrameRendData;

		// _sphereCenter = QVector3D(0.000643963, 0.0176615, -0.0011032);
		// _sphereRadius = 0.00271875;

		_sphereCenter = QVector3D(0, 0, 0);
		_sphereRadius = 1;
		_streamLineNum = num;
		_integrateRatio = ratio;
	}

	mPostStreamLineData1::~mPostStreamLineData1()
	{
		QList<int> allNodeIDs = _nodeData1.keys();
		for_each(allNodeIDs.begin(), allNodeIDs.end(), [this](int id) {mPostMeshNodeData1* nodeData = _nodeData1.value(id); delete nodeData; nodeData = nullptr; });
		QList<int> allMeshIDs = _meshData1.keys();
		for_each(allMeshIDs.begin(), allMeshIDs.end(), [this](int id) {mPostMeshData1* meshData = _meshData1.value(id); delete meshData; meshData = nullptr; });
		QList<QString> allPartNames = _partData1.keys();
		for_each(allPartNames.begin(), allPartNames.end(), [this](QString name) {mPostMeshPartData1* partData = _partData1.value(name); delete partData; partData = nullptr; });
	}

	void mPostStreamLineData1::setSphereParameter(QVector3D center, float radius)
	{
		_sphereCenter = center;
		_sphereRadius = radius;
	}

	void mPostStreamLineData1::setStreamLineNum(int num)
	{
		_streamLineNum = num;
	}

	void mPostStreamLineData1::OrganizeModeldata()
	{
		if (_oneFrameData == nullptr)
		{
			return;
		}
		if (_currentFrameRendData == nullptr)
		{
			return;
		}


		_vectorValue = _currentFrameRendData->getVectorRendData();
		_value = _currentFrameRendData->getRendValue();
		if (_vectorValue.size() == 0)
		{
			return;
		}
		

		_minval = _currentFrameRendData->getMinData();
		_maxval = _currentFrameRendData->getMaxData();
		_fenmu = _maxval - _minval;

		int frameID = _currentFrameRendData->getRendID();
		int partID = 0;
		auto iter = _oneFrameData->getMeshPartIterator();
		while (iter.hasNext())
		{
			iter.next();
			QString partName = iter.key();
			mPostMeshPartData1 *partData1 = iter.value();
			if (partData1 == nullptr)
			{
				continue;
			}

			if (!partData1->getPartVisual())
			{
				continue;
			}

			partID++;
			//创建部件
			mPostMeshPartData1 *partData = new mPostMeshPartData1(partName,partID);
			_partData1.insert(partName, partData);

			//三维网格
			set<int> meshIDs = partData1->getMeshIDs3();
			for(int meshID : meshIDs)
			{
				mPostMeshData1 *meshData1 = _oneFrameData->getMeshDataByID(meshID);
				if (meshData1 == nullptr)
				{
					continue;
				}

				if (!meshData1->getMeshVisual())
				{
					continue;
				}
				QVector<int> indexs = meshData1->getNodeIndex();
				if (meshData1->getMeshType() ==  MeshTet)
				{
					mPostMeshData1 *meshData = new mPostMeshData1(meshID, MeshTet, D3T4, partName);
					meshData->setNodeIndex(indexs);
					_meshData1.insert(meshID, meshData);
					partData->appendMesh3(meshData);

					for (int index : indexs)
					{
						mPostMeshNodeData1 *nodeData = _nodeData1.value(index);
						if (nodeData == nullptr)
						{
							nodeData = new mPostMeshNodeData1(index, _oneFrameData->getNodeDataByID(index)->getNodeVertex());
							_nodeData1.insert(index, nodeData);
						}
					}

				}
				else if (meshData1->getMeshType() == MeshPyramid)
				{
					mPostMeshData1 *meshData = new mPostMeshData1(meshID, MeshPyramid, D3P5, partName);
					meshData->setNodeIndex(indexs);
					for (int index : indexs)
					{
						mPostMeshNodeData1 *nodeData = _nodeData1.value(index);
						if (nodeData == nullptr)
						{
							nodeData = new mPostMeshNodeData1(index, _oneFrameData->getNodeDataByID(index)->getNodeVertex());
							_nodeData1.insert(index, nodeData);
						}
					}

					splitPyramid(indexs,meshData, partData);

					delete meshData;
					meshData = nullptr;
				}
				else if (meshData1->getMeshType() == MeshWedge)
				{
					mPostMeshData1 *meshData = new mPostMeshData1(meshID, MeshWedge, D3P6, partName);
					meshData->setNodeIndex(indexs);
					for (int index : indexs)
					{
						mPostMeshNodeData1 *nodeData = _nodeData1.value(index);
						if (nodeData == nullptr)
						{
							nodeData = new mPostMeshNodeData1(index, _oneFrameData->getNodeDataByID(index)->getNodeVertex());
							_nodeData1.insert(index, nodeData);
						}
					}

					splitWedge(indexs, meshData, partData);

					delete meshData;
					meshData = nullptr;
				}
				else if (meshData1->getMeshType() == MeshHex)
				{
					mPostMeshData1 *meshData = new mPostMeshData1(meshID, MeshHex, D3H8, partName);
					meshData->setNodeIndex(indexs);
					for (int index : indexs)
					{
						mPostMeshNodeData1 *nodeData = _nodeData1.value(index);
						if (nodeData == nullptr)
						{
							nodeData = new mPostMeshNodeData1(index, _oneFrameData->getNodeDataByID(index)->getNodeVertex());
							_nodeData1.insert(index, nodeData);
						}
					}

					splitHexahedron(indexs, meshData, partData);

					delete meshData;
					meshData = nullptr;
				}
			}
		}
	
		MViewBasic::NodeOrElement vectorType = _currentFrameRendData->getVectorValueType();
		MViewBasic::NodeOrElement valueType = _currentFrameRendData->getNodeOrElement();
		//判断计算出来的矢量值或者标量值是否是节点变量，是节点变量的话就覆盖
		if (vectorType == MViewBasic::PostNode)
		{
			_vectorValue = _currentFrameRendData->getVectorRendData();
		}
		if (valueType == MViewBasic::PostNode)
		{
			_value = _currentFrameRendData->getRendValue();
		}

		//根据输入球参数来确定初始点所在的网格编号
		QVector<int> pmeshIDs;
		QHashIterator<int, MDataPost::mPostMeshData1*> iterMesh(_meshData1);
		while (iterMesh.hasNext())
		{
			iterMesh.next();
			mPostMeshData1 *meshData = iterMesh.value();
			if (meshData==nullptr)
			{
				continue;
			}

			QVector3D point;
			for (int index:meshData->getNodeIndex())
			{
				mPostMeshNodeData1 *nodeData = _nodeData1.value(index);
				if (nodeData!=nullptr)
				{
					point += nodeData->getNodeVertex();
				}
			}
			point /= meshData->getNodeIndex().size();
			if (point.distanceToPoint(_sphereCenter)<=_sphereRadius)
			{
				pmeshIDs.append(meshData->getMeshID());
			}
		}
		if (pmeshIDs.size()>_streamLineNum)
		{
			pmeshIDs.resize(_streamLineNum);
		}
		for (int i: pmeshIDs)
		{
			this->caculateStreamLineData(true, i, _forwardLineVertex, _forwardlineTexture);
			this->caculateStreamLineData(false, i,_backwardLineVertex,_backwardLineTexture);
		}

	}

	QVector<QVector3D> mPostStreamLineData1::getForwardStreamLineVertex()
	{
		return _forwardLineVertex;
	}

	QVector<float> mPostStreamLineData1::getForwardStreamLineTexture()
	{
		return _forwardlineTexture;
	}

	QVector<QVector3D> mPostStreamLineData1::getBackwardStreamLineVertex()
	{
		return _backwardLineVertex;
	}

	QVector<float> mPostStreamLineData1::getBackwardStreamLineTexture()
	{
		return _backwardLineTexture;
	}

	void mPostStreamLineData1::splitHexahedron(QVector<int> index, MDataPost::mPostMeshData1* meshData, MDataPost::mPostMeshPartData1 *partData)
	{
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(0), index.at(1), index.at(3), index.at(4)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(4), index.at(1), index.at(3), index.at(7)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(4), index.at(7), index.at(5), index.at(1)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(1), index.at(2), index.at(3), index.at(7)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(1), index.at(2), index.at(7), index.at(5)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(2), index.at(7), index.at(5), index.at(6)}, partData);
	}

	void mPostStreamLineData1::splitWedge(QVector<int> index, MDataPost::mPostMeshData1* meshData, MDataPost::mPostMeshPartData1* partData)
	{
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(0), index.at(2), index.at(1), index.at(3)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(1), index.at(4), index.at(3), index.at(2)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(2), index.at(4), index.at(3), index.at(5)}, partData);
	}

	void mPostStreamLineData1::splitPyramid(QVector<int> index, MDataPost::mPostMeshData1* meshData, MDataPost::mPostMeshPartData1* partData)
	{
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(0), index.at(1), index.at(2), index.at(4)}, partData);
		createTetrahedron(meshData->getMeshID(), QVector<int>{index.at(0), index.at(2), index.at(3), index.at(4)}, partData);
	}

	void mPostStreamLineData1::createTetrahedron(int meshID,QVector<int> index, MDataPost::mPostMeshPartData1* partData)
	{
		maxMeshID++;
		mPostMeshData1 *meshData = new mPostMeshData1(maxMeshID, MeshTet, D3T4, partData->getPartName());
		meshData->setNodeIndex(index);
		_meshData1.insert(maxMeshID, meshData);
		_vectorValue.insert(maxMeshID, _vectorValue.value(meshID));
		_value.insert(maxMeshID, _value.value(meshID));
		partData->appendMesh3(meshData);

		_gongxianwangge[QSet<int>{index.at(0), index.at(1)}].append(maxMeshID);
		_gongxianwangge[QSet<int>{index.at(1), index.at(2)}].append(maxMeshID);
		_gongxianwangge[QSet<int>{index.at(0), index.at(2)}].append(maxMeshID);
		_gongxianwangge[QSet<int>{index.at(0), index.at(3)}].append(maxMeshID);
		_gongxianwangge[QSet<int>{index.at(1), index.at(3)}].append(maxMeshID);
		_gongxianwangge[QSet<int>{index.at(2), index.at(3)}].append(maxMeshID);

	}

	void mPostStreamLineData1::caculateStreamLineData(bool trueorfalse, int pmeshID, QVector<QVector3D> &lineVertex, QVector<float> &lineTexture)
	{
		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;

		if (_oneFrameData == nullptr)
		{
			return;
		}
		if (_currentFrameRendData == nullptr)
		{
			return;
		}
		int frameID = _currentFrameRendData->getRendID();

		const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _currentFrameRendData->getDeformationScale();

		MViewBasic::NodeOrElement vectorType = _currentFrameRendData->getVectorValueType();
		MViewBasic::NodeOrElement valueType = _currentFrameRendData->getNodeOrElement();

		//计数
		int integrationSteps = 0;
		//初始积分步长
		mPostMeshData1 *meshData = _meshData1.value(pmeshID);
		if (meshData == nullptr)
		{
			return;
		}
		QVector<int> index = meshData->getNodeIndex();
		vertex0 = _nodeData1.value(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
		vertex1 = _nodeData1.value(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
		vertex2 = _nodeData1.value(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
		vertex3 = _nodeData1.value(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

		//标量和矢量都是单元变量（都不用插值）
		if (valueType == MViewBasic::PostElement && vectorType == MViewBasic::PostElement)//标量和矢量都是单元,不用插值
		{
			QVector3D p = (vertex0 + vertex1 + vertex2 + vertex3)/ 4.0;
			QVector3D vector = _vectorValue.value(pmeshID);
			float value = _value.value(pmeshID);

			QVector3D AB = vertex1 - vertex0;
			QVector3D AC = vertex2 - vertex0;
			QVector3D AD = vertex3 - vertex0;

			//四面体体积
			float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
			float h0 = pow(fabs(V), 1.0 / 3.0) / vector.length()*_integrateRatio;

			lineVertex.append(p);
			//_lineValue.append(v.length());
			lineTexture.append((value - _minval) / _fenmu);

			while (true)
			{
				integrationSteps++;
				//初始点所在的四面体网格的体积
				meshData = _meshData1.value(pmeshID);
				if (meshData == nullptr)
				{
					return;
				}

				index = meshData->getNodeIndex();
				vertex0 = _nodeData1.value(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = _nodeData1.value(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = _nodeData1.value(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = _nodeData1.value(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

				vector = _vectorValue.value(pmeshID);
				value = _value.value(pmeshID);
				if (vector == QVector3D())//速度为零返回
				{
					lineVertex.removeLast();
					lineTexture.removeLast();
					return;
				}

				AB = vertex1 - vertex0;
				AC = vertex2 - vertex0;
				AD = vertex3 - vertex0;

				//四面体体积
				V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
				//初始积分步长
				h0 = pow(fabs(V), 1.0 / 3.0) / vector.length() * _integrateRatio;

				//下一步位置
				QVector3D p1 = p + vector * h0;
				if (!trueorfalse)
				{
					p1 = p - vector * h0;
				}

				//保存下一步位置
				p = p1;

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))//在四面体内
				{
					continue;
				}

				if (true)
				{
					QVector<int> meshIDs;
					bool isin{ false };
					index = index;
					QVector<QSet<int>> sss{
						QSet<int>{index.at(0), index.at(1)},
						QSet<int>{index.at(1), index.at(2)},
						QSet<int>{index.at(0), index.at(2)},
						QSet<int>{index.at(0), index.at(3)},
						QSet<int>{index.at(1), index.at(3)},
						QSet<int>{index.at(2), index.at(3)},
					};
					for (QSet<int> s : sss)
					{
						meshIDs = _gongxianwangge.value(s);
						for (int me : meshIDs)
						{
							mPostMeshData1 *meshData1 = _meshData1.value(me);
							if (meshData1 == nullptr)
							{
								return;
							}
							QVector<int> index1 = meshData->getNodeIndex();
							vertex0 = _nodeData1.value(index.at(0))->getNodeVertex();
							vertex1 = _nodeData1.value(index.at(1))->getNodeVertex();
							vertex2 = _nodeData1.value(index.at(2))->getNodeVertex();
							vertex3 = _nodeData1.value(index.at(3))->getNodeVertex();

							if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))
							{
								pmeshID = me;
								isin = true;
								break;
							}
						}
						if (isin)
						{
							break;
						}
					}
					if (!isin)
					{
						lineVertex.removeLast();
						lineTexture.removeLast();
						return;
					}
				}
			}
		}
		//标量是单元变量，矢量是节点变量（标量不用插值，矢量要插值）
		else if (valueType == MViewBasic::PostElement && vectorType == MViewBasic::PostNode)
		{
			QVector3D p = (vertex0 + vertex1 + vertex2 + vertex3) / 4.0;
			QVector3D vector = caculatePointValueInTetrahedron(p, QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3},
				QVector<QVector3D>{_vectorValue.value(index.at(0)),
				_vectorValue.value(index.at(1)),
				_vectorValue.value(index.at(2)),
				_vectorValue.value(index.at(3))});
			float value = _value.value(pmeshID);

			QVector3D AB = vertex1 - vertex0;
			QVector3D AC = vertex2 - vertex0;
			QVector3D AD = vertex3 - vertex0;

			//四面体体积
			float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
			float h0 = pow(fabs(V), 1.0 / 3.0) / vector.length()*_integrateRatio;

			lineVertex.append(p);
			lineTexture.append((value - _minval) / _fenmu);

			while (true)
			{
				integrationSteps++;
				//初始点所在的四面体网格的体积
				meshData = _meshData1.value(pmeshID);
				if (meshData == nullptr)
				{
					return;
				}
				index = meshData->getNodeIndex();
				vertex0 = _nodeData1.value(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = _nodeData1.value(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = _nodeData1.value(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = _nodeData1.value(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

				vector = caculatePointValueInTetrahedron(p, QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3},
					QVector<QVector3D>{_vectorValue.value(index.at(0)),
					_vectorValue.value(index.at(1)),
					_vectorValue.value(index.at(2)),
					_vectorValue.value(index.at(3))});

				value = _value.value(pmeshID);

				if (vector == QVector3D())//速度为零返回
				{
					lineVertex.removeLast();
					lineTexture.removeLast();
					return;
				}

				AB = vertex1 - vertex0;
				AC = vertex2 - vertex0;
				AD = vertex3 - vertex0;

				//四面体体积
				V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
				//初始积分步长
				h0 = pow(fabs(V), 1.0 / 3.0) / vector.length() * _integrateRatio;
				
				//下一步位置
				QVector3D p1 = p + vector * h0;
				if (!trueorfalse)
				{
					p1 = p - vector * h0;
				}

				//保存下一步位置
				p = p1;

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))//在四面体内
				{
					continue;
				}

				if (true)
				{
					QVector<int> meshIDs;
					bool isin{ false };
					index = meshData->getNodeIndex();
					QVector<QSet<int>> sss{
						QSet<int>{index.at(0), index.at(1)},
						QSet<int>{index.at(1), index.at(2)},
						QSet<int>{index.at(0), index.at(2)},
						QSet<int>{index.at(0), index.at(3)},
						QSet<int>{index.at(1), index.at(3)},
						QSet<int>{index.at(2), index.at(3)},
					};
					for (QSet<int> s : sss)
					{
						meshIDs = _gongxianwangge.value(s);
						for (int me : meshIDs)
						{
							mPostMeshData1 *meshData1 = _meshData1.value(me);
							if (meshData1 == nullptr)
							{
								return;
							}
							QVector<int> index1 = meshData1->getNodeIndex();
							vertex0 = _nodeData1.value(index.at(0))->getNodeVertex();
							vertex1 = _nodeData1.value(index.at(1))->getNodeVertex();
							vertex2 = _nodeData1.value(index.at(2))->getNodeVertex();
							vertex3 = _nodeData1.value(index.at(3))->getNodeVertex();

							if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))
							{
								pmeshID = me;
								isin = true;
								break;
							}
						}
						if (isin)
						{
							break;
						}
					}
					if (!isin)
					{
						lineVertex.removeLast();
						lineTexture.removeLast();
						return;
					}
				}
			}
		}
		//标量是节点变量，矢量是单元变量（标量要插值，矢量不用插值）
		else if (valueType == MViewBasic::PostNode && vectorType == MViewBasic::PostElement)
		{
			QVector3D p = (vertex0 + vertex1 + vertex2 + vertex3) / 4.0;
			QVector3D vector = _vectorValue.value(pmeshID);
			float value = caculatePointValueInTetrahedron(p, QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3},
				QVector<float>{_value.value(index.at(0)),
				_value.value(index.at(1)),
				_value.value(index.at(2)),
				_value.value(index.at(3))});

			QVector3D AB = vertex1 - vertex0;
			QVector3D AC = vertex2 - vertex0;
			QVector3D AD = vertex3 - vertex0;

			//四面体体积
			float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
			float h0 = pow(fabs(V), 1.0 / 3.0) / vector.length()*_integrateRatio;

			lineVertex.append(p);
			lineTexture.append((value - _minval) / _fenmu);

			while (true)
			{
				integrationSteps++;
				//初始点所在的四面体网格的体积
				meshData = _meshData1.value(pmeshID);
				if (meshData == nullptr)
				{
					return;
				}
				index = meshData->getNodeIndex();
				vertex0 = _nodeData1.value(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = _nodeData1.value(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = _nodeData1.value(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = _nodeData1.value(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

				vector = _vectorValue.value(pmeshID);
				value = caculatePointValueInTetrahedron(p, QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3},
					QVector<float>{_value.value(index.at(0)),
					_value.value(index.at(1)),
					_value.value(index.at(2)),
					_value.value(index.at(3))});

				if (vector == QVector3D())//速度为零返回
				{
					lineVertex.removeLast();
					lineTexture.removeLast();
					return;
				}

				AB = vertex1 - vertex0;
				AC = vertex2 - vertex0;
				AD = vertex3 - vertex0;

				//四面体体积
				V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
				//初始积分步长
				h0 = pow(fabs(V), 1.0 / 3.0) / vector.length() * _integrateRatio;

				//下一步位置
				QVector3D p1 = p + vector * h0;
				if (!trueorfalse)
				{
					p1 = p - vector * h0;
				}

				//保存下一步位置
				p = p1;

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))//在四面体内
				{
					continue;
				}

				if (true)
				{
					QVector<int> meshIDs;
					bool isin{ false };
					index = meshData->getNodeIndex();
					QVector<QSet<int>> sss{
						QSet<int>{index.at(0), index.at(1)},
						QSet<int>{index.at(1), index.at(2)},
						QSet<int>{index.at(0), index.at(2)},
						QSet<int>{index.at(0), index.at(3)},
						QSet<int>{index.at(1), index.at(3)},
						QSet<int>{index.at(2), index.at(3)},
					};
					for (QSet<int> s : sss)
					{
						meshIDs = _gongxianwangge.value(s);
						for (int me : meshIDs)
						{
							mPostMeshData1 *meshData1 = _meshData1.value(me);
							if (meshData1 == nullptr)
							{
								return;
							}
							QVector<int> index1 = meshData1->getNodeIndex();
							vertex0 = _nodeData1.value(index1.at(0))->getNodeVertex();
							vertex1 = _nodeData1.value(index1.at(1))->getNodeVertex();
							vertex2 = _nodeData1.value(index1.at(2))->getNodeVertex();
							vertex3 = _nodeData1.value(index1.at(3))->getNodeVertex();

							if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))
							{
								pmeshID = me;
								isin = true;
								break;
							}
						}
						if (isin)
						{
							break;
						}
					}
					if (!isin)
					{
						lineVertex.removeLast();
						lineTexture.removeLast();
						return;
					}
				}
			}
		}
		else if (valueType == MViewBasic::PostNode && vectorType == MViewBasic::PostNode)
		{
			QVector3D p = (vertex0 + vertex1 + vertex2 + vertex3) / 4.0;
			QPair<QVector3D,float> res = caculatePointValueInTetrahedron(p, QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3},
				QVector<QVector3D>{_vectorValue.value(index.at(0)),
				_vectorValue.value(index.at(1)),
				_vectorValue.value(index.at(2)),
				_vectorValue.value(index.at(3))},
				QVector<float>{_value.value(index.at(0)),
				_value.value(index.at(1)),
				_value.value(index.at(2)),
				_value.value(index.at(3))});
			QVector3D vector = res.first;
			float value = res.second;

			QVector3D AB = vertex1 - vertex0;
			QVector3D AC = vertex2 - vertex0;
			QVector3D AD = vertex3 - vertex0;

			//四面体体积
			float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
			float h0 = pow(fabs(V), 1.0 / 3.0) / vector.length()*_integrateRatio;

			lineVertex.append(p);
			lineTexture.append((value - _minval) / _fenmu);

			while (true)
			{
				integrationSteps++;
				//初始点所在的四面体网格的体积
				meshData = _meshData1.value(pmeshID);
				if (meshData == nullptr)
				{
					return;
				}

				index = meshData->getNodeIndex();
				vertex0 = _nodeData1.value(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = _nodeData1.value(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = _nodeData1.value(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = _nodeData1.value(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

				res = caculatePointValueInTetrahedron(p, QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3},
					QVector<QVector3D>{_vectorValue.value(index.at(0)),
					_vectorValue.value(index.at(1)),
					_vectorValue.value(index.at(2)),
					_vectorValue.value(index.at(3))},
					QVector<float>{_value.value(index.at(0)),
					_value.value(index.at(1)),
					_value.value(index.at(2)),
					_value.value(index.at(3))});
				vector = res.first;
				value = res.second;

				if (vector == QVector3D())//速度为零返回
				{
					lineVertex.removeLast();
					lineTexture.removeLast();
					return;
				}

				AB = vertex1 - vertex0;
				AC = vertex2 - vertex0;
				AD = vertex3 - vertex0;

				//四面体体积
				V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));
				//初始积分步长
				h0 = pow(fabs(V), 1.0 / 3.0) / vector.length() * _integrateRatio;

				//积分步长计算
				if (lineVertex.size() >= 3)
				{
					int size = lineVertex.size();
					QVector3D P1 = lineVertex.at(size - 2) - lineVertex.at(size - 3);
					QVector3D P2 = lineVertex.at(size - 1) - lineVertex.at(size - 2);

					float theta = fabs(180 * acos(QVector3D::dotProduct(P1, P2) / (P1.length()*P2.length()))/3.1415926);
					if (theta <= 0.5)
					{
						h0 *= 2;
					}
					else if (theta > 5)
					{
						h0 *= 0.5;
					}
				}

				//下一步位置
				QVector3D p1 = p + vector * h0;
				if (!trueorfalse)
				{
					p1 = p - vector * h0;
				}

				//保存下一步位置
				p = p1;

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				lineVertex.append(p);
				lineTexture.append((value - _minval) / _fenmu);

				if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))//在四面体内
				{
					continue;
				}

				if (true)
				{
					QVector<int> meshIDs;
					bool isin{ false };
					index = meshData->getNodeIndex();
					QVector<QSet<int>> sss{
						QSet<int>{index.at(0), index.at(1)},
						QSet<int>{index.at(1), index.at(2)},
						QSet<int>{index.at(0), index.at(2)},
						QSet<int>{index.at(0), index.at(3)},
						QSet<int>{index.at(1), index.at(3)},
						QSet<int>{index.at(2), index.at(3)},
					};
					for (QSet<int> s : sss)
					{
						meshIDs = _gongxianwangge.value(s);
						for (int me : meshIDs)
						{
							mPostMeshData1 *meshData1 = _meshData1.value(me);
							if (meshData1 == nullptr)
							{
								return;
							}
							QVector<int> index1 = meshData1->getNodeIndex();
							vertex0 = _nodeData1.value(index1.at(0))->getNodeVertex();
							vertex1 = _nodeData1.value(index1.at(1))->getNodeVertex();
							vertex2 = _nodeData1.value(index1.at(2))->getNodeVertex();
							vertex3 = _nodeData1.value(index1.at(3))->getNodeVertex();

							if (jdugePointIsInTetrahedron(QVector<QVector3D>{vertex0, vertex1, vertex2, vertex3}, p))
							{
								pmeshID = me;
								isin = true;
								break;
							}
						}
						if (isin)
						{
							break;
						}
					}
					if (!isin)
					{
						lineVertex.removeLast();
						lineTexture.removeLast();
						return;
					}
				}
			}
		}

		//积分步长计算
		//if (lineVertex.size() >= 3)
		//{
		//	int size = lineVertex.size();
		//	QVector3D P1 = lineVertex.at(size - 2) - lineVertex.at(size - 3);
		//	QVector3D P2 = lineVertex.at(size - 1) - lineVertex.at(size - 2);

		//	float theta = acos(QVector3D::dotProduct(P1, P2) / (P1.length()*P2.length()));
		//	if (theta <= 0.5)
		//	{
		//		h0 *= 2;
		//	}
		//	else if (theta > 5)
		//	{
		//		h0 *= 0.5;
		//	}
		//}
	}

	bool mPostStreamLineData1::jdugePointIsInTetrahedron(QVector<QVector3D> vertexs, QVector3D p)
	{
		QVector3D center = (vertexs[0] + vertexs[2] + vertexs[1]) / 3.0;
		QVector3D normal = QVector3D::crossProduct(vertexs[2] - vertexs[0], vertexs[1] - vertexs[2]);

		float wa = QVector3D::dotProduct(p - center, normal);

		center = (vertexs[0] + vertexs[1] + vertexs[3]) / 3.0;
		normal = QVector3D::crossProduct(vertexs[1] - vertexs[0], vertexs[3] - vertexs[1]);
		float wb = QVector3D::dotProduct(p - center, normal);

		center = (vertexs[1] + vertexs[2] + vertexs[3]) / 3.0;
		normal = QVector3D::crossProduct(vertexs[2] - vertexs[1], vertexs[3] - vertexs[2]);
		float wc = QVector3D::dotProduct(p - center, normal);

		center = (vertexs[0] + vertexs[3] + vertexs[2]) / 3.0;
		normal = QVector3D::crossProduct(vertexs[3] - vertexs[0], vertexs[2] - vertexs[3]);
		float wd = QVector3D::dotProduct(p - center, normal);

		if (wa<0&&wb<0&&wc<0&&wd<0)
		{
			return true;
		}

		return false;
	}

	QVector3D mPostStreamLineData1::caculatePointValueInTetrahedron(QVector3D p1, QVector<QVector3D> vertexs, QVector<QVector3D> vectorValues)
	{
		QVector3D AB = vertexs[1] - vertexs[0];
		QVector3D AC = vertexs[2] - vertexs[0];
		QVector3D AD = vertexs[3] - vertexs[0];
		//计算p点所在的四面体网格单元
		QVector3D AP = p1 - vertexs[0];
		QVector3D BP = p1 - vertexs[1];
		QVector3D BC = vertexs[2] - vertexs[1];
		QVector3D BD = vertexs[3] - vertexs[1];

		float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));

		float wa = fabs((1.0 / 6.0 * QVector3D::dotProduct(BD, QVector3D::crossProduct(BC, BP))) / V);//BDCP
		float wb = fabs((1.0 / 6.0 * QVector3D::dotProduct(AC, QVector3D::crossProduct(AD, AP))) / V);//ACDP
		float wc = fabs((1.0 / 6.0 * QVector3D::dotProduct(AD, QVector3D::crossProduct(AB, AP))) / V);//ADBP
		float wd = fabs((1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AP))) / V);//ABCP

		QVector3D value = vectorValues[0] * wa + vectorValues[1] * wb + vectorValues[2] * wc + vectorValues[3] * wd;

		return value;
	}

	float mPostStreamLineData1::caculatePointValueInTetrahedron(QVector3D p1, QVector<QVector3D> vertexs, QVector<float> values)
	{
		QVector3D AB = vertexs[1] - vertexs[0];
		QVector3D AC = vertexs[2] - vertexs[0];
		QVector3D AD = vertexs[3] - vertexs[0];
		//计算p点所在的四面体网格单元
		QVector3D AP = p1 - vertexs[0];
		QVector3D BP = p1 - vertexs[1];
		QVector3D BC = vertexs[2] - vertexs[1];
		QVector3D BD = vertexs[3] - vertexs[1];

		float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));

		float wa = fabs((1.0 / 6.0 * QVector3D::dotProduct(BD, QVector3D::crossProduct(BC, BP))) / V);//BDCP
		float wb = fabs((1.0 / 6.0 * QVector3D::dotProduct(AC, QVector3D::crossProduct(AD, AP))) / V);//ACDP
		float wc = fabs((1.0 / 6.0 * QVector3D::dotProduct(AD, QVector3D::crossProduct(AB, AP))) / V);//ADBP
		float wd = fabs((1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AP))) / V);//ABCP

		float value = values[0] * wa + values[1] * wb + values[2] * wc + values[3] * wd;

		return value;
	}
	QPair<QVector3D, float> mPostStreamLineData1::caculatePointValueInTetrahedron(QVector3D p1, QVector<QVector3D> vertexs, QVector<QVector3D> vectorValues, QVector<float> values)
	{
		QVector3D AB = vertexs[1] - vertexs[0];
		QVector3D AC = vertexs[2] - vertexs[0];
		QVector3D AD = vertexs[3] - vertexs[0];
		//计算p点所在的四面体网格单元
		QVector3D AP = p1 - vertexs[0];
		QVector3D BP = p1 - vertexs[1];
		QVector3D BC = vertexs[2] - vertexs[1];
		QVector3D BD = vertexs[3] - vertexs[1];

		float V = 1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD));

		float wa = fabs((1.0 / 6.0 * QVector3D::dotProduct(BD, QVector3D::crossProduct(BC, BP))) / V);//BDCP
		float wb = fabs((1.0 / 6.0 * QVector3D::dotProduct(AC, QVector3D::crossProduct(AD, AP))) / V);//ACDP
		float wc = fabs((1.0 / 6.0 * QVector3D::dotProduct(AD, QVector3D::crossProduct(AB, AP))) / V);//ADBP
		float wd = fabs((1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AP))) / V);//ABCP

		QVector3D vectorValue = vectorValues[0] * wa + vectorValues[1] * wb + vectorValues[2] * wc + vectorValues[3] * wd;
		float value = values[0] * wa + values[1] * wb + values[2] * wc + values[3] * wd;

		return QPair<QVector3D, float>(vectorValue,value);
	}
	mPostStreamLineRender::mPostStreamLineRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mOneFrameData1 * oneFrameData, shared_ptr<mPostRendStatus> rendStatus, mPostOneFrameRendData * currentFrameRendData, int num, float ratio)
	{
		_app = app;
		_parent = parent;

		_lineDrawable = nullptr;

		_geode = MakeAsset<mxr::Geode>();
		_parent->addChild(_geode);

		_postStreamLineData = new mPostStreamLineData1(oneFrameData, currentFrameRendData, num, ratio);

	}
	mPostStreamLineRender::~mPostStreamLineRender()
	{

	}
	void mPostStreamLineRender::setLineStateSet(std::shared_ptr<mxr::StateSet> stateSet)
	{
		_lineStateSet = stateSet;
	}
	void mPostStreamLineRender::setPointStateSet(std::shared_ptr<mxr::StateSet> stateSet)
	{
		_pointStateSet = stateSet;
	}
	void mPostStreamLineRender::setSphereParameter(QVector3D center, float radius)
	{
		_postStreamLineData->setSphereParameter(center, radius);
	}
	void mPostStreamLineRender::setStreamLineNum(int num)
	{
		_postStreamLineData->setStreamLineNum(num);
	}
	void mPostStreamLineRender::setStreamLineShowForm(int streamLineShowForm)
	{
		if (streamLineShowForm == 0)
		{
			_lineDrawable->setStateSet(_lineStateSet);
		}
		else
		{
			_lineDrawable->setStateSet(_pointStateSet);
		}
	}
	void mPostStreamLineRender::setIntergrateDirection(int intergrateDirection)
	{
		if (_lineDrawable)
		{
			_geode->removeChild(_lineDrawable);
		}

		_lineDrawable = MakeAsset<mxr::Drawable>();
		this->setStreamLineShowForm(_rendStatus->_streamLineStyle);
		_geode->addChild(_lineDrawable);

		if (_rendStatus->_streamLineDirection == 0)
		{
			_lineDrawable->setVertexAttribArray(0, MakeAsset<mxr::Vec3Array>(_postStreamLineData->getForwardStreamLineVertex() + _postStreamLineData->getBackwardStreamLineVertex()));
			_lineDrawable->setVertexAttribArray(1, MakeAsset<mxr::FloatArray>(_postStreamLineData->getForwardStreamLineTexture() + _postStreamLineData->getBackwardStreamLineTexture()));
		}
		else if (_rendStatus->_streamLineDirection == 1)
		{
			_lineDrawable->setVertexAttribArray(0, MakeAsset<mxr::Vec3Array>(_postStreamLineData->getForwardStreamLineVertex()));
			_lineDrawable->setVertexAttribArray(1, MakeAsset<mxr::FloatArray>(_postStreamLineData->getForwardStreamLineTexture()));
		}
		else
		{
			_lineDrawable->setVertexAttribArray(0, MakeAsset<mxr::Vec3Array>(_postStreamLineData->getBackwardStreamLineVertex()));
			_lineDrawable->setVertexAttribArray(1, MakeAsset<mxr::FloatArray>(_postStreamLineData->getBackwardStreamLineTexture()));
		}
	}
	void mPostStreamLineRender::OrganizeModeldata()
	{
		_postStreamLineData->OrganizeModeldata();

		this->setIntergrateDirection(_rendStatus->_streamLineDirection);
	}
}
