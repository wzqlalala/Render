#pragma once
namespace MViewBasic
{

	/*
	 * ����ģ�Ͳ���ö��
	 */
	enum PostModelOperateEnum
	{
		//�����ļ�����
		ImportOperate,

		//������
		NoOperate,

		//������������
		HideOnePartOperate,//����
		ShowOnePartOperate,//��ʾ
		ChangeColorOnePartOperate,//�ı���ɫ
		ColorOnePartOperate,//��ʾģ����ɫ
		TransparentOnePartOperate,//��ʾ͸��
		TextureOnePartOperate,//��ʾ����
		ReplaceOnePartOperate,//�滻����

		//ȫ����������
		HideAllPartOperate,//��������Isshow
		ShowAllPartOperate,//��ʾ����Isshow
		ChangeColorAllPartOperate,//�ı���ɫMaterial
		ColorAllPartOperate,//��ʾģ����ɫisColor
		TransparentAllPartOperate,//��ʾ͸��isColor
		TextureAllPartOperate,//��ʾ����isColor
		UpdateVariableValue,//���±���ֵvalue
		UpdateMinMax,//������ֵ
		UpdateNormal,//���·�����Normal
		ShowNodeformation,//��ʾ����ǰ��ͼ
		HideNodeformation,//���ر���ǰ��ͼ
	};

	//������Ⱦģʽ
	enum PostMode
	{

		No,

		//��֡
		OneFrame,

		//����
		Animation,

		//��֡���Զ���
		OneFrameLinearAnimation,

		//��֡���Ҷ���
		OneFrameSinAnimation,
	};

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

	enum class MultiplyPickMode
	{
		NoPick,

		RoundPick,

		QuadPick,

		PolygonPick

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
	 * ��ά��������
	 */
	enum PlotType
	{
		//����ͼ
		LineChart,

		//�⻬����ͼ
		SmoothChart,

		//��Ծ����ͼ
		StepChart,

		//�ֲ��⻬����ͼ��ǰ��Ϊ�⻬�����Ϊֱ�ߣ�
		SmoothStepChart,
	};

	/*
	* ������״
	*/
	enum MeshType
	{
		//�޽ڵ㣨���ⵥԪ��
		MeshNone = 0,

		//��
		MeshPoint,

		//��
		MeshBeam,

		//������
		MeshTri,

		//�ı���
		MeshQuad,

		//������
		MeshTet,

		//������
		MeshWedge,

		//������
		MeshHex,

		//����׶
		MeshPyramid,
	};

	/*
	 * ��Ԫ����
	 */
	enum ElementType
	{
		/*
		* �޵�Ԫ
		*/
		NoneElement,

		/*
		* ����ʽ��Ԫ
		*/

		//��������Ԫ(�޽ڵ�)
		Sensor,

		//�Ž�����Ԫ���޽ڵ㣩
		Pretensioner,

		//������Ԫ��һ���ڵ㣩
		Mass1,

		//������Ԫ��һ���ڵ㣩
		ElementSPH,

		//���ֵ�Ԫ��һ���ڵ㣩
		Slipring,

		//��������Ԫ��һ���ڵ㣩
		Retractor,

		//��ȫ����Ԫ���ߣ�
		SeatBelt,

		//һ�׶�ά�˵�Ԫ
		D2R2,

		//���׶�ά�˵�Ԫ
		D2R3,

		//һ����ά�˵�Ԫ
		D3R2,

		//������ά�˵�Ԫ
		D3R3,

		//һ�׶�ά����Ԫ
		D2B2,

		//���׶�ά����Ԫ
		D2B3,

		//һ����ά����Ԫ
		D3B2,

		//������ά����Ԫ
		D3B3,

		//һ����ά�ܵ�Ԫ
		Pipe,

		//���ɵ�Ԫ
		ELAS1,

		//���ɵ�Ԫ
		SPRING1,

		//���ɵ�Ԫ(һ���ڵ�)
		SPRING2,

		//���ɵ�Ԫ
		SPRING3,

		//���ɵ�Ԫ
		GAP,

		//���ɵ�Ԫ
		CBUSH,

		//������ѹ�˵�Ԫ
		TS2,

		//��ɢ��
		DB2,

		//������
		HB2,

		//��������
		BS2,

		//��ɢ��Ԫ
		Discrete,

		//һ��������ƽ��Ӧ�䵥Ԫ
		PST3,

		//����������ƽ��Ӧ�䵥Ԫ
		PST6,

