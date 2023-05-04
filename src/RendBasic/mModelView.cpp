#include "mModelView.h"
#include "mViewToolClass.h"
#include"mMeshViewEnum.h"


namespace MViewBasic
{
	mModelView::mModelView()
	{
		InitialModelView();
	}
	mModelView::~mModelView()
	{

	}
	void mModelView::InitialModelView()
	{
		InitialOrtho();
		InitialCamera();
	}
	void mModelView::InitialOrtho()
	{
		//����ͶӰ����
		_Left_ini = -1;
		_Right_ini = 1;
		_Bottom_ini = -1;
		_Top_ini = 1;
		_NearPlane = 0.1;
		_FarPlane = 99;
		_Left_res = _Left_ini;
		_Right_res = _Right_ini;
		_Bottom_res = _Bottom_ini;
		_Top_res = _Top_ini;
		

	}
	void mModelView::InitialCamera()
	{
		//�ӽǲ���
		_Eye = QVector3D(0, 0, 1);
		_Eye_0 = QVector3D(0, 0, 1);
		_Center = QVector3D(0, 0, 0);
		_Up = QVector3D(0, 1, 0);
		_MaxRadius = 1.0;
	}
	void mModelView::SetOrthoByRatio(float scr_width, float scr_height)
	{
		SCR_WIDTH = scr_width;
		SCR_HEIGHT = scr_height;
		
		_Ratio_New = scr_width / scr_height; //�µ���Ļ��С����
		if (scr_width <= scr_height)
		{
			_Left = _Left_res;
			_Right = _Right_res;
			_Bottom = _Bottom_res / _Ratio_New;
			_Top = _Top_res / _Ratio_New;
		}
		else
		{
			_Left = _Left_res * _Ratio_New;
			_Right = _Right_res * _Ratio_New;
			_Bottom = _Bottom_res;
			_Top = _Top_res;
		}
		_Ratio_Old = _Ratio_New;//���� �ɵ���Ļ��С����
		SetPVMValue();
	}
	void mModelView::ReturnOrthoByRatio()
	{

		if (SCR_WIDTH <= SCR_HEIGHT)
		{
			_Left_res = _Left;
			_Right_res = _Right;
			_Top_res = _Top * _Ratio_Old;
			_Bottom_res = _Bottom * _Ratio_Old;
		}
		else
		{
			_Left_res = _Left / _Ratio_Old;
			_Right_res = _Right / _Ratio_Old;
			_Top_res = _Top;
			_Bottom_res = _Bottom;
		}
	}

