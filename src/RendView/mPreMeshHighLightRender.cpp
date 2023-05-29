#include "mPreMeshHighLightRender.h"
#include "mPreMeshPickData1.h"

#include "mPreRendStatus.h"
#include <renderpch.h>
#include "Space.h"
#include "texture.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

//BasicData
#include <MeshMessage.h>


using namespace mxr;
using namespace std;
namespace MPreRend
{
	mPreMeshHighLightRender::mPreMeshHighLightRender(shared_ptr<mPreRendStatus> rendStatus, mPreMeshPickData1 *meshPickData):_rendStatus(rendStatus), _meshPickData(meshPickData)
	{
		_geode = MakeAsset<Geode>();
		_viewer = nullptr;
		//_edgelineRender = nullptr;
		_facelineRender = nullptr;
		_lineRender = nullptr;
		_pointRender = nullptr;

		this->initial();
	}
	mPreMeshHighLightRender::~mPreMeshHighLightRender()
	{
		
	}

	void mPreMeshHighLightRender::updateHighLightRender()
	{
		
		_pointRender = make_shared<mGroupRender1<Vec3Array>>(_geode);
		_lineRender = make_shared<mGroupRender1<Vec3Array>>(_geode);
		_facelineRender = make_shared<mGroupRender2<Vec3Array, FloatArray>>(_geode);

		_facelineRender->setStateSet(_facelineStateSet);
		_lineRender->setStateSet(_lineStateSet);
		_pointRender->setStateSet(_pointStateSet);

		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
		set<MXMeshVertex*> nodeids = _meshPickData->getPickNodeIDs();
		for (auto iter : nodeids)
		{
			if (iter == nullptr)
			{
				continue;
			}
			_pointRender->_vertex0->append(QVector3D(iter->vx(), iter->vy(), iter->vz()));
		}

		QVector<MXMeshVertex*> nodeorderids = _meshPickData->getPickNodeIDsOrder();
		for (auto iter : nodeorderids)
		{
			if (iter == nullptr)
			{
				continue;
			}
			_pointRender->_vertex0->append(QVector3D(iter->vx(), iter->vy(), iter->vz()));
		}
		
		set<MXMeshElement*> meshIDs = _meshPickData->getPickMeshIDs();
		for (auto iter : meshIDs)
		{
			if (iter == nullptr)
			{
				continue;
			}
			QVector<QVector3D> vertexs/* = iter->getNodeIndex()*/;
			switch (iter->getMeshType())
			{
			case MeshPoint:
			{
				_pointRender->_vertex0->append(vertexs.at(0));
				break;
			}
			case MeshBeam:
			{
				_lineRender->_vertex0->append(vertexs.at(0));
				_lineRender->_vertex0->append(vertexs.at(1));
				break;
			}
			case MeshTri:
			{
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex1->append(QVector<float>(3,1));

				break;
			}
			case MeshQuad:
			{
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex1->append(QVector<float>(6, 0));
				break;
			}
			case MeshTet:
			{
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(1));
												 
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(3));
												
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(3));
												 
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(2));

