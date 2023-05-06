#pragma once
#include"mLableDataTemplate.h"
#include"mLableDataEnum.h"
#include <QObject>
#include<QPair>
namespace MDataLable
{
	typedef QPair<QVector3D, QVector3D> POS_DIR;
	typedef QPair<QVector<QVector3D>, QVector3D> POS_COLOR;
	typedef QPair<QVector3D, QPair<QString, QVector3D>> POS_TXT_DIR;
	typedef QPair<QVector3D, QString> POS_TXT;
	struct POS_DIR_SIZE_COLOR
	{
		QVector3D pos = { 0,0,0 };
		QVector3D dir = { 0,0,0 };
		float size = 1;
		QVector3D color = { 0,0,0 };
	};
	struct POS_DIR_COLOR
	{
		QVector3D pos = { 0,0,0 };
		QVector3D dir = { 0,0,0 };
		QVector3D color = { 0,0,0 };
	};
	struct  Font {};
	struct  Lable {};
	struct  Lable_Lable {};
	struct  Lable_Font {};
	struct  Lable_Lable_Lable {};
	struct  Lable_Lable_Lable_Font {};

	//***���ﲻ��������������Ӱ���ж�***
	/*
	* ͨ��ģ��
	*/
	//�̶�����
	struct Fixed_Font_Common
	{
		LableData1<QVector<POS_TXT>> fontData;
	};
	//ͨ�����֣���ʾ�ڽڵ㴦��
	struct Common_Font_Common
	{
		LableData1<QVector<POS_TXT>> fontData;
	};
	//X����ͼ�����ݣ���ת���ģ�
	struct X_Point_Common
	{
		LableData1<QVector<QVector3D>> lableData;
	};
	//��ʱ������
	struct TempPoint_Common
	{
		LableData1<QVector<QVector3D>> lableData; //����
		LableData1<QVector<POS_TXT>> fontData;	  //����
	};
	//��ʱ�������񣨽Ӵ���ʱ�����������������ʱ������
	struct TempMesh_Common
	{
		LableData1<QVector<QVector3D>> lableData; //����
	};

	////��ͷ+���� �������ͼ�����ݣ�ȫ������ϵ��
	struct Arrow_Font_Axis_Common
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef GlobalAxis;

