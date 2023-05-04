#pragma once
namespace MViewBasic
{

	/*
	* ʰȡģʽ,add,reduce,replace
	*/
	enum class PickFuntion
	{
		//���ʰȡ
		AddPick,

		//����ʰȡ
		ReducePick,

		//�滻ʰȡ
		ReplacePick,
	};

	/*
	* ʰȡɸѡ��:ȫ��,�ڵ�,��,��,������,�ı���,������,������,������,��ʰȡ,��Ԫ��,��Ԫ�棬�������������ƫ�ã�����׶
	*/
	enum class PickFilter
	{
		//ʰȡ�κ�����ֻ�������ͽڵ㣬���������ߺ������棩
		PickAny,

		//ʰȡ��
		PickPoint,

		//ʰȡһά����
		Pick1DMesh,

		//ʰȡ��ά����
		Pick2DMesh,

		//ʰȡ������
		PickTri,

		//ʰȡ�ı���
		PickQuad,

		//ʰȡ������
		PickTet,

		//ʰȡ����׶
		PickPyramid,

		//ʰȡ������
		PickWedge,

		//ʰȡ������
		PickHex,

		//��ѡ��ģʽ
		PickNothing,

		//ʰȡ�ڵ�
		PickNode,

		//ʰȡ�ڵ㣨����ʰȡ˳��
		PickNodeOrder,

		//ʰȡ�ڵ�(ͨ��·��)
		PickNodePath,

		//ʰȡ��������
		PickAnyMesh,

		//ʰȡ��Ԫ��
		PickMeshLine,

		//ʰȡ��Ԫ��
		PickMeshFace,

		//ʰȡ�ڵ�ͨ������
		PickNodeByPart,

		//ʰȡ��������ͨ������
		PickAnyMeshByPart,

		//ʰȡ��Ԫ��ͨ������
		PickMeshLineByPart,

		//ʰȡ��Ԫ��ͨ������
		PickMeshFaceByPart,

		//ʰȡ�ڵ�ͨ����
		PickNodeByLine,

		//ʰȡ��Ԫͨ����
		PickMeshByLine,

		//ʰȡ��Ԫ��ͨ����
		PickMeshLineByLine,

		//ʰȡ�ڵ�ͨ����
		PickNodeByFace,

		//ʰȡ��Ԫͨ����
		PickMeshByFace,

		//ʰȡ��Ԫ��ͨ����
		PickMeshFaceByFace,

		//ʰȡ�ڵ�ͨ���߽Ƕȣ�������1ά��Ԫ���ߵ�Ԫ�ߣ��ڲ��Զ��жϣ�
		PickNodeByLineAngle,

		//ʰȡ�ڵ�ͨ����Ƕȣ�������2ά��Ԫ���ߵ�Ԫ�棬�ڲ��Զ��жϣ�
		PickNodeByFaceAngle,

		//ʰȡ1ά��Ԫͨ���Ƕ�
		Pick1DMeshByAngle,

		//ʰȡ2ά��Ԫͨ���Ƕ�
		Pick2DMeshByAngle,

		//ʰȡ��Ԫ��ͨ���Ƕ�
		PickMeshLineByAngle,

		//ʰȡ��Ԫ��ͨ���Ƕ�
		PickMeshFaceByAngle,

		//ʰȡ���񲿼�
		PickMeshPart,

		//�����ʷ�ʱ��������������
		PickAdjustNum,

		//�����ʷ�ʱ����ƫ�õ�����
		PickBiasNum,

		//ʰȡ������
		PickGeoSolid,

		//ʰȡ������
		PickGeoFace,

		//ʰȡ������
		PickGeoLine,

		//ʰȡ���ε�
		PickGeoPoint,

		//ʰȡ������ͨ������
		PickGeoSolidByPart,

		//ʰȡ������ͨ������
		PickGeoFaceByPart,

		//ʰȡ������ͨ������
		PickGeoLineByPart,

		//ʰȡ���ε�ͨ������
		PickGeoPointByPart,

		//ʰȡ���β���
		PickGeoPart,

		//ʰȡ�������ϵ������
		pickVertexOnGeoLine,

		//ʰȡ�������ϵ������
		pickVertexOnGeoFace,

		//ʰȡ��Ļ�ϵ������
		pickVertexOnScreen,

		//ʰȡ�����ϵ������
		PickAnyPositionValue,
	};

	//ʰȡ����
	enum PickObject
	{
		//����
		Geometry,

		//����
		Mesh,

		//����
		Other,
	};

	enum class CameraType
	{
		Camera1,

		Camera2,
	};

	enum class ViewOperateMode
	{
		NoViewOperate,

