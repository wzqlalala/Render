#include "MxRendTest.h"
#include "mPreRend.h"
#include "mPreRender.h"
#include "mPostRend.h"
#include "mTestRender.h"
#include "mPostRender.h"
#include "mModelTestRender.h"
#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
#include "mModelView.h"

#include "mContainers.h"

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostMeshPickData.h"

//MDataGeo
#include "mGeoModelData1.h"
#include "mGeoPartData1.h"
#include "mGeoSolidData1.h"
#include "mGeoFaceData1.h"
#include "mGeoLineData1.h"
#include "mGeoPointData1.h"
#include "mGeoPickData1.h"

#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QCollator>

#include "mIMxdbFile1.h"

using namespace MDataPost;
MxRendTest::MxRendTest(int id)
{
	_id = id;
    ui.setupUi(this);
	_preRend = nullptr;
	_postRend = nullptr;
	_testRender = nullptr;
	this->showMaximized();
	if (id %2 == 0)
	{
		_preRend = new MPreRend::mPreRend(QString::number(id)); ui.gridLayout->addWidget(_preRend);
	}
	else
	{
		_postRend = new MPostRend::mPostRend(QString::number(id)); ui.gridLayout->addWidget(_postRend);
	}

	
}

void MxRendTest::keyPressEvent(QKeyEvent * event)
{
	if (_id % 2 == 1)//����
	{
		switch (event->key())
		{
		case Qt::Key_Escape:
		{
			if (_postRender != nullptr)
			{
				_postRender->clearRender();
			};
			break;
		}

		case Qt::Key_F2:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			//_postRender = make_shared<MPostRend::mPostRender>(_postRend->getApplication(), _postRend->getRoot());
			_postRender = _postRend->getPostRender();
			shared_ptr<mDataPost1> dp = make_shared<mDataPost1>();
			mPostOneFrameRendData *oneFrameRendData = new mPostOneFrameRendData();
			//oneFrameRendData->setDeformationScale(QVector3D(10.0f, 10.0f, 10.0f));
			if (!getMxDbData(dp, oneFrameRendData))
			{
				delete oneFrameRendData;
				return;
			}
			_postRender->setPostData(dp);
			_postRender->setRendCurrentFrameData(oneFrameRendData);
			//_postRend->addBeforeRender(_postRender);
			Space::AABB aabb = _postRender->getOneFrameRender()->getModelRender()->getModelAABB();
			QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
			float radius = (aabb.maxEdge - aabb.minEdge).length() / 2.0;
			_postRend->getCamera()->ResetOrthoAndCamera(center, radius);
			_postRend->getCamera1()->ResetOrthoAndCamera(center, radius);
			_postRender->setShowFuntion(ElementFace);
			break;
		}
		case Qt::Key_Tab:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			if (_cameraType == CameraType::Camera1)
			{
				_cameraType = CameraType::Camera2;
			}
			else
			{
				_cameraType = CameraType::Camera1;
			}
			_postRend->setCameraType(_cameraType);
			break;
		}
		case Qt::Key_1:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->setIsShowInitialShape(_isShowInitialShape);
			_isShowInitialShape = !_isShowInitialShape;
			//_postRender->getOneFrameRender()->updateAllModelOperate(ShowNodeformation);
			//_postRender->getOneFrameRender()->updateOneModelOperate({ HideOnePartOperate, set<QString>{"Bolt"} });
			break;
		}
		case Qt::Key_2:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->updateAllModelOperate(_modelOperate);
			int id = int(_modelOperate);
			if (_modelOperate == TextureAllPartOperate)
			{
				_modelOperate = ColorAllPartOperate;
			}
			else
			{
				id++;
				_modelOperate = PostModelOperateEnum(id);
			}
			break;
		}
		case Qt::Key_3:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->updateAllModelOperate(_modelOperate1);
			if (_modelOperate1 == HideAllPartOperate)
			{
				auto list = _postRender->getOneFrameRender()->getOneFrameData()->getAllPartNameList();
				_hideNames.insert(list.begin(), list.end());
				_modelOperate1 = ShowAllPartOperate;
			}
			else
			{
				_hideNames.clear();
				_modelOperate1 = HideAllPartOperate;
			}
			break;
		}
		case Qt::Key_4:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->setDeformationScale(QVector3D(1.0, 1.0, 1.0));
			break;
		}
		case Qt::Key_5:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->setDispersed(_dispersed);
			_dispersed = !_dispersed;
			break;
		}
		case Qt::Key_6:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->setDispersIsEquivariance(_isEquivariance);
			_isEquivariance = !_isEquivariance;
			break;
		}
		case Qt::Key_7:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			set<QString> names = _postRender->getOneFrameRender()->getOneFrameData()->getAllPartNames();
			if (names.size() == 0)
			{
				return;
			}
			_hideNames.insert(*names.begin());
			_postRender->updateOneModelOperate({ HideOnePartOperate, set<QString>{*names.begin()} });
			break;
		}
		case Qt::Key_8:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			if (_hideNames.size() == 0)
			{
				return;
			}
			QString name = *_hideNames.rbegin();
			_hideNames.erase(name);
			_postRender->updateOneModelOperate({ ShowOnePartOperate, set<QString>{name} });
			break;
		}
		case Qt::Key_Q:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			Space::AABB aabb = _postRender->getOneFrameRender()->getModelRender()->getModelAABB();
			float radius = (aabb.maxEdge - aabb.minEdge).length() / 2.0;
			QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
			_postRender->createCuttingPlane(_cuttingPlaneNum, planeNormals.at(_cuttingPlaneNum), center);
			_postRender->setPlaneData(_cuttingPlaneNum, planeNormals.at(_cuttingPlaneNum), center, radius);
			_cuttingPlaneNum++;
			break;
		}
		case Qt::Key_Y:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			if (_cuttingPlaneNum > 0)
			{
				_postRender->deleteCuttingPlane(0);
				_cuttingPlaneNum--;
			}
			break;
		}
		case Qt::Key_E:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			if (_cuttingPlaneNum > 0)
			{
				_postRender->deleteCuttingPlane(_cuttingPlaneNum - 1);
				_cuttingPlaneNum--;
			}
			break;
		}
		case Qt::Key_R:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			if (_cuttingPlaneNum > 0)
			{
				_postRender->reverseCuttingPlaneNormal(_cuttingPlaneNum - 1);
			}
			break;
		}
		case Qt::Key_T:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_isShowPlane = !_isShowPlane;
			_postRender->setIsShowPlane(_isShowPlane);
			break;
		}
		case Qt::Key_U:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_isOnlyShowPlane = !_isOnlyShowPlane;
			_postRender->setOnlyShowCuttingPlane(_isOnlyShowPlane);
			break;
		}
		case Qt::Key_G:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->createLinearAnimation(_isLinearAnimation ? OneFrameLinearAnimation : OneFrameSinAnimation);
			break;
		}
		case Qt::Key_F:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->setPostMode(_postMode);
			break;
		}
		case Qt::Key_H:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->setTimerOn(_isLinearAnimation);
			_isLinearAnimation = !_isLinearAnimation;
			break;
		}
		case Qt::Key_J:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_postRender->deleteAnimation();
			break;
		}
		case Qt::Key_Z:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			int id = int(_multuiplyPickMode);
			if (_multuiplyPickMode == MultiplyPickMode::PolygonPick)
			{
				_multuiplyPickMode = MultiplyPickMode::QuadPick;
			}
			else
			{
				id++;
				_multuiplyPickMode = MultiplyPickMode(id);
			}
			_postRend->setMultiplyPickMode(_multuiplyPickMode);
			break;
		}
		case Qt::Key_X:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			if (_pickfilterID == (_pickfilters.size() - 1))
			{
				_pickfilterID = 0;
			}
			else
			{
				_pickfilterID++;
			}
			_postRend->setPickFilter(_pickfilters.at(_pickfilterID));
			break;
		}
		case Qt::Key_C:
		{
			if (_postRend == nullptr)
			{
				return;
			}

			_postRender->getMeshPickData()->clearAllPickData();
			_postRender->updateHighLightRender();
			break;
		}
		case Qt::Key_K:
		{
			if (_postRend == nullptr)
			{
				return;
			}

			_postRend->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Zoom);
			_postRend->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Rotate);
			_postRend->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Translate);
			break;
		}
		case Qt::Key_L:
		{
			if (_postRend == nullptr)
			{
				return;
			}

			_postRend->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			_postRend->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), PickMode::SoloPick);
			break;
		}
		case Qt::Key_O:
		{
			if (_postRend == nullptr)
			{
				return;
			}

			_postRend->getArrowRender()->appendCommonArrow("test0", QVector<QVector3D>{QVector3D(0, 0, 0)}, QVector<QVector3D>{QVector3D(1, 0, 0)});
			_postRend->getArrowRender()->appendCommonArrow("test1", QVector<QVector3D>{QVector3D(0.5, 0, 0)}, QVector<QVector3D>{QVector3D(0, 1, 0)});
			_postRend->getArrowRender()->appendCommonArrow("test2", QVector<QVector3D>{QVector3D(0, 0, 0.5)}, QVector<QVector3D>{QVector3D(0, 0, 1)});
			break;
		}
		case Qt::Key_P:
		{
			if (_postRend == nullptr)
			{
				return;
			}

			_postRend->getArrowRender()->appendCommonArrow("test", QVector<QVector3D>{QVector3D(0, 0.5, 0)}, QVector<QVector3D>{QVector3D(1, 0, 0)});

			_postRend->getFontRender()->appendFixedFont("test", QVector<QVector2D>{QVector2D(0.5, 0.5)}, QVector<QString>{QString("test2dfont")});
			_postRend->getFontRender()->appendCommonFont("test", QVector<QVector3D>{QVector3D(0, 0, 0)}, QVector<QString>{QString("test3dfont")});
			break;
		}
		case Qt::Key_Asterisk:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			int show = int(_showfuntion);
			if (_showfuntion == SmoothShaded)
			{
				show = 0;
			}
			else
			{
				show++;
			}
			_showfuntion = ShowFuntion(show);
			_postRender->setShowFuntion(_showfuntion);
			break;
		}
		case Qt::Key_Minus:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_pointSize += 1;
			if (_pointSize > 20.0f)
			{
				_pointSize = 1.0f;
			}
			_postRender->setPointSize(_pointSize);
			break;
		}
		case Qt::Key_Plus:
		{
			if (_postRend == nullptr)
			{
				return;
			}
			_lineWidth += 0.1f;
			if (_lineWidth > 20.0f)
			{
				_lineWidth = 1.0f;
			}

			_postRender->setEdgeLineWidth(_lineWidth);			
			break;
		}
		case Qt::Key_0:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			if (!_testRender)
			{
				_testRender = make_shared<MBaseRend::mTestRender>(_preRend->getApplication(), _preRend->getRoot());
				_preRend->addBeforeRender(_testRender);
			}
			_testRender->appendOnePart();
			_preRend->getCamera()->ResetOrthoAndCamera(QVector3D(0, 0, 0), 5.0);
		}
		default:
			break;
		}
	}
	else//ǰ����
	{
		//qDebug() << event->key();
		switch (event->key())
		{
		case Qt::Key_F1:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			QString filename = QFileDialog::getOpenFileName(this, "ѡ��obj�ļ�", qApp->applicationDirPath(), "*.obj");
			QFileInfo info(filename);

			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string warn;
			std::string err;
			QString baseDir = info.absolutePath();
			bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.toLocal8Bit(), baseDir.toLocal8Bit(), true);
			if (!warn.empty()) {
				qDebug() << "WARN: " << QString::fromStdString(warn) << endl;
			}

			if (!err.empty()) {
				qDebug() << "ERR: " << QString::fromStdString(err) << endl;
			}

			if (!ret) {
				printf("Failed to load/parse .obj.\n");
				return;
			}
			if (!_modelRender)
			{
				//_preRend->removeRender(_modelRender);
			}

			_modelRender = make_shared<MBaseRend::mModelTestRender>(_preRend->getApplication(), _preRend->getRoot());
			auto model = _modelRender->setData(attrib, shapes, materials);
			_preRend->addBeforeRender(_modelRender);
			_preRend->getCamera()->ResetOrthoAndCamera(model.first, model.second);
			break;
		}
		case Qt::Key_Escape:
		{
			if (_preRender != nullptr)
			{
				_preRender->clearRender();
			}
			break;
		}

		case Qt::Key_F2:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			//_postRender = make_shared<MPostRend::mPostRender>(_postRend->getApplication(), _postRend->getRoot());
			_preRender = _preRend->getPreRender();
			mGeoModelData1 *geoModelData = _preRender->getGeoModelData();
			if (!createGeo(geoModelData))
			{
				return;
			}
			break;
		}
		case Qt::Key_F3://��һ���ļ������������ʸ����Ϣ
		{
			if (_preRend == nullptr)
			{
				return;
			}
			//_postRender = make_shared<MPostRend::mPostRender>(_postRend->getApplication(), _postRend->getRoot());
			_preRender = _preRend->getPreRender();
			if (readTxtFile())
			{
				return;
			}
			//_preRend->slotResetOrthoAndCamera();
			break;
		}
		case Qt::Key_0:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			_preRend->FitView();
			break;
		}
		case Qt::Key_1:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			_preRend->FitView();
			break;
		}
		case Qt::Key_4:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			set<int> ids = _preRender->getGeoPickData()->getPickLineIDs();
			if (ids.size() == 0)
			{
				return;
			}
			_preRender->getGeoModelData()->setGeoLineProperty(ids, GeoLineProperty::DottedTopology);
			_preRender->getGeoPickData()->clearAllPickData();
			_preRender->updateHighLightRender();
			break;
		}
		case Qt::Key_7:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			set<QString> names = _preRender->getGeoModelData()->getAllGeoPartNames();
			if (names.size() == 0)
			{
				return;
			}
			_hideNames.insert(*names.begin());
			_preRender->getGeoModelData()->setGeoPartVisual(*names.begin(), false);
			break;
		}
		case Qt::Key_8:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			if (_hideNames.size() == 0)
			{
				return;
			}
			QString name = *_hideNames.rbegin();
			_hideNames.erase(name);
			_preRender->getGeoModelData()->setGeoPartVisual(name, true);
			break;
		}
		case Qt::Key_Tab:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			if (_cameraType == CameraType::Camera1)
			{
				_cameraType = CameraType::Camera2;
			}
			else
			{
				_cameraType = CameraType::Camera1;
			}
			_preRend->setCameraType(_cameraType);
			break;
		}
		case Qt::Key_Z:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			int id = int(_multuiplyPickMode);
			if (_multuiplyPickMode == MultiplyPickMode::PolygonPick)
			{
				_multuiplyPickMode = MultiplyPickMode::QuadPick;
			}
			else
			{
				id++;
				_multuiplyPickMode = MultiplyPickMode(id);
			}
			_preRend->setMultiplyPickMode(_multuiplyPickMode);
			break;
		}
		case Qt::Key_X:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			if (_pickfilterID == (_pickfilters1.size() - 1))
			{
				_pickfilterID = 0;
			}
			else
			{
				_pickfilterID++;
			}
			_preRend->setPickFilter(_pickfilters1.at(_pickfilterID));
			break;
		}
		case Qt::Key_Minus:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			_pointSize += 1;
			if (_pointSize > 20.0f)
			{
				_pointSize = 1.0f;
			}
			_preRender->setPointSize(_pointSize);
			break;
		}
		case Qt::Key_Plus:
		{
			if (_preRend == nullptr)
			{
				return;
			}
			_lineWidth += 0.1f;
			if (_lineWidth > 20.0f)
			{
				_lineWidth = 1.0f;
			}

			_preRender->setEdgeLineWidth(_lineWidth);
			break;
		}
		}

	}

	if (_preRend)
	{
		_preRend->update();
	}
	if (_postRend)
	{
		_postRend->update();
	}

}

