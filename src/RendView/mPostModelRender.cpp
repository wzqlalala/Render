#include "mPostModelRender.h"
#include "mPostPartRender.h"
#include <renderpch.h>
#include "Space.h"

#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostMeshPartData1.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mPostRendStatus.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
using namespace MViewBasic;
namespace MPostRend
{
	mPostModelRender::mPostModelRender(std::shared_ptr<mxr::Group> parent, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData):
		_parent(parent), _rendStatus(rendStatus), _oneFrameData(oneFrameData), _oneFrameRendData(oneFrameRendData)
	{
		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);	

		//updateAllModelOperate(ImportOperate);
	}

	void mPostModelRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		//_facerend->setStateSet(_faceStateSet);
	}
	void mPostModelRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	{
		_faceTransparentStateSet = faceTransparentStateSet;
	}
	void mPostModelRender::setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_faceTransparentNodeformationStateSet = faceTransparentNoDeformationStateSet;
	}
	void mPostModelRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelineStateSet = edgelineStateSet;
		//_meshlinerend->setStateSet(_edgelineStateSet);
	}
	void mPostModelRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		//_facelinerend->setStateSet(_facelineStateSet);
	}
	void mPostModelRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
	}
	void mPostModelRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
	}
	void mPostModelRender::hideModelRender()
	{
		_geode->setNodeMask(1);
	}
	mPostModelRender::~mPostModelRender()
	{
		_parent->removeChild(_geode);
		_partRenders.clear();
	}
	void mPostModelRender::UpdateModelBuffer()
	{
		QHash<int, float> values = _oneFrameRendData->getRendValue();
		QHash<int, QVector3D> dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		std::set<QString> hidePartNames;
		//auto iter = _oneFrameData->getMeshPartIterator();
		_partOrder = _oneFrameData->getAllPartNameList();
		for (auto partName : _partOrder)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getOnePartDataByName(partName);
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				hidePartNames.insert(partName);
			}
			asset_ref<mPostPartRender> part = MakeAsset<mPostPartRender>(_geode, _oneFrameData, partData);
			part->setFaceStateSet(_faceStateSet);
			part->setFaceTransparentNodeformationStateSet(_faceTransparentNodeformationStateSet);
			part->setEdgeLineStateSet(_edgelineStateSet);
			part->setFaceLineStateSet(_facelineStateSet);
			part->setLineStateSet(_lineStateSet);
			part->setPointStateSet(_pointStateSet);
			part->ImportPart(deformationScale, values, dis, _oneFrameRendData->getNodeOrElement());
			part->calculateSpaceTreeThread(dis);

			_partRenders.insert(partName, part);
			_oneFrameRendData->setPartMinMaxData(partName, part->_minValue, part->_maxValue);
		}
		updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(HideOnePartOperate, hidePartNames));
	}
	void mPostModelRender::updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>> postModelOperates)
	{
		PostModelOperateEnum postModelOperate = postModelOperates.first;
		set<QString> partNames = postModelOperates.second;
		if (postModelOperate == HideOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_oneFrameData->setMeshPartVisual(partName, false);
				_partRenders[partName]->getGeode()->setNodeMask(1);
			}
		}
		else if (postModelOperate == ShowOnePartOperate)
		{
			for (QString partName : partNames)
			{
				//_partRenders[partName]->getGeode()->setNodeMask(0);
				_oneFrameData->setMeshPartVisual(partName, true);
				_partRenders[partName]->setShowFuntion(_rendStatus->_showFunction);
				_partRenders[partName]->setIsShowInitialShape(_rendStatus->_isShowInitialShape);
				//parts[partName]->getFaceDrawable()->setNodeMask(0);
			}
			//setShowFuntion(_rendStatus->_showFunction);
			//setIsShowInitialShape(_rendStatus->_isShowInitialShape);
		}
		else if (postModelOperate == ColorOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->setFaceStateSet(_faceStateSet);

				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(2);
				QVector<float> data(array->size());
				array->getData(data.data());
				for (int i = 0; i < data.size(); i++)
				{
					data[i] = int(data[i]) % 2;//取余数，只能为0或者1
				}
				array->updata(data.data());

				array = _partRenders[partName]->getLineDrawable()->getVertexAttribArray(2);
				QVector<float> data1(array->size());
				array->getData(data1.data());
				for (int i = 0; i < data1.size(); i++)
				{
					data1[i] = int(data1[i]) % 2;//取余数，只能为0或者1
				}
				array->updata(data1.data());

				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(2);
				QVector<float> data2(array->size());
				array->getData(data2.data());
				for (int i = 0; i < data2.size(); i++)
				{
					data2[i] = int(data2[i]) % 2;//取余数，只能为0或者1
				}
				array->updata(data2.data());
			}
		}
		else if (postModelOperate == TextureOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->setFaceStateSet(_faceStateSet);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(2);
				//float *data = (float*)array->getData();
				QVector<float> data(array->size());
				array->getData(data.data());
				for (int i = 0; i < array->size(); i++)
				{
					if (data[i] < 2)
					{
						data[i] = data[i] + 2;//只能为3或者4
					}
				}
				array->updata(data.data());

				array = _partRenders[partName]->getLineDrawable()->getVertexAttribArray(2);
				QVector<float> data1(array->size());
				array->getData(data1.data());
				for (int i = 0; i < data1.size(); i++)
				{
					if (data1[i] < 2)
					{
						data1[i] = data1[i] + 2;//只能为3或者4
					}
				}
				array->updata(data1.data());

				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(2);
				QVector<float> data2(array->size());
				array->getData(data2.data());
				for (int i = 0; i < data2.size(); i++)
				{
					if (data2[i] < 2)
					{
						data2[i] = data2[i] + 2;//只能为3或者4
					}
				}
				array->updata(data2.data());
			}
		}
		else if (postModelOperate == TransparentOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->setFaceStateSet(_faceTransparentStateSet);
			}
		}
		else if (postModelOperate == ChangeColorOnePartOperate)
		{
			for (QString partName : partNames)
			{
				QVector3D color = _oneFrameData->getMeshPartColor(partName);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size()/3, color).data());
				array = _partRenders[partName]->getLineDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size()/3, color).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size()/3, color).data());
			}
		}
	}
	void mPostModelRender::updateAllModelOperate(MViewBasic::PostModelOperateEnum postModelOperate)
	{
		std::set<QString> partnames{ _partOrder.begin(),_partOrder.end()};
		if (postModelOperate == ImportOperate)
		{
			UpdateModelBuffer();
			setIsShowInitialShape(_rendStatus->_isShowInitialShape);
			setShowFuntion(_rendStatus->_showFunction);
			//UpdateRigidWallBuffer();
			//updateAllModelOperate(UpdateMinMax);
		}
		else if (postModelOperate == HideAllPartOperate)
		{
			updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(HideOnePartOperate, partnames));
		}
		else if (postModelOperate == ShowAllPartOperate)
		{
			updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(ShowOnePartOperate, partnames));
		}
		else if (postModelOperate == TextureAllPartOperate)
		{
			updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(TextureOnePartOperate, partnames));
		}
		else if (postModelOperate == TransparentAllPartOperate)
		{
			updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(TransparentOnePartOperate, partnames));
		}
		else if (postModelOperate == ColorAllPartOperate)
		{
			updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(ColorOnePartOperate, partnames));
		}
		else if (postModelOperate == UpdateVariableValue)
		{
			QHash<int, float> values = _oneFrameRendData->getRendValue();
			for (auto it : _partRenders)
			{
				it->UpdatePartVariableValue(values, _oneFrameRendData->getNodeOrElement());
			}
		}
		else if (postModelOperate == ChangeColorAllPartOperate)
		{
			updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>>(ColorOnePartOperate, partnames));
		}
		else if (postModelOperate == UpdateMinMax)
		{
			if (_oneFrameRendData == nullptr)
			{
				return;
			}
			float minValue;
			float maxValue;
			if (_rendStatus->_rangeType == 0)
			{
				minValue = _oneFrameRendData->getMinData();
				maxValue = _oneFrameRendData->getMaxData();
				_oneFrameRendData->updateColorTableMinMaxData();
			}
			else
			{
				minValue = FLT_MAX;
				maxValue = -FLT_MAX;
				QHash<QString, QVector2D> ha = _oneFrameRendData->getPartMinMaxData();
				QHashIterator<QString, QVector2D> iter(ha);
				while (iter.hasNext())
				{
					iter.next();
					if (!_oneFrameData->getMeshPartVisual(iter.key()))
					{
						continue;
					}
					minValue = min(minValue, iter.value().x());
					maxValue = max(maxValue, iter.value().y());
				}
				_oneFrameRendData->setCurrentMinMaxData(minValue, maxValue);
			}
		}
		else if (postModelOperate == UpdateNormal)
		{


		}
	}

	//void mPostModelRender::setDistancePlane(std::vector<QVector4D> cutplane)
	//{
	//	if (_faceStateSet)
	//	{
	//		std::vector<GLuint> fs;
	//		for (int i = 0; i < 7; i++)
	//		{
	//			if (i < cutplane.size())
	//			{
	//				_faceStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetData(cutplane[i]);
	//				_faceStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(true);
	//				_edgelineStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetData(cutplane[i]);
	//				_edgelineStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(true);
	//				fs.push_back(i);
	//			}
	//			else
	//			{
	//				_faceStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(false);
	//				_edgelineStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(false);
	//			}
	//		}
	//		_faceStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(fs), 1);
	//		_edgelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(fs), 1);
	//	}
	//}

	//void mPostModelRender::setModelView(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	//{
	//	if (_faceStateSet)
	//	{
	//		//updateAllModelOperate(ColorAllPartOperate);
	//		_faceStateSet->getUniform("projection")->SetData(projection);
	//		_faceStateSet->getUniform("view")->SetData(view);
	//		_faceStateSet->getUniform("model")->SetData(model);
	//		_edgelineStateSet->getUniform("projection")->SetData(projection);
	//		_edgelineStateSet->getUniform("view")->SetData(view);
	//		_edgelineStateSet->getUniform("model")->SetData(model);
	//	}
	//}

	void mPostModelRender::setDeformationScale(QVector3D deformationScale)
	{
		if (_faceStateSet)
		{
			_faceStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_faceTransparentStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_edgelineStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_facelineStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_lineStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_pointStateSet->getUniform("deformationScale")->SetData(deformationScale);
		}
	}

	//void mPostModelRender::setLight(PostLight light, QVector3D viewpos, int lightIsOn)
	//{
	//	if (_faceStateSet)
	//	{
	//		_faceStateSet->getUniform("light.position")->SetData(light.lightPosition);
	//		_faceStateSet->getUniform("light.ambient")->SetData(light.ambient);
	//		_faceStateSet->getUniform("light.diffuse")->SetData(light.diffuse);
	//		_faceStateSet->getUniform("light.specular")->SetData(light.specular);
	//		_faceStateSet->getUniform("light.shiness")->SetData(light.shiness);
	//		_faceStateSet->getUniform("viewPos")->SetData(viewpos);
	//		_faceStateSet->getUniform("lightIsOn")->SetData(lightIsOn);

	//	}
	//}

	void mPostModelRender::setTextureArgument(int isEquivariance, float minValue, float maxValue, float textureCoordRatio)
	{
		if (_faceStateSet)
		{
			_faceStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_faceStateSet->getUniform("minValue")->SetData(minValue);
			_faceStateSet->getUniform("maxValue")->SetData(maxValue);
			_faceStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
			_facelineStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_facelineStateSet->getUniform("minValue")->SetData(minValue);
			_facelineStateSet->getUniform("maxValue")->SetData(maxValue);
			_facelineStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
			_lineStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_lineStateSet->getUniform("minValue")->SetData(minValue);
			_lineStateSet->getUniform("maxValue")->SetData(maxValue);
			_lineStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
			_pointStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_pointStateSet->getUniform("minValue")->SetData(minValue);
			_pointStateSet->getUniform("maxValue")->SetData(maxValue);
			_pointStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
		}
	}
	void mPostModelRender::setShowFuntion(ShowFuntion showFuntion)
	{
		if (_faceStateSet)
		{
			for (auto iter : _partRenders)
			{
				iter->setShowFuntion(showFuntion);
			}
		}
		if (showFuntion == WireFrame)
		{
			_facelineStateSet->getUniform("isAllColor")->SetData(float(0));
		}
		else
		{
			_facelineStateSet->getUniform("isAllColor")->SetData(float(1));
		}
	}
	void mPostModelRender::setIsShowInitialShape(bool isShowInitialShape)
	{

		for (auto iter : _partRenders)
		{
			iter->setIsShowInitialShape(isShowInitialShape);
		}
		
	}

	Space::AABB mPostModelRender::getModelAABB()
	{
		Space::AABB aabb;
		for(auto partrend : _partRenders)
		{
			if (partrend->getPartSpaceTree())
			{
				Space::AABB aabb1 = partrend->getPartSpaceTree()->space;

				aabb.push(Space::AABB::move(partrend->getPartSpaceTree()->space,_oneFrameRendData->getPartExplodeDis(partrend->getpartData()->getPartName())));
			}
		}
		return aabb;
	}
	Space::SpaceTree *mPostModelRender::getPartSpaceTree(QString partName)
	{
		return _partRenders.value(partName)->getPartSpaceTree();
	}
	void mPostModelRender::createExplodedGraph()
	{
		const QHash<int, QVector3D> &disp = _oneFrameRendData->getNodeDisplacementData();
		QVector<shared_ptr<mPostPartRender>> renders;
		shared_ptr<mPostPartRender> baseLinePartRender = nullptr;
		//根据面积最大的找到模型基准部件
		for (auto partRender : _partRenders)
		{
			if (baseLinePartRender == nullptr)
			{
				baseLinePartRender = partRender;
			}
			else if (partRender->getPartSpaceTree()->space.getSurfaceArea() > baseLinePartRender->getPartSpaceTree()->space.getSurfaceArea())
			{
				renders.append(baseLinePartRender);
				baseLinePartRender = partRender;
				renders.removeOne(partRender);
			}
			else
			{
				renders.append(partRender);
			}
		}

		Space::AABB aabb = baseLinePartRender->getPartSpaceTree()->space;
		QVector3D modelCenter = (aabb.maxEdge + aabb.minEdge) / 2.0;

		//根据距离模型的距离顺序进行从小到大排序
		sort(renders.begin(), renders.end(), [baseLinePartRender, modelCenter](std::shared_ptr<mPostPartRender> a, std::shared_ptr<mPostPartRender> b) {
			QVector3D center = a->getPartSpaceTree()->space.getCenter();
			float distance = modelCenter.distanceToPoint(center);
			QVector3D center1 = b->getPartSpaceTree()->space.getCenter();
			float distance1 = modelCenter.distanceToPoint(center1);

			return distance < distance1;
		});
		renders.prepend(baseLinePartRender);
		//与基准部件有交叉干涉的都往某方向偏移
		for (int i = 1; i < renders.size(); ++i)
		{
			auto partRender = renders.at(i);			
			QString partName = partRender->getpartData()->getPartName();
			Space::AABB aabb1 = this->getPartSpaceTree(partName)->space;

			//QVector3D center1_2 = (aabb1.maxEdge + aabb1.minEdge) / 2.0 - (aabb.maxEdge + aabb.minEdge) / 2.0;
			//center1_2 = QVector3D(abs(center1_2.x()), abs(center1_2.y()), abs(center1_2.z()));

			for (int j = i - 1; j < i; j++)
			{
				auto baseLine = renders.at(j);
				QString partName1 = baseLine->getpartData()->getPartName();
				Space::AABB baseLineaabb = baseLine->getPartSpaceTree()->space;
				baseLineaabb.move(_oneFrameRendData->getPartExplodeDis(partName1));

				aabb.push(baseLineaabb);
				if (aabb1.IsIntersect(aabb))
				{
					QVector3D center1_2 = (aabb.maxEdge + aabb.minEdge) / 2.0 - (aabb1.maxEdge + aabb1.minEdge) / 2.0;
					center1_2 = QVector3D(abs(center1_2.x()), abs(center1_2.y()), abs(center1_2.z()));

					float x = min(abs(aabb1.minEdge.x() - aabb.maxEdge.x()), abs(aabb1.maxEdge.x() - aabb.minEdge.x()));
					float y = min(abs(aabb1.minEdge.y() - aabb.maxEdge.y()), abs(aabb1.maxEdge.y() - aabb.minEdge.y()));
					float z = min(abs(aabb1.minEdge.z() - aabb.maxEdge.z()), abs(aabb1.maxEdge.z() - aabb.minEdge.z()));
					QVector3D dir(0,0,0);
					if (x <= y && x <= z)//x方向最小
					{
						float x1 = aabb1.minEdge.x() - aabb.maxEdge.x();
						float x2 = aabb1.maxEdge.x() - aabb.minEdge.x();
						if (abs(x1) < abs(x2))//
						{
							dir.setX(-x1);
						}
						else
						{
							dir.setX(-x2);
						}				
					}
					else if (y <= z)//y方向最小
					{
						float y1 = aabb1.minEdge.y() - aabb.maxEdge.y();
						float y2 = aabb1.maxEdge.y() - aabb.minEdge.y();
						if (abs(y1) < abs(y2))//
						{
							dir.setY(-y1);
						}
						else
						{
							dir.setY(-y2);
						}
					}
					else
					{
						float z1 = aabb1.minEdge.z() - aabb.maxEdge.z();
						float z2 = aabb1.maxEdge.z() - aabb.minEdge.z();
						if (abs(z1) < abs(z2))//
						{
							dir.setZ(-z1);
						}
						else
						{
							dir.setZ(-z2);
						}
					}
					//float factor = (min(min(x, y), z));

					//QVector3D partCenter = (aabb1.maxEdge + aabb1.minEdge) / 2.0;
					//QVector3D dir = partCenter - modelCenter;
					//QVector3D res = dir * factor;
					_oneFrameRendData->setPartExplodeDis(partName, dir);
					_partRenders[partName]->UpdatePartExplodeDis(dir);
					//_partRenders[partName]->calculateSpaceTreeThread(disp, dir);
				}
			}
		}
	}
	void mPostModelRender::createExplodedGraphByTransplatePart(set<QString> partNames, QVector3D dis)
	{
		const QHash<int, QVector3D> &disp = _oneFrameRendData->getNodeDisplacementData();
		for (auto partName : partNames)
		{
			QVector3D res = _oneFrameRendData->getPartExplodeDis(partName) + dis;
			_oneFrameRendData->setPartExplodeDis(partName, res);
			_partRenders[partName]->UpdatePartExplodeDis(res);
			//_partRenders[partName]->calculateSpaceTreeThread(disp, res);
		}
	}
	void mPostModelRender::createExplodedGraphByModelCenter(set<QString> partNames, QVector3D factor)
	{
		Space::AABB aabb = this->getModelAABB();
		const QHash<int, QVector3D> &disp = _oneFrameRendData->getNodeDisplacementData();
		QVector3D modelCenter = (aabb.maxEdge + aabb.minEdge) / 2.0;
		for (auto partName : partNames)
		{
			Space::AABB aabb1 = this->getPartSpaceTree(partName)->space;
			QVector3D partCenter = (aabb1.maxEdge + aabb1.minEdge) / 2.0;
			QVector3D dir = partCenter - modelCenter;
			QVector3D res = _oneFrameRendData->getPartExplodeDis(partName) + dir * factor;
			_oneFrameRendData->setPartExplodeDis(partName, res);
			_partRenders[partName]->UpdatePartExplodeDis(res);
			//_partRenders[partName]->calculateSpaceTreeThread(disp, res);
		}
	}
	void mPostModelRender::clearExplodedGraph()
	{
		const QHash<int, QVector3D> &disp = _oneFrameRendData->getNodeDisplacementData();
		QHashIterator<QString, shared_ptr<mPostPartRender>> iter(_partRenders);
		while (iter.hasNext())
		{
			iter.next();
			_oneFrameRendData->setPartExplodeDis(iter.key(), QVector3D(0,0,0));
			iter.value()->UpdatePartExplodeDis(QVector3D(0, 0, 0));
			//iter.value()->calculateSpaceTreeThread(disp, QVector3D(0, 0, 0));
		}
	}
}