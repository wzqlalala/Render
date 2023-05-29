//#include "mPreMeshPickThread.h"
//
//#include "MeshMessage.h"
//
////MViewBasic
//#include "mMeshViewEnum.h"
//#include "mPickToolClass.h"
//
////Qt
//#include <QVector2D>
//#include <QVector4D>
//#include <QString>
//#include <QMutex>
//#include <QtConcurrent/QtConcurrent>
//#include <QFuture>
//#include <qmath.h>
//
//using namespace MViewBasic;
//using namespace MDataPost;
//namespace MPreRend
//{
//	QMutex pickMutex;
//
//	QVector2D mBasePick::WorldvertexToScreenvertex(QVector3D Worldvertex)
//	{
//		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
//		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_width, _height - (res.y() / res.w() / 2.0 + 0.5)*_height);
//	}
//
//	QVector<QVector2D> mBasePick::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs)
//	{
//		QVector<QVector2D> vertexs;
//		for (auto Worldvertex : Worldvertexs)
//		{
//			vertexs.append(WorldvertexToScreenvertex(Worldvertex));
//		}
//		return vertexs;
//	}
//
//	QVector<QVector2D> mBasePick::getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge)
//	{
//		QVector<QVector2D> ap;
//		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), minEdge.y(), minEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), minEdge.y(), maxEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), maxEdge.y(), maxEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), maxEdge.y(), minEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), minEdge.y(), minEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), minEdge.y(), maxEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), maxEdge.y(), maxEdge.z())));
//		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), maxEdge.y(), minEdge.z())));
//		return ap;
//	}
//
//	QVector3D mBasePick::ScreenvertexToWorldvertex(QVector3D vertex)
//	{
//		QMatrix4x4 i_mvp = (_pvm).inverted();
//		float ndc_x = 2.0*vertex.x() / (float)_width - 1.0f;
//		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_height);
//		float ndc_z = vertex.z() * 2 - 1.0;
//
//		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
//		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
//	}
//
//	QVector2D mBasePick::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
//	{
//		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
//		depth = res.z();
//		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_width, _height - (res.y() / res.w() / 2.0 + 0.5)*_height);
//	}
//
//	void mBasePick::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
//	{
//		for (auto Worldvertex : Worldvertexs)
//		{
//			float depth;
//			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
//			depths.insert(depth);
//		}
//	}
//
//	bool mQuadPick::getGeoPointIsInPick(QVector3D pointCenter)
//	{
//		if (mPickToolClass::IsPointInQuad(WorldvertexToScreenvertex(pointCenter), _center, _boxXY_2))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mQuadPick::getGeoLineIsInPick(QVector<QVector3D> vertexs)
//	{
//		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
//		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshBeam) || mPickToolClass::IsPointInQuad(tempQVector2D, _center, _boxXY_2))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mQuadPick::getGeoFaceIsInPick(QVector<QVector3D> vertexs)
//	{
//		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
//		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshTri) || mPickToolClass::IsPointInQuad(tempQVector2D, _center, _boxXY_2) || mPickToolClass::IsPointInMesh(_center, tempQVector2D, MeshTri))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mQuadPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
//	{
//		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInQuad(ap, _center, _boxXY_2))
//		{
//			if (mPickToolClass::IsPointInMesh(_center, ap, MeshHex))
//			{
//				return true;
//			}	
//			return false;
//		}
//		return true;
//	}
//
//	bool mQuadPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
//	{
//		if (mPickToolClass::IsAllPointInQuad(ap, _center, _boxXY_2))//包含
//		{
//			return true;
//		}
//		return false;
//	}
//
//	
//	bool mPolygonPick::getGeoPointIsInPick(QVector3D pointCenter)
//	{
//		if (mPickToolClass::IsPointInPolygon(WorldvertexToScreenvertex(pointCenter), _center, _multiQuad))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mPolygonPick::getGeoLineIsInPick(QVector<QVector3D> vertexs)
//	{
//		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
//		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshBeam) || mPickToolClass::IsPointInPolygon(tempQVector2D, _center, _multiQuad))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mPolygonPick::getGeoFaceIsInPick(QVector<QVector3D> vertexs)
//	{
//		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
//		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshTri) || mPickToolClass::IsPointInPolygon(tempQVector2D, _center, _multiQuad) || mPickToolClass::IsPointInMesh(_center, tempQVector2D, MeshTri))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mPolygonPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
//	{
//		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInPolygon(ap, _center, _multiQuad))
//		{
//			if (mPickToolClass::IsPointInMesh(_center, ap, MeshHex))
//			{
//				return true;
//			}
//			return false;
//		}
//		return true;
//	}
//
//	bool mPolygonPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
//	{
//		if (mPickToolClass::IsAllPointInPolygon(ap, _center, _multiQuad))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mRoundPick::getGeoPointIsInPick(QVector3D pointCenter)
//	{
//		if (mPickToolClass::IsPointInRound(pointCenter, _centerPoint, _centerDirection, _radius))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mRoundPick::getGeoLineIsInPick(QVector<QVector3D> vertexs)
//	{
//		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
//		if (mPickToolClass::IsLineIntersectionWithRound(tempQVector2D.first(), tempQVector2D.last(), _screenCenter, _screenRadius))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mRoundPick::getGeoFaceIsInPick(QVector<QVector3D> vertexs)
//	{
//		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
//		if (mPickToolClass::IsTriIntersectionWithCircle(tempQVector2D, _screenCenter, _screenRadius))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mRoundPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
//	{
//		if (mPickToolClass::IsPointInRound(ap, _screenCenter, _screenRadius) || mPickToolClass::IsAABBIntersectionWithCircle(ap, _screenCenter, _screenRadius))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mRoundPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
//	{
//		if (mPickToolClass::IsAllPointInRound(ap, _screenCenter, _screenRadius))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	
//	mPreMeshPickThread::mPreMeshPickThread(mPreMeshPickData1 *pickData)
//	{
//		_isfinished = false;
//		_pickData = pickData;
//		
//	}
//
//	mPreMeshPickThread::~mPreMeshPickThread()
//	{
//	}
//
//	void mPreMeshPickThread::setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode)
//	{
//		_pickMode = pickMode;
//		_multiplyPickMode = multiplyPickMode;
//	}
//
//	void mPreMeshPickThread::setPickFilter(MViewBasic::PickFilter * pickFilter)
//	{
//		_pickFilter = pickFilter;
//	}
//
//	void mPreMeshPickThread::setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
//	{
//		_projection = projection;
//		_view = view;
//		_model = model;
//	}
//
//	void mPreMeshPickThread::setMatrix(QMatrix4x4 pvm)
//	{
//		_pvm = pvm;
//	}
//
//	void mPreMeshPickThread::setLocation(const QVector2D& pos, float depth)
//	{
//		_pos = pos;
//		soloQuad = QVector<QVector2D>{ QVector2D(pos.x() + 5,pos.y() + 5),QVector2D(pos.x() + 5,pos.y() - 5),QVector2D(pos.x() - 5,pos.y() - 5),QVector2D(pos.x() - 5,pos.y() + 5) };
//		_depth = depth;
//		_p = ScreenvertexToWorldvertex(QVector3D(pos, depth));
//		_origin = ScreenvertexToWorldvertex(QVector3D(pos, -1.0));
//		_dir = (_p - _origin).normalized();
//		//_pickSoloOrMutiply = MViewBasic::SoloPick;
//	}
//
//	void mPreMeshPickThread::setLocation(QVector<QVector2D> pickQuad, QVector3D direction)
//	{
//		if (pickQuad.size() < 1)
//		{
//			return;
//		}
//		//multiQuad = QVector<QVector2D>{ pickQuad.first(), QVector2D(pickQuad.first().x(), pickQuad.last().y()), pickQuad.last(), QVector2D(pickQuad.last().x(), pickQuad.first().y()) };
//		//_centerBox = (pickQuad.first() + pickQuad.last()) / 2.0;
//		//_boxXY_2 = pickQuad.first() - _centerBox; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);	
//		//_centerX = centerX;
//		//_centerY = centerY;
//		//_boxW = boxW;
//		//_boxY = boxY;
//		//multiQuad = pickQuad;
//		switch (_multiplyPickMode)
//		{
//		case MViewBasic::MultiplyPickMode::QuadPick:_pick = make_shared<mQuadPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad);
//			break;
//		case MViewBasic::MultiplyPickMode::PolygonPick:_pick = make_shared<mPolygonPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad);
//			break;
//		case MViewBasic::MultiplyPickMode::RoundPick:_pick = make_shared<mRoundPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad.first(), pickQuad.last(), direction);
//			break;
//		default:
//			break;
//		}
//		
//		//if (_multiplyPickMode == MultiplyPickMode::RoundPick)
//		//{
//		//	_centerScreenPoint = _centerBox;
//		//	QVector3D centerPoint = (_pvm.inverted() * QVector4D(2 * pickQuad.last().x() / _Win_WIDTH - 1, 1 - 2 * pickQuad.last().y() / _Win_HEIGHT, 0.0, 1.0)).toVector3D();//算出圆心射线坐标
//		//	_centerPoint = (_pvm.inverted() * QVector4D(2 * _centerScreenPoint.x() / _Win_WIDTH - 1, 1 - 2 * _centerScreenPoint.y() / _Win_HEIGHT, 0.0, 1.0)).toVector3D();//算出圆上一点的坐标
//		//	_radius = _centerPoint.distanceToPoint(centerPoint);
//		//	_screenRadius = _centerBox.distanceToPoint(pickQuad.first());
//		//	_centerDirection = direction;
//		//}
//		//_pickSoloOrMutiply = MViewBasic::MultiplyPick;
//	}
//
//	void mPreMeshPickThread::setWidget(int w, int h)
//	{
//		_Win_WIDTH = w;
//		_Win_HEIGHT = h;
//	}
//
//	bool mPreMeshPickThread::isFinished()
//	{
//		return _isfinished;
//	}
//
//	void mPreMeshPickThread::setFinished()
//	{
//		_isfinished = false;
//	}
//
//	void mPreMeshPickThread::doSoloPick(QString partName)
//	{
//		//判断该部件是否存在碰撞
//		//判断点选是否在部件的包围盒内
//		QVector3D worldVertex = _p;
//		Space::AABB aabb;//部件包围盒
//		if (aabb.ContainPoint(_p))
//		{
//			switch (*_pickFilter)
//			{
//			case PickFilter::PickNothing:; break;
//			case PickFilter::PickGeoPoint:SoloPickGeoPoint(partName); break;
//			case PickFilter::PickGeoLine:SoloPickGeoLine(partName); break;
//			case PickFilter::PickGeoFace:SoloPickGeoFace(partName); break;
//			case PickFilter::PickGeoSolid:SoloPickGeoSolid(partName); break;
//			case PickFilter::PickGeoPart:SoloPickGeoPart(partName); break;
//			case PickFilter::PickGeoPointByPart:SoloPickGeoPointByPart(partName); break;
//			case PickFilter::PickGeoLineByPart:SoloPickGeoLineByPart(partName); break;
//			case PickFilter::PickGeoFaceByPart:SoloPickGeoFaceByPart(partName); break;
//			case PickFilter::PickGeoSolidByPart:SoloPickGeoSolidByPart(partName); break;
//			case PickFilter::pickVertexOnGeoLine:SoloPickVertexOnGeoLine(partName); break;
//			case PickFilter::pickVertexOnGeoFace:SoloPickVertexOnGeoFace(partName); break;
//			default:break;
//			}
//		}
//	}
//
//	void mPreMeshPickThread::doMultiplyPick(QString partName)
//	{
//		Space::AABB aabb;//部件包围盒
//		QVector<QVector2D> ap = _pick->getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
//		bool isAllIn{ false };
//		bool isIntersetion{ false };
//		if (_pick->isAABBPointIsAllInPick(ap))//拾取到整个部件
//		{
//			isAllIn = true; isIntersetion = true;
//		}
//		else if (_pick->isIntersectionAABBAndPick(ap))//有相交
//		{
//			isIntersetion = true;		
//		}
//		if (isIntersetion)
//		{
//			switch (*_pickFilter)
//			{
//			case PickFilter::PickNothing:; break;
//			case PickFilter::PickGeoPoint:MultiplyPickGeoPoint(partName, isAllIn); break;
//			case PickFilter::PickGeoLine:MultiplyPickGeoLine(partName, isAllIn); break;
//			case PickFilter::PickGeoFace:MultiplyPickGeoFace(partName, isAllIn); break;
//			case PickFilter::PickGeoSolid:MultiplyPickGeoSolid(partName, isAllIn); break;
//			case PickFilter::PickGeoPart:MultiplyPickGeoPart(partName, isAllIn); break;
//			case PickFilter::PickGeoPointByPart:MultiplyPickGeoPointByPart(partName, isAllIn); break;
//			case PickFilter::PickGeoLineByPart:MultiplyPickGeoLineByPart(partName, isAllIn); break;
//			case PickFilter::PickGeoFaceByPart:MultiplyPickGeoFaceByPart(partName, isAllIn); break;
//			case PickFilter::PickGeoSolidByPart:MultiplyPickGeoSolidByPart(partName, isAllIn); break;
//			default:break;
//			}
//		}
//	}
//
//	void mPreMeshPickThread::SoloPickGeoPoint(QString partName)
//	{
//		int id = 0;
//		float depth = 1.0;
//		float depth1 = 1.0f;
//		////MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> pointIDs = partData->getGeoPointIDs();
//		for (int pointID : pointIDs)
//		{
//			MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(pointID);
//			QVector2D ap1 = WorldvertexToScreenvertex(geoPointData->getGeoPointVertex(), depth1);
//			if (fabs(ap1.x() - _pos.x()) <= 3 && fabs(ap1.y() - _pos.y()) <= 3 && depth1 < depth)
//			{
//				depth = depth1;
//				id = pointID;
//			}
//		}
//		if (id == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setSoloPickGeoPointData(id, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoLine(QString partName)
//	{
//		int id = 0;
//		float depth = 1.0;
//		float depth1 = 1.0f, depth2 = 1.0f;
//		float uv[2];
//		////MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> lineIDs = partData->getGeoLineIDs();
//		for (int lineID : lineIDs)
//		{
//			MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
//			if (geoLineData->getGeoLineAABB().ContainPoint(_p))
//			{
//				for (int j = 0; j < geoLineData->getGeoLineVertex().size(); j += 2)
//				{
//					QVector2D ap1 = WorldvertexToScreenvertex(geoLineData->getGeoLineVertex().at(j), depth1);
//					QVector2D ap2 = WorldvertexToScreenvertex(geoLineData->getGeoLineVertex().at(j + 1), depth2);
//					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
//					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MViewBasic::MeshBeam))
//					{
//						//根据线段长度计算被点击的点的深度值
//						float d = mPickToolClass::CaculatePointInLineDepth(ap1, ap2, _pos, depth1, depth2, uv);
//						if (d < depth)
//						{
//							id = lineID;
//							depth = d;
//							//break;
//						}
//
//					}
//				}
//			}
//
//		}
//		if (id == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setSoloPickGeoLineData(id, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoFace(QString partName)
//	{
//		int id = 0;
//		float depth = FLT_MAX;
//		float uv[2];
//		float t;
//		//float depth1 = 1.0f, depth2 = 1.0f, depth3 = 1.0f;
//		////MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> faceIDs = partData->getGeoFaceIDs();
//		for (int faceID : faceIDs)
//		{
//			MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//			if (geoFaceData->getGeoFaceAABB().ContainPoint(_p))
//			{
//				for (int j = 0; j < geoFaceData->getGeoFaceVertex().size(); j += 3)
//				{
//					if (mPickToolClass::rayTriangleIntersect(_origin, _dir, geoFaceData->getGeoFaceVertex().mid(j, 3), uv, t))
//					{
//						id = faceID;
//						depth = t;
//					}
//					//QVector2D ap1 = WorldvertexToScreenvertex(geoFaceData->getGeoFaceVertex().at(j), depth1);
//					//QVector2D ap2 = WorldvertexToScreenvertex(geoFaceData->getGeoFaceVertex().at(j + 1), depth2);
//					//QVector2D ap3 = WorldvertexToScreenvertex(geoFaceData->getGeoFaceVertex().at(j + 2), depth3);
//					//QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
//					//if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MViewBasic::MeshTri))
//					//{
//					//	//根据三角形面积计算被点击的点的深度值
//					//	float d = mPickToolClass::CaculatePointInTriDepth(ap1, ap2, ap3, _pos, depth1, depth2, depth3);
//					//	if (d < depth)
//					//	{
//					//		id = faceID;
//					//		depth = d;
//					//		//break;
//					//	}
//					//}
//				}
//			}
//
//		}
//		if (id == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setSoloPickGeoFaceData(id, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoSolid(QString partName)
//	{
//		int id = 0;
//		bool isInSolid = false;
//		float depth = FLT_MAX;
//		float uv[2];
//		float t;
//		//float depth1 = 1.0f, depth2 = 1.0f, depth3 = 1.0f;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> solidIDs = partData->getGeoSolidIDs();
//		for (int solidID : solidIDs)
//		{
//			MDataGeo::mGeoSolidData1* geoSolidData = _geoModelData->getGeoSolidDataByID(solidID);
//			if (geoSolidData->getGeoSolidAABB().ContainPoint(_p))
//			{
//				set<int> faceIDs = geoSolidData->getGeoSolidFaceIDs();
//				for (int faceID : faceIDs)
//				{
//					MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//					if (geoFaceData->getGeoFaceAABB().ContainPoint(_p))
//					{
//						for (int k = 0; k < geoFaceData->getGeoFaceVertex().size(); k += 3)
//						{
//							//QVector2D ap1 = WorldvertexToScreenvertex(geoFaceData->getGeoFaceVertex().at(k), depth1);
//							//QVector2D ap2 = WorldvertexToScreenvertex(geoFaceData->getGeoFaceVertex().at(k + 1), depth2);
//							//QVector2D ap3 = WorldvertexToScreenvertex(geoFaceData->getGeoFaceVertex().at(k + 2), depth3);
//							//QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
//							//if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MViewBasic::MeshTri))
//							{
//								//根据三角形面积计算被点击的点的深度值
//								if (mPickToolClass::rayTriangleIntersect(_origin, _dir, geoFaceData->getGeoFaceVertex().mid(k, 3), uv, t))
//								{
//									id = solidID;
//									//isInSolid = true;
//									depth = t;
//									//break;
//								}
//							}
//						}
//					}
//					//if (isInSolid)
//					//{
//					//	isInSolid = false;
//					//	break;
//					//}
//				}
//			}
//
//		}
//		if (id == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setSoloPickGeoSolidData(id, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoPart(QString partName)
//	{
//		float depth = 1.0;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		//if (!IsSoloPickGeoPart(partData, depth))
//		{
//			return;
//		}
//		pickMutex.lock();
//		//_pickData->setSoloPickGeoPartData(_partName, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickVertexOnGeoLine(QString partName)
//	{
//		int id = 0;
//		QVector3D vertex;
//		float depth = 1.0;
//		float depth1 = 1.0f, depth2 = 1.0f;
//		float uv[2];
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> lineIDs = partData->getGeoLineIDs();
//		for (int lineID : lineIDs)
//		{
//			MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
//			for (int j = 0; j < geoLineData->getGeoLineVertex().size() - 1; j++)
//			{
//				QVector2D ap1 = WorldvertexToScreenvertex(geoLineData->getGeoLineVertex().at(j), depth1);
//				QVector2D ap2 = WorldvertexToScreenvertex(geoLineData->getGeoLineVertex().at(j + 1), depth2);
//				QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
//				if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MViewBasic::MeshBeam))
//				{
//					//根据线段长度计算被点击的点的深度值
//					float d = mPickToolClass::CaculatePointInLineDepth(ap1, ap2, _pos, depth1, depth2, uv);
//					if (d < depth)
//					{
//						vertex = geoLineData->getGeoLineVertex().at(j) * uv[0] + geoLineData->getGeoLineVertex().at(j + 1) * uv[1];
//						id = lineID;
//						depth = d;
//					}
//
//				}
//			}
//
//		}
//		if (id == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setSoloPickVertexOnGeoLineData(id, vertex, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickVertexOnGeoFace(QString partName)
//	{
//		int id = 0;
//		QVector3D vertex;
//		float depth = FLT_MAX;
//		float uv[2];
//		float t;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> faceIDs = partData->getGeoFaceIDs();
//		for (int faceID : faceIDs)
//		{
//			MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//			for (int j = 0; j < geoFaceData->getGeoFaceVertex().size(); j += 3)
//			{
//				if (mPickToolClass::rayTriangleIntersect(_origin, _dir, geoFaceData->getGeoFaceVertex().mid(j, 3), uv, t))
//				{
//					id = faceID;
//					vertex = geoFaceData->getGeoFaceVertex().at(j) * (1 - uv[0] - uv[1]) + geoFaceData->getGeoFaceVertex().at(j + 1) * uv[0] + geoFaceData->getGeoFaceVertex().at(j + 2) * uv[1];
//					depth = t;
//				}
//			}
//		}
//		if (id == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setSoloPickVertexOnGeoFaceData(id, vertex, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoPointByPart(QString partName)
//	{
//		float depth = 1.0;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> pointIDs = partData->getGeoPointIDs();
//		//if (!IsSoloPickGeoPart(partData, depth))
//		{
//			return;
//		}
//		set<int> ids{ pointIDs.begin(),pointIDs.end() };
//		pickMutex.lock();
//		_pickData->setSoloPickGeoPointByPartData(ids, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoLineByPart(QString partName)
//	{
//		float depth = 1.0;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int>lineIDs = partData->getGeoLineIDs();
//		//if (!IsSoloPickGeoPart(partData, depth))
//		{
//			return;
//		}
//		set<int> ids{ lineIDs.begin(),lineIDs.end() };
//		pickMutex.lock();
//		_pickData->setSoloPickGeoLineByPartData(ids, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoFaceByPart(QString partName)
//	{
//		float depth = 1.0;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> faceIDs = partData->getGeoFaceIDs();
//		//if (!IsSoloPickGeoPart(partData, depth))
//		{
//			return;
//		}
//		set<int> ids{ faceIDs.begin(),faceIDs.end() };
//		pickMutex.lock();
//		_pickData->setSoloPickGeoFaceByPartData(ids, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::SoloPickGeoSolidByPart(QString partName)
//	{
//		float depth = 1.0;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> solidIDs = partData->getGeoSolidIDs();
//		//if (!IsSoloPickGeoPart(partData, depth))
//		{
//			return;
//		}
//		set<int> ids{ solidIDs.begin(),solidIDs.end() };
//		pickMutex.lock();
//		_pickData->setSoloPickGeoSolidByPartData(ids, depth);
//		pickMutex.unlock();
//		return;
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoPoint(QString partName, bool isAllIn)
//	{
//		std::set<int> pickGeoPointIDs;
//		set<int> pointIDs = partData->getGeoPointIDs();
//		if (isAllIn)
//		{
//			pickGeoPointIDs = pointIDs;
//		}
//		else
//		{
//			for (int pointID : pointIDs)
//			{
//				MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(pointID);
//				if(IsMultiplyPickGeoPoint(geoPointData))
//				{
//					pickGeoPointIDs.insert(pointID);
//				}
//			}
//		}
//		if (pickGeoPointIDs.size() == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoPointData(pickGeoPointIDs);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoPart(QString partName, bool isAllIn)
//	{
//		if (isAllIn)
//		{
//
//		}
//		else if (!isMultiplyPickGeoPart(partData))
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoPartData(partData->getPartName());
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoPointByPart(QString partName, bool isAllIn)
//	{
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (isAllIn)
//		{
//
//		}
//		else if (!isMultiplyPickGeoPart(partData))
//		{
//			return;
//		}
//		set<int> ids = partData->getGeoPointIDs();
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoPointData(ids);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoLineByPart(QString partName, bool isAllIn)
//	{
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (isAllIn)
//		{
//
//		}
//		else if (!isMultiplyPickGeoPart(partData))
//		{
//			return;
//		}
//		set<int>ids = partData->getGeoLineIDs();
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoLineData(ids);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoFaceByPart(QString partName, bool isAllIn)
//	{
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (isAllIn)
//		{
//
//		}
//		else if (!isMultiplyPickGeoPart(partData))
//		{
//			return;
//		}
//		set<int> ids = partData->getGeoFaceIDs();
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoFaceData(ids);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoSolidByPart(QString partName, bool isAllIn)
//	{
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (isAllIn)
//		{
//
//		}
//		else if (!isMultiplyPickGeoPart(partData))
//		{
//			return;
//		}
//		set<int>ids = partData->getGeoSolidIDs();
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoSolidData(ids);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoLine(QString partName, bool isAllIn)
//	{
//		std::set<int> pickGeoLineIDs;
//		set<int> lineIDs = partData->getGeoLineIDs();
//		if (isAllIn)
//		{
//			pickGeoLineIDs = lineIDs;
//		}
//		else
//		{
//			for (int lineID : lineIDs)
//			{
//				MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
//				for (int j = 0; j < geoLineData->getGeoLineVertex().size(); j+=2)
//				{
//					if(_pick->getGeoLineIsInPick(geoLineData->getGeoLineVertex().mid(j, 2)))			
//					{
//						pickGeoLineIDs.insert(lineID);
//						break;
//					}
//				}
//			}
//		}
//		if (pickGeoLineIDs.size() == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoLineData(pickGeoLineIDs);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoFace(QString partName, bool isAllIn)
//	{
//		std::set<int> pickGeoFaceIDs;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		set<int> faceIDs = partData->getGeoFaceIDs();
//		if (isAllIn)
//		{
//			pickGeoFaceIDs = faceIDs;
//		}
//		else
//		{
//			for (int faceID : faceIDs)
//			{
//				MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//				if (IsMultiplyPickGeoFace(geoFaceData))
//				{
//					pickGeoFaceIDs.insert(faceID);
//				}
//			}
//		}
//		if (pickGeoFaceIDs.size() == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoFaceData(pickGeoFaceIDs);
//		pickMutex.unlock();
//	}
//
//	void mPreMeshPickThread::MultiplyPickGeoSolid(QString partName, bool isAllIn)
//	{
//		std::set<int> pickGeoSolidIDs;
//		bool isInSolid = false;
//		//MDataGeo::QString partName = _geoModelData->getGeoPartDataByPartName(_partName);
//		if (partData == nullptr)
//		{
//			return;
//		}
//		if (!partData->getPartVisual())
//		{
//			return;
//		}
//		set<int> solidIDs = partData->getGeoSolidIDs();
//		for (int solidID : solidIDs)
//		{
//			MDataGeo::mGeoSolidData1* geoSolidData = _geoModelData->getGeoSolidDataByID(solidID);
//			if(IsMultiplyPickGeoSolid(geoSolidData))
//			{
//				pickGeoSolidIDs.insert(solidID);
//			}			
//		}
//		if (pickGeoSolidIDs.size() == 0)
//		{
//			return;
//		}
//		pickMutex.lock();
//		_pickData->setMutiplyPickGeoSolidData(pickGeoSolidIDs);
//		pickMutex.unlock();
//	}
//
//	bool mPreMeshPickThread::IsSoloPickMeshPart(MDataPost::mPostMeshPartData1 * meshPartData, float &depth)
//	{
//		return false;
//	}
//
//	bool mPreMeshPickThread::IsMultiplyPickGeoPoint(MDataGeo::mGeoPointData1 * geoPointData)
//	{
//		if (_pick->getGeoPointIsInPick(geoPointData->getGeoPointVertex()))
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool mPreMeshPickThread::IsMultiplyPickGeoLine(MDataGeo::mGeoLineData1 * geoLineData)
//	{
//		QVector<QVector2D> ap = _pick->getAABBToScreenVertex(geoLineData->getGeoLineAABB().minEdge, geoLineData->getGeoLineAABB().maxEdge);
//		if (_pick->isAABBPointIsAllInPick(ap))//拾取到整个几何面
//		{
//			return true;
//		}
//		else if (_pick->isIntersectionAABBAndPick(ap))//有相交
//		{
//			for (int j = 0; j < geoLineData->getGeoLineVertex().size(); j += 2)
//			{
//				if (_pick->getGeoLineIsInPick(geoLineData->getGeoLineVertex().mid(j, 2)))
//				{
//					return true;
//				}
//			}
//		}
//		return false;
//	}
//
//	bool mPreMeshPickThread::IsMultiplyPickGeoFace(MDataGeo::mGeoFaceData1 * geoFaceData)
//	{
//		QVector<QVector2D> ap = _pick->getAABBToScreenVertex(geoFaceData->getGeoFaceAABB().minEdge, geoFaceData->getGeoFaceAABB().maxEdge);
//		if (_pick->isAABBPointIsAllInPick(ap))//拾取到整个几何面
//		{
//			return true;
//		}
//		else if (_pick->isIntersectionAABBAndPick(ap))//有相交
//		{
//			for (int j = 0; j < geoFaceData->getGeoFaceVertex().size(); j += 3)
//			{
//				if (_pick->getGeoFaceIsInPick(geoFaceData->getGeoFaceVertex().mid(j, 3)))
//				{
//					return true;
//				}
//			}
//		}
//		return false;
//	}
//
//	bool mPreMeshPickThread::IsMultiplyPickGeoSolid(MDataGeo::mGeoSolidData1 * geoSolidData)
//	{
//		QVector<QVector2D> ap = _pick->getAABBToScreenVertex(geoSolidData->getGeoSolidAABB().minEdge, geoSolidData->getGeoSolidAABB().maxEdge);
//		if (_pick->isAABBPointIsAllInPick(ap))//拾取到整个几何体
//		{
//			return true;
//		}
//		else if (_pick->isIntersectionAABBAndPick(ap))//有相交
//		{
//			set<int> faceIDs = geoSolidData->getGeoSolidFaceIDs();
//			for (int faceID : faceIDs)
//			{
//				MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//				if (IsMultiplyPickGeoFace(geoFaceData))
//				{
//					return true;
//				}
//			}
//		}
//		return false;
//	}
//
//	bool mPreMeshPickThread::isMultiplyPickGeoPart(MDataGeo::mGeoPartData1 *geoPartData)
//	{
//		switch (geoPartData->getGeoShapeType())
//		{
//			//判断面，线，点即可
//		case 0://混合
//		{
//			set<int> faceIDs = geoPartData->getGeoFaceIDs();
//			for (int faceID : faceIDs)
//			{
//				MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//				if (IsMultiplyPickGeoFace(geoFaceData))
//				{
//					return true;
//				}
//			}
//			set<int> lineIDs = geoPartData->getGeoLineIDs();
//			for (int lineID : lineIDs)
//			{
//				MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
//				if (IsMultiplyPickGeoLine(geoLineData))
//				{
//					return true;
//				}
//			}
//			set<int> pointIDs = geoPartData->getGeoPointIDs();
//			for (int pointID : pointIDs)
//			{
//				MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(pointID);
//				if (IsMultiplyPickGeoPoint(geoPointData))
//				{
//					return true;
//				}
//			}
//			return false;
//		}
//		//判断面即可
//		case 1://多实体
//		case 2://实体
//		case 4://面
//		{
//			set<int> faceIDs = geoPartData->getGeoFaceIDs();
//			for (int faceID : faceIDs)
//			{
//				MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceID);
//				if (IsMultiplyPickGeoFace(geoFaceData))
//				{
//					return true;
//				}
//			}
//			return false;
//		}
//		//判断线即可
//		case 6://线
//		{
//			set<int> lineIDs = geoPartData->getGeoLineIDs();
//			for (int lineID : lineIDs)
//			{
//				MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
//				if (IsMultiplyPickGeoLine(geoLineData))
//				{
//					return true;
//				}
//			}
//			return false;
//		}
//		//判断点即可
//		case 7://点
//		{
//			set<int> pointIDs = geoPartData->getGeoPointIDs();
//			for (int pointID : pointIDs)
//			{
//				MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(pointID);
//				if (IsMultiplyPickGeoPoint(geoPointData))
//				{
//					return true;
//				}
//			}
//			return false;
//		}
//		return false;
//		}
//	}
//
//	void mPreMeshPickThread::startPick()
//	{
//		_isfinished = false;
//		QVector<QFuture<void>> futures;
//		if (_pickMode == PickMode::SoloPick)
//		{
//			if (*_pickFilter == PickFilter::pickVertexOnScreen)
//			{
//				this->SoloPickScreen();
//			}
//			else
//			{
//				if (_depth == 1.0)
//				{
//					return;
//				}
//				auto iter = _geoModelData->getPartIterator();
//				while (iter.hasNext())
//				{
//					iter.next();
//					if (!iter.value()->getPartVisual())
//					{
//						return;
//					}
//					futures.append(QtConcurrent::run(this, &mPreMeshPickThread::doSoloPick, iter.value()));
//				}
//				while (!futures.empty())
//				{
//					futures.back().waitForFinished();
//					futures.takeLast();
//				}
//				_pickData->setSoloPickData();
//			}
//
//		}
//		else
//		{
//			auto iter = _geoModelData->getPartIterator();
//			while (iter.hasNext())
//			{
//				iter.next();
//				if (!iter.value()->getPartVisual())
//				{
//					return;
//				}
//				futures.append(QtConcurrent::run(this, &mPreMeshPickThread::doMultiplyPick, iter.value()));
//			}
//			while (!futures.empty())
//			{
//				futures.back().waitForFinished();
//				futures.takeLast();
//			}
//		}
//
//	}
//
//	QVector2D mPreMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex)
//	{
//		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
//		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
//	}
//
//	QVector3D mPreMeshPickThread::ScreenvertexToWorldvertex(QVector3D vertex)
//	{
//		QMatrix4x4 i_mvp = (_pvm).inverted();
//		float ndc_x = 2.0*vertex.x() / (float)_Win_WIDTH - 1.0f;
//		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_Win_HEIGHT);
//		float ndc_z = vertex.z() * 2 - 1.0;
//
//		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
//		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
//	}
//
//	QVector2D mPreMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
//	{
//		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
//		depth = res.z() + 0.5;
//		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
//	}
//
//	void mPreMeshPickThread::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
//	{
//		for (auto Worldvertex : Worldvertexs)
//		{
//			float depth;
//			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
//			depths.insert(depth);
//		}
//	}
//	QVector3D mPreMeshPickThread::getCenter(QVector<QVector3D> vertexs)
//	{
//		QVector3D nodePos;
//		for (auto node : vertexs)
//		{
//			nodePos += node;
//		}
//		return nodePos / vertexs.size();
//	}
//}