bool MxRendTest::getMxDbData(shared_ptr<mDataPost1> dp, mPostOneFrameRendData *oneFrameRendData)
{
	QStringList names = QFileDialog::getOpenFileNames(this, "ѡ��obj�ļ�", qApp->applicationDirPath(), "*.mxdb;*.mxdb0");
	
	if (names.empty())
	{
		return false;
	}

	//��������ɸѡ����
	QMultiHash<QString, QString> postframefiles;
	QHash<QString, QString> postinitfile;

	//��ȡ��ǰ�ļ��������е�mxdb0�ļ�
	QFileInfo initinfo(names.at(0));
	QDir filedir = initinfo.absoluteDir();
	QStringList db0filter = QStringList{ QString("*.mxdb0") };
	QFileInfoList fileInfos = filedir.entryInfoList(db0filter, QDir::Files, QDir::Time | QDir::Reversed);
	for (QFileInfo finfo : fileInfos)
	{
		postinitfile.insert(finfo.completeBaseName(), finfo.absoluteFilePath());
	}

	//��ȡ����ѡ�е�mxdb�ļ�
	QStringList initnames = postinitfile.keys();
	for (QString filename : names)
	{
		QFileInfo finfo(filename);
		QString fsuffix = finfo.suffix();
		QString fname = finfo.completeBaseName();
		if (fsuffix == "mxdb")
		{
			for (QString initname : initnames)
			{
				int nbyte = initname.size();
				if (fname.left(nbyte) == initname && fname.at(nbyte) == "_")
				{
					postframefiles.insert(initname, filename);
					break;
				}
			}
		}
	}

	QStringList postnames = postframefiles.uniqueKeys();
	for (QString pname : postnames)
	{
		QStringList framefiles = postframefiles.values(pname);
		if (framefiles.size() < 1)
		{

			continue;
		}
		//��û��ѡ���ʼ�����ļ������Զ���ȡ
		QString initfile = postinitfile.value(pname);
		if (initfile.isEmpty())
		{

			continue;
		}
		//��֡�ļ���������
		QCollator collator;
		collator.setNumericMode(true);
		std::sort(framefiles.begin(), framefiles.end(), [&collator](const QString & str1, const QString & str2)
		{
			return collator.compare(str1, str2) < 0;
		});

		//��ȡ�ļ�
		MIOFile::mIMxdbFile1* mxdbThread1 = new MIOFile::mIMxdbFile1(initfile, framefiles);
		mxdbThread1->setDataPost(dp);
		mxdbThread1->setOneFrameRenderData(oneFrameRendData);
		mxdbThread1->slot_importAllMxDbFile();
		mxdbThread1->readCloudData();
		mxdbThread1->readDisplacementData();
		delete mxdbThread1;
	}

	return true;
}