		CameraOperate,

		PickOperate,
	};

	enum class CameraOperateMode
	{
		NoCameraOperate,

		Rotate,

		Translate,

		Zoom,

	};

	//ʰȡģʽ
	enum class PickMode
	{
		//��ʰȡ
		NoPick,

		//��ק
		DragPick,

		//��ѡ
		SoloPick,

		//��ѡ
		MultiplyPick,



		/***************��ѡģʽ*****************/
		//


		/***************��קģʽ*****************/

		//��ק��Сֵ����
		//DragMinLine,

		//��ק���ֵ����
		//DragMaxLine,
	};

	enum class MultiplyPickMode
	{
		/**************��ѡģʽ***********/
		//���ο�ѡ���ǵ㣩
		QuadPick,

		//Բ�ο�ѡ(Բ�ĺͰ뾶)
		RoundPick,

		//����ο�ѡ������ߣ�
		PolygonPick,
	};

	/*
	* ��ʾģʽ���߽���ģʽ��͸���߿�ģʽ�������棬���������
	*/
	enum ShowFuntion
	{
		WireEdge,
		WireFrame,
		ElementFace,
		SmoothShaded,
	};

	/*
	 * ����ɫģʽ����ɫ�������ҽ��䣬���ϵ��½���
	 */
	enum BackGroundMode
	{
		PureColor,
		GradientL_R,
		GradientT_B,
	};

	/*
	 * ��ͼ��ʾģʽ���ڵ㻹������
	 */
	enum NodeOrElement
	{
		//�ڵ�
		PostNode,

		//��Ԫ
		PostElement,
	};

	/*
	* �����ӽǣ���������ӽǣ����˵��ӽǣ�
	*/
	enum ScalePerspectice
	{
		Model,
		Eye,
	};

	/*
	* �������ͣ��Ŵ���С��
	*/
	enum ScaleDirection		//��������
	{
		Zoom_in,		//�Ŵ�
		Zoom_out,		//��С
	};

	/*
	* ��ת���ͣ�XYƽ����ת����Z��ת��
	*/
	enum RotateType
	{
		Rotate_XY,
		Rotate_Z,
	};

	/*
	* �۲��ӽ�
	*/
	enum Perspective
	{
		Front,			//ǰ��
		Rear,			//����
		Top,			//����
		Bottom,			//����
		Left,			//����
		Right,			//����
		ISO1,			//��׼1asiAlgo_AAG
		ISO2,			//��׼2
		ISO3,			//��׼3
		ISO4,			//��׼4

	};


	/*
	* ����������
	*/
	enum AxisType
	{
		Local_Rectangular,		//�ֲ�ֱ������ϵRigid
		Local_Cylinder,			//�ֲ�ֱ������ϵ
		Local_Sphere,			//�ֲ�������ϵ
	};


	/*
	* ����������Ⱦ��������
	*/
	enum UpdateConnectDataType
	{

		//�������Ӳ���
		//��������
		Append_Rigid_Single,
		Delete_Rigid_Single,
		Hide_Rigid_Single,
		Show_Rigid_Single,
		Highlight_Rigid_Single,
		NoHighlight_Rigid_Single,

		//��������
		Append_Flex_Single,
		Delete_Flex_Single,
		Hide_Flex_Single,
		Show_Flex_Single,
		Highlight_Flex_Single,
		NoHighlight_Flex_Single,

		//MPC����
		Append_MPC_Single,
		Delete_MPC_Single,
		Hide_MPC_Single,
		Show_MPC_Single,
		Highlight_MPC_Single,
		NoHighlight_MPC_Single,

		//�����
		Append_FaceTie_Single,
		Delete_FaceTie_Single,
		Hide_FaceTie_Single,
		Show_FaceTie_Single,
		Highlight_FaceTie_Single,
		NoHighlight_FaceTie_Single,

		//�������
		Append_PotentialCoupling_Single,
		Delete_PotentialCoupling_Single,
		Hide_PotentialCoupling_Single,
		Show_PotentialCoupling_Single,
		Highlight_PotentialCoupling_Single,
		NoHighlight_PotentialCoupling_Single,

		//�㺸
		Append_Spotweld_Single,
		Delete_Spotweld_Single,
		Hide_Spotweld_Single,
		Show_Spotweld_Single,
		Highlight_Spotweld_Single,
		NoHighlight_Spotweld_Single,

		//�ڵ����
		Append_Mpc1_Single,
		Delete_Mpc1_Single,
		Hide_Mpc1_Single,
		Show_Mpc1_Single,
		Highlight_Mpc1_Single,
		NoHighlight_Mpc1_Single,

