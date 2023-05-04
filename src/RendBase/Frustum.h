#pragma once
#include <qmatrix4x4.h>
#include "Space.h"
#include "../core/rendbase_global.h"
//��������ͶӰ
namespace mxr
{
	class RENDBASE_EXPORT Ortho
	{
	public:
		Ortho() {}
		bool containAABB(AABB aabb)
		{
			return OBB::AABBInOtherobb(aabb, orthogonal);
		}

		void Updata(QVector3D axisX, QVector3D axisY, QVector3D axisZ, QVector3D center, QVector3D size)
		{
			orthogonal.Set(axisX, axisY, axisZ, center, size);
		}

	private:
		OBB orthogonal;

	};
}