bool MxRendTest::createGeo(MDataGeo::mGeoModelData1 * geoModelData)
{
	if (geoModelData == nullptr)
	{
		return false;
	}

	QString partName;
	mGeoPartData1 *geoPartData;
	mGeoLineData1 *geoLineData;
	mGeoFaceData1 *geoFaceData;
	mGeoPointData1 *geoPointData;
	QVector<QVector3D> vertexs;

	/**********����*****************************************************************************************/
	_globalPartId++;
	partName = "part1";
	geoPartData = new mGeoPartData1(geoModelData, partName, _globalPartId);
	/*******************��**************/
	_globalFaceId++;
	geoFaceData = new mGeoFaceData1(geoModelData, partName, _globalFaceId);
	vertexs = QVector<QVector3D>{ QVector3D(0,0,0),QVector3D(0,1,0),QVector3D(1,0,0) };
	geoFaceData->appendGeoFaceData(_globalFaceId, vertexs);
	geoPartData->appendGeoFaceID(_globalFaceId);
	/**********����*****************************************************************************************/
	_globalPartId++;
	partName = "part2";
	geoPartData = new mGeoPartData1(geoModelData, partName, _globalPartId);
	/*******************��**************/
	_globalFaceId++;
	geoFaceData = new mGeoFaceData1(geoModelData, partName, _globalFaceId);
	vertexs = QVector<QVector3D>{ QVector3D(1,1,0),QVector3D(0,1,0),QVector3D(1,0,0) };
	geoFaceData->appendGeoFaceData(_globalFaceId, vertexs);
	geoPartData->appendGeoFaceID(_globalFaceId);
	///*******************�߽���**************/
	//_globalLineId++;
	//geoLineData = new mGeoLineData1(geoModelData, partName, _globalLineId);
	//geoLineData->setGeoLineProperty(GeoLineProperty::EdgeOnFace);
	//vertexs = QVector<QVector3D>{ QVector3D(0,0,0),QVector3D(0,1,0)};
	//geoLineData->appendGeoLineData(_globalLineId, vertexs);
	//geoPartData->appendGeoLineID(_globalLineId);
	//_globalLineId++;
	//geoLineData = new mGeoLineData1(geoModelData, partName, _globalLineId);
	//geoLineData->setGeoLineProperty(GeoLineProperty::EdgeOnFace);
	//vertexs = QVector<QVector3D>{ QVector3D(0,1,0),QVector3D(1,0,0) };
	//geoLineData->appendGeoLineData(_globalLineId, vertexs);
	//geoPartData->appendGeoLineID(_globalLineId);
	//_globalLineId++;
	//geoLineData = new mGeoLineData1(geoModelData, partName, _globalLineId);
	//geoLineData->setGeoLineProperty(GeoLineProperty::EdgeOnFace);
	//vertexs = QVector<QVector3D>{ QVector3D(1,0,0),QVector3D(0,0,0) };
	//geoLineData->appendGeoLineData(_globalLineId, vertexs);
	//geoPartData->appendGeoLineID(_globalLineId);

	///**********����*****************************************************************************************/
	//_globalPartId++;
	//partName = "part2";
	//geoPartData = new mGeoPartData1(geoModelData, partName, _globalPartId);
	///*******************��**************/
	//_globalFaceId++;
	//geoFaceData = new mGeoFaceData1(geoModelData, partName, _globalFaceId);
	//vertexs = QVector<QVector3D>{ QVector3D(1,1,2),QVector3D(2,1,3),QVector3D(0,0,1) };
	//geoFaceData->appendGeoFaceData(_globalFaceId, vertexs);
	//geoPartData->appendGeoFaceID(_globalFaceId);

	///**********����*****************************************************************************************/
	//_globalPartId++;
	//partName = "part3";
	//geoPartData = new mGeoPartData1(geoModelData, partName, _globalPartId);
	///*****************��***********/
	//_globalLineId++;
	//geoLineData = new mGeoLineData1(geoModelData, partName, _globalLineId);
	//vertexs = QVector<QVector3D>{ QVector3D(0,0,0),QVector3D(1,1,1),QVector3D(1,1,1),QVector3D(3,3,3) };
	//geoLineData->appendGeoLineData(_globalLineId, vertexs);
	//geoPartData->appendGeoLineID(_globalLineId);

	///**********����*****************************************************************************************/
	//_globalPartId++;
	//partName = "part4";
	//geoPartData = new mGeoPartData1(geoModelData, partName, _globalPartId);
	///*****************��***********/
	//_globalPointId++;
	//geoPointData = new mGeoPointData1(geoModelData, partName, _globalPointId);
	//geoPointData->setPointData(_globalPointId, QVector3D(1, 2, 3));
	//geoPartData->appendGeoPointID(_globalPointId);
	
	return true;
}