		//�������Ӳ���
		Append_AllConnect,
		Delete_AllConnect,
		Hide_AllConnect,
		Show_AllConnect,
		Highlight_AllConnect,
		NoHighlight_AllConnect,
	};

	/*
	* ���¸���ǽ��Ⱦ��������
	*/
	enum UpdateRigidWallDataType
	{

		//��������ǽ����
		//���޴�ƽ��
		Append_Planar_Single,
		Delete_Planar_Single,
		Hide_Planar_Single,
		Show_Planar_Single,
		Highlight_Planar_Single,
		NoHighlight_Planar_Single,	

		//���޴�ƽ��
		Append_Flat_Single,
		Delete_Flat_Single,
		Hide_Flat_Single,
		Show_Flat_Single,
		Highlight_Flat_Single,
		NoHighlight_Flat_Single,

		//���������ǽ
		Append_Cuboid_Single,
		Delete_Cuboid_Single,
		Hide_Cuboid_Single,
		Show_Cuboid_Single,
		Highlight_Cuboid_Single,
		NoHighlight_Cuboid_Single,

		//Բ�������ǽ
		Append_Cylinder_Single,
		Delete_Cylinder_Single,
		Hide_Cylinder_Single,
		Show_Cylinder_Single,
		Highlight_Cylinder_Single,
		NoHighlight_Cylinder_Single,

		//�������ǽ
		Append_Sphere_Single,
		Delete_Sphere_Single,
		Hide_Sphere_Single,
		Show_Sphere_Single,
		Highlight_Sphere_Single,
		NoHighlight_Sphere_Single,

		//��ͷ����
		Append_RigidWallArrow_Single,
		Delete_RigidWallArrow_Single,
		Hide_RigidWallArrow_Single,
		Show_RigidWallArrow_Single,
		Highlight_RigidWallArrow_Single,
		NoHighlight_RigidWallArrow_Single,


		//���и���ǽ����
		Append_AllRigidWall,
		Delete_AllRigidWall,
		Hide_AllRigidWall,
		Show_AllRigidWall,
		Highlight_AllRigidWall,
		NoHighlight_AllRigidWall,
	};
	/*
	* ���º�����Ⱦ��������
	*/
	enum UpdateBoxDataType
	{
		//�������
		Append_NormalBox_Single,
		Delete_NormalBox_Single,
		Hide_NormalBox_Single,
		Show_NormalBox_Single,
		Highlight_NormalBox_Single,
		NoHighlight_NormalBox_Single,

		//ָ������
		Append_SpecialBox_Single,
		Delete_SpecialBox_Single,
		Hide_SpecialBox_Single,
		Show_SpecialBox_Single,
		Highlight_SpecialBox_Single,
		NoHighlight_SpecialBox_Single,

		//���и���ǽ����
		Append_AllBox,
		Delete_AllBox,
		Hide_AllBox,
		Show_AllBox,
		Highlight_AllBox,
		NoHighlight_AllBox,
	};	

	/*
	* ���¸���������Ⱦ��������
	*/
	enum UpdateRigidConnectDataType
	{
		//�������Ӳ���

		//����RCSpring����
		Append_RCSpring_Single,
		Delete_RCSpring_Single,
		Hide_RCSpring_Single,
		Show_RCSpring_Single,
		Highlight_RCSpring_Single,
		NoHighlight_RCSpring_Single,

		//����RCPrismatic����
		Append_RCPrismatic_Single,
		Delete_RCPrismatic_Single,
		Hide_RCPrismatic_Single,
		Show_RCPrismatic_Single,
		Highlight_RCPrismatic_Single,
		NoHighlight_RCPrismatic_Single,

		//����RCRevolute����
		Append_RCRevolute_Single,
		Delete_RCRevolute_Single,
		Hide_RCRevolute_Single,
		Show_RCRevolute_Single,
		Highlight_RCRevolute_Single,
		NoHighlight_RCRevolute_Single,

		//����RCSpherical����
		Append_RCSpherical_Single,
		Delete_RCSpherical_Single,
		Hide_RCSpherical_Single,
		Show_RCSpherical_Single,
		Highlight_RCSpherical_Single,
		NoHighlight_RCSpherical_Single,

		//����RCCylindrical����
		Append_RCCylindrical_Single,
		Delete_RCCylindrical_Single,
		Hide_RCCylindrical_Single,
		Show_RCCylindrical_Single,
		Highlight_RCCylindrical_Single,
		NoHighlight_RCCylindrical_Single,

		//����RCPlanar����
		Append_RCPlanar_Single,
		Delete_RCPlanar_Single,
		Hide_RCPlanar_Single,
		Show_RCPlanar_Single,
		Highlight_RCPlanar_Single,
		NoHighlight_RCPlanar_Single,

