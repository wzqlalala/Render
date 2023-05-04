#pragma once

namespace MDataPost
{
	enum class PostSetType
	{
		Node,

		Element,

		ElementSurface,

		CuttingPlane,

		Part,

	};

	enum class PostRigidWallType
	{
		InfinitelyLargePlaneRigidWall,//���ߴ�ƽ�����ǽ

		LimitedLargePlanesRigidWall,//���޴�ƽ�����ǽ

		RectangularRigidWall,//���������ǽ

		CylindricalRigidWall,//Բ��״����ǽ

		SphericalRigidWall//���θ���ǽ
	};

}

