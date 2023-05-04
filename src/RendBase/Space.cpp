#include "Space.h"
namespace mxr
{
	AABB::AABB(QVector3D p1, QVector3D p2)
	{
		maxEdge.setX(std::max(p1.x(), p2.x()));
		maxEdge.setY(std::max(p1.y(), p2.y()));
		maxEdge.setZ(std::max(p1.z(), p2.z()));
		minEdge.setX(std::min(p1.x(), p2.x()));
		minEdge.setY(std::min(p1.y(), p2.y()));
		minEdge.setZ(std::min(p1.z(), p2.z()));
	}

	void AABB::init(QVector3D p)
	{
		maxEdge = minEdge = p;
	}

	AABB::AABB()
	{
		maxEdge = QVector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		minEdge = QVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	bool AABB::ContainPoint(QVector3D p1)
	{
		if (minEdge.x() < p1.x() && minEdge.y() < p1.y() && minEdge.z() < p1.z() &&
			maxEdge.x() > p1.x() && maxEdge.y() > p1.y() && maxEdge.z() > p1.z())
		{
			return true;
		}
		return false;
	}

	void AABB::push(AABB aabb)
	{
		minEdge.setX(std::min(aabb.minEdge.x(), minEdge.x()));
		minEdge.setY(std::min(aabb.minEdge.y(), minEdge.y()));
		minEdge.setZ(std::min(aabb.minEdge.z(), minEdge.z()));

		maxEdge.setX(std::max(aabb.maxEdge.x(), maxEdge.x()));
		maxEdge.setY(std::max(aabb.maxEdge.y(), maxEdge.y()));
		maxEdge.setZ(std::max(aabb.maxEdge.z(), maxEdge.z()));
	}

	void AABB::push(QVector3D p1, QVector3D p2)
	{
		minEdge.setX(std::min(p1.x(), minEdge.x()));
		minEdge.setY(std::min(p1.y(), minEdge.y()));
		minEdge.setZ(std::min(p1.z(), minEdge.z()));

		maxEdge.setX(std::max(p2.x(), maxEdge.x()));
		maxEdge.setY(std::max(p2.y(), maxEdge.y()));
		maxEdge.setZ(std::max(p2.z(), maxEdge.z()));
	}



	void AABB::push(QVector3D p1)
	{
		maxEdge.setX(std::max(p1.x(), maxEdge.x()));
		maxEdge.setY(std::max(p1.y(), maxEdge.y()));
		maxEdge.setZ(std::max(p1.z(), maxEdge.z()));
		minEdge.setX(std::min(p1.x(), minEdge.x()));
		minEdge.setY(std::min(p1.y(), minEdge.y()));
		minEdge.setZ(std::min(p1.z(), minEdge.z()));
	}

	bool AABB::empty()
	{
		if (maxEdge.x() == minEdge.x() && maxEdge.y() == minEdge.y() && maxEdge.z() == minEdge.z())
		{
			return true;
		}
		return false;
	}


	bool AABB::intersects(QVector3D origin, QVector3D dir, QVector3D &ptOnPlane)
	{
		//�������Χ��ĳ��Ľ���
		QVector3D min = minEdge; //aabb��Χ����С������
		QVector3D max = maxEdge; //aabb��Χ����������

		float t;

		//�ֱ��ж������������ཻ���

		//�ж��������Χ��x�᷽������Ƿ��н���
		if (dir.x() != 0.f) //����x�᷽�������Ϊ0 �����߷���ʸ����x�����Ϊ0�����߲����ܾ�����Χ�г�x�᷽���������
		{
			/*
			  ʹ��������ƽ���ཻ�Ĺ�ʽ�󽻵�
			 */
			if (dir.x() > 0)//��������x��������ƫ��
				t = (min.x() - origin.x()) / dir.x();
			else  //������x�Ḻ����ƫ��
				t = (max.x() - origin.x()) / dir.x();

			if (t > 0.f) //t>0ʱ��������ƽ���ཻ
			{
				ptOnPlane = origin + t * dir; //���㽻������
				//�жϽ����Ƿ��ڵ�ǰ����
				if (min.y() < ptOnPlane.y() && ptOnPlane.y() < max.y() && min.z() < ptOnPlane.z() && ptOnPlane.z() < max.z())
				{
					return true; //�������Χ���н���
				}
			}
		}

		//��������y�᷽���з��� �ж��Ƿ����Χ��y�᷽���н���
		if (dir.y() != 0.f)
		{
			if (dir.y() > 0)
				t = (min.y() - origin.y()) / dir.y();
			else
				t = (max.y() - origin.y()) / dir.y();

			if (t > 0.f)
			{
				ptOnPlane = origin + t * dir;

				if (min.z() < ptOnPlane.z() && ptOnPlane.z() < max.z() && min.x() < ptOnPlane.x() && ptOnPlane.x() < max.x())
				{
					return true;
				}
			}
		}

		//��������z�᷽���з��� �ж��Ƿ����Χ��y�᷽���н���
		if (dir.z() != 0.f)
		{
			if (dir.z() > 0)
				t = (min.z() - origin.z()) / dir.z();
			else
				t = (max.z() - origin.z()) / dir.z();

			if (t > 0.f)
			{
				ptOnPlane = origin + t * dir;

				if (min.x() < ptOnPlane.x() && ptOnPlane.x() < max.x() && min.y() < ptOnPlane.y() && ptOnPlane.y() < max.y())
				{
					return true;
				}
			}
		}

		return false;
	}

	bool AABB::isContain(AABB aabb)
	{
		if (minEdge.x() < aabb.minEdge.x() && minEdge.y() < aabb.minEdge.y() && minEdge.z() < aabb.minEdge.z()
			&& maxEdge.x() > aabb.maxEdge.x() && maxEdge.y() > aabb.maxEdge.y() && maxEdge.z() > aabb.maxEdge.z()
			)
		{
			return true;
		}
		return false;
	}

	bool AABB::IsIntersect(AABB aabb)
	{
		if (maxEdge.x() < aabb.minEdge.x() || minEdge.x() > aabb.maxEdge.x())
		{
			return false;
		}
		if (maxEdge.y() < aabb.minEdge.y() || minEdge.y() > aabb.maxEdge.y())
		{
			return false;
		}
		if (maxEdge.z() < aabb.minEdge.z() || minEdge.z() > aabb.maxEdge.z())
		{
			return false;
		}
		return true;
	}
}

