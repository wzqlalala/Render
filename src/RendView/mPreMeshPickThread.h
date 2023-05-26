//#pragma once
////解决中文乱码
//#pragma execution_character_set("utf-8")
//
//#include "renddata_global.h"
//
//#include <QObject>
//#include <QMatrix4x4>
//#include <QVector>
//#include <QVector2D>
//#include <QThread>
//#include <set>
//
//#include "SpaceTree.h"
//
//#include "mMeshViewEnum.h"
//
//using namespace MViewBasic;
//using namespace std;
//class MXGeoSolid;
//class MXGeoFace;
//class MXGeoEdge;
//class MXGeoPoint;
//namespace MPreRend
//{
//	class mPreMeshPickData1;
//	class mBasePick
//	{
//	public:
//
//		mBasePick(QMatrix4x4 pvm, int w, int h) { _pvm = pvm; _width = w; _height = h; };
//
//		virtual bool getGeoPointIsInPick(QVector3D pointCenter) = 0;//0维
//
//		virtual bool getGeoLineIsInPick(QVector<QVector3D> vertexs) = 0;//1维
//
//		virtual bool getGeoFaceIsInPick(QVector<QVector3D> vertexs) = 0;//2维
//
//		virtual bool isIntersectionAABBAndPick(QVector<QVector2D> ap) = 0;//判断是否相交
//
//		virtual bool isAABBPointIsAllInPick(QVector<QVector2D> ap) = 0;//判断点是否全部
//
//		//将AABB的八个顶点转化为二维屏幕的点
//		QVector<QVector2D> getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge);
//		//将屏幕坐标转化为世界坐标
//		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
//		//将世界坐标转化为屏幕坐标
//		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);
//		QVector<QVector2D> WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs);
//		//将世界坐标转化为屏幕坐标并且返回他的深度值
//		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);
//		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);
//
//	protected:
//		QMatrix4x4 _pvm;
//		int _width, _height;
//		MViewBasic::MultiplyPickMode _multiplyPickMode;
//
//	};
//
//
//	class mQuadPick :public mBasePick
//	{
//	public:
//		mQuadPick(QMatrix4x4 pvm, int w, int h, QVector<QVector2D> multiQuad) :mBasePick(pvm, w, h), _multiQuad(multiQuad)
//		{
//			_center = (_multiQuad.first() + _multiQuad.last()) / 2.0;
//			_boxXY_2 = _multiQuad.first() - _center; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);
//		};
//
//		bool getGeoPointIsInPick(QVector3D pointCenter) override;//0维
//
//		bool getGeoLineIsInPick(QVector<QVector3D> vertexs) override;//1维
//
//		bool getGeoFaceIsInPick(QVector<QVector3D> vertexs) override;//2维
//
//		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;
//
//		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部
//
//	protected:
//		QVector<QVector2D> _multiQuad;
//
//		QVector2D _center, _boxXY_2;
//
//	};
//
//	class mPolygonPick :public mBasePick
//	{
//	public:
//		mPolygonPick(QMatrix4x4 pvm, int w, int h, QVector<QVector2D> multiQuad) :mBasePick(pvm, w, h), _multiQuad(multiQuad)
//		{
//			_center = (_multiQuad.first() + _multiQuad.last()) / 2.0;
//		};
//
//		bool getGeoPointIsInPick(QVector3D pointCenter) override;//0维
//
//		bool getGeoLineIsInPick(QVector<QVector3D> vertexs) override;//1维
//
//		bool getGeoFaceIsInPick(QVector<QVector3D> vertexs) override;//2维
//
//		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;
//
//		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部
//
//	protected:
//		QVector<QVector2D> _multiQuad;
//
//		QVector2D _center;
//
//	};
//
//	class mRoundPick :public mBasePick
//	{
//	public:
//		mRoundPick(QMatrix4x4 pvm, int w, int h, QVector2D p1, QVector2D p2, QVector3D centerDirection)
//			:mBasePick(pvm, w, h)
//		{
//			_screenCenter = (p1 + p2) / 2.0;
//			QVector3D Point = ScreenvertexToWorldvertex(p2);//算出圆上一点的坐标
//			_centerPoint = ScreenvertexToWorldvertex(_screenCenter);//算出圆心坐标
//			_radius = _centerPoint.distanceToPoint(Point);
//			_screenRadius = _screenCenter.distanceToPoint(p1);
//			_centerDirection = centerDirection;
//		};
//
//		bool getGeoPointIsInPick(QVector3D pointCenter) override;//0维
//
//		bool getGeoLineIsInPick(QVector<QVector3D> vertexs) override;//1维
//
//		bool getGeoFaceIsInPick(QVector<QVector3D> vertexs) override;//2维
//
//		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;
//
//		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部
//
//	protected:
//		QVector3D _centerPoint; QVector3D _centerDirection; double _radius; //空间
//		QVector2D _screenCenter; double _screenRadius;//屏幕
//	};
//	class RENDDATA_EXPORT mPreMeshPickThread : public QObject
//	{
//		Q_OBJECT
//
//	public:
//		/*
//		* 复制构造函数
//		*/
//		mPreMeshPickThread(mPreMeshPickData1 *pickData);
//
//		/*
//		* 析构函数
//		*/
//		~mPreMeshPickThread();
//
//		/*
//		* 设置当前的拾取模式
//		*/
//		void setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode);
//
//		/*
//		* 设置当前的拾取过滤器
//		*/
//		void setPickFilter(MViewBasic::PickFilter *pickFilter);
//
//		/*
//		 * 开始拾取
//		 */
//		void startPick();
//
//		/*
//		 * 设置矩阵
//		 */
//		void setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
//
//		/*
//		* 设置矩阵
//		*/
//		void setMatrix(QMatrix4x4 pvm);
//
//		/*
//		 * 设置单选位置
//		 */
//		void setLocation(const QVector2D & pos, float depth);
//
//		/*
//		* 设置位置(矩形或者多边形框选)
//		*/
//		void setLocation(QVector<QVector2D> pickQuad, QVector3D direction = QVector3D());
//
//		/*
//		 * 设置窗口大小
//		 */
//		void setWidget(int w, int h);
//
//		/*
//		 *	是否完成拾取
//		 */
//		bool isFinished();
//
//		/*
//		 * 拾取完成设置为false
//		 */
//		void setFinished();
//	private:
//		//拾取主程序		
//		void doSoloPick(QString partName);
//		void doMultiplyPick(QString partName);
//
//		/*
//		 * 单选
//		 */
//		int SoloPickAny(QString partName);
//		int SoloPickPoint(QString partName);
//		int SoloPick1DMesh(QString partName);
//		int SoloPick2DMesh(QString partName);
//		int SoloPickTri(QString partName);
//		int SoloPickQuad(QString partName);
//		int SoloPickTet(QString partName);
//		int SoloPickPyramid(QString partName);
//		int SoloPickWedge(QString partName);
//		int SoloPickHex(QString partName);
//
//		int SoloPickNode(QString partName);
//		int SoloPickAnyMesh(QString partName);
//		int SoloPickMeshLine(QString partName);
//		int SoloPickMeshFace(QString partName);
//		int SoloPickMeshPart(QString partName);
//		int SoloPickNodeByPart(QString partName);
//		int SoloPickAnyMeshByPart(QString partName);
//		int SoloPickMeshLineByPart(QString partName);
//		int SoloPickMeshFaceByPart(QString partName);
//		int SoloPickNodeByLine(QString partName);
//		int SoloPickMeshByLine(QString partName);
//		int SoloPickMeshLineByLine(QString partName);
//		int SoloPickNodeByFace(QString partName);
//		int SoloPickMeshByFace(QString partName);
//		int SoloPickMeshFaceByFace(QString partName);
//		int SoloPickNodeByLineAngle(QString partName);
//		int SoloPickNodeByFaceAngle(QString partName);
//		int SoloPick1DMeshByAngle(QString partName);
//		int SoloPick2DMeshByAngle(QString partName);
//		int SoloPickMeshLineByAngle(QString partName);
//		int SoloPickMeshFaceByAngle(QString partName);
//
//		//矩形框选拾取
//		void MultiplyPickGeoLine(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoFace(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoSolid(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoPoint(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoPart(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoPointByPart(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoLineByPart(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoFaceByPart(QString partName, bool isAllIn = false);
//		void MultiplyPickGeoSolidByPart(QString partName, bool isAllIn = false);
//
//		/***********框选*********/
//		//判断是否拾取到该几何点
//		bool IsMultiplyPickGeoPoint(MXGeoPoint* geoPointData);
//
//		//判断是否拾取到该几何线
//		bool IsMultiplyPickGeoLine(MXGeoEdge* geoLineData);
//
//		//判断是否拾取到该几何面
//		bool IsMultiplyPickGeoFace(MXGeoFace* geoFaceData);
//
//		//判断是否拾取到该几何体
//		bool IsMultiplyPickGeoSolid(MXGeoSolid* geoSolidData);
//
//		//判断框选是否拾取到该部件
//		bool isMultiplyPickGeoPart(QString partName);
//
//
//		//将世界坐标转化为屏幕坐标
//		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);
//
//		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
//
//		//将世界坐标转化为屏幕坐标并且返回他的深度值
//		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);
//
//		/*
//		 * 将世界坐标转化为屏幕坐标并且返回他的深度值
//		 */
//		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);
//
//		//获取形心
//		QVector3D getCenter(QVector<QVector3D> vertexs);
//
//
//	signals:
//		/*
//		 * 发送信号完成
//		 */
//		void finishedPickSig();
//
//
//	public slots:
//
//
//	private:
//
//		//拾取过滤器
//		MViewBasic::PickFilter *_pickFilter;
//
//		//拾取模式
//		MViewBasic::PickMode _pickMode;
//
//		//框选拾取模式
//		MViewBasic::MultiplyPickMode _multiplyPickMode;
//
//		//窗口大小
//		int _Win_WIDTH, _Win_HEIGHT;
//
//		//着色器视图矩阵
//		QMatrix4x4 _projection, _view, _model, _pvm;
//
//		/*
//		* 拾取数据
//		*/
//		mPreMeshPickData1 *_pickData;
//
//		/*
//		 * 点选位置
//		 */
//		QVector2D _pos;
//		float _depth;
//		QVector3D _p;
//		QVector3D _origin;
//		QVector3D _dir;
//
//		/*********/
//		std::shared_ptr<mBasePick> _pick;
//		/*
//		 * 点选矩形
//		 */
//		QVector<QVector2D> soloQuad;
//
//		/*
//		 * 是否完成
//		 */
//		bool _isfinished;
//
//
//	};
//}
