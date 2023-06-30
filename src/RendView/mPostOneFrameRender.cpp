#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mPostCuttingPlaneRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
#include "mPostColorTableRender.h"
#include "mPostContourRender.h"
#include "mPostColorTableData.h"
#include "mPostStreamLineRender.h"

#include "mPostRendStatus.h"
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

#include "mPostOneFrameRendData.h"
#include "mOneFrameData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostOneFrameRender::mPostOneFrameRender(std::shared_ptr<mxr::Application> app, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData) : 
		_app(app), _rendStatus(rendStatus), _oneFrameData(oneFrameData), _oneFrameRendData(oneFrameRendData)
	{
		_cuttingPlaneStateSet = nullptr;
		_colorTableRender = nullptr;
		_postStreamLineRender = nullptr;


		_geode = MakeAsset<Geode>();
		_viewer = nullptr;

		this->initial();

		_modelRender = make_shared<mPostModelRender>(_geode, _rendStatus, oneFrameData, oneFrameRendData);

		_fontRender = make_shared<MBaseRend::mFontRender>(_app, _geode);
		_arrowRender = make_shared<MBaseRend::mArrowRender>(_app, _geode);

		//_fontRender->appendFixedFont("test", QVector<QVector2D>{QVector2D(0.5, 0.5)}, QVector<QString>{QString("test")});
		//_arrowRender->appendCommonArrow("test", QVector<QVector3D>{QVector3D(0, 0, 0)}, QVector<QVector3D>{QVector3D(1, 0, 0)});
		//_fontRender->setFixedFontIsShow("test", false);
		//_fontRender->appendFixedFont("test", QVector<QVector2D>{QVector2D(0.5, 0.5)}, QVector<QString>{QString("test2dfont")});
		//_fontRender->appendCommonFont("test", QVector<QVector3D>{QVector3D(0.5, 0.5, 0.5)}, QVector<QString>{QString("test3dfont")});
	}
	mPostOneFrameRender::~mPostOneFrameRender()
	{
		_modelRender.reset();
		delete _oneFrameRendData;
	}

	void mPostOneFrameRender::bufferThisFrame(QOpenGLContext *context)
	{
		context->makeCurrent(context->surface());
		_viewer->compile();
	}

	void mPostOneFrameRender::deleteThieFrame()
	{
		if (_viewer)
		{
			_viewer->deleteAllData();
		}
	}

	void mPostOneFrameRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		if (_modelRender)
		{
			//std::vector<QVector4D> cutplanes;
			//for (int i = 0; i < _cuttingPlaneRenders.size(); i++)
			//{
			//	cutplanes.push_back(_cuttingPlaneRenders[i]->getCuttingPlane());
			//}

			//模型
			_modelRender->setDeformationScale(_oneFrameRendData->getDeformationScale());
			_modelRender->setTextureArgument(_rendStatus->_isEquivariance, _oneFrameRendData->getCurrentMinData(), _oneFrameRendData->getCurrentMaxData(), _oneFrameRendData->getTextureCoordScale());
			if (_cuttingPlaneStateSet)
			{
				_cuttingPlaneStateSet->getUniform("isEquivariance")->SetData(_rendStatus->_isEquivariance);
				_cuttingPlaneStateSet->getUniform("minValue")->SetData(_oneFrameRendData->getCurrentMinData());
				_cuttingPlaneStateSet->getUniform("maxValue")->SetData(_oneFrameRendData->getCurrentMaxData());
				_cuttingPlaneStateSet->getUniform("textureCoordRatio")->SetData(_oneFrameRendData->getTextureCoordScale());
			}
			if (_contourLineStateSet)
			{
				_contourLineStateSet->getUniform("isEquivariance")->SetData(_rendStatus->_isEquivariance);
				_contourLineStateSet->getUniform("minValue")->SetData(_oneFrameRendData->getCurrentMinData());
				_contourLineStateSet->getUniform("maxValue")->SetData(_oneFrameRendData->getCurrentMaxData());
			}
			if (_contourFaceStateSet)
			{
				_contourFaceStateSet->getUniform("isEquivariance")->SetData(_rendStatus->_isEquivariance);
				_contourFaceStateSet->getUniform("minValue")->SetData(_oneFrameRendData->getCurrentMinData());
				_contourFaceStateSet->getUniform("maxValue")->SetData(_oneFrameRendData->getCurrentMaxData());
			}
			_colorTableRender->updatePostColorTable(_oneFrameRendData->getTextureCoordScale());
			//_modelRender->setTexture(_texture);
			//_modelRender->setDistancePlane(cutplanes);
		}
		if (_fontRender)
		{
			_fontRender->updateUniform(modelView, commonView);
		}
		if (_arrowRender)
		{
			_arrowRender->updateUniform(modelView, commonView);
		}
		_viewer->noClearRun();
	}

	void mPostOneFrameRender::resizeWindow(int w, int h)
	{
		if (_colorTableRender)
		{
			_colorTableRender->resizeWindow(_oneFrameRendData);
		}
	}

	void mPostOneFrameRender::updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>> postModelOperates)
	{
		if (_modelRender)
		{
			_modelRender->updateOneModelOperate(postModelOperates);
		}
	}

	void mPostOneFrameRender::updateAllModelOperate(MViewBasic::PostModelOperateEnum postModelOperate)
	{
		if (_modelRender)
		{
			_modelRender->updateAllModelOperate(postModelOperate);
		}
		if (postModelOperate == UpdateMinMax)//更新文字
		{
			_colorTableRender->updateText(_oneFrameRendData);
		}
	}

	void mPostOneFrameRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		_modelRender->setFaceStateSet(_faceStateSet);

		_colorTableRender = make_shared<mPostColorTableRender>(_fontRender, _oneFrameRendData, _rendStatus, _geode, _faceStateSet->getTexture("texture"));
	}

	void mPostOneFrameRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	{
		_faceTransparentStateSet = faceTransparentStateSet;
		_modelRender->setFaceTransparentStateSet(_faceTransparentStateSet);
	}

	void mPostOneFrameRender::setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNodeformationStateSet)
	{
		_faceTransparentNodeformationStateSet = faceTransparentNodeformationStateSet;
		_modelRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
	}

	void mPostOneFrameRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelineStateSet = edgelineStateSet;
		_modelRender->setEdgeLineStateSet(_edgelineStateSet);
	}

	void mPostOneFrameRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		_modelRender->setFaceLineStateSet(_facelineStateSet);
	}

	void mPostOneFrameRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
		_modelRender->setLineStateSet(_lineStateSet);
	}

	void mPostOneFrameRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
		_modelRender->setPointStateSet(_pointStateSet);
	}

	void mPostOneFrameRender::setTexture(mxr::Texture * texture)
	{
		_texture = texture;
	}

	void mPostOneFrameRender::setDeformationScale(QVector3D deformationScale)
	{
		if (_oneFrameRendData)
		{
			_oneFrameRendData->setDeformationScale(deformationScale);
		}
		if (_modelRender)
		{
			_modelRender->updateAllModelOperate(UpdateNormal);
		}
	}

	void mPostOneFrameRender::setTextureCoordScale(float textureCoordScale)
	{
		if (_oneFrameRendData)
		{
			_oneFrameRendData->setTextureCoordScale(textureCoordScale);
		}
	}

	void mPostOneFrameRender::setMinMaxData(float maxValue, float minValue)
	{
		if (_oneFrameRendData)
		{
			_oneFrameRendData->setMinMaxData(maxValue, minValue);
			this->updateAllModelOperate(UpdateMinMax);
		}
	}

	bool mPostOneFrameRender::deleteCuttingPlane(int num)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return false;
		}
		for (int i = num; i < _cuttingPlaneRenders.size(); i++)
		{
			_cuttingPlaneRenders[i]->updateCuttingPlaneIndex(i - 1);
		}
		_cuttingPlaneRenders[num].reset();
		_cuttingPlaneRenders[num] = nullptr;
		_cuttingPlaneRenders.removeAt(num);
		return true;
	}

	bool mPostOneFrameRender::reverseCuttingPlaneNormal(int num)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return false;
		}
		_cuttingPlaneRenders[num]->reverseCuttingPlaneNormal();
		return true;
	}

	void mPostOneFrameRender::setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane)
	{
		if (isOnlyShowCuttingPlane)
		{
			_modelRender->hideModelRender();
		}
		else
		{
			this->updateOneModelOperate({ShowOnePartOperate ,_oneFrameData->getAllPartNames() });
		}
		//_modelRender
	}


	void mPostOneFrameRender::setIsShowCuttingPlane(int num, bool isShow)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return;
		}
		_cuttingPlaneRenders[num]->setIsShowCuttingPlane(isShow);
	}

	bool mPostOneFrameRender::createCuttingPlane(std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet, int num, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		_cuttingPlaneStateSet = cuttingPlaneStateSet;
		_transparentPlaneStateSet = transparentPlaneStateSet;
		if (num > 7 || num > _cuttingPlaneRenders.size())
		{
			return false;
		}
		std::shared_ptr<mPostCuttingPlaneRender> planerender = make_shared<mPostCuttingPlaneRender>(_geode, cuttingPlaneStateSet, transparentPlaneStateSet);
		planerender->createCuttingPlane(_oneFrameData, _oneFrameRendData, num, normal, vertex, hasVector);
		if (num < _cuttingPlaneRenders.size())
		{
			_cuttingPlaneRenders[num].reset();
			_cuttingPlaneRenders.replace(num, planerender);
		}
		else if (num == _cuttingPlaneRenders.size())
		{
			_cuttingPlaneRenders.append(planerender);
		}

		return true;
	}

	void mPostOneFrameRender::setPlaneData(int num, QVector3D normal, QVector3D centervertex, float maxR)
	{
		if (num > 7 || num > _cuttingPlaneRenders.size())
		{
			return;
		}
		if (num < _cuttingPlaneRenders.size())
		{
			auto postCuttingPlane = _cuttingPlaneRenders[num];
			postCuttingPlane->setPlaneData(normal, centervertex, maxR);
		}
	}

	void mPostOneFrameRender::setIsShowPlane(bool isShow)
	{
		_rendStatus->_isShowTransparentPlane = isShow;
		for (auto cuttingPlane : _cuttingPlaneRenders)
		{
			cuttingPlane->setIsShowCuttingPlane(isShow);
		}
		//_isShowPlane = isShow;
	}
	void mPostOneFrameRender::createVectorGraph(QVector<QPair<QString, QVector3D>> type_color, double size)
	{
		QPair<QVector<QVector3D>, QVector<QVector3D>> res = this->getCuttingNodeData();	//通过原模型生成节点
		createVectorGraph(res);
	}
	void mPostOneFrameRender::createVectorGraph(std::set<int> nodeIDs, QVector<QPair<QString, QVector3D>> type_color, double size)
	{
		QPair<QVector<QVector3D>, QVector<QVector3D>> res = this->getPickingNodeData(nodeIDs);//通过拾取节点生成
		createVectorGraph(res);
	}
	void mPostOneFrameRender::deleteVectorGraph()
	{
		_arrowRender->clearAllRender();
	}
	void mPostOneFrameRender::createVectorGraph(QPair<QVector<QVector3D>, QVector<QVector3D>> res)
	{
		QVector<QPair<QString, QVector3D>> type_color = _rendStatus->_vectorArrowTypeColor;
		for (int i = 0; i < type_color.size(); ++i)
		{
			QVector<QVector3D> pos;
			QVector<QVector3D> dir;
			for (int j = 0; j < res.first.size(); ++j)
			{
				QVector3D d = res.second[j];
				pos.append(res.first[j]);
				if (type_color[i].first == "X")
				{
					d = QVector3D(d.x(), 0, 0);
				}
				else if (type_color[i].first == "Y")
				{
					d = QVector3D(0, d.y(), 0);

				}
				else if (type_color[i].first == "Z")
				{
					d = QVector3D(0, 0, d.z());

				}
				else if (type_color[i].first == "XY")
				{
					d = QVector3D(d.x(), d.y(), 0);

				}
				else if (type_color[i].first == "XZ")
				{
					d = QVector3D(d.x(), 0, d.z());

				}
				else if (type_color[i].first == "YZ")
				{
					d = QVector3D(0, d.y(), d.z());
				}
				else if (type_color[i].first == "XYZ")
				{
					d = QVector3D(d.x(), d.y(), d.z());
				}
				dir.append(d);
			}
			_arrowRender->appendCommonArrow(QString("vectorGraph%1").arg(type_color[i].first), pos, dir, type_color.at(i).second, _rendStatus->_vectorArrowSize);
		}
	}
	void mPostOneFrameRender::deleteContourGraph()
	{
		_postContourRenders.clear();
	}
	void mPostOneFrameRender::deleteContourGraph(int i)
	{
		if (i >= _postContourRenders.size())
		{
			return;
		}
		_postContourRenders.removeAt(i);
	}
	void mPostOneFrameRender::createContourGraph(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_contourLineStateSet = lineStateSet;
		_contourFaceStateSet = faceStateSet;
		deleteContourGraph();

		std::shared_ptr<mPostContourRender> render = MakeAsset<mPostContourRender>(_app, _geode, _oneFrameData, _oneFrameRendData);
		render->setLineStateSet(lineStateSet);
		render->setFaceStateSet(faceStateSet);
		//render->resetDrawable();
		render->updateData(_oneFrameRendData->getCurrentMinData(), _oneFrameRendData->getCurrentMaxData(), _oneFrameRendData->getRendColorTable()->getPostColorTableNum() - 1);
		_postContourRenders.append(render);
	}
	void mPostOneFrameRender::createContourGraph(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> faceStateSet, int i, double value, bool isshow)
	{
		_contourLineStateSet = lineStateSet;
		_contourFaceStateSet = faceStateSet;
		std::shared_ptr<mPostContourRender> render;
		if (i == _postContourRenders.size())//添加
		{
			render = MakeAsset<mPostContourRender>(_app, _geode, _oneFrameData, _oneFrameRendData);
			render->setLineStateSet(faceStateSet);
			render->setFaceStateSet(faceStateSet);
			render->setVisiable(isshow);
			_postContourRenders.append(render);
		}
		//render->resetDrawable();
		_postContourRenders[i]->updateData(_oneFrameRendData->getCurrentMinData(), _oneFrameRendData->getCurrentMaxData(), QVector<float>{float(value)});
		_postContourRenders[i]->setVisiable(isshow);
	}
	void mPostOneFrameRender::setContourGraph(int i, bool isshow)
	{
		if (i >= _postContourRenders.size())
		{
			return;
		}
		_postContourRenders[i]->setVisiable(isshow);
	}
	void mPostOneFrameRender::deleteStreamLine()
	{
		if (_postStreamLineRender)
		{
			_postStreamLineRender.reset();
		}
	}
	void mPostOneFrameRender::setStreamLineShowForm(int streamLineShowForm)
	{
		if (_postStreamLineRender)
		{
			_postStreamLineRender->setStreamLineShowForm(streamLineShowForm);
		}
	}
	void mPostOneFrameRender::setIntergrateDirection(int intergrateDirection)
	{
		if (_postStreamLineRender)
		{
			_postStreamLineRender->setIntergrateDirection(intergrateDirection);
		}
	}
	void mPostOneFrameRender::createStreamLine(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> pointStateSet, QVector3D center, float radius, int streamLineNum, float ratio)
	{
		_streamlinePointStateSet = pointStateSet;
		if (!_postStreamLineRender)
		{
			_postStreamLineRender = MakeAsset<mPostStreamLineRender>(_app, _geode, _oneFrameData, _rendStatus, _oneFrameRendData, streamLineNum, ratio);
			_postStreamLineRender->setLineStateSet(lineStateSet);
			_postStreamLineRender->setPointStateSet(pointStateSet);
			_postStreamLineRender->setSphereParameter(center, radius);
			_postStreamLineRender->OrganizeModeldata();
		}
	}
	void mPostOneFrameRender::createExplodedGraph()
	{
		if (_modelRender)
		{
			_modelRender->createExplodedGraph();
		}
	}
	void mPostOneFrameRender::createExplodedGraphByTransplatePart(set<QString> partNames, QVector3D dis)
	{
		if (_modelRender)
		{
			_modelRender->createExplodedGraphByTransplatePart(partNames, dis);
		}
	}
	void mPostOneFrameRender::createExplodedGraphByModelCenter(set<QString> partNames, QVector3D factor)
	{
		if (_modelRender)
		{
			_modelRender->createExplodedGraphByModelCenter(partNames, factor);
		}
	}
	QPair<QVector<QVector3D>, QVector<QVector3D>> mPostOneFrameRender::getPickingNodeData(std::set<int> nodeIds)
	{
		if (_oneFrameData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}

		if (_oneFrameRendData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}

		QVector<QVector3D> resVertexs;
		QVector<QVector3D> resValues;

		QHash<int, QVector3D> values = _oneFrameRendData->getVectorRendData();
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();

		for (auto nodeID : nodeIds)
		{
			mPostMeshNodeData1 *nodeData = _oneFrameData->getNodeDataByID(nodeID);
			if (nodeData == nullptr)
			{
				continue;
			}
			resVertexs.append(nodeData->getNodeVertex() + deformationScale * dis.value(nodeID));
			resValues.append(values.value(nodeID));
		}

		return QPair<QVector<QVector3D>, QVector<QVector3D>>{ resVertexs, resValues};
	}
	QPair<QVector<QVector3D>, QVector<QVector3D>> MPostRend::mPostOneFrameRender::getCuttingNodeData()
	{
		if (_oneFrameData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}

		if (_oneFrameRendData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}
		QVector<QPair<QVector3D, QVector3D>> postCuttingNormalVertexs;
		for (auto cuttingPlane : _cuttingPlaneRenders)
		{
			if (cuttingPlane != nullptr&&cuttingPlane->_isShowCuttingPlane)
			{
				postCuttingNormalVertexs.append({ cuttingPlane->getCuttingPlaneData()->getCuttingPlaneNormal(), cuttingPlane->getCuttingPlaneData()->getCuttingPlaneVertex() });
			}
		}

		if (_oneFrameData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}

		if (_oneFrameRendData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}

		QVector<QVector3D> resVertexs;
		QVector<QVector3D> resValues;

		QHash<int, QVector3D> values = _oneFrameRendData->getVectorRendData();
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();

		QHashIterator<QString, mPostMeshPartData1*> iter(_oneFrameData->getMeshPartIterator());
		while (iter.hasNext())
		{
			iter.next();
			QString partName = iter.key();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr || !partData->getPartVisual())
			{
				continue;
			}

			//三维网格的节点
			set<int> meshIDs = partData->getMeshIDs3();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					bool isCutting{ false };
					for (QPair<QVector3D, QVector3D> normalVertex : postCuttingNormalVertexs)
					{
						if (vertex.distanceToPlane(normalVertex.second, normalVertex.first) < 0)
						{
							isCutting = true;
							break;
						}
					}
					if (!isCutting)//没有被裁剪
					{
						resVertexs.append(vertex);
						resValues.append(values.value(meshData->getNodeIndex().at(j)));
					}

				}
			}

			//一维网格的节点
			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < meshData->getNodeIndex().size(); ++j)
				{
					QVector3D vertex = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					bool isCutting{ false };
					for (QPair<QVector3D, QVector3D> normalVertex : postCuttingNormalVertexs)
					{
						if (vertex.distanceToPlane(normalVertex.second, normalVertex.first) < 0)
						{
							isCutting = true;
							break;
						}
					}
					if (!isCutting)//没有被裁剪
					{
						resVertexs.append(vertex);
						resValues.append(values.value(meshData->getNodeIndex().at(j)));
					}
				}
			}

			//二维网格的节点
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex();

					QVector3D vertex = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					bool isCutting{ false };
					for (QPair<QVector3D, QVector3D> normalVertex : postCuttingNormalVertexs)
					{
						if (vertex.distanceToPlane(normalVertex.second, normalVertex.first) < 0)
						{
							isCutting = true;
							break;
						}
					}
					if (!isCutting)//没有被裁剪
					{
						resVertexs.append(vertex);
						resValues.append(values.value(meshData->getNodeIndex().at(j)));
					}
				}
			}
		}

		return QPair<QVector<QVector3D>, QVector<QVector3D>>{ resVertexs, resValues};
	}
	QVector<QVector3D> MPostRend::mPostOneFrameRender::getCuttingPlaneData(int index)
	{
		return _cuttingPlaneRenders[index]->getCuttingPlaneData()->getTriVertex();
	}
	void mPostOneFrameRender::getMinMaxIDs(QVector<int>& minids, QVector<int>& maxids)
	{
		QHash<int, float> values = _oneFrameRendData->getRendValue();
		QSet<int> s = values.keys().toSet();
		if (_oneFrameData == nullptr)
		{
			return;
		}
		float _minValue = _oneFrameRendData->getCurrentMinData();
		float _maxValue = _oneFrameRendData->getCurrentMaxData();
		set<int> ids;
		if (_oneFrameRendData->getNodeOrElement() == PostElement)
		{
			ids = _oneFrameData->getAllMeshIDs();		
		}
		else
		{
			ids = _oneFrameData->getAllNodeIDs();	
		}
		for (int meshID : ids)
		{
			if (s.contains(meshID))
			{
				float value = values.value(meshID);
				if (value <= _minValue)
				{
					minids.append(meshID);
				}
				else if (value >= _maxValue)
				{
					maxids.append(meshID);
				}
			}
		}
	}
	void mPostOneFrameRender::getVertexs(NodeOrElement nodeOrElement, QVector<int> minids, QVector<int> maxids, QVector<QVector3D>& minvertexs, QVector<QVector3D>& maxvertexs)
	{
		QHash<int, QVector3D> dispdata = _oneFrameRendData->getNodeDisplacementData();
		QVector3D dfactor = _oneFrameRendData->getDeformationScale();
		if (nodeOrElement == PostNode)
		{
			for (int nodeID : minids)
			{
				QVector3D vertex;
				mPostMeshNodeData1* nodedata = _oneFrameData->getNodeDataByID(nodeID);
				if (nodedata == nullptr)
				{
					continue;
				}
				vertex = nodedata->getNodeVertex() + dispdata.value(nodeID) * dfactor;

				minvertexs.append(vertex);
			}
			for (int nodeID : maxids)
			{
				QVector3D vertex;
				mPostMeshNodeData1* nodedata = _oneFrameData->getNodeDataByID(nodeID);
				if (nodedata == nullptr)
				{
					continue;
				}
				vertex = nodedata->getNodeVertex() + dispdata.value(nodeID) * dfactor;

				maxvertexs.append(vertex);
			}
		}
		else
		{
			for (int meshID : minids)
			{
				//获取单元数据
				mPostMeshData1* meshdata = _oneFrameData->getMeshDataByID(meshID);
				if (meshdata == nullptr)
				{
					continue;
				}
				QVector3D vertex;
				//获取单元节点
				QVector<int> nodeIDs = meshdata->getNodeIndex();
				for (int nodeID : nodeIDs)
				{
					mPostMeshNodeData1* nodedata = _oneFrameData->getNodeDataByID(nodeID);
					if (nodedata == nullptr)
					{
						continue;
					}
					vertex += nodedata->getNodeVertex() + dispdata.value(nodeID) * dfactor;
				}
				minvertexs.append(vertex / nodeIDs.size());
			}
			for (int meshID : maxids)
			{
				//获取单元数据
				mPostMeshData1* meshdata = _oneFrameData->getMeshDataByID(meshID);
				if (meshdata == nullptr)
				{
					continue;
				}
				QVector3D vertex;
				//获取单元节点
				QVector<int> nodeIDs = meshdata->getNodeIndex();
				for (int nodeID : nodeIDs)
				{
					mPostMeshNodeData1* nodedata = _oneFrameData->getNodeDataByID(nodeID);
					if (nodedata == nullptr)
					{
						continue;
					}
					vertex += nodedata->getNodeVertex() + dispdata.value(nodeID) * dfactor;
				}
				maxvertexs.append(vertex / nodeIDs.size());
			}
		}
	}
	void mPostOneFrameRender::initial()
	{
		if (!_viewer)
		{
			_viewer = MakeAsset<mxr::Viewer>();
			_viewer->setSceneData(_geode);
		}
	}

	void mPostOneFrameRender::makeCurrent()
	{
		_app->GLContext()->makeCurrent(_app->GLContext()->surface());
	}

	void mPostOneFrameRender::doneCurrent()
	{
		_app->GLContext()->doneCurrent();
	}

}