		//һ�������νڵ����ƽ��Ӧ�䵥Ԫ
		PST3_1,

		//һ���������ȶ��ڵ����ƽ��Ӧ�䵥Ԫ
		PST3_2,

		//һ�������α߹⻬ƽ��Ӧ�䵥Ԫ
		PST3_3,

		//�����οǵ�ԪTRIA3
		TRIA3,

		//�����οǵ�ԪC0
		C0,

		//һ��������ƽ��Ӧ����Ԫ(����)
		PET3,

		//����������ƽ��Ӧ����Ԫ(����)
		PET6,

		//һ�������νڵ����ƽ��Ӧ�䵥Ԫ
		PET3_1,

		//һ���������ȶ��ڵ����ƽ��Ӧ�䵥Ԫ
		PET3_2,

		//һ�������α߹⻬ƽ��Ӧ�䵥Ԫ
		PET3_3,

		//һ��������ƽ��Ӧ����Ԫ(�ӽ�ԪH)
		PET3H,

		//����������ƽ��Ӧ����Ԫ(�ӽ�ԪH)
		PET6H,

		//һ����������ԳƵ�Ԫ(����)
		AST3,

		//������������ԳƵ�Ԫ(����)
		AST6,

		//һ����������ԳƵ�Ԫ(�ӽ�ԪH)
		AST3H,

		//������������ԳƵ�Ԫ(�ӽ�ԪH)
		AST6H,

		//�����οǵ�ԪCPDSG
		CPDSG,

		//�����οǵ�ԪCPDSG
		DSG,

		//�����οǵ�ԪSST3
		SST3,

		//�����οǵ�ԪSST6
		SST6,

		//�����α߹⻬�ǵ�Ԫ
		CPDSG_3,

		//һ��������Ĥ��Ԫ
		MBT3,

		//����������Ĥ��Ԫ
		MBT6,

		//�����μ����嵥Ԫ
		SPT3,

		//һ���ı���ƽ��Ӧ�䵥Ԫ(����)
		PSQ4,

		//�����ı���ƽ��Ӧ�䵥Ԫ(����)
		PSQ8,

		//һ���ı���ƽ��Ӧ�䵥Ԫ(��������R)
		PSQ4R,

		//�����ı���ƽ��Ӧ�䵥Ԫ(��������R)
		PSQ8R,

		//һ���ı���ƽ��Ӧ�䵥Ԫ(��Э��ԪI)
		PSQ4I,

		//�����ı���ƽ��Ӧ�䵥Ԫ(��Э��ԪI)
		PSQ8I,

		//һ���ı���ƽ��Ӧ����Ԫ(����)
		PEQ4,

		//�����ı���ƽ��Ӧ����Ԫ(����)
		PEQ8,

		//һ���ı���ƽ��Ӧ����Ԫ(��������R)
		PEQ4R,

		//�����ı���ƽ��Ӧ����Ԫ(��������R)
		PEQ8R,

		//һ���ı���ƽ��Ӧ����Ԫ(�ӽ�ԪH)
		PEQ4H,

		//�����ı���ƽ��Ӧ����Ԫ(�ӽ�ԪH)
		PEQ8H,

		//һ���ı���ƽ��Ӧ����Ԫ(��Э��ԪI)
		PEQ4I,

		//�����ı���ƽ��Ӧ����Ԫ(��Э��ԪI)
		PEQ8I,

		//һ���ı�����ԳƵ�Ԫ(����)
		ASQ4,

		//�����ı�����ԳƵ�Ԫ(����)
		ASQ8,

		//һ���ı�����ԳƵ�Ԫ(��������R)
		ASQ4R,

		//�����ı�����ԳƵ�Ԫ(��������R)
		ASQ8R,

		//һ���ı�����ԳƵ�Ԫ(�ӽ�ԪH)
		ASQ4H,

		//�����ı�����ԳƵ�Ԫ(�ӽ�ԪH)
		ASQ8H,

		//һ���ı�����ԳƵ�Ԫ (��Э��ԪI)
		ASQ4I,

		//�����ı�����ԳƵ�Ԫ (��Э��ԪI)
		ASQ8I,

		//�ı��οǵ�ԪQPH
		QPH,

		//�ı��οǵ�ԪQ4R
		Q4R,

		//�ı��οǵ�ԪMITC4
		MITC4,

		//�ı��οǵ�ԪSSQ4
		SSQ4,

