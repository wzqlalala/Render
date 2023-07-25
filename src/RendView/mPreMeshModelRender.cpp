#include "mPreMeshModelRender.h"

#include "mMeshModelData.h"
#include "mMeshData.h"
#include "mMeshFaceData.h"
#include "mMeshPartData.h"

#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mPreRendStatus.h"

//��ͼ��
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
using namespace MViewBasic;
using namespace MDataMesh;
namespace MPreRend
{
	mPreMeshModelRender::mPreMeshModelRender(std::shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus, std::shared_ptr<mMeshModelData> meshModelData):
		_parent(parent), _rendStatus(rendStatus), _meshModelData(meshModelData)
	{
		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);	

		//updateAllModelOperate(ImportOperate);
	}

	void mPreMeshModelRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		//_facerend->setStateSet(_faceStateSet);
	}
	void mPreMeshModelRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	{
		_faceTransparentStateSet = faceTransparentStateSet;
	}
	void mPreMeshModelRender::setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_faceTransparentNodeformationStateSet = faceTransparentNoDeformationStateSet;
	}
	void mPreMeshModelRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelineStateSet = edgelineStateSet;
		//_meshlinerend->setStateSet(_edgelineStateSet);
	}
	void mPreMeshModelRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		//_facelinerend->setStateSet(_facelineStateSet);
	}
	//void mPreMeshModelRender::setQuadFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	//{
	//	_quadfacelineStateSet = facelineStateSet;
	//}
	void mPreMeshModelRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
	}
	void mPreMeshModelRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
	}
	void mPreMeshModelRender::setNodeStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_nodeStateSet = stateset;
	}
	mPreMeshModelRender::~mPreMeshModelRender()
	{
		_parent->removeChild(_geode);
		_partRenders.clear();
	}
	bool mPreMeshModelRender::updateRender()
	{
		bool isUpdateCamera{ false };
		QTime time;
		//if (MeshMessage::getInstance()->hasUpdatedRenderState())
		//{
		//	time.start();
		//}
		//while (MeshMessage::getInstance()->hasUpdatedRenderState())
		//{
		//	isUpdateCamera = isUpdateCamera | updateModelOperate(MeshMessage::getInstance()->getUpdatedRenderState());
		//}
		if (isUpdateCamera)
		{
			qDebug() << "��Ⱦʱ��" << time.elapsed();
		}
		return isUpdateCamera;
	}

	bool mPreMeshModelRender::updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>> postModelOperates)
	{
		ModelOperateEnum postModelOperate = postModelOperates.first;
		set<QString> partNames = postModelOperates.second;
		if (postModelOperate == AppendOnePart || postModelOperate == ReplaceOnePart)
		{
			for (QString partName : partNames)
			{
				//mGeoPartData1 *partData = _geoModelData->getGeoPartDataByPartName(partName);
				//if (partData)
				{
					asset_ref<mPreMeshPartRender> part = MakeAsset<mPreMeshPartRender>(_geode, _meshModelData->getMeshPartDataByPartName(partName));
					part->setFaceStateSet(_faceStateSet);
					part->setFaceTransparentNodeformationStateSet(_faceTransparentNodeformationStateSet);
					part->setEdgeLineStateSet(_edgelineStateSet);
					part->setFaceLineStateSet(_facelineStateSet);
					//part->setQuadFaceLineStateSet(_quadfacelineStateSet);
					part->setLineStateSet(_lineStateSet);
					part->setPointStateSet(_pointStateSet);
					part->setShowFuntion(_rendStatus->_showFunction);
					_partRenders[partName] = part;
				}
			}
			return true;
		}
		else if (postModelOperate == DeleteOnePart)
		{
			for (QString partName : partNames)
			{
				_partRenders.remove(partName);
			}
			return true;
		}
		else if (postModelOperate == HideOnePart)
		{
			for (QString partName : partNames)
			{
				if (_partRenders.contains(partName))
				{
					_partRenders[partName]->getGeode()->setNodeMask(1);
				}

			}
			return true;
		}
		else if (postModelOperate == ShowOnePart)
		{
			for (QString partName : partNames)
			{
				if (_partRenders.contains(partName))
				{
					_partRenders[partName]->getGeode()->setNodeMask(0);
					_partRenders[partName]->setShowFuntion(_rendStatus->_showFunction);
				}
			}
			return true;
		}
		else if (postModelOperate == ColorOnePart)
		{
			for (QString partName : partNames)
			{
				if (_partRenders.contains(partName))
				{
					QVector3D color = _meshModelData->getMeshPartColor(partName);
					Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(1);
					array->updata(QVector<QVector3D>(array->size() / 3.0, color).data());
					array = _partRenders[partName]->getLineDrawable()->getVertexAttribArray(1);
					array->updata(QVector<QVector3D>(array->size() / 3.0, color).data());
					array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(1);
					array->updata(QVector<QVector3D>(array->size() / 3.0, color).data());
				}
			}
		}
		else if (postModelOperate == RenameOnePart)
		{
			QString oldname = *partNames.begin();
			QString newname = *(++partNames.begin());
			if (_partRenders.contains(newname))
			{
				swap(oldname, newname);
			}
			_partRenders[newname] = _partRenders.take(oldname);
		}
		else
		{
			QList<QString> partNames = _partRenders.keys();
			std::set<QString> partnames{ partNames.begin(),partNames.end() };
			if (postModelOperate == ImportPart)
			{
				//UpdateModelBuffer();
				setShowFuntion(_rendStatus->_showFunction);
				//UpdateRigidWallBuffer();
				//updateAllModelOperate(UpdateMinMax);
			}
			else if (postModelOperate == HideAllPart)
			{
				updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>>(HideOnePart, partnames));
			}
			else if (postModelOperate == ShowAllPart)
			{
				updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>>(ShowOnePart, partnames));
			}
			else if (postModelOperate == ColorAllPart)
			{
				updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>>(ColorOnePart, partnames));
			}
			else if (postModelOperate == DeleteAllPart)
			{
				updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>>(DeleteOnePart, partnames));
			}
		}
		return false;
	}

	void mPreMeshModelRender::setShowFuntion(ShowFuntion showFuntion)
	{
		if (_faceStateSet)
		{
			for (auto iter : _partRenders)
			{
				iter->setShowFuntion(showFuntion);
			}
			//if (showFuntion == WireFrame)
			//{
			//	_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::LINE), 1);
			//}
			//else
			//{
			//	_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
			//}
		}
	}

	Space::AABB mPreMeshModelRender::getModelAABB()
	{
		Space::AABB aabb;
		for (auto partrend : _partRenders)
		{
			aabb.push(partrend->getPartAABB());
		}
		return aabb;
	}
	Space::AABB mPreMeshModelRender::getPartAABB(QString partName)
	{
		return _partRenders.value(partName)->getPartAABB();
	}
	mPreMeshPartRender::mPreMeshPartRender(std::shared_ptr<mxr::Group> parent, std::shared_ptr<mMeshPartData> partData):_parent(parent)
	{
		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);

		_facerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_facetransparentnodeformationrend = MakeAsset<mGroupRender1<Vec3Array>>(_geode);
		_edgelinerend = MakeAsset<mGroupRender1<Vec3Array>>(_geode);
		_facelinerend = MakeAsset<mGroupRender2<Vec3Array, FloatArray>>(_geode);
		//_quadfacelinerend = MakeAsset<mGroupRender1<Vec3Array>>(_geode);
		_linerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_pointrend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_partData= partData;
		appendPart();
	}
	mPreMeshPartRender::~mPreMeshPartRender()
	{
		_parent->removeChild(_geode);
		//Space::destoryTree(_spaceTree);
	}
	void mPreMeshPartRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_facerend->setStateSet(faceStateSet);
	}
	void mPreMeshPartRender::setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_facetransparentnodeformationrend->setStateSet(faceTransparentNoDeformationStateSet);
	}
	void mPreMeshPartRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelinerend->setStateSet(edgelineStateSet);
	}

	void mPreMeshPartRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelinerend->setStateSet(facelineStateSet);
	}
	//void mPreMeshPartRender::setQuadFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	//{
	//	_quadfacelinerend->setStateSet(facelineStateSet);
	//}
	void mPreMeshPartRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_linerend->setStateSet(lineStateSet);
	}
	void mPreMeshPartRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointrend->setStateSet(pointStateSet);
	}
	Space::AABB mPreMeshPartRender::getPartAABB()
	{
		return _aabb;
	}

	QVector<int> quadToTriIndex{ 0, 1, 2, 2, 3, 0 };

	void mPreMeshPartRender::appendPart()
	{
		QVector3D color = _partData->getPartColor();

		//��ά
		auto meshFaceDatas = _partData->getMeshFaceData();
		for (auto meshFaceData : meshFaceDatas)
		{
			QVector<QVector3D> vertexs = meshFaceData->getNodeVertex();
			appendFace(vertexs, color);
		}

		//��ά
		auto meshDatas = _partData->getMeshDatas2();
		for (auto meshData : meshDatas)
		{
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			QVector<QVector3D> vertexs = meshData->getNodeVertex();
			appendFace(vertexs, color);
		}

		//һά
		meshDatas = _partData->getMeshDatas1();
		for (auto meshData : meshDatas)
		{
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			QVector<QVector3D> vertexs = meshData->getNodeVertex();
			appendLine(vertexs, color);
		}

		//0ά
		meshDatas = _partData->getMeshDatas0();
		for (auto meshData : meshDatas)
		{
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			QVector<QVector3D> vertexs = meshData->getNodeVertex();
			appendPoint(vertexs, color);
		}

		//�߽���

		
		_aabb.push(_partData->getMeshPartAABB());
	}
	void mPreMeshPartRender::setShowFuntion(ShowFuntion showFuntion)
	{
		//if (!_partData->getPartVisual())
		//{
		//	return;
		//}
		_pointrend->getDrawable()->setNodeMask(0);
		_linerend->getDrawable()->setNodeMask(0);
		if (showFuntion == ElementFace)
		{
			_facerend->getDrawable()->setNodeMask(0);
			_facelinerend->getDrawable()->setNodeMask(1);
			//_quadfacelinerend->getDrawable()->setNodeMask(1);
			_edgelinerend->getDrawable()->setNodeMask(1);
		}
		else if (showFuntion == SmoothShaded)
		{
			_facerend->getDrawable()->setNodeMask(0);
			_facelinerend->getDrawable()->setNodeMask(0);
			//_quadfacelinerend->getDrawable()->setNodeMask(0);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
		else if (showFuntion == WireFrame)
		{
			_facerend->getDrawable()->setNodeMask(1);
			_facelinerend->getDrawable()->setNodeMask(0);
			//_quadfacelinerend->getDrawable()->setNodeMask(0);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
		else if (showFuntion == WireEdge)
		{
			_facerend->getDrawable()->setNodeMask(1);
			_facelinerend->getDrawable()->setNodeMask(1);
			//_quadfacelinerend->getDrawable()->setNodeMask(1);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
	}
	void mPreMeshPartRender::appendFace(QVector<QVector3D> vertexs, QVector3D color)
	{
		switch (vertexs.size())
		{
		case 3:
			_facerend->_vertex0->append(vertexs);
			_facerend->_vertex1->append(QVector<QVector3D>(3, color));

			_facelinerend->_vertex0->append(vertexs);
			_facelinerend->_vertex1->append(QVector<float>(3, 1.0f));
			break;
		case 4:
			for (int i = 0; i < 6; i++)
			{
				int index = quadToTriIndex.at(i);
				_facerend->_vertex0->append(vertexs.at(index));
				_facerend->_vertex1->append(color);
				_facelinerend->_vertex0->append(vertexs.at(index));
			}
			_facelinerend->_vertex1->append(QVector<float>(6, 0.0f));
			break;
		default:
			break;
		}
	}

	void mPreMeshPartRender::appendLine(QVector<QVector3D> vertexs, QVector3D color)
	{
		_linerend->_vertex0->append(vertexs);
		_linerend->_vertex1->append(QVector<QVector3D>(2, color));
	}

	void mPreMeshPartRender::appendPoint(QVector<QVector3D> vertexs, QVector3D color)
	{
		_pointrend->_vertex0->append(vertexs);
		_pointrend->_vertex1->append(color);
	}

	void mPreMeshPartRender::appendMeshLine(QVector<QVector3D> vertexs)
	{
		_edgelinerend->_vertex0->append(vertexs);
	}	
}