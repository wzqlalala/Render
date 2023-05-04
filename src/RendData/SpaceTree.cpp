#include "SpaceTree.h"
#include "mPostMeshNodeData1.h"
#include "mPostMeshData1.h"
#include "mOneFrameData1.h"
#include <qdebug.h>
namespace Space
{
	AABB::AABB(QVector3D p)
	{
		maxEdge = minEdge = p;
	}

	AABB::AABB(QVector3D p1, QVector3D p2)
	{
		maxEdge.setX( std::max(p1.x(), p2.x()) );
		maxEdge.setY( std::max(p1.y(), p2.y()) );
		maxEdge.setZ( std::max(p1.z(), p2.z()) );
		minEdge.setX( std::min(p1.x(), p2.x()) );
		minEdge.setY( std::min(p1.y(), p2.y()) );
		minEdge.setZ( std::min(p1.z(), p2.z()) );
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
		AABB aabb(this->minEdge, this->maxEdge);
		float longestEdge = aabb.getLongestEdge();
		float Edge = aabb.maxEdge.x() - aabb.minEdge.x();
		if ((longestEdge / Edge)>1000)
		{
			aabb.maxEdge[0] += longestEdge * 1e-2;
			aabb.minEdge[0] -= longestEdge * 1e-2;
		}
		else
		{
			aabb.maxEdge[0] += Edge * 1e-2;
			aabb.minEdge[0] -= Edge * 1e-2;
		}
		Edge = aabb.maxEdge.y() - aabb.minEdge.y();
		if ((longestEdge / Edge) > 1000)
		{
			aabb.maxEdge[1] += longestEdge * 1e-2;
			aabb.minEdge[1] -= longestEdge * 1e-2;
		}
		else
		{
			aabb.maxEdge[1] += Edge * 1e-2;
			aabb.minEdge[1] -= Edge * 1e-2;
		}
		Edge = aabb.maxEdge.z() - aabb.minEdge.z();
		if ((longestEdge / Edge) > 1000)
		{
			aabb.maxEdge[2] += longestEdge * 1e-2;
			aabb.minEdge[2] -= longestEdge * 1e-2;
		}
		else
		{
			aabb.maxEdge[2] += Edge * 1e-2;
			aabb.minEdge[2] -= Edge * 1e-2;
		}
		if (aabb.minEdge.x() < p1.x() && aabb.minEdge.y() < p1.y() && aabb.minEdge.z() < p1.z() &&
			aabb.maxEdge.x() > p1.x() && aabb.maxEdge.y() > p1.y() && aabb.maxEdge.z() > p1.z())
		{
			return true;
		}
		return false;
		/*if (!qFuzzyCompare(aabb.maxEdge.x(), aabb.minEdge.x()))
		{
			float f = (aabb.maxEdge.x() - aabb.minEdge.x()) * 1e-2;
			aabb.maxEdge[0] += f;
			aabb.minEdge[0] -= f;
		}
		else
		{
			aabb.maxEdge[0] += 0.1;
			aabb.minEdge[0] -= 0.1;
		}
		if (!qFuzzyCompare(aabb.maxEdge.y(), aabb.minEdge.y()))
		{
			float f = (aabb.maxEdge.y() - aabb.minEdge.y()) * 1e-2;
			aabb.maxEdge[1] += f;
			aabb.minEdge[1] -= f;
		}
		else
		{
			aabb.maxEdge[1] += 0.1;
			aabb.minEdge[1] -= 0.1;
		}
		if (!qFuzzyCompare(aabb.maxEdge.z(), aabb.minEdge.z()))
		{
			float f = (aabb.maxEdge.z() - aabb.minEdge.z()) * 1e-2;
			aabb.maxEdge[2] += f;
			aabb.minEdge[2] -= f;
		}
		else
		{
			aabb.maxEdge[2] += 0.1;
			aabb.minEdge[2] -= 0.1;
		}
		if (worldVertex.x() > aabb.maxEdge.x() || worldVertex.x() < aabb.minEdge.x()
			|| worldVertex.y() > aabb.maxEdge.y() || worldVertex.y() < aabb.minEdge.y()
			|| worldVertex.z() > aabb.maxEdge.z() || worldVertex.z() < aabb.minEdge.z())
		{
			return;
		}
		if (minEdge.x() < p1.x() && minEdge.y() < p1.y() && minEdge.z() < p1.z() &&
			maxEdge.x() > p1.x() && maxEdge.y() > p1.y() && maxEdge.z() > p1.z())
		{
			return true;
		}*/
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
		maxEdge.setX( std::max(p1.x(), maxEdge.x()) );
		maxEdge.setY( std::max(p1.y(), maxEdge.y()) );
		maxEdge.setZ( std::max(p1.z(), maxEdge.z()) );
		minEdge.setX( std::min(p1.x(), minEdge.x()) );
		minEdge.setY( std::min(p1.y(), minEdge.y()) );
		minEdge.setZ( std::min(p1.z(), minEdge.z()) );
	}