		//����RCDamper����
		Append_RCDamper_Single,
		Delete_RCDamper_Single,
		Hide_RCDamper_Single,
		Show_RCDamper_Single,
		Highlight_RCDamper_Single,
		NoHighlight_RCDamper_Single,

		//�������Ӳ���
		Append_AllRigidConnect,
		Delete_AllRigidConnect,
		Hide_AllRigidConnect,
		Show_AllRigidConnect,
		Highlight_AllRigidConnect,
		NoHighlight_AllRigidConnect,
	};
	/*
	* ���½Ӵ���Ⱦ��������
	*/
	enum UpdateContactDataType
	{

		//�����Ӵ�����
		//��ʽ����Ӵ�
		Append_Contact_Single,
		Delete_Contact_Single,
		Hide_Contact_Single,
		Show_Contact_Single,
		Highlight_Contact_Single,
		NoHighlight_Contact_Single,
		//��ʾ����Ӵ�
		Append_FaceToFace_Single,
		Delete_FaceToFace_Single,
		Hide_FaceToFace_Single,
		Show_FaceToFace_Single,
		Highlight_FaceToFace_Single,
		NoHighlight_FaceToFace_Single,
		//��ʾ����Ӵ�
		Append_PointToFace_Single,
		Delete_PointToFace_Single,
		Hide_PointToFace_Single,
		Show_PointToFace_Single,
		Highlight_PointToFace_Single,
		NoHighlight_PointToFace_Single,
		//���нӴ�����
		Append_AllContact,
		Delete_AllContact,
		Hide_AllContact,
		Show_AllContact,
		Highlight_AllContact,
		NoHighlight_AllContact,
		//�Ӵ��������
		Edit_Contact_HightLightState,
	};
	/*
	* ���½Ӵ���Ⱦ��������
	*/
	enum UpdateVectorDataType
	{
		//������������
		Append_Vector_Single,
		Delete_Vector_Single,
		Hide_Vector_Single,
		Show_Vector_Single,
		Highlight_Vector_Single,
		NoHighlight_Vector_Single,

		//������������
		Append_AllVector,
		Delete_AllVector,
		Hide_AllVector,
		Show_AllVector,
		Highlight_AllVector,
		NoHighlight_AllVector,
	};
	/*
	* �����غɱ߽���Ⱦ��������
	*/
	enum UpdateLoadBCDataType
	{
		//�����غɲ���
		//������
		Append_Force_Single,
		Delete_Force_Single,
		Hide_Force_Single,
		Show_Force_Single,
		Highlight_Force_Single,
		NoHighlight_Force_Single,

		//������
		Append_CentrifugalForce_Single,
		Delete_CentrifugalForce_Single,
		Hide_CentrifugalForce_Single,
		Show_CentrifugalForce_Single,
		Highlight_CentrifugalForce_Single,
		NoHighlight_CentrifugalForce_Single,

		//�����غ�
		Append_RigidBodyForce_Single,
		Delete_RigidBodyForce_Single,
		Hide_RigidBodyForce_Single,
		Show_RigidBodyForce_Single,
		Highlight_RigidBodyForce_Single,
		NoHighlight_RigidBodyForce_Single,

		//����
		Append_BodyForce_Single,
		Delete_BodyForce_Single,
		Hide_BodyForce_Single,
		Show_BodyForce_Single,
		Highlight_BodyForce_Single,
		NoHighlight_BodyForce_Single,

		//����
		Append_Gravity_Single,
		Delete_Gravity_Single,
		Hide_Gravity_Single,
		Show_Gravity_Single,
		Highlight_Gravity_Single,
		NoHighlight_Gravity_Single,

		//�����غ�
		Append_Distribute_Single,
		Delete_Distribute_Single,
		Hide_Distribute_Single,
		Show_Distribute_Single,
		Highlight_Distribute_Single,
		NoHighlight_Distribute_Single,

		//ѹǿ
		Append_Pressure_Single,
		Delete_Pressure_Single,
		Hide_Pressure_Single,
		Show_Pressure_Single,
		Highlight_Pressure_Single,
		NoHighlight_Pressure_Single,

		//���غ�
		Append_LineLoad_Single,
		Delete_LineLoad_Single,
		Hide_LineLoad_Single,
		Show_LineLoad_Single,
		Highlight_LineLoad_Single,
		NoHighlight_LineLoad_Single,

		//��˨Ԥ����
		Append_Pretension_Single,
		Delete_Pretension_Single,
		Hide_Pretension_Single,
		Show_Pretension_Single,
		Highlight_Pretension_Single,
		NoHighlight_Pretension_Single,

