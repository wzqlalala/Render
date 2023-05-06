#pragma once
#include<QHash>
#include"mLableDataStruct.h"
#include"rendlabeldata_global.h"
namespace MDataLable
{
	class RENDLABELDATA_EXPORT mLableData_common : public QObject
	{
		Q_OBJECT
	public:
		/*
		* �̶�����
		*/
		QHash<QString, Fixed_Font_Common> _font_Fixed_Common_Data;
		/*
		* ͨ�����֣�����ڽڵ㴦��
		*/
		QHash<QString, Common_Font_Common> _font_Common_Common_Data;
		/*
		* X����ͼ�꣨��ת���ģ�
		*/
		QHash<QString, X_Point_Common> _XPoint_Common_Data;
		/*
		* ��ʱ������
		*/
		QHash<QString, TempPoint_Common> _tempPoint_Common_Data;
		/*
		* ��ʱ��������
		*/
		QHash<QString, TempMesh_Common> _tempMesh_Common_Data;
		/*
		* ȫ������ϵ����
		*/
		QHash<QString, Arrow_Font_Axis_Common> _globalAxis_Common_Data;
		/*
		* ��ͷ
		*/
		QHash<QString, Arrow_Common> _arrow_Common_Data;

	};
	class RENDLABELDATA_EXPORT mLableData_implicit : public QObject
	{
		Q_OBJECT
	public:
		/*
		* ����ϵ
		*/
		//��ͷ+�����������ͼ�����ݣ�����ϵ��
		QHash<QString, Arrow_Font_Axis_Implicit> _arrow_Font_Axis_Implicit_Data;

		/*
		* �غ�
		*/
		//��ͷ+�����������ͼ�����ݣ������������������ء����غɡ�ѹǿ��Ԥ��������������
		QHash<QString, Arrow_Font_Load_Implicit> _arrow_Font_Load_Implicit_Data;
		//������+���� �������ͼ�����ݣ������ܶȣ�����ܶȣ�
		QHash<QString, Triangle_Font_Load_Implicit> _triangle_Font_Load_Implicit_Data;
		//����ͼ��+���� �������ͼ�����ݣ��������ȣ��ȷ��䣩
		QHash<QString, Rhombus_Font_Load_Implicit> _rhombus_Font_Load_Implicit_Data;
		//ʮ��ͼ��+���� �������ͼ������(�ڲ���Դ,�¶��غ�)
		QHash<QString, X_Font_Load_Implicit> _X_Font_Load_Implicit_Data;
		//������ͼ��+���� �������ͼ����Ⱦ����(���)
		QHash<QString, Lightning_Font_Load_Implicit> _lightning_Font_Load_Implicit_Data;

		/*
		* �߽�
		*/
		//Լ�����ɶ�����ͼ�����ݣ�λ��ת�� �̶�/�Գ�/���Գ�  //��ʼ�ٶ�  //��ʼλ�� ����Լ����
		QHash<QString, DF_BC_Implicit> _df_BC_Implicit_Data;
		//������ ͼ�����ݣ���ʼ�߽�����(��ʼ�¶ȣ����¶ȱ߽磬���ƣ���
		QHash<QString, Home_BC_Implicit> _home_BC_Implicit_Data;

		/*
		* ����
		*/
		//ֱ����ͼ�����ݣ����ԡ����ԡ�MPC���ӣ�
		QHash<QString, Line_Connect_Implicit> _line_Connect_Implicit_Data;
		//��������ͼ�����ݣ���������ӣ�
		QHash<QString, FaceToFace_Connect_Implicit> _faceToFace_Connect_Implicit_Data;

		/*
		* �Ӵ�
		*/
		//��������ͼ�����ݣ��Ӵ���
		QHash<QString, FaceToFace_Contact_Implicit> _faceToFace_Contact_Implicit_Data;

		/*
		* ��������
		*/
		//��ת����ƽ�渱��Բ���θ�
		QHash<QString, Type1_RC_Implicit> _Type1_RC_Implicit_Data;
		//ƽ�Ƹ�
		QHash<QString, Type2_RC_Implicit> _Type2_RC_Implicit_Data;
		//ֱ���ͣ����� ���ᣩ
		QHash<QString, Line_RC_Implicit> _line_RC_Implicit_Data;
		//Բ��¸���Բ��¸���
		QHash<QString, Sphere_RC_Implicit> _sphere_RC_Implicit_Data;

		/*
		* ����Լ��
		*/
		//Լ�����ɶ�����ͼ�����ݣ�����Լ����
		QHash<QString, DF_RBC_Implicit> _df_RBC_Implicit_Data;