				_facelineRender->_vertex1->append(QVector<float>(12, 1));
				break;
			}
			case MeshPyramid:
			{
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(4));
												 
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(2));
											
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(3));
											
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(4));

				_facelineRender->_vertex1->append(QVector<float>(12, 1));

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex1->append(QVector<float>(6, 0));
				break;
			}
			case MeshWedge:
			{
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(1));
											
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(5));

				_facelineRender->_vertex1->append(QVector<float>(6, 1));

				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(0));
												
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(1));
											
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex1->append(QVector<float>(18, 0));
				break;
			}
			case MeshHex:
			{
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(0));
											
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(6));
				_facelineRender->_vertex0->append(vertexs.at(6));
				_facelineRender->_vertex0->append(vertexs.at(7));
				_facelineRender->_vertex0->append(vertexs.at(4));
											
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(0));

				_facelineRender->_vertex0->append(vertexs.at(1));
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(6));
				_facelineRender->_vertex0->append(vertexs.at(6));
				_facelineRender->_vertex0->append(vertexs.at(5));
				_facelineRender->_vertex0->append(vertexs.at(1));
												 
				_facelineRender->_vertex0->append(vertexs.at(2));
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(7));
				_facelineRender->_vertex0->append(vertexs.at(7));
				_facelineRender->_vertex0->append(vertexs.at(6));
				_facelineRender->_vertex0->append(vertexs.at(2));
											
				_facelineRender->_vertex0->append(vertexs.at(3));
				_facelineRender->_vertex0->append(vertexs.at(0));
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(4));
				_facelineRender->_vertex0->append(vertexs.at(7));
				_facelineRender->_vertex0->append(vertexs.at(3));

				_facelineRender->_vertex1->append(QVector<float>(36, 0));
				break;
			}
			default:break;
			}
		}

		/*
		set<int> meshfaceids = _meshPickData->getPickMeshFaceIDs();
		for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
		{
			mPostMeshFaceData1 *meshFaceData = oneFrameData->getMeshFaceDataByID(*iter);
			if (meshFaceData == nullptr)
			{
				continue;
			}
			QVector<int> index = meshFaceData->getNodeIndex();
			if (index.size() == 3)
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex1->append(QVector<float>(3, 1));
			}
			else
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex1->append(QVector<float>(6, 0));
			}
		}
		*/
		set<QString> partNames = _meshPickData->getPickMeshPartNames();
		for (QString partName : partNames)
		{
			QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
			//边界线		
			for (auto geoEdge : geoEdges)
			{
				for (auto meshLine : geoEdge->_mLines)
				{
					if (meshLine == nullptr)
					{
						continue;
					}
					_lineRender->_vertex0->append(QVector3D(meshLine->getVertex(0)->vx(), meshLine->getVertex(0)->vy(), meshLine->getVertex(0)->vz()));
					_lineRender->_vertex0->append(QVector3D(meshLine->getVertex(1)->vx(), meshLine->getVertex(1)->vy(), meshLine->getVertex(1)->vz()));
				}
			}

			QVector<MXGeoPoint*> geoPoints = MeshMessage::getInstance()->getGeoPointSamePart(partName);
			//点网格
			for (auto geoPoint : geoPoints)
			{
				if (geoPoint->_mVertex == nullptr)
				{
					continue;
				}
				_pointRender->_vertex0->append(QVector3D(geoPoint->_mVertex->vx(), geoPoint->_mVertex->vy(), geoPoint->_mVertex->vz()));

			}
		}
		
		
	}

	void mPreMeshHighLightRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		//_modelRender->setDeformationScale(_oneFrameRendData->getDeformationScale());
		_facelineStateSet->getUniform("projection")->SetData(modelView->_projection);
		_facelineStateSet->getUniform("view")->SetData(modelView->_view);
		_facelineStateSet->getUniform("model")->SetData(modelView->_model);
		_lineStateSet->getUniform("projection")->SetData(modelView->_projection);
		_lineStateSet->getUniform("view")->SetData(modelView->_view);
		_lineStateSet->getUniform("model")->SetData(modelView->_model);
		_pointStateSet->getUniform("projection")->SetData(modelView->_projection);
		_pointStateSet->getUniform("view")->SetData(modelView->_view);
		_pointStateSet->getUniform("model")->SetData(modelView->_model);

		_pointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);

		if (_rendStatus->_lightIsDependOnCamera)
		{
			_pointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
		}
		else
		{
			_pointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		}

		_viewer->noClearRun();
	}


	void mPreMeshHighLightRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_lineStateSet = edgelineStateSet;
		//_modelRender->setEdgeLineStateSet(_lineStateSet);
	}

	void mPreMeshHighLightRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		//_modelRender->setFaceLineStateSet(_facelineStateSet);
	}

	void mPreMeshHighLightRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
		//_modelRender->setLineStateSet(_lineStateSet);
	}

	void mPreMeshHighLightRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
		//_modelRender->setPointStateSet(_pointStateSet);
	}

	void mPreMeshHighLightRender::initial()
	{
		if (!_viewer)
		{
			_viewer = MakeAsset<mxr::Viewer>();
			_viewer->setSceneData(_geode);
		}

		_facelineStateSet = MakeAsset<StateSet>();
		mxr::Shader * facelineshader = mShaderManage::GetInstance()->GetShader("PostHighLightFaceLine");
		_facelineStateSet->setShader(facelineshader);
		_facelineStateSet->setDrawMode(GL_TRIANGLES);
		_facelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);

		_facelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));

		//edgeline
		_lineStateSet = MakeAsset<StateSet>();
		mxr::Shader * meshlineshader = mShaderManage::GetInstance()->GetShader("PostHighLightLine");
		_lineStateSet->setShader(meshlineshader);
		_lineStateSet->setDrawMode(GL_LINES);
		_lineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_lineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);

		_lineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));

		_pointStateSet = MakeAsset<StateSet>();
		mxr::Shader * pointshader = mShaderManage::GetInstance()->GetShader("PostHighLightPoint");
		_pointStateSet->setShader(pointshader);
		_pointStateSet->setDrawMode(GL_POINTS);
		_pointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_pointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);

		_pointStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_pointStateSet->setUniform(MakeAsset<Uniform>("PointSize", 10));
	}

}