		//�����ܶ�
		Append_Flux_Single,
		Delete_Flux_Single,
		Hide_Flux_Single,
		Show_Flux_Single,
		Highlight_Flux_Single,
		NoHighlight_Flux_Single,


		//��������
		Append_Conv_Single,
		Delete_Conv_Single,
		Hide_Conv_Single,
		Show_Conv_Single,
		Highlight_Conv_Single,
		NoHighlight_Conv_Single,


		//�¶��غ�
		Append_TempLoad_Single,
		Delete_TempLoad_Single,
		Hide_TempLoad_Single,
		Show_TempLoad_Single,
		Highlight_TempLoad_Single,
		NoHighlight_TempLoad_Single,


		//�ڲ���Դ
		Append_InterHeat_Single,
		Delete_InterHeat_Single,
		Hide_InterHeat_Single,
		Show_InterHeat_Single,
		Highlight_InterHeat_Single,
		NoHighlight_InterHeat_Single,

		//�ߵ��Դ
		Append_LineQSource_Single,
		Delete_LineQSource_Single,
		Hide_LineQSource_Single,
		Show_LineQSource_Single,
		Highlight_LineQSource_Single,
		NoHighlight_LineQSource_Single,

		//�ߵ���Դ
		Append_LineJSource_Single,
		Delete_LineJSource_Single,
		Hide_LineJSource_Single,
		Show_LineJSource_Single,
		Highlight_LineJSource_Single,
		NoHighlight_LineJSource_Single,

		//�ڵ�/����Դ
		Append_QSource_Single,
		Delete_QSource_Single,
		Hide_QSource_Single,
		Show_QSource_Single,
		Highlight_QSource_Single,
		NoHighlight_QSource_Single,

		//�ڵ����Դ
		Append_JSource_Single,
		Delete_JSource_Single,
		Hide_JSource_Single,
		Show_JSource_Single,
		Highlight_JSource_Single,
		NoHighlight_JSource_Single,

		//����ܶ�
		Append_QDensity_Single,
		Delete_QDensity_Single,
		Hide_QDensity_Single,
		Show_QDensity_Single,
		Highlight_QDensity_Single,
		NoHighlight_QDensity_Single,

		//�����ܶ�
		Append_FaceQDensity_Single,
		Delete_FaceQDensity_Single,
		Hide_FaceQDensity_Single,
		Show_FaceQDensity_Single,
		Highlight_FaceQDensity_Single,
		NoHighlight_FaceQDensity_Single,

		//������ܶ�
		Append_FaceJDensity_Single,
		Delete_FaceJDensity_Single,
		Hide_FaceJDensity_Single,
		Show_FaceJDensity_Single,
		Highlight_FaceJDensity_Single,
		NoHighlight_FaceJDensity_Single,

		//�����ܶ�
		Append_BodyQDensity_Single,
		Delete_BodyQDensity_Single,
		Hide_BodyQDensity_Single,
		Show_BodyQDensity_Single,
		Highlight_BodyQDensity_Single,
		NoHighlight_BodyQDensity_Single,

		//������ܶ�
		Append_BodyJDensity_Single,
		Delete_BodyJDensity_Single,
		Hide_BodyJDensity_Single,
		Show_BodyJDensity_Single,
		Highlight_BodyJDensity_Single,
		NoHighlight_BodyJDensity_Single,

		//��ͨ�ܶ�
		Append_BDensity_Single,
		Delete_BDensity_Single,
		Hide_BDensity_Single,
		Show_BDensity_Single,
		Highlight_BDensity_Single,
		NoHighlight_BDensity_Single,

		//��ͨ�ܶ�
		Append_MagneticCoercivity_Single,
		Delete_MagneticCoercivity_Single,
		Hide_MagneticCoercivity_Single,
		Show_MagneticCoercivity_Single,
		Highlight_MagneticCoercivity_Single,
		NoHighlight_MagneticCoercivity_Single,

		//����糡ǿ��
		Append_FaceEIntensity_Single,
		Delete_FaceEIntensity_Single,
		Hide_FaceEIntensity_Single,
		Show_FaceEIntensity_Single,
		Highlight_FaceEIntensity_Single,
		NoHighlight_FaceEIntensity_Single,


		//λ��/��λ��
		Append_Displacement_Single,
		Delete_Displacement_Single,
		Hide_Displacement_Single,
		Show_Displacement_Single,
		Highlight_Displacement_Single,
		NoHighlight_Displacement_Single,

		//λ��/��λ��
		Append_Velocity_Single,
		Delete_Velocity_Single,
		Hide_Velocity_Single,
		Show_Velocity_Single,
		Highlight_Velocity_Single,
		NoHighlight_Velocity_Single,

