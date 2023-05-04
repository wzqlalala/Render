#pragma once
#include "rendbasic_global.h"

#include "mBasicEnum.h"

#include <QObject>

namespace MViewBasic
{
	class RENDBASIC_EXPORT mPickToolClass : public QObject
	{
	public:
		static QHash<MBasicFunction::MeshType, QVector<int>> LineIntersectionWithQuadVector;

		static QHash<MBasicFunction::MeshType, QVector<QVector<int>>> QuadPointInMeshVector;

		/*
		 * �ж�һ���߶κ�һ������ѡ����Ƿ��ཻ
		 */
		static bool IsLineIntersectionWithQuad(const QVector<QVector2D> &Line1, const QVector<QVector2D> &Quad, MBasicFunction::MeshType meshtype);

		/*
		* �ж�һ���߶κ�һ��Բ��ѡ����Ƿ��ཻ
		*/
		static bool IsLineIntersectionWithRound(QVector2D p1, QVector2D p2, QVector2D center, float radius);

		/*
		 * �жϵ��Ƿ��������ڲ�
		 */
		static bool IsPointInMesh(const QVector2D & pos, const QVector<QVector2D> &Line1, MBasicFunction::MeshType meshtype);

		/*
		 * �ж�����ĵ��Ƿ���ʰȡ���ο��ڲ�
		 */
		static bool IsMeshPointInQuad(const QVector<QVector2D> &Line1, int centerX, int centerY, int boxW, int boxH);

		/*
		 * �жϵ��Ƿ���ʰȡ���ο��ڲ�
		 */
		static bool IsPointInQuad(QVector2D point, QVector2D boxCenter, QVector2D boxXY_2);
		static bool IsPointInQuad(const QVector<QVector2D> &Line1, QVector2D boxCenter, QVector2D boxXY_2);//�������ڲ�����
		static bool IsAllPointInQuad(const QVector<QVector2D> &Line1, QVector2D boxCenter, QVector2D boxXY_2);//ȫ�����ڲ�

		/*
		 * �жϵ��Ƿ���ʰȡ����ο��ڲ�
		 */
		static bool IsPointInPolygon(QVector2D point, QVector2D boxCenter, const QVector<QVector2D> &polygons);
		static bool IsPointInPolygon(const QVector<QVector2D> &Line1, QVector2D boxCenter, const QVector<QVector2D> &polygons);//�������ڲ�����
		static bool IsAllPointInPolygon(const QVector<QVector2D> &Line1, QVector2D boxCenter, const QVector<QVector2D> &polygons);//ȫ�����ڲ�

		/*
		* �ж�����Ķ�����Ƿ��Բ��ѡ����ཻ
		*/
		static bool IsTriIntersectionWithCircle(const QVector<QVector2D> &Line1, QVector2D circleCenter, double radius);

		/*�жϰ�Χ���Ƿ���Բ��ѡ����ཻ*/
		static bool IsAABBIntersectionWithCircle(const QVector<QVector2D> &Line1, QVector2D circleCenter, double radius);

		/*
		* �жϵ��Ƿ�ȫ��Բ���ڲ�
		*/
		static bool IsPointInRound(QVector3D point, QVector3D center, QVector3D direction, float r);
		static bool IsPointInRound(QVector2D point, QVector2D center, float r);
		static bool IsPointInRound(const QVector<QVector2D> &Line1, QVector2D center, float r);//�������ڲ�����
		static bool IsAllPointInRound(const QVector<QVector2D> &Line1, QVector2D center, float r);//ȫ�����ڲ�

		/*
		 * �ж���������ĺ͵�ѡ���ο��λ���Ƿ�����ʰȡҪ��
		 */
		static bool IsMeshCenterInPickQuad(const QPoint & pos, const QVector<QVector2D> &Line1);

		/*
		 * �����������ڲ�������ֵ
		 */
		static float CaculatePointInTriDepth(QVector2D ap1, QVector2D ap2, QVector2D ap3, QPoint pos, float depth1, float depth2, float depth3);

		/*
		 * �������ڲ�������ֵ
		 */
		static float CaculatePointInLineDepth(QVector2D ap1, QVector2D ap2, QVector2D pos, float depth1, float depth2);

		//�ж��������������Ƿ��ཻ
		static bool rayTriangleIntersect(const QVector3D & orig, const QVector3D & dir, const QVector<QVector3D>& tri, float uv[2], float & t);

		//�ж��������ı����Ƿ��ཻ
		static bool rayQuadIntersect(const QVector3D & orig, const QVector3D & dir, const QVector<QVector3D>& tri, float uv[2], float & t);
	};
}
