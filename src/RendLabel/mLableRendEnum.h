#pragma once
namespace MLableRend
{
	/*
	* ��ɫ������
	*/
	enum ShaderType
	{
		//����
		ShaderType_Font_Arrow,//��ͷ���˵�����
		ShaderType_Font_FixShape,//�̶�ͼ�������
		ShaderType_Font_Common,//ͨ����������
		ShaderType_Font_Fixed,//�̶���������
		ShaderType_Font_GlobalAxis,//ȫ������ϵ����
		//ͼ��
		ShaderType_GlobalAxis, //��ͷ����
		ShaderType_Arrow, //��ͷ����
		ShaderType_FixShape, //�̶���״����
		ShaderType_BCDF, //Լ�����ɶ�����ͼ��
		ShaderType_Model,//ģ������ϵ�»���ͼ���ͨ����ɫ��
		ShaderType_Model_Point, //ģ������ϵ�»��Ƶ�
		ShaderType_FixShapeRotate,//���ڹ̶���״+��ת����
		ShaderType_FixShapeRotate2,//���ڹ̶���״+��ת2������
	};

	/*
	* ͼ��������ͣ���ɾ������ʾ�����ء�������ȡ�������ȣ�
	*/
	enum  LableOperateMode
	{
		Append, //���
		Delete, //ɾ��
		Show,	//��ʾ
		Hide,	//����
		Highlight, //����
		NoHighlight, //ȡ������
		SetColor,	 //������ɫ
		SetSize		 //���ô�С
	};


	/*
	* �������ͣ�ǰ��������
	*/
	enum InterfaceType
	{
		InterfaceType_Pre, //ǰ����
		InterfaceType_Post, //����
	};

	/*
	* ��������
	*/
	enum FontType
	{
		FontType_Commom,  //�������ֻ��߹̶�����
		FontType_Arrow,		//��ͷĩ������
		FontType_FixShape,	//�̶�ͼ�괦������
	};
	/*
	* �ֲ�����������
	*/
	enum LocalAxisType
	{
		LocalAxisType_Rectangular,		//�ֲ�ֱ������ϵ
		LocalAxisType_Cylinder,			//�ֲ�ֱ������ϵ
		LocalAxisType_Sphere,			//�ֲ�������ϵ
	};

}