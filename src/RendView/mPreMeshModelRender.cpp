#include "mPreMeshModelRender.h"

//#include "MeshMessage.h"
//#include "MXGeoPoint.h"
//#include "MXGeoEdge.h"
//#include "MXGeoFace.h"
//#include "MXGeoSolid.h"
//#include "MeshEntity.h"
//#include "MXMeshTetrahedron.h"
//#include "MXMeshHexahedral.h"
//#include "MXMeshTriangle.h"
//#include "MXMeshQuadrangle.h"

#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mPreRendStatus.h"

// ”Õº¿‡
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
using namespace MViewBasic;
namespace MPreRend
{
	mPreMeshModelRender::mPreMeshModelRender(std::shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus/*, mGeoModelData1 *geoModelData*/):
		_parent(parent), _rendStatus(rendStatus)/*, _geoModelData(geoModelData)*/
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
	void mPreMeshModelRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
	}
	void mPreMeshModelRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
	}
	mPreMeshModelRender::~mPreMeshModelRender()
	{
		_parent->removeChild(_geode);
		_partRenders.clear();
	}
	bool mPreMeshModelRender::updateRender()
	{
		//if (!_geoModelData->_modelOperate.empty())
		{
			bool isUpdateCamera{ false };
			//while (!_geoModelData->_modelOperate.empty())
			{
				//isUpdateCamera = isUpdateCamera | updateModelOperate(_geoModelData->_modelOperate.takeFirst());
			}
			return isUpdateCamera;
		}
		return false;
	}
	void mPreMeshModelRender::UpdateModelBuffer()
	{
		std::set<QString> hidePartNames;
		//auto iter = _oneFrameData->getMeshPartIterator();
		//_partOrder = _geoModelData->getAllPartName();
		for (auto partName : _partOrder)
		{
			//mGeoPartData1 *partData = _geoModelData->getGeoPartDataByPartName(partName);
			//if (partData == nullptr)
			//{
			//	continue;
			//}
			//if (!partData->getPartVisual())
			{
				hidePartNames.insert(partName);
			}
			asset_ref<mPreMeshPartRender> part = MakeAsset<mPreMeshPartRender>(_geode/*, partData*/);
			part->setFaceStateSet(_faceStateSet);
			part->setFaceTransparentNodeformationStateSet(_faceTransparentNodeformationStateSet);
			part->setEdgeLineStateSet(_edgelineStateSet);
			part->setFaceLineStateSet(_facelineStateSet);
			part->setLineStateSet(_lineStateSet);
			part->setPointStateSet(_pointStateSet);

			_partRenders.insert(partName, part);
		}
		updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>>(HideOnePart, hidePartNames));
	}

	bool mPreMeshModelRender::updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>> postModelOperates)
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
					asset_ref<mPreMeshPartRender> part = MakeAsset<mPreMeshPartRender>(_geode/*, partData*/);
					part->setFaceStateSet(_faceStateSet);
					part->setFaceTransparentNodeformationStateSet(_faceTransparentNodeformationStateSet);
					part->setEdgeLineStateSet(_edgelineStateSet);
					part->setFaceLineStateSet(_facelineStateSet);
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
				_partRenders[partName]->getGeode()->setNodeMask(1);
			}
			return true;
		}
		else if (postModelOperate == ShowOnePart)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->getGeode()->setNodeMask(0);
				_partRenders[partName]->setShowFuntion(_rendStatus->_showFunction);
			}
			return true;
		}
		else if (postModelOperate == ColorOnePart)
		{
			for (QString partName : partNames)
			{
				QVector3D color/* = _geoModelData->getGeoPartColor(partName)*/;
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getLineDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
			}
		}
		else
		{
			QList<QString> partNames = _partRenders.keys();
			std::set<QString> partnames{ partNames.begin(),partNames.end() };
			if (postModelOperate == ImportPart)
			{
				UpdateModelBuffer();
				setShowFuntion(_rendStatus->_showFunction);
				//UpdateRigidWallBuffer();
				//updateAllModelOperate(UpdateMinMax);
			}
			else if (postModelOperate == HideAllPart)
			{
				updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>>(HideOnePart, partnames));
			}
			else if (postModelOperate == ShowAllPart)
			{
				updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>>(ShowOnePart, partnames));
			}
			else if (postModelOperate == ColorAllPart)
			{
				updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>>(ColorOnePart, partnames));
			}
			else if (postModelOperate == DeleteAllPart)
			{
				updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>>(DeleteOnePart, partnames));
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
		}
	}

	Space::AABB mPreMeshModelRender::getModelAABB()
	{
		Space::AABB aabb;
		for(auto partrend : _partRenders)
		{
			if (partrend->getPartSpaceTree())
			{
				aabb.push(partrend->getPartSpaceTree()->space);
			}
		}
		return aabb;
	}
	Space::SpaceTree *mPreMeshModelRender::getPartSpaceTree(QString partName)
	{
		return _partRenders.value(partName)->getPartSpaceTree();
	}
	mPreMeshPartRender::mPreMeshPartRender(std::shared_ptr<mxr::Group> parent/*, MDataGeo::mGeoPartData1 * part*/)
	{
		_facerend = MakeAsset<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>(_geode);
		_facetransparentnodeformationrend = MakeAsset<mGroupRender1<Vec3Array>>(_geode);
		_edgelinerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_facelinerend = MakeAsset<mGroupRender3<Vec3Array, Vec3Array, FloatArray>>(_geode);
		_linerend = MakeAsset<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>(_geode);
		_pointrend = MakeAsset<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>(_geode);

		appendPart();
	}
	mPreMeshPartRender::~mPreMeshPartRender()
	{
		_parent->removeChild(_geode);
		Space::destoryTree(_spaceTree);
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
	void mPreMeshPartRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_linerend->setStateSet(lineStateSet);
	}
	void mPreMeshPartRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointrend->setStateSet(pointStateSet);
	}
	Space::SpaceTree * mPreMeshPartRender::getPartSpaceTree()
	{
		return nullptr;
	}
	void mPreMeshPartRender::appendPart()
	{
		
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
			_edgelinerend->getDrawable()->setNodeMask(1);
		}
		else if (showFuntion == SmoothShaded)
		{
			_facerend->getDrawable()->setNodeMask(0);
			_facelinerend->getDrawable()->setNodeMask(0);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
		else if (showFuntion == WireEdge)
		{
			_facerend->getDrawable()->setNodeMask(1);
			_facelinerend->getDrawable()->setNodeMask(1);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
	}
}