	void mModelView::Translate(float xoffset, float yoffset)
	{
		float XOffsetOfOrtho = xoffset * (_Right - _Left) / SCR_WIDTH;
		float YOffsetOfOrtho = yoffset * (_Top - _Bottom) / SCR_HEIGHT;
		_Left += XOffsetOfOrtho;
		_Right += XOffsetOfOrtho;
		_Top += YOffsetOfOrtho;
		_Bottom += YOffsetOfOrtho;

		//��ԭͨ����Ļ��С��������ͶӰǰ�Ĳ��������ڱ���ģ�͵�ǰ�ı任λ�á�
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::Rotate(float xoffset, float yoffset, RotateType rotateType, float sensitivity)
	{
		QVector3D _AuxY = QVector3D(_Up.x(), _Up.y(), _Up.z()).normalized(); //������
		QVector3D _AuxZ = QVector3D(_Eye_0.x() - 0, _Eye_0.y() - 0, _Eye_0.z() - 0).normalized();
		QVector3D _AuxX = _AuxX.crossProduct(_AuxY, _AuxZ); //Y���Z���˽��ΪX��
		if (rotateType == Rotate_XY)
		{
			QVector3D _MouseTrace = _AuxX * xoffset + _AuxY * yoffset;//���켣����
			//��ȡ��ת������������ϵ�еı�ʾ
			QVector3D  _AxisRotate = _AxisRotate.crossProduct(_AuxZ, _MouseTrace).normalized();    //��ת��
			//������ת����
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(-_MouseTrace.length()*sensitivity, _AxisRotate); //������ת����
			//��ת
			QVector4D NewEye4D = QVector4D(_Eye_0, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//����ͶӰ�ӽ���ԭ�㴦����ת
			_Eye_0 = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			//����ͶӰ�ӽ���ԭ����ת�����ƶ����������ڵ�λ��
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (rotateType == Rotate_Z)
		{
			QVector3D _MouseTrace = _AuxX * xoffset;//���켣����
				//��ȡ��ת������������ϵ�еı�ʾ
			QVector3D  _AxisRotate = _AuxZ;    //��ת��
			//������ת����
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			if (xoffset <= 0)
			{
				_MatrixRotate.rotate(-_MouseTrace.length()*sensitivity, _AxisRotate); //������ת����
			}
			else
			{
				_MatrixRotate.rotate(_MouseTrace.length()*sensitivity, _AxisRotate); //������ת����
			}
			//��ת
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//����ͶӰ�ӽ���ԭ����ת�����ƶ����������ڵ�λ��
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		SetPVMValue();
	}
	void mModelView::Rotate_ByBotton(float angle)
	{
		QVector3D _AuxZ = QVector3D(_Eye_0.x() - 0, _Eye_0.y() - 0, _Eye_0.z() - 0).normalized();
		//������ת����
		QMatrix4x4 _MatrixRotate;
		_MatrixRotate.setToIdentity();
		_MatrixRotate.rotate(angle, _AuxZ); 
		//��ת
		QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
		//����ͶӰ�ӽ���ԭ����ת�����ƶ����������ڵ�λ��
		_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		SetPVMValue();
	}
	void mModelView::ZoomAtMouse_Bywheel(int posX, int posY, int rollervalue, ScalePerspectice scalePerspectice, float sensitivity)
	{
		double ZoomAtPointXv = (posX - SCR_WIDTH / 2)* (_Right - _Left) / SCR_WIDTH;
		double ZoomAtPointYv = -(posY - SCR_HEIGHT / 2)* (_Top - _Bottom) / SCR_HEIGHT;
		//���Ź���������ӿ�����
		double ZoomAtPointXv_end;
		double ZoomAtPointYv_end;
		//�ӿڵĿ�Ⱥ͸߶�
		double width = _Right - _Left;
		double height = _Top - _Bottom;
		//�ӿڵ����ĵ�����
		double Zerox = (_Left + _Right) / 2;
		double Zeroy = (_Bottom + _Top) / 2;
		//�������ŵķ���
		if (scalePerspectice == Eye)
		{
			if (rollervalue <= 0)
			{

				//���Ź���������ӿ�����
				ZoomAtPointXv_end = ZoomAtPointXv * sensitivity;
				ZoomAtPointYv_end = ZoomAtPointYv * sensitivity;
				//����
				width *= sensitivity;
				height *= sensitivity;
				//�����ӿ��������ź���ӿڵı߽�����
				_Left = Zerox - width / 2;
				_Right = Zerox + width / 2;
				_Bottom = Zeroy - height / 2;
				_Top = Zeroy + height / 2;

				//��������������ź���ӿڵı߽����꣨���գ�

				_Left += ZoomAtPointXv - ZoomAtPointXv_end;
				_Right += ZoomAtPointXv - ZoomAtPointXv_end;
				_Bottom += ZoomAtPointYv - ZoomAtPointYv_end;
				_Top += ZoomAtPointYv - ZoomAtPointYv_end;

				_ScaleProportion *= sensitivity;

			}
			else
			{
				ZoomAtPointXv_end = ZoomAtPointXv / sensitivity;
				ZoomAtPointYv_end = ZoomAtPointYv / sensitivity;

				width /= sensitivity;
				height /= sensitivity;

				_Left = Zerox - width / 2;
				_Right = Zerox + width / 2;
				_Bottom = Zeroy - height / 2;
				_Top = Zeroy + height / 2;

				_Left += ZoomAtPointXv - ZoomAtPointXv_end;
				_Right += ZoomAtPointXv - ZoomAtPointXv_end;
				_Bottom += ZoomAtPointYv - ZoomAtPointYv_end;
				_Top += ZoomAtPointYv - ZoomAtPointYv_end;

				_ScaleProportion /= sensitivity;
			}
		}
		else if (scalePerspectice == Model)
		{
			if (rollervalue > 0)
			{

				//���Ź���������ӿ�����
				ZoomAtPointXv_end = ZoomAtPointXv * sensitivity;
				ZoomAtPointYv_end = ZoomAtPointYv * sensitivity;
				//����
				width *= sensitivity;
				height *= sensitivity;
				//�����ӿ��������ź���ӿڵı߽�����
				_Left = Zerox - width / 2;
				_Right = Zerox + width / 2;
				_Bottom = Zeroy - height / 2;
				_Top = Zeroy + height / 2;

				//��������������ź���ӿڵı߽����꣨���գ�

				_Left += ZoomAtPointXv - ZoomAtPointXv_end;
				_Right += ZoomAtPointXv - ZoomAtPointXv_end;
				_Bottom += ZoomAtPointYv - ZoomAtPointYv_end;
				_Top += ZoomAtPointYv - ZoomAtPointYv_end;

				_ScaleProportion *= sensitivity;

			}
			else
			{
				ZoomAtPointXv_end = ZoomAtPointXv / sensitivity;
				ZoomAtPointYv_end = ZoomAtPointYv / sensitivity;

				width /= sensitivity;
				height /= sensitivity;

				_Left = Zerox - width / 2;
				_Right = Zerox + width / 2;
				_Bottom = Zeroy - height / 2;
				_Top = Zeroy + height / 2;

				_Left += ZoomAtPointXv - ZoomAtPointXv_end;
				_Right += ZoomAtPointXv - ZoomAtPointXv_end;
				_Bottom += ZoomAtPointYv - ZoomAtPointYv_end;
				_Top += ZoomAtPointYv - ZoomAtPointYv_end;

				_ScaleProportion /= sensitivity;
			}
		}


		//��ԭͨ����Ļ��С��������ͶӰǰ�Ĳ��������ڱ���ģ�͵�ǰ�ı任λ�á�
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::ZoomAtFrameCenter(int CenterposX, int CenterposY, float sensitivity)
	{
		QVector2D ZoomCenter = mViewToolClass::PixelToOrthoPosition(CenterposX, CenterposY, _Left, _Right, _Bottom, _Top, SCR_WIDTH, SCR_HEIGHT);

		//�ӿڵĿ�Ⱥ͸߶�
		double width = _Right - _Left;
		double height = _Top - _Bottom;
	
		width /= sensitivity;
		height /= sensitivity;

		_ScaleProportion /= sensitivity;

		_Left = ZoomCenter.x() - width / 2;
		_Right = ZoomCenter.x() + width / 2;
		_Bottom = ZoomCenter.y() - height / 2;
		_Top = ZoomCenter.y() + height / 2;

		//��ԭͨ����Ļ��С��������ͶӰǰ�Ĳ��������ڱ���ģ�͵�ǰ�ı任λ�á�
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::ZoomAtViewCenter_ByBotton( ScaleDirection scaleDirection, float sensitivity)
	{
		QVector2D ViewCenter = QVector2D((_Left + _Right) / 2, (_Top + _Bottom) / 2);//��ͼ���ĵ���ͼ����
		float width = _Right - _Left;
		float height = _Top - _Bottom;
		if (scaleDirection == Zoom_in)
		{
			width *= sensitivity;
			height *= sensitivity;
			_ScaleProportion *= sensitivity;
		}
		else if (scaleDirection == Zoom_out)
		{
			width /= sensitivity;
			height /= sensitivity;
			_ScaleProportion /= sensitivity;
		}
		_Left = ViewCenter.x() - width / 2;
		_Right = ViewCenter.x() + width / 2;
		_Bottom = ViewCenter.y() - height / 2;
		_Top = ViewCenter.y() + height / 2;
		//��ԭͨ����Ļ��С��������ͶӰǰ�Ĳ��������ڱ���ģ�͵�ǰ�ı任λ�á�
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::ZoomAtViewCenter_ByPercent(float percent)
	{
		QVector2D ViewCenter = QVector2D((_Left + _Right) / 2, (_Top + _Bottom) / 2);//��ͼ���ĵ���ͼ����

		float left, right, top, bottom;
		//��¼��ʼ״̬�µĴ�С
		if (SCR_WIDTH <= SCR_HEIGHT)
		{
			left = _Left_ini;
			right = _Right_ini;
			bottom = _Bottom_ini / _Ratio_New;
			top = _Top_ini / _Ratio_New;
		}
		else
		{
			left = _Left_ini * _Ratio_New;
			right = _Right_ini * _Ratio_New;
			bottom = _Bottom_ini;
			top = _Top_ini;
		}
		float width, height;
		width = (right - left)/(percent / 100);
		height = (top - bottom)/(percent / 100);

		_ScaleProportion /= (percent / 100);

		_Left = ViewCenter.x() - width / 2;
		_Right = ViewCenter.x() + width / 2;
		_Bottom = ViewCenter.y() - height / 2;
		_Top = ViewCenter.y() + height / 2;
		//��ԭͨ����Ļ��С��������ͶӰǰ�Ĳ��������ڱ���ģ�͵�ǰ�ı任λ�á�
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::ZoomAtViewCenter_ByMove(int FirstPosx, int FirstPosy, int nowPosx, int nowPosy, float sensitivity )
	{
		if (IfFirstMouse == true)
		{
			width_firstmouse = _Right - _Left;
			height_firstmouse = _Top - _Bottom;
			IfFirstMouse = false;
		}
		QVector2D ViewCenter = QVector2D((_Left + _Right) / 2, (_Top + _Bottom) / 2);//��ͼ���ĵ���ͼ����
		int offset = (nowPosx - FirstPosx)+ (FirstPosy -nowPosy);
		float width,height;
		float Sens;//����������
		if (offset >= 0)
		{
			float value = offset / (SCR_WIDTH + SCR_HEIGHT) * 9+1 ;//��offsetֵ�ķ�Χӳ�䵽(1-10)
			Sens = value * sensitivity;
			width = width_firstmouse / Sens;
			height = height_firstmouse/ Sens;
			_ScaleProportion /= sensitivity;
			
		}
		else if (offset < 0)
		{
			float value = -offset / (SCR_WIDTH + SCR_HEIGHT) * 9+1 ;//��offsetֵ�ķ�Χӳ�䵽(1-10)
			Sens = value * sensitivity;
			width = width_firstmouse * Sens;
			height = height_firstmouse * Sens;
			_ScaleProportion *= sensitivity;

		}
		_Left = ViewCenter.x() - width / 2;
		_Right = ViewCenter.x() + width / 2;
		_Bottom = ViewCenter.y() - height / 2;
		_Top = ViewCenter.y() + height / 2;

		//��ԭͨ����Ļ��С��������ͶӰǰ�Ĳ��������ڱ���ģ�͵�ǰ�ı任λ�á�
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::SetPerspective(Perspective type)
	{
		if (type == Front)//�����ӽ�ΪX������
		{
			_Eye_0 = QVector3D(0.0, 0.0, _MaxRadius + 1);
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(0.0, 1.0, 0.0);

		}
		else if (type == Rear)
		{
			_Eye_0 = QVector3D(0.0, 0.0, -(_MaxRadius + 1));
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(0.0, 1.0, 0.0);
		}
		else if (type == Top)
		{
			_Eye_0 = QVector3D(0.0, _MaxRadius + 1, 0.0);
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(0.0, 0.0, -1.0);
		}
		else if (type == Bottom)
		{
			_Eye_0 = QVector3D(0.0, -(_MaxRadius + 1), 0.0);
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(0.0, 0.0, 1.0);
		}
		else if (type == Left)
		{
			_Eye_0 = QVector3D(-(_MaxRadius + 1), 0.0, 0.0);
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(0.0, 1.0, 0.0);
		}
		else if (type == Right)
		{
			_Eye_0 = QVector3D((_MaxRadius + 1), 0.0, 0.0);
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(0.0, 1.0, 0.0);
		}
		else if (type == ISO1)
		{
			_Eye_0 = QVector3D(0, 0, _MaxRadius + 1);
			_Up = QVector3D(0.0, 1.0, 0.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(30, QVector3D(1, 0, 0));
			_MatrixRotate.rotate(-45, QVector3D(0, 1, 0));
			QVector4D NewEye4D = QVector4D(_Eye_0, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//����ͶӰ�ӽ���ԭ�㴦����ת
			_Eye_0 = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (type == ISO2)
		{
			_Eye_0 = QVector3D(0, 0, _MaxRadius + 1);
			_Up = QVector3D(0.0, 1.0, 0.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(30, QVector3D(1, 0, 0));
			_MatrixRotate.rotate(45, QVector3D(0, 1, 0));
			QVector4D NewEye4D = QVector4D(_Eye_0, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//����ͶӰ�ӽ���ԭ�㴦����ת
			_Eye_0 = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (type == ISO3)
		{
			_Eye_0 = QVector3D(_MaxRadius + 1, 0, 0);
			_Up = QVector3D(0.0, 0.0, 1.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(30, QVector3D(0, 1, 0));
			_MatrixRotate.rotate(45, QVector3D(0, 0, 1));
			QVector4D NewEye4D = QVector4D(_Eye_0, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//����ͶӰ�ӽ���ԭ�㴦����ת
			_Eye_0 = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (type == ISO4)
		{
			_Eye_0 = QVector3D(-(_MaxRadius + 1), 0, 0);
			_Up = QVector3D(0.0, 1.0, 0.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(-30, QVector3D(0, 0, 1));
			_MatrixRotate.rotate(45, QVector3D(0, 1, 0));
			QVector4D NewEye4D = QVector4D(_Eye_0, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//����ͶӰ�ӽ���ԭ�㴦����ת
			_Eye_0 = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}

		SetPVMValue();
		
	}
	void mModelView::FitView(QVector3D center)
	{
		QVector3D GeoCenter = center;//������������
		QVector3D GeoCenter_norm = mViewToolClass::ModelToNormPosition(GeoCenter, _projection, _view, _model);//����������OpenGL��׼����ϵ�µ�����
		QVector2D ViewCenter = QVector2D((_Left + _Right) / 2, (_Top + _Bottom) / 2);//��ͼ���ĵ���ͼ����
		//������������ͼ����ϵ�µ�����
		QVector2D GeoCenter_view = QVector2D(ViewCenter.x() + GeoCenter_norm.x()*(_Right - _Left) / 2, ViewCenter.y() + GeoCenter_norm.y()*(_Top - _Bottom) / 2);
		//Ϊ�˱�֤ģ��λ�ô������ģ��ƶ�����������Ĳ���
		float Xoffset = GeoCenter_view.x() - ViewCenter.x();
		float Yoffset = GeoCenter_view.y() - ViewCenter.y();
		_Left += Xoffset;
		_Right += Xoffset;
		_Bottom += Yoffset;
		_Top += Yoffset;
		//��ԭ����ǰ����ͼ��С
		float Ratio = float(SCR_WIDTH / SCR_HEIGHT); //�µ���Ļ��С����
		float width, height;//��ʼ��������ͶӰ�Ŀ�Ⱥ͸߶��ڵ�һ�θ�����Ļ�������ֵ
		if (SCR_WIDTH <= SCR_HEIGHT)
		{
			width = _Right_ini - _Left_ini;
			height = (_Top_ini - _Bottom_ini) / Ratio;
		}
		else
		{
			width = (_Right_ini - _Left_ini) * Ratio;
			height = _Top_ini - _Bottom_ini;
		}
		ViewCenter = QVector2D((_Left + _Right) / 2, (_Bottom + _Top) / 2);
		_Left = ViewCenter.x() - width / 2;
		_Right = ViewCenter.x() + width / 2;
		_Bottom = ViewCenter.y() - height / 2;
		_Top = ViewCenter.y() + height / 2;

		_ScaleProportion = 1;
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::SaveCurrentView()
	{
		_Eye_Saved = _Eye;
		_Eye_0_Saved = _Eye_0;
		_Center_Saved = _Center;
		_Up_Saved = _Up;
		_MaxRadius_Saved = _MaxRadius;
		_Left_Saved = _Left; _Right_Saved = _Right; _Bottom_Saved = _Bottom; _Top_Saved = _Top; _NearPlane_Saved = _NearPlane; _FarPlane_Saved = _FarPlane;
		_Left_ini_Saved = _Left_ini; _Right_ini_Saved = _Right_ini; _Bottom_ini_Saved = _Bottom_ini; _Top_ini_Saved = _Top_ini;
		_Left_res_Saved = _Left_res; _Right_res_Saved = _Right_res; _Bottom_res_Saved = _Bottom_res; _Top_res_Saved = _Top_res;
		_projection_Saved = _projection; _view_Saved = _view; _model_Saved = _model;
	}
	void mModelView::CallSavedView()
	{
		_Eye= _Eye_Saved;
		_Eye_0 = _Eye_0_Saved;
		_Center = _Center_Saved;
		_Up = _Up_Saved;
		_MaxRadius = _MaxRadius_Saved;
		_Left = _Left_Saved; _Right = _Right_Saved; _Bottom= _Bottom_Saved; _Top = _Top_Saved; _NearPlane = _NearPlane_Saved; _FarPlane = _FarPlane_Saved;
		_Left_ini= _Left_ini_Saved; _Right_ini= _Right_ini_Saved; _Bottom_ini= _Bottom_ini_Saved; _Top_ini = _Top_ini_Saved;
		_Left_res = _Left_res_Saved; _Right_res = _Right_res_Saved; _Bottom_res = _Bottom_res_Saved; _Top_res = _Top_res_Saved;
		_projection = _projection_Saved; _view = _view_Saved; _model = _model_Saved;
	}

	void mModelView::SetRotateCenterToPoint(QVector3D center, float maxRadius)
	{
		QVector3D lastCenter = _Center; //��һ�ε���ת����
		//��ȡ���ص��ģ������
		QVector3D newCenter = center;// mViewToolClass::PixelToModelPosition(Posx, Posy, projection, view, model, SCR_WIDTH, SCR_HEIGHT);
		//��������ת�뾶
		_MaxRadius = maxRadius;
		//����������ת���Ĺ���Ϊ�˱�֤ģ��λ�ò���ı䣬�ƶ�����������Ĳ���
		QVector3D lastCenter_view = mViewToolClass::ModelToViewPosition(lastCenter, _view, _model);
		QVector3D newCenter_view = mViewToolClass::ModelToViewPosition(center, _view, _model);
		float Xoffset = newCenter_view.x() - lastCenter_view.x();
		float Yoffset = newCenter_view.y() - lastCenter_view.y();

		_Left -= Xoffset;
		_Right -= Xoffset;
		_Bottom -= Yoffset;
		_Top -= Yoffset;

		//���������ӽǲ���
		_Center = newCenter;
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1); //��������ԭ�㴦������۾�����
		_Eye = QVector3D(_Eye_0.x() + newCenter.x(), _Eye_0.y() + newCenter.y(), _Eye_0.z() + newCenter.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;

		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::SetRotateCenterToViewCenter(QVector3D center, float maxRadius)
	{
		QVector3D lastCenter = _Center; //��һ�ε���ת����
		QVector3D ViewCenter_norm = QVector3D(0, 0, 0); //��ͼ���ĵı�׼���꣬���ֵΪ0
		QVector3D ViewCenter_model = center;
		//��������ת�뾶
		_MaxRadius = maxRadius;
		//����������ת���Ĺ���Ϊ�˱�֤ģ��λ�ò���ı䣬�ƶ�����������Ĳ���
		QVector3D lastCenter_view = mViewToolClass::ModelToViewPosition(lastCenter, _view, _model);
		QVector3D newCenter_view = mViewToolClass::ModelToViewPosition(center, _view, _model);
		float Xoffset = newCenter_view.x() - lastCenter_view.x();
		float Yoffset = newCenter_view.y() - lastCenter_view.y();
		_Left -= Xoffset;
		_Right -= Xoffset;
		_Bottom -= Yoffset;
		_Top -= Yoffset;

		//���������ӽǲ���
		_Center = ViewCenter_model;
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1); //��������ԭ�㴦������۾�����
		_Eye = QVector3D(_Eye_0.x() + ViewCenter_model.x(), _Eye_0.y() + ViewCenter_model.y(), _Eye_0.z() + ViewCenter_model.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;

		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::SetRotateCenterToModelCenter(QVector3D center, float maxRadius)
	{
		QVector3D lastCenter = _Center; //��һ�ε���ת����

		//��������ת�뾶
		_MaxRadius = maxRadius;
		//����������ת���Ĺ���Ϊ�˱�֤ģ��λ�ò���ı䣬�ƶ�����������Ĳ���
		QVector3D lastCenter_view = mViewToolClass::ModelToViewPosition(lastCenter, _view, _model);
		QVector3D newCenter_view = mViewToolClass::ModelToViewPosition(center, _view, _model);
		float Xoffset = newCenter_view.x() - lastCenter_view.x();
		float Yoffset = newCenter_view.y() - lastCenter_view.y();

		_Left -= Xoffset;
		_Right -= Xoffset;
		_Bottom -= Yoffset;
		_Top -= Yoffset;

		//���������ӽǲ���
		_Center = center;
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1); //��������ԭ�㴦������۾�����
		_Eye = QVector3D(_Eye_0.x() + center.x(), _Eye_0.y() + center.y(), _Eye_0.z() + center.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;

		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mModelView::ResetOrthoAndCamera(QVector3D center, float maxRadius)
	{
		//��ȡģ�����ĺ������ת�뾶
		_Center = center;
		_MaxRadius = maxRadius;
		//��ʼ������ӽǣ�������һ�ε��ӽ�λ�ã�
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1); //��������ԭ�㴦������۾�����
		_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());

		//��ȡ����ͶӰ��ʼ������
		_Left_ini = -_MaxRadius * 1.1;
		_Right_ini = _MaxRadius * 1.1;
		_Bottom_ini = -_MaxRadius * 1.1;
		_Top_ini = _MaxRadius * 1.1;

		_Left_res = _Left_ini;
		_Right_res = _Right_ini;
		_Bottom_res = _Bottom_ini;
		_Top_res = _Top_ini;
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;

		//���������������ͶӰ
		SetOrthoByRatio(SCR_WIDTH, SCR_HEIGHT);
		//����PVM����
		SetPVMValue();
	}
	void mModelView::UpdateOrthoAndCamera(QVector<QVector3D> Allvertex, QVector<QVector3D> AddVertex)
	{
		if (Allvertex.size() == 0 || AddVertex.size() == 0)
		{
			return;
		}
		_MaxRadius = mViewToolClass::GetMaxRadius(Allvertex, _Center);
		//���³�ʼֵ
		_Left_ini = -_MaxRadius * 1.1;
		_Right_ini = _MaxRadius * 1.1;
		_Bottom_ini = -_MaxRadius * 1.1;
		_Top_ini = _MaxRadius * 1.1;
		//ģ�����껻��Ortho����
		for (int i = 0; i < AddVertex.size(); i++)
		{
			QVector2D PixelPos = mViewToolClass::ModelToPixelPositon(AddVertex[i], _projection, _view, _model, SCR_WIDTH, SCR_HEIGHT);
			QVector2D OrthoPos = mViewToolClass::PixelToOrthoPosition(PixelPos.x(), PixelPos.y(), _Left, _Right, _Bottom, _Top, SCR_WIDTH, SCR_HEIGHT);
			//�жϸõ��Ƿ�����ͼ��
			if (OrthoPos.x() < _Left || OrthoPos.x() > _Right || OrthoPos.y() < _Bottom || OrthoPos.y() > _Top)
			{
				_Left_res = _Left_ini;
				_Right_res = _Right_ini;
				_Bottom_res = _Bottom_ini;
				_Top_res = _Top_ini;
				SetOrthoByRatio(SCR_WIDTH, SCR_HEIGHT);
			}
		}
		//������һ�ε���ת����
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1);
		_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;
		SetPVMValue();
	}
	void mModelView::UpdateOrthoAndCamera(float maxRadius, QVector<QVector3D> AddVertex)
	{
		if ( AddVertex.size() == 0)
		{
			return;
		}
		_MaxRadius = maxRadius;
		//���³�ʼֵ
		_Left_ini = -_MaxRadius * 1.1;
		_Right_ini = _MaxRadius * 1.1;
		_Bottom_ini = -_MaxRadius * 1.1;
		_Top_ini = _MaxRadius * 1.1;

		//ģ�����껻��Ortho����
		//
		for (int i = 0; i < AddVertex.size(); i++)
		{
			QVector2D PixelPos = mViewToolClass::ModelToPixelPositon(AddVertex[i], _projection, _view, _model, SCR_WIDTH, SCR_HEIGHT);
			QVector2D OrthoPos = mViewToolClass::PixelToOrthoPosition(PixelPos.x(), PixelPos.y(), _Left, _Right, _Bottom, _Top, SCR_WIDTH, SCR_HEIGHT);
			//�жϸõ��Ƿ�����ͼ��
			if (OrthoPos.x() < _Left || OrthoPos.x() > _Right || OrthoPos.y() < _Bottom || OrthoPos.y() > _Top)
			{
				_Left_res = _Left_ini;
				_Right_res = _Right_ini;
				_Bottom_res = _Bottom_ini;
				_Top_res = _Top_ini;
				SetOrthoByRatio(SCR_WIDTH, SCR_HEIGHT);
			}
		}
		//������һ�ε���ת����
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1);
		_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;
		SetPVMValue();
	}
	void mModelView::UpdateOrthoAndCamera(QVector<QVector3D> Allvertex)
	{
		if (Allvertex.size() == 0)
		{
			return;
		}
		_MaxRadius = mViewToolClass::GetMaxRadius(Allvertex, _Center);
		//���³�ʼֵ
		_Left_ini = -_MaxRadius * 1.1;
		_Right_ini = _MaxRadius * 1.1;
		_Bottom_ini = -_MaxRadius * 1.1;
		_Top_ini = _MaxRadius * 1.1;
		
		//������һ�ε���ת����
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1);
		_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;

		//���������������ͶӰ
		SetOrthoByRatio(SCR_WIDTH, SCR_HEIGHT);
		SetPVMValue();
	}
	void mModelView::UpdateOrthoAndCamera(float maxRadius_modelcenter, float maxRadius_nowcenter)
	{

		_MaxRadius = maxRadius_nowcenter;
		//���³�ʼֵ
		_Left_ini = -maxRadius_modelcenter * 1.1;
		_Right_ini = maxRadius_modelcenter * 1.1;
		_Bottom_ini = -maxRadius_modelcenter * 1.1;
		_Top_ini = maxRadius_modelcenter * 1.1;

		//������һ�ε���ת����
		_Eye_0 = _Eye_0.normalized()*(_MaxRadius + 1);
		_Eye = QVector3D(_Eye_0.x() + _Center.x(), _Eye_0.y() + _Center.y(), _Eye_0.z() + _Center.z());
		_NearPlane = 0.1;
		_FarPlane = 2 * _MaxRadius + 2;
		SetPVMValue();
	}
}	
