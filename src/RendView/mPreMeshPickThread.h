#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "rendview_global.h"

#include <QObject>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QThread>
#include <set>

#include "SpaceTree.h"

#include "mMeshViewEnum.h"
#include "mElementFunction.h"

using namespace MViewBasic;
using namespace Space;
using namespace std;
using namespace MxFunctions;
class MXGeoSolid;
class MXGeoFace;
class MXGeoEdge;
class MXGeoPoint;
class MFace;
class MEdge;
class MXMeshElement;
class MXMeshVertex;
namespace MDataPre
{
	class mPreMeshPickData1;
}
using namespace MDataPre;
namespace MPreRend
{
	class mBasePick
	{
	public:

		mBasePick(QMatrix4x4 pvm, int w, int h) { _pvm = pvm; _width = w; _height = h; };

		virtual bool getPickIsIntersectionWithAABB(Space::AABB aabb) = 0;

		virtual bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) = 0;//0维和2维和3维

		virtual bool get1DMeshIsInPick(QVector<QVector3D> vertexs) = 0;//1维

		virtual bool isIntersectionAABBAndPick(QVector<QVector2D> ap) = 0;//判断是否相交

		virtual bool isAABBPointIsAllInPick(QVector<QVector2D> ap) = 0;//判断点是否全部