		//�ı��οǵ�ԪSSQ8
		SSQ8,

		//�ı��οǵ�ԪBT
		BT,

		//�ı��οǵ�ԪHL
		HL,

		//�ı��οǵ�ԪCY
		CY_,

		//�ı��οǵ�ԪBTSHELL
		BTShell,

		//һ���ı���Ĥ��Ԫ
		MBQ4,

		//һ���ı���Ĥ��Ԫ(��������)
		MBQ4R,

		//�����ı���Ĥ��Ԫ
		MBQ8,

		//�ı��μ����嵥Ԫ
		SPQ4,

		//һ�������嵥Ԫ
		D3T4,

		//���������嵥Ԫ
		D3T10,

		//һ��������ѹ�絥Ԫ
		D3T4E,

		//һ�׽ڵ���������嵥Ԫ
		D3T4_1,

		//һ���ȶ��ڵ���������嵥Ԫ
		D3T4_2,

		//һ�ױ߹⻬�����嵥Ԫ
		D3T4_3,

		//����������������ʽ
		D3T10M,

		//����������ѹ�絥Ԫ
		D3T10E,

		//һ������׶��Ԫ
		D3P5,

		//һ����������Ԫ(����)
		D3P6,

		//һ����������Ԫ(�ӽ�ԪH)
		D3P6H,

		//һ����������Ԫ(ѹ�絥Ԫ)
		D3P6E,

		//������������Ԫ(����)
		D3P15,

		//������������Ԫ(�ӽ�ԪH)
		D3P15H,

		//������������Ԫ(ѹ�絥Ԫ)
		D3P15E,

		//һ��������ʵ��ǵ�Ԫ(��������R)
		D3S6R,

		//ͨ��ʵ�嵥Ԫ
		SOLID,

		//һ�������嵥Ԫ(����)
		D3H8,

		//���������嵥Ԫ(����)
		D3H20,

		//һ�������嵥Ԫ(��������R)
		D3H8R,

		//���������嵥Ԫ(��������R)
		D3H20R,

		//һ�������嵥Ԫ(�ӽ�ԪH)
		D3H8H,

		//���������嵥Ԫ(�ӽ�ԪH)
		D3H20H,

		//һ�������嵥Ԫ(��Э��ԪI)
		D3H8I,

		//���������嵥Ԫ(��Э��ԪI)
		D3H20I,

		//һ�������嵥Ԫ(ѹ�絥Ԫ)
		D3H8E,

		//���������嵥Ԫ(ѹ�絥Ԫ)
		D3H20E,

		//һ��������ʵ��ǵ�Ԫ(��������R)
		D3S8R,

		/*
		*���ģ�鵥Ԫ
		*/

		//һ�������嵥Ԫ
		EMD3T4,

		//һ�ױ߹⻬�����嵥Ԫ
		EMD3T4_3,

		//һ����������Ԫ(����)
		EMD3P6,

		//һ�������嵥Ԫ(����)
		EMD3H8
	};

	/*
	* ����ͼ���ģ�Ͳ���ö��
	*/
	enum ModelOperateEnum
	{
		//�����ļ�����
		ImportPart,

		//������
		OperateNothing,

		//�������β���
		AppendOnePart,
		DeleteOnePart,
		HideOnePart,
		ShowOnePart,
		ColorOnePart,
		ReplaceOnePart,
		RenameOnePart,

		//ȫ�����β���
		DeleteAllPart,
		HideAllPart,
		ShowAllPart,
		ColorAllPart,
	};

	/*
	* �����ʷ�ö��
	*/
	enum MeshingEnum
	{
		//δ�ʷ�
		NoMeshing,

		//�����ʷ�
		Meshing,

		//�������ʷ�
		AdjustingMeshing,

		//ƫ�ú��ʷ�
		BiasingMeshing,

		//���������
		Adjusting,

		//�ڵ�ƫ����
		Biasing,

		//�ʷ����
		FinishMeshing,

		//��ɵ���
		FinishAdjusting,

		//���ƫ��
		FinishBiasing,
	};


	//ʰȡ����
	enum class PickObjectType
	{
		Mesh1D,

		MeshEdge,

		Mesh2D,

		MeshFace,
	};

	enum class GeoLineProperty
	{
		IndependentEdge,//������
		EdgeOnFace,//���ϵı�
		DottedTopology//�����˱�
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

		//˳��ʰȡ���ε�
		PickGeoPointOrder,

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

}
