#include "mPreMeshPickThread.h"
#include "mPreMeshPickData1.h"

#include "MeshMessage.h"
#include "MXMeshTetrahedron.h"
#include "MXMeshHexahedral.h"
#include "MXMeshTriangle.h"
#include "MXMeshQuadrangle.h"
#include "MXMeshElement.h"
#include "MFace.h"
#include "MEdge.h"

//MViewBasic
#include "mMeshViewEnum.h"
#include "mPickToolClass.h"

//Qt
#include <QVector2D>
#include <QVector4D>
#include <QString>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <qmath.h>

using namespace MViewBasic;
using namespace MDataPost;
namespace MPreRend
{
	QMutex pickMutex;

	QVector2D mBasePick::WorldvertexToScreenvertex(QVector3D Worldvertex)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_width, _height - (res.y() / res.w() / 2.0 + 0.5)*_height);
	}

	QVector<QVector2D> mBasePick::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs)
	{
		QVector<QVector2D> vertexs;
		for (auto Worldvertex : Worldvertexs)
		{
			vertexs.append(WorldvertexToScreenvertex(Worldvertex));
		}
		return vertexs;
	}

	QVector<QVector2D> mBasePick::getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge)
	{
		QVector<QVector2D> ap;
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), minEdge.y(), minEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), minEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), maxEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), maxEdge.y(), minEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), minEdge.y(), minEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), minEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), maxEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), maxEdge.y(), minEdge.z())));
		return ap;
	}

	QVector3D mBasePick::ScreenvertexToWorldvertex(QVector3D vertex)
	{
		QMatrix4x4 i_mvp = (_pvm).inverted();
		float ndc_x = 2.0*vertex.x() / (float)_width - 1.0f;
		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_height);
		float ndc_z = vertex.z() * 2 - 1.0;

		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
	}

	QVector2D mBasePick::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		depth = res.z();
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_width, _height - (res.y() / res.w() / 2.0 + 0.5)*_height);
	}

	void mBasePick::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
	{
		for (auto Worldvertex : Worldvertexs)
		{
			float depth;
			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
			depths.insert(depth);
		}
	}

	bool mQuadPick::getPickIsIntersectionWithAABB(AABB aabb)
	{
		QVector<QVector2D> ap = getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInQuad(ap, _center, _boxXY_2))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mQuadPick::get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter)
	{
		if (mPickToolClass::IsPointInQuad(WorldvertexToScreenvertex(pointCenter), _center, _boxXY_2))
		{
			return true;
		}
		return false;
	}

	bool mQuadPick::get1DMeshIsInPick(QVector<QVector3D> vertexs)
	{
		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshBeam) || mPickToolClass::IsPointInQuad(tempQVector2D, _center, _boxXY_2))
		{
			return true;
		}
		return false;
	}

	bool mQuadPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
	{
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInQuad(ap, _center, _boxXY_2))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mQuadPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsAllPointInQuad(ap, _center, _boxXY_2))//����
		{
			return true;
		}
		return false;
	}

	bool mPolygonPick::getPickIsIntersectionWithAABB(AABB aabb)
	{
		QVector<QVector2D> ap = getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInPolygon(ap, _center, _multiQuad))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mPolygonPick::get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter)
	{
		if (mPickToolClass::IsPointInPolygon(WorldvertexToScreenvertex(pointCenter), _center, _multiQuad))
		{
			return true;
		}
		return false;
	}

	bool mPolygonPick::get1DMeshIsInPick(QVector<QVector3D> vertexs)
	{
		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshBeam) || mPickToolClass::IsPointInPolygon(tempQVector2D, _center, _multiQuad))
		{
			return true;
		}
		return false;
	}

	bool mPolygonPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
	{
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInPolygon(ap, _center, _multiQuad))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mPolygonPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsAllPointInPolygon(ap, _center, _multiQuad))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::getPickIsIntersectionWithAABB(AABB aabb)
	{
		QVector<QVector2D> ap = getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		if (mPickToolClass::IsPointInRound(ap, _screenCenter, _screenRadius) || mPickToolClass::IsAABBIntersectionWithCircle(ap, _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter)
	{
		if (mPickToolClass::IsPointInRound(pointCenter, _centerPoint, _centerDirection, _radius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::get1DMeshIsInPick(QVector<QVector3D> vertexs)
	{
		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
		if (mPickToolClass::IsLineIntersectionWithRound(tempQVector2D.first(), tempQVector2D.last(), _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsPointInRound(ap, _screenCenter, _screenRadius) || mPickToolClass::IsAABBIntersectionWithCircle(ap, _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsAllPointInRound(ap, _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	
	mPreMeshPickThread::mPreMeshPickThread(mPreMeshPickData1 *pickData)
	{
		_isfinished = false;
		_pickData = pickData;
		_pickAngleValue = 60;
		
	}

	mPreMeshPickThread::~mPreMeshPickThread()
	{
	}

	void mPreMeshPickThread::setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode)
	{
		_pickMode = pickMode;
		_multiplyPickMode = multiplyPickMode;
	}

	void mPreMeshPickThread::setPickFilter(PickFilter * pickFilter)
	{
		_pickFilter = pickFilter;
	}

	void mPreMeshPickThread::startPick()
	{
		_isfinished = false;
		QVector<QFuture<void>> futures;
		if (_pickMode == PickMode::SoloPick)
		{
			QVector<QString> partNames = MeshMessage::getInstance()->getAllPartNames();
			for (auto partName : partNames)
			{
				if (MeshMessage::getInstance()->getPartMask(partName))
				{
					continue;
				}
				futures.append(QtConcurrent::run(this, &mPreMeshPickThread::doSoloPick, partName));
			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
			}
			if (*_pickFilter == PickFilter::PickNodeOrder)
			{
				_pickData->setSoloOrderPickData();
			}
			else if (*_pickFilter == PickFilter::PickNodePath)
			{
				int size = _pickData->getPickNodeIDsOrder().size();
				_pickData->setSoloOrderPickData();
				int size1 = _pickData->getPickNodeIDsOrder().size();
				if (size == size1)//û��ʰȡ��
				{
					return;
				}
				else if (size == (size1 + 1) || size1 == 1)//��ѡ���ߵ�һ����
				{
					return;
				}
				else//��ѡ
				{
					//��������ʰȡ
					QtConcurrent::run(this, &mPreMeshPickThread::SoloPickNodePath).waitForFinished();
				}
			}
			else
			{
				_pickData->setSoloPickData();
				if (*_pickFilter == PickFilter::PickNodeByLineAngle || *_pickFilter == PickFilter::PickNodeByFaceAngle || *_pickFilter == PickFilter::Pick2DMeshByAngle || *_pickFilter == PickFilter::Pick1DMeshByAngle
					|| *_pickFilter == PickFilter::PickMeshLineByAngle || *_pickFilter == PickFilter::PickMeshFaceByAngle || *_pickFilter == PickFilter::PickNodePath)
				{
					//��������ʰȡ
					QtConcurrent::run(this, &mPreMeshPickThread::doAnglePick).waitForFinished();
				}
			}
		}
		else
		{
			QVector<QString> partNames = MeshMessage::getInstance()->getAllPartNames();
			for (auto partName : partNames)
			{
				if (MeshMessage::getInstance()->getPartMask(partName))
				{
					continue;
				}
				futures.append(QtConcurrent::run(this, &mPreMeshPickThread::doMultiplyPick, partName));
			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
			}
		}
	}

	void mPreMeshPickThread::setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	{
		_projection = projection;
		_view = view;
		_model = model;
	}

	void mPreMeshPickThread::setMatrix(QMatrix4x4 pvm)
	{
		_pvm = pvm;
	}

	void mPreMeshPickThread::setLocation(const QVector2D& pos, float depth)
	{
		_pos = pos;
		soloQuad = QVector<QVector2D>{ QVector2D(pos.x() + 5,pos.y() + 5),QVector2D(pos.x() + 5,pos.y() - 5),QVector2D(pos.x() - 5,pos.y() - 5),QVector2D(pos.x() - 5,pos.y() + 5) };
		_depth = depth;
		_p = ScreenvertexToWorldvertex(QVector3D(pos, depth));
		_origin = ScreenvertexToWorldvertex(QVector3D(pos, -1.0));
		_dir = (_p - _origin).normalized();
		//_pickSoloOrMutiply = MViewBasic::SoloPick;
	}

	void mPreMeshPickThread::setLocation(QVector<QVector2D> pickQuad, QVector3D direction)
	{
		if (pickQuad.size() < 1)
		{
			return;
		}
		//multiQuad = QVector<QVector2D>{ pickQuad.first(), QVector2D(pickQuad.first().x(), pickQuad.last().y()), pickQuad.last(), QVector2D(pickQuad.last().x(), pickQuad.first().y()) };
		//_centerBox = (pickQuad.first() + pickQuad.last()) / 2.0;
		//_boxXY_2 = pickQuad.first() - _centerBox; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);	
		//_centerX = centerX;
		//_centerY = centerY;
		//_boxW = boxW;
		//_boxY = boxY;
		//multiQuad = pickQuad;
		switch (_multiplyPickMode)
		{
		case MViewBasic::MultiplyPickMode::QuadPick:_pick = make_shared<mQuadPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad);
			break;
		case MViewBasic::MultiplyPickMode::PolygonPick:_pick = make_shared<mPolygonPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad);
			break;
		case MViewBasic::MultiplyPickMode::RoundPick:_pick = make_shared<mRoundPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad.first(), pickQuad.last(), direction);
			break;
		default:
			break;
		}
		
		//if (_multiplyPickMode == MultiplyPickMode::RoundPick)
		//{
		//	_centerScreenPoint = _centerBox;
		//	QVector3D centerPoint = (_pvm.inverted() * QVector4D(2 * pickQuad.last().x() / _Win_WIDTH - 1, 1 - 2 * pickQuad.last().y() / _Win_HEIGHT, 0.0, 1.0)).toVector3D();//���Բ����������
		//	_centerPoint = (_pvm.inverted() * QVector4D(2 * _centerScreenPoint.x() / _Win_WIDTH - 1, 1 - 2 * _centerScreenPoint.y() / _Win_HEIGHT, 0.0, 1.0)).toVector3D();//���Բ��һ�������
		//	_radius = _centerPoint.distanceToPoint(centerPoint);
		//	_screenRadius = _centerBox.distanceToPoint(pickQuad.first());
		//	_centerDirection = direction;
		//}
		//_pickSoloOrMutiply = MViewBasic::MultiplyPick;
	}

	void mPreMeshPickThread::setWidget(int w, int h)
	{
		_Win_WIDTH = w;
		_Win_HEIGHT = h;
	}

	bool mPreMeshPickThread::isFinished()
	{
		return _isfinished;
	}

	void mPreMeshPickThread::setFinished()
	{
		_isfinished = false;
	}

	void mPreMeshPickThread::doSoloPick(QString partName)
	{
		//�жϸò����Ƿ������ײ
		//�жϵ�ѡ�Ƿ��ڲ����İ�Χ����
		QVector3D worldVertex = _p;
		auto part = MeshMessage::getInstance()->getBoundBoxSamePart(partName);
		Space::AABB aabb(part.first, part.second);//������Χ��
		if (aabb.ContainPoint(_p))
		{
			switch (*_pickFilter)
			{
			case PickFilter::PickNothing:; break;
			case PickFilter::PickNode:
			case PickFilter::PickNodeOrder:
			case PickFilter::PickNodePath:SoloPickNode(partName); break;
			case PickFilter::PickPoint:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshPoint}); break;
			case PickFilter::Pick1DMesh:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshBeam}); break;
			case PickFilter::Pick2DMesh:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshTri, MeshQuad}); break;
			case PickFilter::PickTri:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshTri}); break;
			case PickFilter::PickQuad:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshQuad}); break;
			case PickFilter::PickTet:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshTet}); break;
			case PickFilter::PickPyramid:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshPyramid}); break;
			case PickFilter::PickWedge:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshWedge}); break;
			case PickFilter::PickHex:SoloPickMeshTypeFilter(partName, QVector<MeshType>{MeshHex}); break;
			case PickFilter::PickAnyMesh:SoloPickAnyMesh(partName); break;
			case PickFilter::PickMeshLine:SoloPickMeshLine(partName); break;
			case PickFilter::PickMeshFace:SoloPickMeshFace(partName); break;
			case PickFilter::PickMeshPart:SoloPickMeshPart(partName); break;
			case PickFilter::PickNodeByPart:SoloPickNodeByPart(partName); break;
			case PickFilter::PickAnyMeshByPart:SoloPickAnyMeshByPart(partName); break;
			case PickFilter::PickMeshLineByPart:SoloPickMeshLineByPart(partName); break;
			case PickFilter::PickMeshFaceByPart:SoloPickMeshFaceByPart(partName); break;
			case PickFilter::PickNodeByLineAngle:SoloPickNodeByLineAngle(partName); break;
			case PickFilter::PickNodeByFaceAngle:SoloPickNodeByFaceAngle(partName); break;
			case PickFilter::Pick1DMeshByAngle:SoloPick1DMeshByAngle(partName); break;
			case PickFilter::Pick2DMeshByAngle:SoloPick2DMeshByAngle(partName); break;
			case PickFilter::PickMeshLineByAngle:SoloPickMeshLineByAngle(partName); break;
			case PickFilter::PickMeshFaceByAngle:SoloPickMeshFaceByAngle(partName); break;
			default:break;
			}
		}
	}

	void mPreMeshPickThread::doMultiplyPick(QString partName)
	{
		auto part = MeshMessage::getInstance()->getBoundBoxSamePart(partName);
		Space::AABB aabb(part.first, part.second);//������Χ��
		QVector<QVector2D> ap = _pick->getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		bool isAllIn{ false };
		bool isIntersetion{ false };
		if (_pick->isAABBPointIsAllInPick(ap))//ʰȡ����������
		{
			isAllIn = true; isIntersetion = true;
		}
		else if (_pick->isIntersectionAABBAndPick(ap))//���ཻ
		{
			isIntersetion = true;		
		}
		if (isIntersetion)
		{
			switch (*_pickFilter)
			{
			case PickFilter::PickNothing:; break;
			case PickFilter::PickNode:MultiplyPickNode(partName, isAllIn); break;
			case PickFilter::PickPoint:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshPoint}, isAllIn); break;
			case PickFilter::Pick1DMesh:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshBeam}, isAllIn); break;
			case PickFilter::Pick2DMesh:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshTri, MeshQuad}, isAllIn); break;
			case PickFilter::PickTri:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshTri}, isAllIn); break;
			case PickFilter::PickQuad:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshQuad}, isAllIn); break;
			case PickFilter::PickTet:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshTet}, isAllIn); break;
			case PickFilter::PickPyramid:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshPyramid}, isAllIn); break;
			case PickFilter::PickWedge:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshWedge}, isAllIn); break;
			case PickFilter::PickHex:MultiplyPickMeshTypeFilter(partName, QVector<MeshType>{MeshHex}, isAllIn); break;
			case PickFilter::PickAnyMesh:MultiplyPickAnyMesh(partName, isAllIn); break;
			case PickFilter::PickMeshLine:MultiplyPickMeshLine(partName, isAllIn); break;
			case PickFilter::PickMeshFace:MultiplyPickMeshFace(partName, isAllIn); break;
			case PickFilter::PickMeshPart:MultiplyPickMeshPart(partName, isAllIn); break;
			default:break;
			}
		}
	}

	void mPreMeshPickThread::doAnglePick()
	{
		switch (*_pickFilter)
		{
		case PickFilter::PickNothing:break;
		case PickFilter::PickNodePath:SoloPickNodePath(); break;
		case PickFilter::PickNodeByLineAngle:SoloPickNodeByLineAngle(); break;
		case PickFilter::PickNodeByFaceAngle:SoloPickNodeByFaceAngle(); break;
		case PickFilter::Pick1DMeshByAngle:SoloPick1DMeshByAngle(); break;
		case PickFilter::PickMeshLineByAngle:SoloPickMeshLineByAngle(); break;
		case PickFilter::Pick2DMeshByAngle:SoloPick2DMeshByAngle(); break;
		case PickFilter::PickMeshFaceByAngle:SoloPickMeshFaceByAngle(); break;
			//case PickAnyPositionValue:SoloPickCuttingPlaneVertex(); break;
		default:break;
		}
	}


	bool mPreMeshPickThread::isMultiplyPickMeshPart(QString partName)
	{
		//��ȡ��Ԫ��
		QVector<MXGeoSolid*> geoSolids = MeshMessage::getInstance()->getGeoSolidSamePart(partName);
		for (auto geoSolid : geoSolids)
		{
			QVector<MFace*> mfaces = geoSolid->surfaceMeshs;
			for (auto mface : mfaces)
			{
				if (mface == nullptr)
				{
					continue;
				}
				QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
				if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
				{
					return true;
				}
			}
		}

		//��ȡ����
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
		for (auto mesh : meshs)
		{
			QVector<QVector3D> vertexs = mesh->getallVertexs1();
			switch (mesh->getMeshType())
			{
			case MeshBeam:
				if (_pick->get1DMeshIsInPick(vertexs))
				{
					return true;
				}
				break;
			case MeshTri:
			case MeshQuad:
				if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
				{
					return true;
				}
				break;
			default:
				break;
			}

		}
		return false;
	}

	QVector2D mPreMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
	}

	QVector3D mPreMeshPickThread::ScreenvertexToWorldvertex(QVector3D vertex)
	{
		QMatrix4x4 i_mvp = (_pvm).inverted();
		float ndc_x = 2.0*vertex.x() / (float)_Win_WIDTH - 1.0f;
		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_Win_HEIGHT);
		float ndc_z = vertex.z() * 2 - 1.0;

		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
	}

	QVector2D mPreMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		depth = res.z() + 0.5;
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
	}

	void mPreMeshPickThread::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
	{
		for (auto Worldvertex : Worldvertexs)
		{
			float depth;
			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
			depths.insert(depth);
		}
	}
	QVector3D mPreMeshPickThread::getCenter(QVector<QVector3D> vertexs)
	{
		QVector3D nodePos;
		for (auto node : vertexs)
		{
			nodePos += node;
		}
		return nodePos / vertexs.size();
	}
	set<MXMeshVertex*> mPreMeshPickThread::getAllNodesByPartName(QString partName)
	{
		set<MXMeshVertex*> picks;
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
		for (auto mesh : meshs)
		{
			if (mesh->getMask())
			{
				continue;
			}
			int num = mesh->getNumVertices();
			for (int i = 0; i < num; ++i)
			{
				MXMeshVertex *vertex = mesh->getVertex(i);
				if (vertex == nullptr)
				{
					continue;
				}
				picks.insert(vertex);
			}
		}
		return picks;
	}
	set<MXMeshElement*> mPreMeshPickThread::getAllMeshsByPartName(QString partName)
	{
		set<MXMeshElement*> picks;
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
		for (auto mesh : meshs)
		{
			if (mesh->getMask())
			{
				continue;
			}
			picks.insert(mesh);
		}

		return picks;
	}
	set<MFace*> mPreMeshPickThread::getAllMeshFacesByPartName(QString partName)
	{
		set<MFace*> picks;
		QVector<MXGeoSolid*> geoSolids = MeshMessage::getInstance()->getGeoSolidSamePart(partName);
		for (auto geoSolid : geoSolids)
		{
			QVector<MFace*> mfaces = geoSolid->surfaceMeshs;
			picks.insert(mfaces.begin(), mfaces.end());
			
		}
		return picks;
	}
	set<MEdge*> mPreMeshPickThread::getAllMeshLinesByPartName(QString partName)
	{
		set<MEdge*> picks;
		QVector<MXGeoSolid*> geoSolids = MeshMessage::getInstance()->getGeoSolidSamePart(partName);
		for (auto geoSolid : geoSolids)
		{
			QVector<MEdge*> medges = geoSolid->boundaryMeshEdges;
			picks.insert(medges.begin(), medges.end());
			//for (auto geoFace : geoSolid->getface())
			//{
			//	medges = geoFace->boundaryMeshEdgesInGface;
			//	picks.insert(medges.begin(), medges.end());
			//}
		}
		QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getFreeGFaceInPart(partName);
		for (auto geoFace : geoFaces)
		{
			QVector<MEdge*> medges = geoFace->boundaryMeshEdgesInGface;
			picks.insert(medges.begin(), medges.end());
		}

		return picks;
	}
	void mPreMeshPickThread::SoloPickMeshTypeFilter(QString partName, QVector<MeshType> filter)
	{
		MXMeshElement* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//��ȡ��Ԫ��
		if (filter.contains(MeshTet) || filter.contains(MeshWedge) || filter.contains(MeshPyramid) || filter.contains(MeshHex))
		{
			set<MFace*> mfaces = this->getAllMeshFacesByPartName(partName);
			for (auto mface : mfaces)
			{
				if (!filter.contains(mface->_linkEleMents_3D[0]->getMeshType()))
				{
					continue;
				}
				MeshType meshType = MeshTri;
				int num = 3;
				if (meshType == MeshQuad)
				{
					num = 4;
				}
				QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
				if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < _meshdepth)
					{
						_meshdepth = t;
						_pickMesh = mface->_linkEleMents_3D[0];
					}
				}
			}
		}

		//��ȡ��ά����
		if (filter.contains(MeshTri)|| filter.contains(MeshQuad))
		{
			QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 2);
			for (auto mesh : meshs)
			{
				int num = 3;
				if (!filter.contains(mesh->getMeshType()))
				{
					continue;
				}
				if (mesh->getMeshType() == MeshQuad)
				{
					num = 4;
				}
				QVector<QVector3D> vertexs = mesh->getallVertexs1();
				if (num == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < _meshdepth)
					{
						_meshdepth = t;
						_pickMesh = mesh;
					}
				}
			}
		}

		//��ȡһά����
		if (filter.contains(MeshBeam))
		{
			QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 1);
			for (auto mesh : meshs)
			{
				int num = 2;
				if (!filter.contains(mesh->getMeshType()))
				{
					continue;
				}
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = mesh->getallVertexs1();
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
				{
					_meshdepth = *depthlist.begin();
					_pickMesh = mesh;
				}
			}
		}


		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshData(_pickMesh, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNode(QString partName)
	{
		MXMeshVertex * _picknodevertex = nullptr;
		float _nodedepth = 1;
		float depth = 1;

		//��ȡ��Ԫ��
		set<MFace*> mfaces = this->getAllMeshFacesByPartName(partName);
		for (auto mface : mfaces)
		{
			//��ȡ���нڵ�
			MeshType meshType = MeshTri;
			int num = 3;
			if (meshType == MeshQuad)
			{
				num = 4;		
			}
			for (int i = 0; i < num; ++i)
			{
				MXMeshVertex *vertex = mface->getVertex(i);
				if (vertex == nullptr)
				{
					continue;
				}
				QVector2D ap1 = WorldvertexToScreenvertex(QVector3D(vertex->vx(), vertex->vy(), vertex->vz()), depth);
				if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
				{
					_nodedepth = depth;
					_picknodevertex = vertex;
				}
			}
		}

		//��ȡ��ά����
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 2);
		for (auto mesh : meshs)
		{
			int num = 3;
			if (mesh->getMask())
			{
				continue;
			}
			if (mesh->getMeshType() == MeshQuad)
			{
				num = 4;
			}
			for (int i = 0; i < num; ++i)
			{
				MXMeshVertex *vertex = mesh->getVertex(i);
				if (vertex == nullptr)
				{
					continue;
				}
				QVector2D ap1 = WorldvertexToScreenvertex(QVector3D(vertex->vx(), vertex->vy(), vertex->vz()), depth);
				if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
				{
					_nodedepth = depth;
					_picknodevertex = vertex;
				}
			}
		}

		//��ȡһά����
		meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 1);
		for (auto mesh : meshs)
		{
			int num = 2;
			if (mesh->getMask())
			{
				continue;
			}
			if (mesh->getMeshType() != MeshBeam)
			{
				continue;
			}
			for (int i = 0; i < num; ++i)
			{
				MXMeshVertex *vertex = mesh->getVertex(i);
				if (vertex == nullptr)
				{
					continue;
				}
				QVector2D ap1 = WorldvertexToScreenvertex(QVector3D(vertex->vx(), vertex->vy(), vertex->vz()), depth);
				if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
				{
					_nodedepth = depth;
					_picknodevertex = vertex;
				}
			}
		}


		if (_picknodevertex == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickNodeData(_picknodevertex, _nodedepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickAnyMesh(QString partName)
	{
		MXMeshElement* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//��ȡ��Ԫ��
		set<MFace*> mfaces = this->getAllMeshFacesByPartName(partName);
		for (auto mface : mfaces)
		{
			//��ȡ���нڵ�
			QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
			if (mface->type() == 1 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mface->_linkEleMents_3D[0];
				}
			}
		}

		//��ȡ��ά����
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 2);
		for (auto mesh : meshs)
		{
			int num = 3;
			if (mesh->getMeshType() == MeshQuad)
			{
				num = 4;
			}
			QVector<QVector3D> vertexs = mesh->getallVertexs1();
			if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mesh;
				}
			}
		}

		//��ȡһά����
		meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 1);
		for (auto mesh : meshs)
		{
			int num = 2;
			if (mesh->getMeshType() != MeshBeam)
			{
				continue;
			}
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = mesh->getallVertexs1();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
			{
				_meshdepth = *depthlist.begin();
				_pickMesh = mesh;
			}
		}


		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshData(_pickMesh, _meshdepth);
		pickMutex.unlock();

	}
	void mPreMeshPickThread::SoloPickMeshLine(QString partName)
	{
		MEdge* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;

		//��ȡ�߽���
		set<MEdge*> medges = this->getAllMeshLinesByPartName(partName);
		for (auto edge : medges)
		{
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = edge->getAllVertexs();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
			{
				_meshdepth = *depthlist.begin();
				_pickMesh = edge;
			}
		}
		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshLineData(_pickMesh, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFace(QString partName)
	{
		MFace* _pickmeshface = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//��ȡ��Ԫ��
		QVector<MFace*> mfaces = MeshMessage::getInstance()->getSurfaceMeshSamePart(partName);
		for (auto mface : mfaces)
		{
			//��ȡ���нڵ�
			MeshType meshType = MeshTri;
			int num = 3;
			if (meshType == MeshQuad)
			{
				num = 4;
			}
			QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
			if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickmeshface = mface;
				}
			}
		}

		if (_pickmeshface == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshFaceData(_pickmeshface, _meshdepth);
		pickMutex.unlock();
		return;
	}
	void mPreMeshPickThread::SoloPickMeshPart(QString partName)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partName, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshPartData(partName, depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodeByPart(QString partName)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partName, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickNodeByPartData(this->getAllNodesByPartName(partName), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickAnyMeshByPart(QString partName)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partName, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshByPartData(this->getAllMeshsByPartName(partName), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshLineByPart(QString partName)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partName, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshLineByPartData(this->getAllMeshLinesByPartName(partName), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFaceByPart(QString partName)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partName, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshFaceByPartData(this->getAllMeshFacesByPartName(partName), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodeByLineAngle(QString partName)
	{
		SoloPick1DMeshByAngle(partName);
		SoloPickMeshLineByAngle(partName);
	}
	void mPreMeshPickThread::SoloPickNodeByFaceAngle(QString partName)
	{
		SoloPick2DMeshByAngle(partName);
		SoloPickMeshFaceByAngle(partName);
	}
	void mPreMeshPickThread::SoloPick1DMeshByAngle(QString partName)
	{
		MXMeshElement* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;

		//��ȡһά����
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 1);
		for (auto mesh : meshs)
		{
			int num = 2;
			if (mesh->getMask())
			{
				continue;
			}
			if (mesh->getMeshType() != MeshBeam)
			{
				continue;
			}
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = mesh->getallVertexs1();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
			{
				_meshdepth = *depthlist.begin();
				_pickMesh = mesh;
			}
		}


		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshDataByAngle(_pickMesh, partName, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPick2DMeshByAngle(QString partName)
	{
		MXMeshElement* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//��ȡ��ά����
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSameDimAndPart(partName, 2);
		for (auto mesh : meshs)
		{
			int num = 3;			
			if (mesh->getMask())
			{
				continue;
			}
			if (mesh->getMeshType() == MeshQuad)
			{
				num = 4;
			}
			QVector<QVector3D> vertexs = mesh->getallVertexs1();
			if (num == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mesh;
				}
			}
		}

		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshDataByAngle(_pickMesh, partName, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshLineByAngle(QString partName)
	{
		MEdge* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;

		//��ȡ�߽���
		set<MEdge*> medges = this->getAllMeshLinesByPartName(partName);
		for (auto edge : medges)
		{
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = edge->getAllVertexs();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
			{
				_meshdepth = *depthlist.begin();
				_pickMesh = edge;
			}
		}
		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshLineDataByAngle(_pickMesh, partName, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFaceByAngle(QString partName)
	{
		MFace* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//��ȡ��Ԫ��
		set<MFace*> mfaces = this->getAllMeshFacesByPartName(partName);
		for (auto mface : mfaces)
		{
			//��ȡ���нڵ�
			QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
			if (mface->type() == 1 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mface;
				}
			}
		}
		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshFaceDataByAngle(_pickMesh, partName, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickNode(QString partName, bool isAllIn)
	{
		set<MXMeshVertex*> _picknodevertexs;
		if (isAllIn)
		{
			_picknodevertexs = this->getAllNodesByPartName(partName);
		}
		else
		{
			//��ȡ����
			QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
			for (auto mesh : meshs)
			{
				if (mesh->getMask())
				{
					continue;
				}
				int num = mesh->getNumVertices();
				for (int i = 0; i < num; ++i)
				{
					MXMeshVertex *vertex = mesh->getVertex(i);
					if (vertex == nullptr)
					{
						continue;
					}
					QVector3D vertex0(vertex->vx(), vertex->vy(), vertex->vz());
					if (_pick->get2DAnd3DMeshCenterIsInPick(vertex0))
					{
						_picknodevertexs.insert(vertex);
					}
				}
			}
		}

		if (_picknodevertexs.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickNodeData(_picknodevertexs);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickAnyMesh(QString partName, bool isAllIn)
	{
		set<MXMeshElement*> pickmeshs;
		if (isAllIn)
		{
			pickmeshs = this->getAllMeshsByPartName(partName);
		}
		else
		{
			//��ȡ����
			QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
			for (auto mesh : meshs)
			{
				QVector<QVector3D> vertexs = mesh->getallVertexs1();
				if (mesh->getMask())
				{
					continue;
				}
				switch (mesh->getMeshType())
				{
				case MeshBeam:
				{
					if (_pick->get1DMeshIsInPick(vertexs))
					{
						pickmeshs.insert(mesh);
					}
				}
				default:
				{
					if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
					{
						pickmeshs.insert(mesh);
					}
				}
					break;
				}
			}
		}

		if (pickmeshs.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(pickmeshs);
		pickMutex.unlock();

	}
	void mPreMeshPickThread::MultiplyPickMeshTypeFilter(QString partName, QVector<MeshType> filters, bool isAllIn)
	{
		set<MXMeshElement*> pickmeshs;
		if (isAllIn)
		{
			//��ȡ����
			QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
			for (auto mesh : meshs)
			{
				if (mesh->getMask())
				{
					continue;
				}
				if (!filters.contains(mesh->getMeshType()))
				{
					continue;
				}
				pickmeshs.insert(mesh);
			}
		}
		else
		{
			//��ȡ����
			QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
			for (auto mesh : meshs)
			{
				if (mesh->getMask())
				{
					continue;
				}
				if (!filters.contains(mesh->getMeshType()))
				{
					continue;
				}
				QVector<QVector3D> vertexs = mesh->getallVertexs1();
				switch (mesh->getMeshType())
				{
				case MeshBeam:
				{
					if (_pick->get1DMeshIsInPick(vertexs))
					{
						pickmeshs.insert(mesh);
					}
				}
				default:
				{
					if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
					{
						pickmeshs.insert(mesh);
					}
				}
				break;
				}
			}
		}

		if (pickmeshs.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(pickmeshs);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshLine(QString partName, bool isAllIn)
	{
		set<MEdge*> pickmeshlines;
		if (isAllIn)
		{
			pickmeshlines = this->getAllMeshLinesByPartName(partName);
		}
		else
		{
			//��ȡ����
			set<MEdge*> meshlines = this->getAllMeshLinesByPartName(partName);
			for (auto meshline : meshlines)
			{
				QVector<QVector3D> vertexs = meshline->getAllVertexs();
				if (_pick->get1DMeshIsInPick(vertexs))
				{
					pickmeshlines.insert(meshline);
				}
			}
		}

		if (pickmeshlines.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshLineData(pickmeshlines);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshFace(QString partName, bool isAllIn)
	{
		set<MFace*> pickmeshfaces;
		if (isAllIn)
		{
			pickmeshfaces = this->getAllMeshFacesByPartName(partName);
		}
		else
		{
			//��ȡ
			QVector<MXGeoSolid*> geoSolids = MeshMessage::getInstance()->getGeoSolidSamePart(partName);
			for (auto geoSolid : geoSolids)
			{
				QVector<MFace*> mfaces = geoSolid->surfaceMeshs;
				for (auto mface : mfaces)
				{
					if (mface == nullptr)
					{
						continue;
					}
					QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
					if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
					{
						pickmeshfaces.insert(mface);
					}
				}
			}
		}

		if (pickmeshfaces.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshFaceData(pickmeshfaces);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshPart(QString partName, bool isAllIn)
	{
		QString _partName;
		if (isAllIn)
		{
			_partName = partName;
		}
		else
		{
			if (isMultiplyPickMeshPart(partName))
			{
				_partName = partName;
			}
		}
		if (_partName.isEmpty())
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshPartData(partName);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickNodeByPart(QString partName, bool isAllIn)
	{
		QString _partName;
		if (isAllIn)
		{
			_partName = partName;
		}
		else
		{
			if (isMultiplyPickMeshPart(partName))
			{
				_partName = partName;
			}
		}
		if (_partName.isEmpty())
		{
			return;
		}
		set<MXMeshVertex*> picks = this->getAllNodesByPartName(partName);
		pickMutex.lock();
		_pickData->setMultiplyPickNodeData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickAnyMeshByPart(QString partName, bool isAllIn)
	{
		QString _partName;
		if (isAllIn)
		{
			_partName = partName;
		}
		else
		{
			if (isMultiplyPickMeshPart(partName))
			{
				_partName = partName;
			}
		}
		if (_partName.isEmpty())
		{
			return;
		}
		set<MXMeshElement*> picks = this->getAllMeshsByPartName(partName);
		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshLineByPart(QString partName, bool isAllIn)
	{
		QString _partName;
		if (isAllIn)
		{
			_partName = partName;
		}
		else
		{
			if (isMultiplyPickMeshPart(partName))
			{
				_partName = partName;
			}
		}
		if (_partName.isEmpty())
		{
			return;
		}
		set<MEdge*> picks = this->getAllMeshLinesByPartName(partName);
		pickMutex.lock();
		_pickData->setMultiplyPickMeshLineData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshFaceByPart(QString partName, bool isAllIn)
	{
		QString _partName;
		if (isAllIn)
		{
			_partName = partName;
		}
		else
		{
			if (isMultiplyPickMeshPart(partName))
			{
				_partName = partName;
			}
		}
		if (_partName.isEmpty())
		{
			return;
		}
		set<MFace*> picks = this->getAllMeshFacesByPartName(partName);
		pickMutex.lock();
		_pickData->setMultiplyPickMeshFaceData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodePath()
	{
		QVector<MXMeshVertex*> nodeIDs = _pickData->getPickNodeIDsOrder();
		if (nodeIDs.size() < 2)
		{
			return;
		}

		QVector<MXMeshVertex*> res;

		MXMeshVertex* lastNodeID = nodeIDs.takeLast();
		MXMeshVertex* firstNodeID = nodeIDs.last();

		QVector3D lastVertex = lastNodeID->getNodeVertex();

		QVector3D dirction = (lastVertex - firstNodeID->getNodeVertex()).normalized();
		//�ڵ�ID
		QQueue<MXMeshVertex*> queue;
		//�洢���ڵ�Ԫ�ķ���Ķ���
		QQueue<QVector3D> queueDirection;
		queue.enqueue(firstNodeID);
		queueDirection.enqueue(dirction);
		//�ж�ά��
		QVector<MXMeshElement*> test = firstNodeID->_linkElements_2D;
		if (!test.isEmpty())//��ά
		{
			while (!queue.empty())
			{
				MXMeshVertex* nodeData = queue.dequeue();
				QVector3D lastDirection = queueDirection.dequeue();
				QVector<MXMeshElement*> meshIDs = nodeData->_linkElements_2D;
				float maxCosValue = 0.0;
				MXMeshVertex* NodeIDData = nullptr;
				QVector3D dirc;
				for (auto meshData : meshIDs)
				{
					if (meshData == nullptr)
					{
						continue;
					}
					nodeIDs = meshData->lineNodes(nodeData);
					for (auto node : nodeIDs)
					{
						if (node == lastNodeID)
						{
							NodeIDData = lastNodeID;
							break;
						}
						if (res.contains(node))
						{
							break;
						}
						dirction = (node->getNodeVertex() - nodeData->getNodeVertex()).normalized();
						//dirction = (lastVertex - node->getNodeVertex()).normalized();
						float cosValue = QVector3D::dotProduct(dirction, lastDirection);
						if (cosValue >= maxCosValue)
						{
							NodeIDData = node;
							maxCosValue = cosValue;
							dirc = (lastVertex - node->getNodeVertex()).normalized();
						}
					}
					if (NodeIDData == lastNodeID)
					{
						NodeIDData = nullptr;
						break;
					}
				}
				if (NodeIDData != nullptr)
				{
					queue.enqueue(NodeIDData);
					queueDirection.enqueue(dirc);
					res.append(NodeIDData);
				}
			}


		}
		else if(!firstNodeID->linkMFaces.isEmpty())
		{
			while (!queue.empty())
			{
				MXMeshVertex* nodeData = queue.dequeue();
				QVector3D lastDirection = queueDirection.dequeue();
				QVector<MFace*> meshFaceIDs = nodeData->linkMFaces;
				float maxCosValue = 0.0;
				MXMeshVertex* NodeIDData = nullptr;
				QVector3D dirc;
				for (auto meshFaceData : meshFaceIDs)
				{
					if (meshFaceData == nullptr)
					{
						continue;
					}
					nodeIDs = meshFaceData->lineNodes(nodeData);
					for (auto node : nodeIDs)
					{
						if (node == lastNodeID)
						{
							NodeIDData = lastNodeID;
							break;
						}
						if (res.contains(node))
						{
							break;
						}
						dirction = (node->getNodeVertex() - nodeData->getNodeVertex()).normalized();
						//dirction = (lastVertex - node->getNodeVertex()).normalized();
						float cosValue = QVector3D::dotProduct(dirction, lastDirection);
						if (cosValue >= maxCosValue)
						{
							NodeIDData = node;
							maxCosValue = cosValue;
							dirc = (lastVertex - node->getNodeVertex()).normalized();
						}
					}
					if (NodeIDData == lastNodeID)
					{
						NodeIDData = nullptr;
						break;
					}
				}
				if (NodeIDData != nullptr)
				{
					queue.enqueue(NodeIDData);
					queueDirection.enqueue(dirc);
					res.append(NodeIDData);
				}
			}

		}
		res.append(lastNodeID);
		_pickData->setMultiplyPickNodeData(res);

	}
	void mPreMeshPickThread::SoloPickNodeByLineAngle()
	{
		QPair<PickObjectType, QPair<QString, void*>> pickObjectID = _pickData->getSoloPickNodeDataByLineAngle();
		QString partName = pickObjectID.second.first;
		//ʰȡ���Ľڵ�
		std::set<MXMeshVertex*> pickNodeIDs;
		if (pickObjectID.first == PickObjectType::Mesh1D)
		{
			MXMeshElement* meshData = static_cast<MXMeshElement*>(pickObjectID.second.second);
			if (meshData == nullptr)
			{
				return;
			}
			//�жϹ��ĵ�ԪID
			std::set<MXMeshElement*> judgeMeshIDs;
			//�洢���ڵ�ԪID�Ķ���
			QQueue<MXMeshElement*> queue;
			//�洢���ڵ�Ԫ�ķ���Ķ���
			QQueue<QVector3D> queueDirection;
			//����
			QVector3D direction, lastDirection;
			//�Ƕ�ֵ
			float angleValue;

			if (meshData != nullptr)
			{
				QVector<QVector3D> vertexs = meshData->getallVertexs1();

				lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
				//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					//lastDirection = -lastDirection;
				}

				queue.enqueue(meshData);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshIDs.insert(meshData).second)
				{
					continue;
				}

				if (meshData != nullptr)
				{
					QVector<QVector3D> vertexs = meshData->getallVertexs1();
					if (vertexs.size() < 2)
					{
						return;
					}
					direction = (vertexs.at(1) - vertexs.at(0)).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
					{
						int num = meshData->getNumVertices();
						for (int i = 0; i < num; i++)
						{
							pickNodeIDs.insert(meshData->getVertex(i));
						}
						//��Ԫ����
						set<MXMeshElement*> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queue.enqueue(mesh);
							queueDirection.enqueue(direction);
						}
					}
				}

			}
		}
		else
		{
			MEdge* meshLineData = static_cast<MEdge*>(pickObjectID.second.second);
			if (meshLineData == nullptr)
			{
				return;
			}

			//�жϹ��ĵ�Ԫ��ID
			std::set<MEdge*> judgeMeshLineIDs;
			//�洢���ڵ�Ԫ��ID�Ķ���
			QQueue<MEdge*> queue;
			//�洢���ڵ�Ԫ�ߵķ���Ķ���
			QQueue<QVector3D> queueDirection;
			//����
			QVector3D direction, lastDirection;
			//�Ƕ�ֵ
			float angleValue;

			if (meshLineData != nullptr)
			{
				QVector<QVector3D> vertexs = meshLineData->getAllVertexs();

				lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
				//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					//lastDirection = -lastDirection;
				}

				queue.enqueue(meshLineData);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshLineData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshLineIDs.insert(meshLineData).second)
				{
					continue;
				}

				if (meshLineData != nullptr)
				{
					QVector<QVector3D> vertexs = meshLineData->getAllVertexs();
					if (vertexs.size() < 2)
					{
						return;
					}
					direction = (vertexs.at(1) - vertexs.at(0)).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
					{
						for (int i = 0; i < 2; i++)
						{
							pickNodeIDs.insert(meshLineData->getVerOfMEdge(i));
						}
						//��Ԫ��
						set<MEdge*> adjacentMeshFaceIDs = meshLineData->linkMEdges();
						for (auto meshline : adjacentMeshFaceIDs)
						{
							queue.enqueue(meshline);
							queueDirection.enqueue(direction);
						}
					}

				}

			}
		}
		_pickData->setMultiplyPickNodeData(pickNodeIDs);

	}
	void mPreMeshPickThread::SoloPickNodeByFaceAngle()
	{
		QPair<PickObjectType, QPair<QString, void*>> pickObjectID = _pickData->getSoloPickNodeDataByFaceAngle();
		QString partName = pickObjectID.second.first;
		//ʰȡ���Ľڵ�
		std::set<MXMeshVertex*> pickNodeIDs;
		if (pickObjectID.first == PickObjectType::Mesh2D)
		{
			MXMeshElement* meshData = static_cast<MXMeshElement*>(pickObjectID.second.second);
			if (meshData == nullptr)
			{
				return;
			}
			//�жϹ��ĵ�ԪID
			std::set<MXMeshElement*> judgeMeshIDs;
			//�洢���ڵ�ԪID�Ķ���
			QQueue<MXMeshElement*> queue;
			//�洢���ڵ�Ԫ�ķ���Ķ���
			QQueue<QVector3D> queueDirection;
			//����
			QVector3D direction, lastDirection;
			//�Ƕ�ֵ
			float angleValue;

			if (meshData->getMeshType() == MeshQuad || meshData->getMeshType() == MeshTri)
			{
				QVector<QVector3D> vertexs = meshData->getallVertexs1();
				if (vertexs.size() < 3)
				{
					return;
				}
				lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				queue.enqueue(meshData);
				queueDirection.enqueue(lastDirection);
			}


			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshIDs.insert(meshData).second)
				{
					continue;
				}

				if (meshData != nullptr/* && meshData->getMeshVisual()*/)
				{
					QVector<QVector3D> vertexs = meshData->getallVertexs1();
					if (vertexs.size() < 3)
					{
						return;
					}
					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{
						int n = meshData->getNumVertices();
						for (int i = 0; i < n; i++)
						{
							pickNodeIDs.insert(meshData->getVertex(i));
						}

						set<MXMeshElement*> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(mesh);
						}
					}
				}

			}
		}
		else
		{
			MFace* meshFaceData = static_cast<MFace*>(pickObjectID.second.second);
			if (meshFaceData == nullptr)
			{
				return;
			}	

			//�жϹ��ĵ�Ԫ��ID
			std::set<MFace*> judgeMeshFaceIDs;
			//�洢���ڵ�Ԫ��ID�Ķ���
			QQueue<MFace*> queue;
			//�洢���ڵ�Ԫ�ķ���Ķ���
			QQueue<QVector3D> queueDirection;
			//����
			QVector3D direction, lastDirection;
			//�Ƕ�ֵ
			float angleValue;

			if (meshFaceData != nullptr)
			{
				QVector<QVector3D> vertexs = meshFaceData->getAllVertexsOfMFace();

				lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					//lastDirection = -lastDirection;
				}

				queue.enqueue(meshFaceData);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshFaceData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshFaceIDs.insert(meshFaceData).second)
				{
					continue;
				}

				if (meshFaceData != nullptr)
				{
					QVector<QVector3D> vertexs = meshFaceData->getAllVertexsOfMFace();
					if (vertexs.size() < 3)
					{
						return;
					}
					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{
						int n = meshFaceData->type() == 1 ? 3 : 4;
						for (int i = 0; i < n; i++)
						{
							pickNodeIDs.insert(meshFaceData->getVertex(i));
						}
						set<MFace*> adjacentMeshFaceIDs = meshFaceData->linkMFaces();
						for (auto meshface : adjacentMeshFaceIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(meshface);
						}
					}

				}

			}
		}

		pickMutex.lock();
		_pickData->setMultiplyPickNodeData(pickNodeIDs);
		pickMutex.unlock();

	}
	void mPreMeshPickThread::SoloPick1DMeshByAngle()
	{
		QPair<QString, void*> partNameMesh = _pickData->getSoloPickMeshLineDataByAngle();
		MXMeshElement* meshData = static_cast<MXMeshElement*>(partNameMesh.second);
		if (meshData == 0)
		{
			return;
		}

		//ʰȡ���ĵ�ԪID
		std::set<MXMeshElement*> pickMeshIDs;
		//�жϹ��ĵ�ԪID
		std::set<MXMeshElement*> judgeMeshIDs;
		//�洢���ڵ�ԪID�Ķ���
		QQueue<MXMeshElement*> queue;
		//�洢���ڵ�Ԫ�ķ���Ķ���
		QQueue<QVector3D> queueDirection;
		//����
		QVector3D direction, lastDirection;
		//�Ƕ�ֵ
		float angleValue;

		if (meshData != nullptr)
		{
			QVector<QVector3D> vertexs = meshData->getallVertexs1();

			lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				//lastDirection = -lastDirection;
			}

			queue.enqueue(meshData);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshIDs.insert(meshData).second)
			{
				continue;
			}

			if (meshData != nullptr)
			{
				QVector<QVector3D> vertexs = meshData->getallVertexs1();
				if (vertexs.size() < 2)
				{
					return;
				}
				direction = (vertexs.at(1) - vertexs.at(0)).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
				{
					if (pickMeshIDs.insert(meshData).second)
					{
						//��Ԫ����
						set<MXMeshElement*> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queue.enqueue(mesh);
							queueDirection.enqueue(direction);
						}
					}
				}

			}

		}


		_pickData->setMultiplyPickMeshData(pickMeshIDs);
	}
	void mPreMeshPickThread::SoloPickMeshLineByAngle()
	{
		QPair<QString, void*> partNameMeshLine = _pickData->getSoloPickMeshLineDataByAngle();
		MEdge* meshLineData = static_cast<MEdge*>(partNameMeshLine.second);
		if (meshLineData == 0)
		{
			return;
		}

		//ʰȡ���ĵ�Ԫ��ID
		std::set<MEdge*> pickMeshLineIDs;
		//�жϹ��ĵ�Ԫ��ID
		std::set<MEdge*> judgeMeshLineIDs;
		//�洢���ڵ�Ԫ��ID�Ķ���
		QQueue<MEdge*> queue;
		//�洢���ڵ�Ԫ�ķ���Ķ���
		QQueue<QVector3D> queueDirection;
		//����
		QVector3D direction, lastDirection;
		//�Ƕ�ֵ
		float angleValue;

		if (meshLineData != nullptr)
		{
			QVector<QVector3D> vertexs = meshLineData->getAllVertexs();

			lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				//lastDirection = -lastDirection;
			}

			queue.enqueue(meshLineData);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshLineData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshLineIDs.insert(meshLineData).second)
			{
				continue;
			}

			if (meshLineData != nullptr)
			{
				QVector<QVector3D> vertexs = meshLineData->getAllVertexs();
				if (vertexs.size() < 2)
				{
					return;
				}
				direction = (vertexs.at(1) - vertexs.at(0)).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
				{
					if (pickMeshLineIDs.insert(meshLineData).second)
					{						
						//��Ԫ����
						set<MEdge*> adjacentMeshFaceIDs = meshLineData->linkMEdges();
						for (auto meshline : adjacentMeshFaceIDs)
						{
							queue.enqueue(meshline);
							queueDirection.enqueue(direction);
						}
					}
				}

			}

		}


		_pickData->setMultiplyPickMeshLineData(pickMeshLineIDs);
	}
	void mPreMeshPickThread::SoloPick2DMeshByAngle()
	{
		QPair<QString, void*> partNameMeshID = _pickData->getSoloPickMeshDataByAngle();
		MXMeshElement* meshData = static_cast<MXMeshElement*>(partNameMeshID.second);
		if (meshData == nullptr)
		{
			return;
		}

		//ʰȡ���ĵ�ԪID
		std::set<MXMeshElement*> pickMeshIDs;
		//�жϹ��ĵ�ԪID
		std::set<MXMeshElement*> judgeMeshIDs;
		//�洢���ڵ�ԪID�Ķ���
		QQueue<MXMeshElement*> queue;
		//�洢���ڵ�Ԫ�ķ���Ķ���
		QQueue<QVector3D> queueDirection;
		//����
		QVector3D direction, lastDirection;
		//�Ƕ�ֵ
		float angleValue;

		if (meshData->getMeshType() == MeshQuad || meshData->getMeshType() == MeshTri)
		{
			QVector<QVector3D> vertexs = meshData->getallVertexs1();
			if (vertexs.size() < 3)
			{
				return;
			}
			lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
			queue.enqueue(meshData);
			queueDirection.enqueue(lastDirection);
		}
		

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshIDs.insert(meshData).second)
			{
				continue;
			}

			if (meshData != nullptr/* && meshData->getMeshVisual()*/)
			{
				QVector<QVector3D> vertexs = meshData->getallVertexs1();
				if (vertexs.size() < 3)
				{
					return;
				}
				direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue)
				{
					if (pickMeshIDs.insert(meshData).second)
					{
						set<MXMeshElement*> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(mesh);
						}
					}
				}
			}

		}

		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(pickMeshIDs);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFaceByAngle()
	{
		QPair<QString, void*> partNameMeshFace = _pickData->getSoloPickMeshFaceDataByAngle();
		MFace* meshFaceData = static_cast<MFace*>(partNameMeshFace.second);
		if (meshFaceData == 0)
		{
			return;
		}

		//ʰȡ���ĵ�Ԫ��ID
		std::set<MFace*> pickMeshFaceIDs;
		//�жϹ��ĵ�Ԫ��ID
		std::set<MFace*> judgeMeshFaceIDs;
		//�洢���ڵ�Ԫ��ID�Ķ���
		QQueue<MFace*> queue;
		//�洢���ڵ�Ԫ�ķ���Ķ���
		QQueue<QVector3D> queueDirection;
		//����
		QVector3D direction, lastDirection;
		//�Ƕ�ֵ
		float angleValue;

		if (meshFaceData != nullptr)
		{
			QVector<QVector3D> vertexs = meshFaceData->getAllVertexsOfMFace();

			lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				//lastDirection = -lastDirection;
			}

			queue.enqueue(meshFaceData);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshFaceData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshFaceIDs.insert(meshFaceData).second)
			{
				continue;
			}

			if (meshFaceData != nullptr)
			{
				QVector<QVector3D> vertexs = meshFaceData->getAllVertexsOfMFace();
				if (vertexs.size() < 3)
				{
					return;
				}
				direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue)
				{
					if (pickMeshFaceIDs.insert(meshFaceData).second)
					{
						set<MFace*> adjacentMeshFaceIDs = meshFaceData->linkMFaces();
						for (auto meshface : adjacentMeshFaceIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(meshface);
						}
					}						
				}
				
			}

		}


		_pickData->setMultiplyPickMeshFaceData(pickMeshFaceIDs);
	}
	bool mPreMeshPickThread::isSoloPickMeshPart(QString partName, float & depth)
	{
		float uv[2];
		float t;
		//��ȡ��Ԫ��
		QVector<MXGeoSolid*> geoSolids = MeshMessage::getInstance()->getGeoSolidSamePart(partName);
		for (auto geoSolid : geoSolids)
		{
			QVector<MFace*> mfaces = geoSolid->surfaceMeshs;
			for (auto mface : mfaces)
			{
				if (mface == nullptr)
				{
					continue;
				}
				QVector<QVector3D> vertexs = mface->getAllVertexsOfMFace();
				if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < depth)
					{
						depth = t;
					}
				}
			}
		}

		//��ȡ����
		QVector<MXMeshElement*> meshs = MeshMessage::getInstance()->getElementsSamePart(partName);
		for (auto mesh : meshs)
		{
			QVector<QVector3D> vertexs = mesh->getallVertexs1();
			switch (mesh->getMeshType())
			{
			case MeshBeam:
			{
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs;
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < depth)
				{
					depth = *depthlist.begin();
				}
			}
				break;
			case MeshTri:
			case MeshQuad:
			{
				QVector<QVector3D> vertexs = mesh->getallVertexs1();
				if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < depth)
					{
						depth = t;
					}
				}
			}
				break;
			default:
				break;
			}

		}

		if (depth != FLT_MAX)
		{
			return true;
		}
		return false;
	}
}