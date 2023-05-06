#pragma once
namespace MDataLable
{
	//ͼ��������� ���غɡ��߽硢���ӵȣ�
	enum  LableOperateType
	{
		LableOperateType_FixedFont,	//�̶�����
		LableOperateType_CommonFont,	//ͨ������
		LableOperateType_XPoint,	//��ת����
		LableOperateType_TempPoint,	//��ʱ������
		LableOperateType_TempMesh,	//��ʱ��������
		LableOperateType_GlobalAxis,	//ȫ������ϵ
		LableOperateType_Arrow,	//��ͷ

		LableOperateType_LocalAxis,//�ֲ�����ϵ
		LableOperateType_Load,//�غ�
		LableOperateType_BC,//�߽�
		LableOperateType_Connect,//����
		LableOperateType_Contact,//�Ӵ�
		LableOperateType_RigidConnect, //��������
		LableOperateType_RigidConstraint, //����Լ��
		LableOperateType_RigidWall ,//����ǽ
		LableOperateType_InitCondition,//��ʼ����
		LableOperateType_Box,//����
		LableOperateType_Vector,//����
		LableOperateType_PlyCoordinate,//�����̲�����ϵ

		//����
		LableOperateType_VecArrow,//ʸ����ͷ
		LableOperateType_FixedFont_Post,//�̶����֣�֡�󶨣�
		
	};
	//�̶���״����ת����
	enum FixShapeRotateType
	{
		//��������  ��ת��
		FixShapeRotateType_Revolute,
		//��������  ƽ�渱
		FixShapeRotateType_Planar,
		//��������  Բ���θ�
		FixShapeRotateType_Cylindrical,
		//��������  ƽ�Ƹ�
		FixShapeRotateType_Prismatic,
	};
	/*
	* ͼ���������ͣ�����״̬������״̬��
	*/
	enum  LableDataType
	{
		LableDataType_ShowState,
		LableDataType_HighLightState,
	};
}