		//将Space::AABB的八个顶点转化为二维屏幕的点
		QVector<QVector2D> getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge);
		//将屏幕坐标转化为世界坐标
		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		//将世界坐标转化为屏幕坐标
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);
		QVector<QVector2D> WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs);
		//将世界坐标转化为屏幕坐标并且返回他的深度值
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);
		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);

	protected:
		QMatrix4x4 _pvm;
		int _width, _height;
		MViewBasic::MultiplyPickMode _multiplyPickMode;

	};


	class mQuadPick :public mBasePick
	{
	public:
		mQuadPick(QMatrix4x4 pvm, int w, int h, QVector<QVector2D> multiQuad) :mBasePick(pvm, w, h), _multiQuad(multiQuad)
		{
			_center = (_multiQuad.first() + _multiQuad.last()) / 2.0;
			_boxXY_2 = _multiQuad.first() - _center; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);
		};

		bool getPickIsIntersectionWithAABB(Space::AABB aabb) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0维和2维和3维

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1维

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部

	protected:
		QVector<QVector2D> _multiQuad;

		QVector2D _center, _boxXY_2;

	};

	class mPolygonPick :public mBasePick
	{
	public:
		mPolygonPick(QMatrix4x4 pvm, int w, int h, QVector<QVector2D> multiQuad) :mBasePick(pvm, w, h), _multiQuad(multiQuad)
		{
			_center = (_multiQuad.first() + _multiQuad.last()) / 2.0;
		};

		bool getPickIsIntersectionWithAABB(Space::AABB aabb) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0维和2维和3维

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1维

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部

	protected:
		QVector<QVector2D> _multiQuad;

		QVector2D _center;

	};

	class mRoundPick :public mBasePick
	{
	public:
		mRoundPick(QMatrix4x4 pvm, int w, int h, QVector2D p1, QVector2D p2, QVector3D centerDirection)
			:mBasePick(pvm, w, h)
		{
			_screenCenter = (p1 + p2) / 2.0;
			QVector3D Point = ScreenvertexToWorldvertex(p2);//算出圆上一点的坐标
			_centerPoint = ScreenvertexToWorldvertex(_screenCenter);//算出圆心坐标
			_radius = _centerPoint.distanceToPoint(Point);
			_screenRadius = _screenCenter.distanceToPoint(p1);
			_centerDirection = centerDirection;
		};

		bool getPickIsIntersectionWithAABB(Space::AABB aabb) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0维和2维和3维

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1维

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部

	protected:
		QVector3D _centerPoint; QVector3D _centerDirection; double _radius; //空间
		QVector2D _screenCenter; double _screenRadius;//屏幕
	};
	class RENDVIEW_EXPORT mPreMeshPickThread : public QObject
	{
		Q_OBJECT

	public:
		/*
		* 复制构造函数
		*/
		mPreMeshPickThread(mPreMeshPickData1 *pickData);

		/*
		* 析构函数
		*/
		~mPreMeshPickThread();

		/*
		* 设置当前的拾取模式
		*/
		void setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode);

		/*
		* 设置当前的拾取过滤器
		*/
		void setPickFilter(PickFilter *pickFilter);

		/*
		 * 开始拾取
		 */
		void startPick();

		/*
		 * 设置矩阵
		 */
		void setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);

		/*
		* 设置矩阵
		*/
		void setMatrix(QMatrix4x4 pvm);

		/*
		* 设置通过角度拾取的角度
		*/
		void setPickAngleValue(double angle) { _pickAngleValue = angle; };

		/*
		 * 设置单选位置
		 */
		void setLocation(const QVector2D & pos, float depth);

		/*
		* 设置位置(矩形或者多边形框选)
		*/
		void setLocation(QVector<QVector2D> pickQuad, QVector3D direction = QVector3D());

		/*
		 * 设置窗口大小
		 */
		void setWidget(int w, int h);

		/*
		 *	是否完成拾取
		 */
		bool isFinished();

		/*
		 * 拾取完成设置为false
		 */
		void setFinished();
	private:
		//拾取主程序		
		void doSoloPick(QString partName);
		void doMultiplyPick(QString partName);
		void doAnglePick();

		/*
		 * 单选
		 */
		//void SoloPickAny(QString partName);
		//void SoloPickPoint(QString partName);
		//void SoloPick1DMesh(QString partName);
		//void SoloPick2DMesh(QString partName);
		//void SoloPickTri(QString partName);
		//void SoloPickQuad(QString partName);
		//void SoloPickTet(QString partName);
		//void SoloPickPyramid(QString partName);
		//void SoloPickWedge(QString partName);
		//void SoloPickHex(QString partName);
		void SoloPickMeshTypeFilter(QString partName, QVector<MeshType> filters);
		void SoloPickNode(QString partName);
		void SoloPickAnyMesh(QString partName);
		void SoloPickMeshLine(QString partName);
		void SoloPickMeshFace(QString partName);
		void SoloPickMeshPart(QString partName);
		void SoloPickNodeByPart(QString partName);
		void SoloPickAnyMeshByPart(QString partName);
		void SoloPickMeshLineByPart(QString partName);
		void SoloPickMeshFaceByPart(QString partName);
		//void SoloPickNodeByLine(QString partName);
		//void SoloPickMeshByLine(QString partName);
		//void SoloPickMeshLineByLine(QString partName);
		//void SoloPickNodeByFace(QString partName);
		//void SoloPickMeshByFace(QString partName);
		//void SoloPickMeshFaceByFace(QString partName);
		//void SoloPickNodeByLineAngle(QString partName);
		void SoloPickNodeByFaceAngle(QString partName);
		//void SoloPick1DMeshByAngle(QString partName);
		void SoloPick2DMeshByAngle(QString partName);
		void SoloPickMeshLineByAngle(QString partName);
		void SoloPickMeshFaceByAngle(QString partName);

		//框选拾取
		void MultiplyPickNode(QString partName, bool isAllIn = false);
		void MultiplyPickAnyMesh(QString partName, bool isAllIn = false);
		void MultiplyPickMeshTypeFilter(QString partName, QVector<MeshType> filters, bool isAllIn = false);
		void MultiplyPickMeshLine(QString partName, bool isAllIn = false);
		void MultiplyPickMeshFace(QString partName, bool isAllIn = false);
		void MultiplyPickMeshPart(QString partName, bool isAllIn = false);
		void MultiplyPickNodeByPart(QString partName, bool isAllIn = false);
		void MultiplyPickAnyMeshByPart(QString partName, bool isAllIn = false);
		void MultiplyPickMeshLineByPart(QString partName, bool isAllIn = false);
		void MultiplyPickMeshFaceByPart(QString partName, bool isAllIn = false);

		//通过角度拾取
		void SoloPickNodeByLineAngle();
		void SoloPickNodeByFaceAngle();
		void SoloPick1DMeshByAngle();
		void SoloPickMeshLineByAngle();
		void SoloPick2DMeshByAngle();
		void SoloPickMeshFaceByAngle();

		/*************单选***********/
		//判断单选是否拾取到部件

		//判断单选是否拾取到该部件
		bool isSoloPickMeshPart(QString partName, float &depth);


		/***********框选*********/
		//判断是否拾取到该几何点
		bool IsMultiplyPickGeoPoint(MXGeoPoint* geoPointData);

		//判断是否拾取到该几何线
		bool IsMultiplyPickGeoLine(MXGeoEdge* geoLineData);

		//判断是否拾取到该几何面
		bool IsMultiplyPickGeoFace(MXGeoFace* geoFaceData);

		//判断是否拾取到该几何体
		bool IsMultiplyPickGeoSolid(MXGeoSolid* geoSolidData);

		//判断框选是否拾取到该部件
		bool isMultiplyPickMeshPart(QString partName);


		//将世界坐标转化为屏幕坐标
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);

		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);

		//将世界坐标转化为屏幕坐标并且返回他的深度值
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);

		/*
		 * 将世界坐标转化为屏幕坐标并且返回他的深度值
		 */
		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);

		//获取形心
		QVector3D getCenter(QVector<QVector3D> vertexs);

		//获取一个部件的所有信息
		set<MXMeshVertex*> getAllNodesByPartName(QString partName);
		set<MXMeshElement*> getAllMeshsByPartName(QString partName);
		set<MFace*> getAllMeshFacesByPartName(QString partName);
		set<MEdge*> getAllMeshLinesByPartName(QString partName);

	signals:
		/*
		 * 发送信号完成
		 */
		void finishedPickSig();


	public slots:


	private:

		//拾取过滤器
		PickFilter *_pickFilter;

		//拾取模式
		MViewBasic::PickMode _pickMode;

		//框选拾取模式
		MViewBasic::MultiplyPickMode _multiplyPickMode;

		//拾取角度
		float _pickAngleValue = 60;

		//窗口大小
		int _Win_WIDTH, _Win_HEIGHT;

		//着色器视图矩阵
		QMatrix4x4 _projection, _view, _model, _pvm;

		/*
		* 拾取数据
		*/
		mPreMeshPickData1 *_pickData;

		/*
		 * 点选位置
		 */
		QVector2D _pos;
		float _depth;
		QVector3D _p;
		QVector3D _origin;
		QVector3D _dir;

		/*********/
		std::shared_ptr<mBasePick> _pick;
		/*
		 * 点选矩形
		 */
		QVector<QVector2D> soloQuad;

		/*
		 * 是否完成
		 */
		bool _isfinished;


	};
}