	//��ͷ
	struct Arrow_Common
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	};
	/*
	* ��ʽģ��
	*/
	///////////////////////////////////����ϵ//////////////////////////////////////////
	////��ͷ+���� �������ͼ�����ݣ��ֲ�����ϵ��
	struct Arrow_Font_Axis_Implicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LocalAxis_i;

	////////////////////////////////////�غ�//////////////////////////////////////////
	//��ͷ+���� �������ͼ������ �������������������ء����غɡ�ѹǿ��Ԥ��������������
	struct Arrow_Font_Load_Implicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef Force, Gravity, Moment, LineForce, Pressure, Pretension, CentrifugalForce;
	
	//������ͼ��+���� (�����ܶȡ�����ܶȣ�
	struct Triangle_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Flux, ChargeDensity;

	//����ͼ��+���� ���������ȣ��ȷ��䣩
	struct Rhombus_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Conv,HeatRadiation;


	//ʮ��ͼ��+���� ���ڲ���Դ,�¶��غɣ�
	struct X_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef InterHeat, TempLoad;

	//������ͼ��+����(���)
	struct Lightning_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Charge;
	
	////////////////////////////////////�߽�//////////////////////////////////////////
	//Լ�����ɶ�����ͼ�꣨λ��ת�� �̶�/�Գ�/���Գ�  //��ʼ�߽�����(��ʼλ�ƣ���ʼ�ٶ�)  //��ʼλ�� ����Լ����
	struct DF_BC_Implicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>λ��  QMap<unsigned int, float>  key��ʾΪ 1 2 3 4 5 6  
	}typedef Displacement, InitialVelocity, InitialDisplacement, VirtualConstraints;

	//������ ͼ�꣨��ʼ�߽�����(��ʼ�¶ȣ����¶ȱ߽磬���ƣ�
	struct Home_BC_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;  
	}typedef InitialTemperature, Temperature, Potential;
	
	////////////////////////////////////����//////////////////////////////////////////
	//��ģ������ϵ�»��Ƶĸ���ͼ�� ���������ӡ��������ӡ�MPC���ӣ�
	struct Line_Connect_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef RigidConnect, FlexConnect, MPCConnect;

	//��ģ������ϵ�»��Ƶĸ���ͼ�� ������󶨣�
	struct FaceToFace_Connect_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;  //����
		LableData1<QVector<QVector3D>> lableData2;  //����
	}typedef TieConnect;

	////////////////////////////////////�Ӵ�//////////////////////////////////////////

	//��ģ������ϵ�»��Ƶĸ���ͼ�� ���Ӵ���
	struct FaceToFace_Contact_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;  //����
		LableData1<QVector<QVector3D>> lableData2;  //����
	}typedef Contact_i;

	////////////////////////////////��������//////////////////////////////////////////
	//���ͼ��1 (��ת����ƽ�渱��Բ���θ�) 
	struct Type1_RC_Implicit
	{
		LableData2<QVector<POS_DIR>, FixShapeRotateType> lableData;  
	}typedef RevoluteRC, CylindricalRC, PlanarRC;
	//���ͼ��2 ��ƽ�Ƹ���
	struct Type2_RC_Implicit
	{
		LableData3<QVector<QVector3D>,QPair<QVector3D, QVector3D>,FixShapeRotateType> lableData;  // pos Pair<dir1, dir2>  type
	}typedef PrismaticRC;
	//ֱ������(����  ����)
	struct Line_RC_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
	}typedef SpringRC, DamperRC;
	//Բ��¸���Բ��¸���
	struct Sphere_RC_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
	}typedef SphericalRC;
	////////////////////////////////////����Լ��//////////////////////////////////////////
	struct DF_RBC_Implicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>λ��  QMap<unsigned int, float>  key��ʾΪ 1 2 3 4 5 6  
	}typedef DisplacementRBC;

	////////////////////////////////////�����̲�ʸ����ͷ//////////////////////////////////////////
	//��ͷ�������̲�����ϵ��
	struct Arrow_PlyCoordinate_Implicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef ArrowPlyCoordinate;

	/*
	* ��ʾģ��
	*/
	///////////////////////////////////����ϵ//////////////////////////////////////////
	////��ͷ+���� �������ͼ�����ݣ��ֲ�����ϵ��
	struct Arrow_Font_Axis_Explicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LocalAxis_e;
	////////////////////////////////////�غ�//////////////////////////////////////////

	//��ͷ+���� �������ͼ������ �����غɡ����غɡ�������
	struct Arrow_Font_Load_Explicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef PointLoad, FaceLoad, Gravity_e;

	//Լ�����ɶ�����ͼ�꣨�����غɣ�
	struct DF_Load_Explicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>λ��  QMap<unsigned int, float>  key��ʾΪ 1 2 3 4 5 6  
	}typedef RigidBodyLoad;

	////////////////////////////////////�߽�//////////////////////////////////////////
	//Լ�����ɶ�����ͼ�꣨λ�ơ��ٶȡ����ٶȣ�
	struct DF_BC_Explicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>λ��  QMap<unsigned int, float>  key��ʾΪ 1 2 3 4 5 6  
	}typedef Displacement_e, Velocity_e, Acceleration_e;
	//��ͷ ����ͼ������ �����ƣ�
	struct Arrow_BC_Explicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef SlidingPlane_e;

	////////////////////////////////////����//////////////////////////////////////////
	//��ģ������ϵ�»��Ƶĸ���ͼ�� ���纸���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ���������-�����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ�
	//�ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�նȣ�
	struct Line_Connect_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Spotweld_e, NodeRigidBody_e, MergeRigidBody_e, ExtraNode_e, RevoluteJoint_e, SphericalJoint_e, CylindricalJoint_e, TranslationalJoint_e, LockingJoint_e, PlanarJoint_e, UniversalJoint_e,
		GearJoint_e, PulleyJoint_e, RackPinionJoint_e, GenButtWeld_e, GenFilletWeld_e, Rivet_e, KinematicCoupling_e, DistributingCoupling_e, TieNode_e, ShellToSolid_e, Translation_e, GeneralizedStiffness_e,
		GenSpotWeld_e, TranslationalStiffness_e, SphericalStiffness_e;
	//����󶨡�ʧЧ�󶨡��Ǳ�-�Ǳ߰�
	struct FaceToFace_Connect_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;  //����
		LableData1<QVector<QVector3D>> lableData2;  //����
	}typedef Tie_e, TieBreak_e, TieEdge_e;
	
	//Լ�����ɶ�����ͼ�꣨�����˶���
	struct DF_Connect_Explicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>λ��  QMap<unsigned int, float>  key��ʾΪ 1 2 3 
	}typedef RigidBodyStoppers_e;
	////////////////////////////////////��ʼ����//////////////////////////////////////////
	//��ͷ+���� �������ͼ������ ����ʼ�ٶȣ�
	struct Arrow_Font_InitCondition_Explicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef InitVelocity_e;

	//������ͼ��+���� (��ʼӦ����SPH��ʼӦ������ʼը�㣩
	struct Triangle_Font_InitCondition_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef InitStress_e, SPHInitStress_e, InitFryingPoint_e;

	////////////////////////////////////�Ӵ�//////////////////////////////////////////
	//�����  �����
	struct FaceToFace_Contact_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;  //����
		LableData1<QVector<QVector3D>> lableData2;  //����
	}typedef PointToFace_e, FaceToFace_e;
	////////////////////////////////////����ǽ//////////////////////////////////////////
	//��
	struct Line_RW_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;	
	};
	//��
	struct Face_RW_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
	};
	//����ϵ
	struct Axis_RW_Explicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	};
	////////////////////////////////////����//////////////////////////////////////////
	//����
	struct Face_Box_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
	};
	struct Line_Box_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
	};	
	////////////////////////////////////����//////////////////////////////////////////
	//��ͷ ����ͼ������ ��������
	struct Arrow_Vector_Explicit
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef Vector_e;

	/*
	* ���ģ��
	*/
	///////////////////////////////////�غ�//////////////////////////////////////////

	//������ͼ��+����(�ڵ�/����/�� ���   �ڵ�/���� ����)
	struct Lightning_Font_Load_Magnet
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef QSource_mag, JSource_mag;
	//��ͷ+���� �������ͼ������ ���� ���/����   �Ż�ǿ�ȣ�
	struct Arrow_Font_Load_Magnet
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LineQSource_mag, LineJSource_mag, BIntensity_mag;

	//������ͼ��+���� (�������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ�ȣ�
	struct Triangle_Font_Load_Magnet
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef QDensity_mag, JDensity_mag, BDensity_mag, EIntensity_mag;

	///////////////////////////////////�߽�//////////////////////////////////////////
	//��ͷ ����ͼ������ ����ʸ��λ��
	struct Arrow_BC_Magnet
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef MagneticVector_mag;
	
	//������ ͼ�꣨������λ��
	struct Home_BC_Magnet
	{
		LableData1<QVector<QVector3D>> lableData;
	}typedef Potential_mag;
		

	///////////////////////////////////����ϵ//////////////////////////////////////////
	////��ͷ+���� �������ͼ�����ݣ��ֲ�����ϵ��
	struct Arrow_Font_Axis_Magnet
	{
		//ͼ�� QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//����  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LocalAxis_mag;


	/*
	* ����ģ��
	*/
	//��ͷ ��ʸ����ͷ��
	struct VectorArrow_Post
	{
		LableData1<QVector<POS_DIR_SIZE_COLOR>> lableData;
	};
	//�̶�����
	struct Fixed_Font_Post
	{
		LableData1<QVector<POS_TXT>> fontData;
	};
}