		//���ٶ�/�Ǽ��ٶ�
		Append_Acceleration_Single,
		Delete_Acceleration_Single,
		Hide_Acceleration_Single,
		Show_Acceleration_Single,
		Highlight_Acceleration_Single,
		NoHighlight_Acceleration_Single,

		//��ʼӦ��
		Append_IniStress_Single,
		Delete_IniStress_Single,
		Hide_IniStress_Single,
		Show_IniStress_Single,
		Highlight_IniStress_Single,
		NoHighlight_IniStress_Single,

		//����Լ��
		Append_VirtualConstraints_Single,
		Delete_VirtualConstraints_Single,
		Hide_VirtualConstraints_Single,
		Show_VirtualConstraints_Single,
		Highlight_VirtualConstraints_Single,
		NoHighlight_VirtualConstraints_Single,

		//�¶�Լ��
		Append_TempConstraints_Single,
		Delete_TempConstraints_Single,
		Hide_TempConstraints_Single,
		Show_TempConstraints_Single,
		Highlight_TempConstraints_Single,
		NoHighlight_TempConstraints_Single,

		//����
		Append_PiezoelectricPotential_Single,
		Delete_PiezoelectricPotential_Single,
		Hide_PiezoelectricPotential_Single,
		Show_PiezoelectricPotential_Single,
		Highlight_PiezoelectricPotential_Single,
		NoHighlight_PiezoelectricPotential_Single,

		//��ʸ���߽�
		Append_MagneticVector_Single,
		Delete_MagneticVector_Single,
		Hide_MagneticVector_Single,
		Show_MagneticVector_Single,
		Highlight_MagneticVector_Single,
		NoHighlight_MagneticVector_Single,

		//��λ�߽�
		Append_PotentialBC_Single,
		Delete_PotentialBC_Single,
		Hide_PotentialBC_Single,
		Show_PotentialBC_Single,
		Highlight_PotentialBC_Single,
		NoHighlight_PotentialBC_Single,

		//�ٶ���ڱ߽�
		Append_VelocityEntryBoundary_Single,
		Delete_VelocityEntryBoundary_Single,
		Hide_VelocityEntryBoundary_Single,
		Show_VelocityEntryBoundary_Single,
		Highlight_VelocityEntryBoundary_Single,
		NoHighlight_VelocityEntryBoundary_Single,

		//ѹ����ڱ߽�
		Append_PressureInletBoundary_Single,
		Delete_PressureInletBoundary_Single,
		Hide_PressureInletBoundary_Single,
		Show_PressureInletBoundary_Single,
		Highlight_PressureInletBoundary_Single,
		NoHighlight_PressureInletBoundary_Single,
		//�����߽�
		Append_OutflowBoundary_Single,
		Delete_OutflowBoundary_Single,
		Hide_OutflowBoundary_Single,
		Show_OutflowBoundary_Single,
		Highlight_OutflowBoundary_Single,
		NoHighlight_OutflowBoundary_Single,
		//ѹ�����ڱ߽�
		Append_PressureOutletBoundary_Single,
		Delete_PressureOutletBoundary_Single,
		Hide_PressureOutletBoundary_Single,
		Show_PressureOutletBoundary_Single,
		Highlight_PressureOutletBoundary_Single,
		NoHighlight_PressureOutletBoundary_Single,
		//���߽�
		Append_WallBoundary_Single,
		Delete_WallBoundary_Single,
		Hide_WallBoundary_Single,
		Show_WallBoundary_Single,
		Highlight_WallBoundary_Single,
		NoHighlight_WallBoundary_Single,
		//������߽�
		Append_CoupledWallBoundary_Single,
		Delete_CoupledWallBoundary_Single,
		Hide_CoupledWallBoundary_Single,
		Show_CoupledWallBoundary_Single,
		Highlight_CoupledWallBoundary_Single,
		NoHighlight_CoupledWallBoundary_Single,
		//�ԳƱ߽�
		Append_SymmetricalBoundary_Single,
		Delete_SymmetricalBoundary_Single,
		Hide_SymmetricalBoundary_Single,
		Show_SymmetricalBoundary_Single,
		Highlight_SymmetricalBoundary_Single,
		NoHighlight_SymmetricalBoundary_Single,

		/****��ѹ�����߽�****/
			 //Զ������
			Append_FluidFarFieldBoundary1_Single,
			Delete_FluidFarFieldBoundary1_Single,
			Hide_FluidFarFieldBoundary1_Single,
			Show_FluidFarFieldBoundary1_Single,
			Highlight_FluidFarFieldBoundary1_Single,
			NoHighlight_FluidFarFieldBoundary1_Single,


