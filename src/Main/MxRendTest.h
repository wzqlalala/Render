#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_MxRendTest.h"

#include "mPostRendStatus.h"

#include <QApplication>
#include <QPixmap>
#include <QScreen>
#include <QRect>
#include <QDesktopWidget>

namespace MPreRend
{
	class mPreRend;
	class mPreRender;
}
namespace MPostRend
{
	class mPostRend;
	class mPostRender;
}
namespace MDataGeo
{
	class mGeoModelData1;
}
namespace MDataPost
{
	class mDataPost1;
	class mPostOneFrameRendData;
}
namespace MBaseRend
{
	class mModelTestRender;
	class mTestRender;
}
using namespace std;
class MxRendTest : public QMainWindow
{
    Q_OBJECT

public:
    MxRendTest(int id = 0);

protected:

	void keyPressEvent(QKeyEvent *event) override;

	bool getMxDbData(shared_ptr<MDataPost::mDataPost1> dp, MDataPost::mPostOneFrameRendData * oneFrameRendData);

	bool createGeo(MDataGeo::mGeoModelData1 *geoModelData);

	bool readTxtFile();

protected:


private:

	// 禁用所有子窗口的事件响应
	void disableChildWidgets(QWidget *widget)
	{
		const QObjectList &children = widget->children();
		for (QObject *child : children) {
			QWidget *childWidget = qobject_cast<QWidget *>(child);
			if (childWidget) {
				childWidget->setEnabled(false);
				disableChildWidgets(childWidget);
			}
		}
	}

	// 启用所有子窗口的事件响应
	void enableChildWidgets(QWidget *widget)
	{
		const QObjectList &children = widget->children();
		for (QObject *child : children) {
			QWidget *childWidget = qobject_cast<QWidget *>(child);
			if (childWidget) {
				childWidget->setEnabled(true);
				enableChildWidgets(childWidget);
			}
		}
	}

private:
    Ui::MxRendTestClass ui;

	int _id = 0;//奇数为后处理，偶数为前处理

	MPreRend::mPreRend *_preRend;
	shared_ptr<MPreRend::mPreRender> _preRender;

	MPostRend::mPostRend *_postRend;
	shared_ptr<MPostRend::mPostRender> _postRender;

	std::shared_ptr<MBaseRend::mModelTestRender> _modelRender;

	shared_ptr<MBaseRend::mTestRender> _testRender;

	bool isStartScreen{false};//是否录屏

	//测试
	PostMode _postMode{ OneFrame};
	ShowFuntion _showfuntion{ ElementFace };
	bool _isShowInitialShape{ false };
	PostModelOperateEnum _modelOperate{ ColorAllPartOperate };
	PostModelOperateEnum _modelOperate1{ HideAllPartOperate };
	set<QString> _hideNames;
	bool _dispersed{ false };
	bool _isEquivariance{ true };
	float _lineWidth = 1.0f;
	int _pointSize = 1.0;
	CameraType _cameraType = CameraType::Camera1;
	//切面
	QVector<QVector3D> planeNormals{ QVector3D(0,0,1), QVector3D(0,1,0),QVector3D(1,0,0), QVector3D(1,1,1), QVector3D(1,1,0),QVector3D(0,1,1),QVector3D(1,0,1), QVector3D(1,2,0) };
	int _cuttingPlaneNum = 0;
	bool _isShowPlane = true;
	bool _isOnlyShowPlane = false;

	//动画
	bool _isStartAnimation = false;
	bool _isLinearAnimation = true;

	//拾取
	MultiplyPickMode _multuiplyPickMode = MultiplyPickMode::QuadPick;

	//后处理
	QVector<PickFilter> _pickfilters{ PickFilter::PickNode, PickFilter::PickAnyMesh, PickFilter::PickMeshFace, PickFilter::Pick1DMesh, PickFilter::Pick2DMesh, PickFilter::PickNodeByLineAngle, PickFilter::PickNodeByFaceAngle, PickFilter::Pick1DMeshByAngle, PickFilter::Pick2DMeshByAngle, PickFilter::PickMeshFaceByAngle };
	int _pickfilterID = 0;

	//前处理几何
	QVector<PickFilter> _pickfilters1{ PickFilter::PickGeoPoint, PickFilter::PickGeoLine, PickFilter::PickGeoFace, PickFilter::PickGeoSolid, PickFilter::PickGeoPart, PickFilter::PickGeoPointByPart, PickFilter::PickGeoLineByPart, PickFilter::PickGeoFaceByPart, PickFilter::PickGeoSolidByPart, PickFilter::pickVertexOnGeoLine, PickFilter::pickVertexOnGeoFace, PickFilter::pickVertexOnScreen };

};
