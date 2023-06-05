#pragma once
//�����������
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

		virtual bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) = 0;//0ά��2ά��3ά

		virtual bool get1DMeshIsInPick(QVector<QVector3D> vertexs) = 0;//1ά

		virtual bool isIntersectionAABBAndPick(QVector<QVector2D> ap) = 0;//�ж��Ƿ��ཻ

		virtual bool isAABBPointIsAllInPick(QVector<QVector2D> ap) = 0;//�жϵ��Ƿ�ȫ��

		//��Space::AABB�İ˸�����ת��Ϊ��ά��Ļ�ĵ�
		QVector<QVector2D> getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge);
		//����Ļ����ת��Ϊ��������
		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		//����������ת��Ϊ��Ļ����
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);
		QVector<QVector2D> WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs);
		//����������ת��Ϊ��Ļ���겢�ҷ����������ֵ
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

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0ά��2ά��3ά

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1ά

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//�жϵ��Ƿ�ȫ��

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

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0ά��2ά��3ά

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1ά

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//�жϵ��Ƿ�ȫ��

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
			QVector3D Point = ScreenvertexToWorldvertex(p2);//���Բ��һ�������
			_centerPoint = ScreenvertexToWorldvertex(_screenCenter);//���Բ������
			_radius = _centerPoint.distanceToPoint(Point);
			_screenRadius = _screenCenter.distanceToPoint(p1);
			_centerDirection = centerDirection;
		};

		bool getPickIsIntersectionWithAABB(Space::AABB aabb) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0ά��2ά��3ά

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1ά

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//�жϵ��Ƿ�ȫ��

	protected:
		QVector3D _centerPoint; QVector3D _centerDirection; double _radius; //�ռ�
		QVector2D _screenCenter; double _screenRadius;//��Ļ
	};
	class RENDVIEW_EXPORT mPreMeshPickThread : public QObject
	{
		Q_OBJECT

	public:
		/*
		* ���ƹ��캯��
		*/
		mPreMeshPickThread(mPreMeshPickData1 *pickData);

		/*
		* ��������
		*/
		~mPreMeshPickThread();

		/*
		* ���õ�ǰ��ʰȡģʽ
		*/
		void setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode);

		/*
		* ���õ�ǰ��ʰȡ������
		*/
		void setPickFilter(PickFilter *pickFilter);

		/*
		 * ��ʼʰȡ
		 */
		void startPick();

		/*
		 * ���þ���
		 */
		void setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);

		/*
		* ���þ���
		*/
		void setMatrix(QMatrix4x4 pvm);

		/*
		* ����ͨ���Ƕ�ʰȡ�ĽǶ�
		*/
		void setPickAngleValue(double angle) { _pickAngleValue = angle; };

		/*
		 * ���õ�ѡλ��
		 */
		void setLocation(const QVector2D & pos, float depth);

		/*
		* ����λ��(���λ��߶���ο�ѡ)
		*/
		void setLocation(QVector<QVector2D> pickQuad, QVector3D direction = QVector3D());

		/*
		 * ���ô��ڴ�С
		 */
		void setWidget(int w, int h);

		/*
		 *	�Ƿ����ʰȡ
		 */
		bool isFinished();

		/*
		 * ʰȡ�������Ϊfalse
		 */
		void setFinished();
	private:
		//ʰȡ������		
		void doSoloPick(QString partName);
		void doMultiplyPick(QString partName);
		void doAnglePick();

		/*
		 * ��ѡ
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

		//��ѡʰȡ
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

		//ͨ���Ƕ�ʰȡ
		void SoloPickNodeByLineAngle();
		void SoloPickNodeByFaceAngle();
		void SoloPick1DMeshByAngle();
		void SoloPickMeshLineByAngle();
		void SoloPick2DMeshByAngle();
		void SoloPickMeshFaceByAngle();

		/*************��ѡ***********/
		//�жϵ�ѡ�Ƿ�ʰȡ������

		//�жϵ�ѡ�Ƿ�ʰȡ���ò���
		bool isSoloPickMeshPart(QString partName, float &depth);


		/***********��ѡ*********/
		//�ж��Ƿ�ʰȡ���ü��ε�
		bool IsMultiplyPickGeoPoint(MXGeoPoint* geoPointData);

		//�ж��Ƿ�ʰȡ���ü�����
		bool IsMultiplyPickGeoLine(MXGeoEdge* geoLineData);

		//�ж��Ƿ�ʰȡ���ü�����
		bool IsMultiplyPickGeoFace(MXGeoFace* geoFaceData);

		//�ж��Ƿ�ʰȡ���ü�����
		bool IsMultiplyPickGeoSolid(MXGeoSolid* geoSolidData);

		//�жϿ�ѡ�Ƿ�ʰȡ���ò���
		bool isMultiplyPickMeshPart(QString partName);


		//����������ת��Ϊ��Ļ����
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);

		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);

		//����������ת��Ϊ��Ļ���겢�ҷ����������ֵ
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);

		/*
		 * ����������ת��Ϊ��Ļ���겢�ҷ����������ֵ
		 */
		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);

		//��ȡ����
		QVector3D getCenter(QVector<QVector3D> vertexs);

		//��ȡһ��������������Ϣ
		set<MXMeshVertex*> getAllNodesByPartName(QString partName);
		set<MXMeshElement*> getAllMeshsByPartName(QString partName);
		set<MFace*> getAllMeshFacesByPartName(QString partName);
		set<MEdge*> getAllMeshLinesByPartName(QString partName);

	signals:
		/*
		 * �����ź����
		 */
		void finishedPickSig();


	public slots:


	private:

		//ʰȡ������
		PickFilter *_pickFilter;

		//ʰȡģʽ
		MViewBasic::PickMode _pickMode;

		//��ѡʰȡģʽ
		MViewBasic::MultiplyPickMode _multiplyPickMode;

		//ʰȡ�Ƕ�
		float _pickAngleValue = 60;

		//���ڴ�С
		int _Win_WIDTH, _Win_HEIGHT;

		//��ɫ����ͼ����
		QMatrix4x4 _projection, _view, _model, _pvm;

		/*
		* ʰȡ����
		*/
		mPreMeshPickData1 *_pickData;

		/*
		 * ��ѡλ��
		 */
		QVector2D _pos;
		float _depth;
		QVector3D _p;
		QVector3D _origin;
		QVector3D _dir;

		/*********/
		std::shared_ptr<mBasePick> _pick;
		/*
		 * ��ѡ����
		 */
		QVector<QVector2D> soloQuad;

		/*
		 * �Ƿ����
		 */
		bool _isfinished;


	};
}