			//���ڱ߽�
			Append_FluidOutflowBoundary1_Single,
			Delete_FluidOutflowBoundary1_Single,
			Hide_FluidOutflowBoundary1_Single,
			Show_FluidOutflowBoundary1_Single,
			Highlight_FluidOutflowBoundary1_Single,
			NoHighlight_FluidOutflowBoundary1_Single,

			//����߽�
			Append_FluidWallBoundary1_Single,
			Delete_FluidWallBoundary1_Single,
			Hide_FluidWallBoundary1_Single,
			Show_FluidWallBoundary1_Single,
			Highlight_FluidWallBoundary1_Single,
			NoHighlight_FluidWallBoundary1_Single,

			//�ԳƱ߽�
			Append_FluidSymmetricBoundary1_Single,
			Delete_FluidSymmetricBoundary1_Single,
			Hide_FluidSymmetricBoundary1_Single,
			Show_FluidSymmetricBoundary1_Single,
			Highlight_FluidSymmetricBoundary1_Single,
			NoHighlight_FluidSymmetricBoundary1_Single,




			//��ʼ�ٶ�
			Append_InitialVelocity_Single,
			Delete_InitialVelocity_Single,
			Hide_InitialVelocity_Single,
			Show_InitialVelocity_Single,
			Highlight_InitialVelocity_Single,
			NoHighlight_InitialVelocity_Single,

			//��ʼλ��
			Append_InitialDisplacement_Single,
			Delete_InitialDisplacement_Single,
			Hide_InitialDisplacement_Single,
			Show_InitialDisplacement_Single,
			Highlight_InitialDisplacement_Single,
			NoHighlight_InitialDisplacement_Single,

			//�����غɲ���
			Append_AllLoad,
			Delete_AllLoad,
			Hide_AllLoad,
			Show_AllLoad,
			Highlight_AllLoad,
			NoHighlight_AllLoad,

			//���б߽����
			Append_AllBC,
			Delete_AllBC,
			Hide_AllBC,
			Show_AllBC,
			Highlight_AllBC,
			NoHighlight_AllBC,

	};
	/*
	* ���³�ʼ������Ⱦ��������
	*/
	enum UpdateInitialConditionDataType
	{
		Append_InitialVelocityEX_Single,
		Delete_InitialVelocityEX_Single,
		Hide_InitialVelocityEX_Single,
		Show_InitialVelocityEX_Single,
		Highlight_InitialVelocityEX_Single,
		NoHighlight_InitialVelocityEX_Single,

		//���в���
		Append_AllInitialCondition,
		Delete_AllInitialCondition,
		Hide_AllInitialCondition,
		Show_AllInitialCondition,
		Highlight_AllInitialCondition,
		NoHighlight_AllInitialCondition,

	};
	/*
	* ���¸���߽���Ⱦ��������
	*/
	enum UpdateRBCDataType
	{
		//λ��/��λ��
		Append_RBCDisplacement_Single,
		Delete_RBCDisplacement_Single,
		Hide_RBCDisplacement_Single,
		Show_RBCDisplacement_Single,
		Highlight_RBCDisplacement_Single,
		NoHighlight_RBCDisplacement_Single,

		//���б߽����
		Append_AllRBC,
		Delete_AllRBC,
		Hide_AllRBC,
		Show_AllRBC,
		Highlight_AllRBC,
		NoHighlight_AllRBC,

	};
	/*
	* ����������Ⱦ��������
	*/
	enum UpdateFontDataType
	{

		//����ͨ�����ֲ���
		Append_CommonFont_Single,
		Delete_CommonFont_Single,
		Hide_CommonFont_Single,
		Show_CommonFont_Single,
		Set_CommonFont_Color,
		Set_CommonFont_Size,
		//����ͨ�����ֲ���
		Append_CommonFont_All,
		Delete_CommonFont_All,
		Hide_CommonFont_All,
		Show_CommonFont_All,

		//�����̶����ֲ���
		Append_FixedFont_Single,
		Delete_FixedFont_Single,
		Hide_FixedFont_Single,
		Show_FixedFont_Single,
		Set_FixedFont_Color,
		Set_FixedFont_Size,
		//���й̶����ֲ���
		Append_FixedFont_All,
		Delete_FixedFont_All,
		Hide_FixedFont_All,
		Show_FixedFont_All,

		//�����غ����ֲ���
		Append_LoadFont_Single,
		Delete_LoadFont_Single,
		Hide_LoadFont_Single,
		Show_LoadFont_Single,
		//�����غ����ֲ���
		Append_LoadFont_All,
		Delete_LoadFont_All,
		Hide_LoadFont_All,
		Show_LoadFont_All,