		/*
		* �����̲�
		*/
		//��ͷ�������̲�����ϵ��
		QHash<QString, Arrow_PlyCoordinate_Implicit> _arrow_PlyCoordinate_Implicit_Data;
		

	};
	class RENDLABELDATA_EXPORT mLableData_explicit : public QObject
	{
		Q_OBJECT
	public:
		/*
		* ����ϵ
		*/
		//��ͷ+�����������ͼ�����ݣ�����ϵ��
		QHash<QString, Arrow_Font_Axis_Explicit> _arrow_Font_Axis_Explicit_Data;
		/*
		* �غ�
		*/
		//��ͷ+�����������ͼ�����ݣ����غɡ����غɡ�������
		QHash<QString, Arrow_Font_Load_Explicit> _arrow_Font_Load_Explicit_Data;
		//Լ�����ɶ�����ͼ�����ݣ������غɣ�
		QHash<QString, DF_Load_Explicit> _df_Load_Explicit_Data;
		/*
		* �߽�
		*/
		//Լ�����ɶ�����ͼ�����ݣ�λ�� �ٶ� ���ٶȣ�
		QHash<QString, DF_BC_Explicit> _df_BC_Explicit_Data;
		//��ͷ����ͼ�����ݣ����ƣ�
		QHash<QString, Arrow_BC_Explicit> _arrow_BC_Explicit_Data;
		/*
		* ����
		*/
		//ֱ����ͼ�����ݣ��纸���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ���������-�����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ�
		//�ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�նȣ�
		QHash<QString, Line_Connect_Explicit> _line_Connect_Explicit_Data;
		//��������ͼ�����ݣ�����󶨡�ʧЧ�����ӣ�
		QHash<QString, FaceToFace_Connect_Explicit> _faceToFace_Connect_Explicit_Data;
		//Լ�����ɶ�����ͼ�����ݣ������˶���
		QHash<QString, DF_Connect_Explicit> _df_Connect_Explicit_Data;
		/*
		* �Ӵ�
		*/
		//��������ͼ�����ݣ������ ����棩
		QHash<QString, FaceToFace_Contact_Explicit> _faceToFace_Contact_Explicit_Data;
		/*
		* ��ʼ����
		*/
		//��ͷ+�����������ͼ�����ݣ���ʼ�ٶȣ�
		QHash<QString, Arrow_Font_InitCondition_Explicit> _arrow_Font_InitCondition_Explicit_Data;
		//������+���� �������ͼ�����ݣ���ʼӦ����SPH��ʼӦ������ʼը�㣩
		QHash<QString, Triangle_Font_InitCondition_Explicit> _triangle_Font_InitCondition_Explicit_Data;
		/*
		* ����ǽ
		*/
		//��
		QHash<QString, Line_RW_Explicit> _line_RW_Explicit_Data;
		//��
		QHash<QString, Face_RW_Explicit> _face_RW_Explicit_Data;
		//����ϵ
		QHash<QString, Axis_RW_Explicit> _axis_RW_Explicit_Data;
		/*
		* ����
		*/
		//��
		QHash<QString, Face_Box_Explicit> _face_Box_Explicit_Data;
		//��
		QHash<QString, Line_Box_Explicit> _line_Box_Explicit_Data;
		/*
		* ����
		*/
		//��ͷ+�����������ͼ�����ݣ�������
		QHash<QString, Arrow_Vector_Explicit> _arrow_Vector_Explicit_Data;


	};
	class RENDLABELDATA_EXPORT mLableData_fluid : public QObject
	{
		Q_OBJECT
	};
	class RENDLABELDATA_EXPORT mLableData_magnet : public QObject
	{
		Q_OBJECT
	public:
		/*
		* ����ϵ
		*/
		//��ͷ+�����������ͼ�����ݣ�����ϵ��
		QHash<QString, Arrow_Font_Axis_Magnet> _arrow_Font_Axis_Magnet_Data;
		/*
		* �غ�
		*/
		//��ͷ+�����������ͼ�����ݣ��� ���/����   �Ż�ǿ�ȣ�
		QHash<QString, Arrow_Font_Load_Magnet> _arrow_Font_Load_Magnet_Data;
		//������+���� �������ͼ�����ݣ��������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ�ȣ�
		QHash<QString, Triangle_Font_Load_Magnet> _triangle_Font_Load_Magnet_Data;
		//������ͼ��+���� �������ͼ����Ⱦ����(�ڵ�/����/�� ���   �ڵ�/���� ����)
		QHash<QString, Lightning_Font_Load_Magnet> _lightning_Font_Load_Magnet_Data;
		/*
		* �߽�
		*/
		//��ͷ+�����������ͼ�����ݣ���ʸ��λ��
		QHash<QString, Arrow_BC_Magnet> _arrow_BC_Magnet_Data;
		//������ ͼ�����ݣ�������λ��
		QHash<QString, Home_BC_Magnet> _home_BC_Magnet_Data;
	};
	class RENDLABELDATA_EXPORT mLableData_post : public QObject
	{
		Q_OBJECT
	public:


		/*
		* ʸ����ͷ
		*/
		QHash<int, QHash<QString, VectorArrow_Post>> _vecArrow_Post_Data;
		/*
		* �̶�����
		*/
		QHash<int, QHash<QString, Fixed_Font_Post>> _font_Fixed_Post_Data;
		
	};
}