	void AABB::push(QVector<QVector3D> p1)
	{
		for (auto p : p1)
		{
			this->push(p);
		}
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

	float AABB::getLongestEdge()
	{
		return max(maxEdge.x() - minEdge.x(), max(maxEdge.y() - minEdge.y(), maxEdge.z() - minEdge.z()));
	}

	SpaceTree::SpaceTree(AABB aabb)
	{
		space = aabb;
	}

	void initSpaceTree(SpaceTree* root, int depth)
	{
		root->depth = depth;
		if (depth > 0)
		{			
			SpaceTree* space1 = new SpaceTree(getSpace(root->space, TopFrontLeft));
			SpaceTree* space2 = new SpaceTree(getSpace(root->space, TopFrontRight));
			SpaceTree* space3 = new SpaceTree(getSpace(root->space, TopBackLeft));
			SpaceTree* space4 = new SpaceTree(getSpace(root->space, TopBackRight));
			SpaceTree* space5 = new SpaceTree(getSpace(root->space, BottomFrontLeft));
			SpaceTree* space6 = new SpaceTree(getSpace(root->space, BottomFrontRight));
			SpaceTree* space7 = new SpaceTree(getSpace(root->space, BottomBackLeft));
			SpaceTree* space8 = new SpaceTree(getSpace(root->space, BottomBackRight));
			root->topFrontLeft = space1;
			root->topFrontRight = space2;
			root->topBackLeft = space3;
			root->topBackRight = space4;
			root->bottomFrontLeft = space5;
			root->bottomFrontRight = space6;
			root->bottomBackLeft = space7;
			root->bottomBackRight = space8;
			initSpaceTree(root->topFrontLeft, depth - 1);
			initSpaceTree(root->topFrontRight, depth - 1);
			initSpaceTree(root->topBackLeft, depth - 1);
			initSpaceTree(root->topBackRight, depth - 1);
			initSpaceTree(root->bottomFrontLeft, depth - 1);
			initSpaceTree(root->bottomFrontRight, depth - 1);
			initSpaceTree(root->bottomBackLeft, depth - 1);
			initSpaceTree(root->bottomBackRight, depth - 1);
		}
	}

	bool SpaceTree::IsIntersect(AABB aabb)
	{
		if (space.maxEdge.x() < aabb.minEdge.x() || space.minEdge.x() > aabb.maxEdge.x())
		{
			return false;
		}
		if (space.maxEdge.y() < aabb.minEdge.y() || space.minEdge.y() > aabb.maxEdge.y())
		{
			return false;
		}
		if (space.maxEdge.z() < aabb.minEdge.z() || space.minEdge.z() > aabb.maxEdge.z())
		{
			return false;
		}
		return true;
	}





	bool SpaceTree::ContainPoint(QVector3D p)
	{
		return space.ContainPoint(p);
	}



	void addMeshAABBToSpace(SpaceTree* root, MDataPost::mPostMeshData1 * mesh, MDataPost::mOneFrameData1 *fd, AABB aabb)
	{
		
		if (root->depth != 0)
		{
			addMeshAABBToSpace(root->topFrontLeft, mesh, fd, aabb);
			addMeshAABBToSpace(root->topFrontRight, mesh, fd, aabb);
			addMeshAABBToSpace(root->topBackLeft, mesh, fd, aabb);
			addMeshAABBToSpace(root->topBackRight, mesh, fd, aabb);
			addMeshAABBToSpace(root->bottomFrontLeft, mesh, fd, aabb);
			addMeshAABBToSpace(root->bottomFrontRight, mesh, fd, aabb);
			addMeshAABBToSpace(root->bottomBackLeft, mesh, fd, aabb);
			addMeshAABBToSpace(root->bottomBackRight, mesh, fd, aabb);
		}
		else
		{
			if (root->IsIntersect(aabb))
			{
				root->meshs.append(mesh);
			}
		}


	}
	
	
	void addMeshToSpace(SpaceTree* root, MDataPost::mPostMeshData1 * mesh, MDataPost::mOneFrameData1 *fd)
	{
		QVector<int> nodes = mesh->getNodeIndex();
		AABB aabb;
		for (int i = 0; i < nodes.size(); i++)
		{
			if (i == 0)
			{
				aabb.init(fd->getNodeDataByID(nodes[i])->getNodeVertex());
			}
			else
			{
				aabb.push(fd->getNodeDataByID(nodes[i])->getNodeVertex());
			}
		}
		addMeshAABBToSpace(root, mesh, fd, aabb);
	}

	
	void printSpace(SpaceTree* root)
	{
		if (root != nullptr)
		{
			std::cout << "max" << root->space.maxEdge.x() << ","
				<< root->space.maxEdge.y() << "," << root->space.maxEdge.z() << std::endl;
			std::cout << "min" << root->space.minEdge.x() << ","
				<< root->space.minEdge.y() << "," << root->space.minEdge.z() << std::endl;
			printSpace(root->topFrontLeft);
			printSpace(root->topFrontRight);
			printSpace(root->topBackLeft);
			printSpace(root->topBackRight);
			printSpace(root->bottomFrontLeft);
			printSpace(root->bottomFrontRight);
			printSpace(root->bottomBackLeft);
			printSpace(root->bottomBackRight);

		}
	}

	AABB getSpace(AABB aabb, SpaceKind mode)
	{
		QVector3D middle = (aabb.maxEdge + aabb.minEdge)* QVector3D(0.5, 0.5, 0.5);;
		QVector3D topFrontleft = QVector3D(aabb.minEdge.x(), aabb.maxEdge.y(), aabb.maxEdge.z());
		QVector3D topFrontright = QVector3D(aabb.maxEdge.x(), aabb.maxEdge.y(), aabb.maxEdge.z());
		QVector3D topBackleft = QVector3D(aabb.minEdge.x(), aabb.maxEdge.y(), aabb.minEdge.z());
		QVector3D topBackright = QVector3D(aabb.maxEdge.x(), aabb.maxEdge.y(), aabb.minEdge.z());

		QVector3D bottomFrontleft = QVector3D(aabb.minEdge.x(), aabb.minEdge.y(), aabb.maxEdge.z());
		QVector3D bottomFrontright = QVector3D(aabb.maxEdge.x(), aabb.minEdge.y(), aabb.maxEdge.z());
		QVector3D bottomBackleft = QVector3D(aabb.minEdge.x(), aabb.minEdge.y(), aabb.minEdge.z());
		QVector3D bottomBackright = QVector3D(aabb.maxEdge.x(), aabb.minEdge.y(), aabb.minEdge.z());

		switch (mode)
		{
		case TopFrontLeft:
			return AABB(topFrontleft, middle);
			break;
		case TopFrontRight:
			return AABB(topFrontright, middle);
			break;
		case TopBackLeft:
			return AABB(topBackleft, middle);
			break;
		case TopBackRight:
			return AABB(topBackright, middle);
			break;
		case BottomFrontLeft:
			return AABB(bottomFrontleft, middle);
			break;
		case BottomFrontRight:
			return AABB(bottomFrontright, middle);
			break;
		case BottomBackLeft:
			return AABB(bottomBackleft, middle);
			break;
		case BottomBackRight:
			return AABB(bottomBackright, middle);
			break;
		default:
			break;
		}

	}



	void getRayToSpaceData(SpaceTree* root, QVector3D origin, QVector3D dir, QVector3D &ping_pos, float &depth, SpaceTree* &reauire)
	{
		if (root)
		{
			if (root->space.intersects(origin, dir, ping_pos))
			{
				if (root->depth != 0)
				{
					getRayToSpaceData(root->topFrontLeft, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->topFrontRight, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->topBackLeft, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->topBackRight, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->bottomFrontLeft, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->bottomFrontRight, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->bottomBackLeft, origin, dir, ping_pos, depth, reauire);
					getRayToSpaceData(root->bottomBackRight, origin, dir, ping_pos, depth, reauire);

				}
				else
				{
					float distance = (ping_pos - origin).length();
					if (depth > distance && root->meshs.size() != 0)
					{
						depth = distance;
						reauire = root;
					}
				}
			}
		}
	}



	void getRayToSpaceData(SpaceTree* root, QVector3D origin, QVector3D dir, QVector<MDataPost::mPostMeshData1*> &mesh)
	{
		QVector3D ping_pos;
		if (root)
		{
			if (root->space.intersects(origin, dir, ping_pos))
			{
				if (root->depth != 0)
				{
					getRayToSpaceData(root->topFrontLeft, origin, dir, mesh);
					getRayToSpaceData(root->topFrontRight, origin, dir, mesh);
					getRayToSpaceData(root->topBackLeft, origin, dir, mesh);
					getRayToSpaceData(root->topBackRight, origin, dir, mesh);
					getRayToSpaceData(root->bottomFrontLeft, origin, dir, mesh);
					getRayToSpaceData(root->bottomFrontRight, origin, dir, mesh);
					getRayToSpaceData(root->bottomBackLeft, origin, dir, mesh);
					getRayToSpaceData(root->bottomBackRight, origin, dir, mesh);

				}
				else
				{
					mesh.append(root->meshs);
				}
			}
		}
	}




	// Vector3 a b c triangle vertexs
// orig is ray original point, dir is direction vector
	bool rayTriangleIntersect(QVector3D orig, QVector3D dir,
		QVector3D a, QVector3D b, QVector3D c, float &t, float &b1, float &b2)
	{
		bool isIn = false;
		QVector3D E1 = b - a;
		QVector3D E2 = c - a;
		QVector3D S = orig - a;
		QVector3D S1 = QVector3D::crossProduct(dir, E2);
		QVector3D S2 = QVector3D::crossProduct(S, E1);

		// ��ͬϵ��
		float coeff = 1.0f / QVector3D::dotProduct(S1, E1);
		t = coeff * QVector3D::dotProduct(S2, E2);
		b1 = coeff * QVector3D::dotProduct(S1, S);
		b2 = coeff * QVector3D::dotProduct(S2, dir);

		

		if (t >= 0 && b1 >= 0 && b2 >= 0 && (1 - b1 - b2) >= 0)
		{
			isIn = true;
		}

		return isIn;
	}

	void getAABBToMeshData(SpaceTree * root, AABB aabb, QVector<MDataPost::mPostMeshData1*>& mesh)
	{
		
		if ( root->IsIntersect(aabb))
		{
			if (root->depth != 0)
			{
				getAABBToMeshData(root->topFrontLeft, aabb, mesh);
				getAABBToMeshData(root->topFrontRight, aabb, mesh);
				getAABBToMeshData(root->topBackLeft, aabb, mesh);
				getAABBToMeshData(root->topBackRight, aabb, mesh);
				getAABBToMeshData(root->bottomFrontLeft, aabb, mesh);
				getAABBToMeshData(root->bottomFrontRight, aabb, mesh);
				getAABBToMeshData(root->bottomBackLeft, aabb, mesh);
				getAABBToMeshData(root->bottomBackRight, aabb, mesh);
			}
			else
			{
				mesh.append(root->meshs);
			}

		}
	}

	void getOBBToMeshData(SpaceTree * root, OBB aabb, QVector<MDataPost::mPostMeshData1*>& mesh)
	{
		if (OBB::IsIntersect3D( aabb, root->space))
		{
			if (root->depth != 0)
			{
				getOBBToMeshData(root->topFrontLeft, aabb, mesh);
				getOBBToMeshData(root->topFrontRight, aabb, mesh);
				getOBBToMeshData(root->topBackLeft, aabb, mesh);
				getOBBToMeshData(root->topBackRight, aabb, mesh);
				getOBBToMeshData(root->bottomFrontLeft, aabb, mesh);
				getOBBToMeshData(root->bottomFrontRight, aabb, mesh);
				getOBBToMeshData(root->bottomBackLeft, aabb, mesh);
				getOBBToMeshData(root->bottomBackRight, aabb, mesh);
			}
			else
			{
				mesh.append(root->meshs);
			}

		}
	}


	void getOBBToMeshData(SpaceTree * root, OBB aabb, QVector<MDataPost::mPostMeshData1*>& mesh, QVector<MDataPost::mPostMeshData1*>& mesh2)
	{
		if (root == nullptr)
		{
			return;
		}
		if (OBB::IsIntersect3D(aabb, root->space))
		{
			if (root->depth != 0)
			{
				getOBBToMeshData(root->topFrontLeft, aabb, mesh, mesh2);
				getOBBToMeshData(root->topFrontRight, aabb, mesh, mesh2);
				getOBBToMeshData(root->topBackLeft, aabb, mesh, mesh2);
				getOBBToMeshData(root->topBackRight, aabb, mesh, mesh2);
				getOBBToMeshData(root->bottomFrontLeft, aabb, mesh, mesh2);
				getOBBToMeshData(root->bottomFrontRight, aabb, mesh, mesh2);
				getOBBToMeshData(root->bottomBackLeft, aabb, mesh, mesh2);
				getOBBToMeshData(root->bottomBackRight, aabb, mesh, mesh2);
			}
			else if (OBB::AABBInOtherobb(root->space, aabb))
			{
				mesh2.append(root->meshs);
			}
			else
			{
				mesh.append(root->meshs);
			}

		}
	}

	void getAABBToMeshData(SpaceTree * root, AABB aabb, QVector<MDataPost::mPostMeshData1*>& mesh, QVector<MDataPost::mPostMeshData1*>& mesh2)
	{
		if (root == nullptr)
		{
			return;
		}
		if (root->IsIntersect(aabb))
		{
			if (root->depth != 0)
			{
				getAABBToMeshData(root->topFrontLeft, aabb, mesh, mesh2);
				getAABBToMeshData(root->topFrontRight, aabb, mesh, mesh2);
				getAABBToMeshData(root->topBackLeft, aabb, mesh, mesh2);
				getAABBToMeshData(root->topBackRight, aabb, mesh, mesh2);
				getAABBToMeshData(root->bottomFrontLeft, aabb, mesh, mesh2);
				getAABBToMeshData(root->bottomFrontRight, aabb, mesh, mesh2);
				getAABBToMeshData(root->bottomBackLeft, aabb, mesh, mesh2);
				getAABBToMeshData(root->bottomBackRight, aabb, mesh, mesh2);
			}
			else if(aabb.isContain(root->space))
			{
				mesh2.append(root->meshs);
			}
			else
			{
				mesh.append(root->meshs);
			}

		}
	}


	bool IntersectTriangle(const QVector3D& orig, const  QVector3D& dir, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2,
		float &u, float &v, float &t, float& distance)
	{

		QVector3D E1 = v1 - v0;
		QVector3D E2 = v2 - v0;
		QVector3D P = dir.crossProduct(dir, E2);
		float det = E1.dotProduct(E1, P);	
		QVector3D T;
		if (det > 0)
		{
			T = orig - v0;
		}
		else
		{
			T = v0 - orig;
			det = -det;
		}
		if (det < 0.0000001f)
			return false;


		u =	T.dotProduct(T, P);
		if (u < 0.0f || u > det)
			return false;

		QVector3D Q = T.crossProduct(T, E1);
		v = dir.dotProduct(dir, Q);
		if (v < 0.0f || u + v > det)
			return false;

		t =  E2.dotProduct(E2, Q);
		float fInvDet = 1.0f / det;
		t *= fInvDet;
		u *= fInvDet;
		v *= fInvDet;
		distance =  t;

		return true;
	}


	void getPointToSpaceTree(SpaceTree* root, QVector3D p, SpaceTree *&require)
	{
		if (root)
		{
			if (root->ContainPoint(p))
			{
				if (root->depth != 0)
				{
					getPointToSpaceTree(root->topFrontLeft, p, require);
					getPointToSpaceTree(root->topFrontRight, p, require);
					getPointToSpaceTree(root->topBackLeft, p, require);
					getPointToSpaceTree(root->topBackRight, p, require);
					getPointToSpaceTree(root->bottomFrontLeft, p, require);
					getPointToSpaceTree(root->bottomFrontRight, p, require);
					getPointToSpaceTree(root->bottomBackLeft, p, require);
					getPointToSpaceTree(root->bottomBackRight, p, require);

				}
				else
				{
					require = root;
				}
			}
		}
	}



	void destoryTree(SpaceTree* root)
	{	
		if (root == nullptr) {
			return ;
		}
		destoryTree(root->topFrontLeft);
		destoryTree(root->topFrontRight);
		destoryTree(root->topBackLeft);
		destoryTree(root->topBackRight);
		destoryTree(root->bottomFrontLeft);
		destoryTree(root->bottomFrontRight);
		destoryTree(root->bottomBackLeft);
		destoryTree(root->bottomBackRight);

		SpaceTree * s = root;
		root = nullptr;
		delete s;
	
	}


}