		//��������ǽ���ֲ���
		Append_RigidWallFont_Single,
		Delete_RigidWallFont_Single,
		Hide_RigidWallFont_Single,
		Show_RigidWallFont_Single,
		//���и���ǽ���ֲ���
		Append_RigidWallFont_All,
		Delete_RigidWallFont_All,
		Hide_RigidWallFont_All,
		Show_RigidWallFont_All,

		//�����������ֲ���
		Append_VectorFont_Single,
		Delete_VectorFont_Single,
		Hide_VectorFont_Single,
		Show_VectorFont_Single,
		//�����������ֲ���
		Append_VectorFont_All,
		Delete_VectorFont_All,
		Hide_VectorFont_All,
		Show_VectorFont_All,

		//������ʼ�������ֲ���
		Append_InitialConditionFont_Single,
		Delete_InitialConditionFont_Single,
		Hide_InitialConditionFont_Single,
		Show_InitialConditionFont_Single,
		//���г�ʼ�������ֲ���
		Append_InitialConditionFont_All,
		Delete_InitialConditionFont_All,
		Hide_InitialConditionFont_All,
		Show_InitialConditionFont_All,

		//�����߽����ֲ���
		Append_BCFont_Single,
		Delete_BCFont_Single,
		Hide_BCFont_Single,
		Show_BCFont_Single,
		//���б߽����ֲ���
		Append_BCFont_All,
		Delete_BCFont_All,
		Hide_BCFont_All,
		Show_BCFont_All,

		//�����������ֲ���
		Append_ConnectFont_Single,
		Delete_ConnectFont_Single,
		Hide_ConnectFont_Single,
		Show_ConnectFont_Single,
		//�����������ֲ���
		Append_ConnectFont_All,
		Delete_ConnectFont_All,
		Hide_ConnectFont_All,
		Show_ConnectFont_All,

		//�����ֲ�����ϵ���ֲ���
		Append_LocalAxisFont_Single,
		Delete_LocalAxisFont_Single,
		Hide_LocalAxisFont_Single,
		Show_LocalAxisFont_Single,
		//���оֲ�����ϵ���ֲ���
		Append_LocalAxisFont_All,
		Delete_LocalAxisFont_All,
		Hide_LocalAxisFont_All,
		Show_LocalAxisFont_All,

		//����
		Initial_Font_Post,
		Append_Font_Post,
		Delete_Font_Post,
		Edit_Font_ShowState_Post,


	};


	/*
	* ���¾ֲ���������Ⱦ��������
	*/
	enum UpdateLocalAxisDataType
	{
		//�����ֲ�����ϵ����
		Append_LocalAxis_Single,
		Delete_LocalAxis_Single,
		Hide_LocalAxis_Single,
		Show_LocalAxis_Single,
		Highlight_LocalAxis_Single,
		NoHighlight_LocalAxis_Single,
		//���оֲ�����ϵ����
		Append_LocalAxis_All,
		Delete_LocalAxis_All,
		Hide_LocalAxis_All,
		Show_LocalAxis_All,
		Highlight_LocalAxis_All,
		NoHighlight_LocalAxis_All,

	};

	/*
	* ���¼�ͷ��Ⱦ��������
	*/
	enum UpdateArrowDataType
	{
		//������ͷ����
		Append_Arrow_Single,
		Delete_Arrow_Single,
		Hide_Arrow_Single,
		Show_Arrow_Single,
		Highlight_Arrow_Single,
		NoHighlight_Arrow_Single,
		//���м�ͷ����
		Append_Arrow_All,
		Delete_Arrow_All,
		Hide_Arrow_All,
		Show_Arrow_All,
		Highlight_Arrow_All,
		NoHighlight_Arrow_All,

	};
	/*
	* 	ͼ��������ͣ���ӡ�ɾ������ʾ�����ء�������ȡ��������
	*/

	enum  Lable_operate_type
	{
		Append,
		Delete,
		Show,
		Hide,
		Highlight,
		NoHighlight,

		//ͨ��������
		SetColor,
		SetSize

	};

	/*
	* 	���º���ʸ����ͷ��Ⱦ��������
	*/
	enum UpdateVecArrowDataType
	{
		//������ͷ����
		Append_VecArrow_Single,
		Delete_VecArrow_Single,
		Hide_VecArrow_Single,
		Show_VecArrow_Single,
		//���м�ͷ����
		Append_VecArrow_All,
		Delete_VecArrow_All,
		Hide_VecArrow_All,
		Show_VecArrow_All,
	};


}