bool MxRendTest::readTxtFile()
{
	QString name = QFileDialog::getOpenFileName(this, "ѡ��txt�ļ�", qApp->applicationDirPath(), "*.txt");

	QFileInfo info(name);
	if (!info.exists())
	{
		return false;
	}

	QFile file(name);
	QString currentline;
	QStringList linelist;
	QString partName;
	shared_ptr<mArrowRender> fontRender = _preRend->getArrowRender();
	MDataGeo::mGeoModelData1 * geoModelData = _preRender->getGeoModelData();
	mGeoPartData1 *geoPartData = nullptr;
	mGeoFaceData1 *geoFaceData = nullptr;
	QVector<QVector3D> faceVertexs;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//�Զ��ķ�ʽ���ļ�
	{
		return false;
	}
	
	while (!file.atEnd())//�ļ�û��ĩβ��ִ��ѭ��������
	{
		if (currentline.contains("*part"))
		{
			linelist = currentline.split("=");
			if (linelist.size() == 2)
			{
				partName = linelist.at(1);
				_globalPartId++;
				geoPartData = new mGeoPartData1(geoModelData, partName, _globalPartId);
			}
			currentline = file.readLine().simplified();
		}
		else if (currentline.contains("*face"))
		{
			_globalFaceId++;
			geoFaceData = new mGeoFaceData1(geoModelData, partName, _globalFaceId);
			currentline = file.readLine().simplified();
			while (!file.atEnd())
			{
				linelist = currentline.split(",");
				if (linelist.size() == 9)
				{
					faceVertexs.append(QVector<QVector3D>{ QVector3D(linelist.at(0).toFloat(), linelist.at(1).toFloat(), linelist.at(2).toFloat()),
						QVector3D(linelist.at(3).toFloat(), linelist.at(4).toFloat(), linelist.at(5).toFloat()),
						QVector3D(linelist.at(6).toFloat(), linelist.at(7).toFloat(), linelist.at(8).toFloat()) });
				}
				else if (currentline.contains("*"))
				{
					geoFaceData->appendGeoFaceData(_globalFaceId, faceVertexs);
					geoPartData->appendGeoFaceID(_globalFaceId);				
					faceVertexs.clear();
					break;
				}
				currentline = file.readLine().simplified();
			}
		}
		else if (currentline.contains("*vector"))
		{
			QVector<QVector3D> vertexs;
			QVector<QVector3D> vectors;
			currentline = file.readLine().simplified();
			while (!file.atEnd())
			{
				linelist = currentline.split(",");
				if (linelist.size() == 6)
				{
					vertexs.append(QVector3D(linelist.at(0).toFloat(), linelist.at(1).toFloat(), linelist.at(2).toFloat()));
					vectors.append(QVector3D(linelist.at(3).toFloat(), linelist.at(4).toFloat(), linelist.at(5).toFloat()));
				}
				currentline = file.readLine().simplified();
			}
			fontRender->appendCommonArrow("1", vertexs, vectors);
		}
		else
		{
			currentline = file.readLine().simplified();
		}
	}

	return true;
}
