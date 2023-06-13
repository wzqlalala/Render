#include "mViewBase.h"
#include "mViewToolClass.h"
#include "mMeshViewEnum.h"


namespace MViewBasic
{
	mViewBase::mViewBase()
	{
		InitialModelView();
	}
	mViewBase::~mViewBase()
	{

	}
	void mViewBase::InitialModelView()
	{
		InitialOrtho();
		InitialCamera();
	}
	void mViewBase::InitialOrtho()
	{
		//正交投影参数
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
	void mViewBase::InitialCamera()
	{
		//视角参数
		_Eye = QVector3D(0, 0, 1);
		_Eye_0 = QVector3D(0, 0, 1);
		_Center = QVector3D(0, 0, 0);
		_Up = QVector3D(0, 1, 0);
	}
	void mViewBase::SetOrthoByRatio(float scr_width, float scr_height)
	{
		SCR_WIDTH = scr_width;
		SCR_HEIGHT = scr_height;

		SetPVMValue();
	}
	void mViewBase::ReturnOrthoByRatio()
	{
	}
	void mViewBase::SetPVMValue()
	{
		_projection.setToIdentity();
		_model.setToIdentity();
		_view.setToIdentity();
		_projection.ortho(_Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane);
		_view.lookAt(_Eye, _Center, _Up);
	}
	void mViewBase::ZoomAtMouse_Bywheel(int posX, int posY, int rollervalue, ScalePerspectice scalePerspectice, float sensitivity)
	{double ZoomAtPointXv = (posX - SCR_WIDTH / 2)* (_Right - _Left) / SCR_WIDTH;
		double ZoomAtPointYv = -(posY - SCR_HEIGHT / 2)* (_Top - _Bottom) / SCR_HEIGHT;
		//缩放过后鼠标点的视口坐标
		double ZoomAtPointXv_end;
		double ZoomAtPointYv_end;
		//视口的宽度和高度
		double width = _Right - _Left;
		double height = _Top - _Bottom;
		//视口的中心点坐标
		double Zerox = (_Left + _Right) / 2;
		double Zeroy = (_Bottom + _Top) / 2;
		//设置缩放的方向
		if (scalePerspectice == Eye)
		{
			if (rollervalue <= 0)
			{

				//缩放过后鼠标点的视口坐标
				ZoomAtPointXv_end = ZoomAtPointXv * sensitivity;
				ZoomAtPointYv_end = ZoomAtPointYv * sensitivity;
				//缩放
				width *= sensitivity;
				height *= sensitivity;
				//绕着视口中心缩放后的视口的边界坐标
				_Left = Zerox - width / 2;
				_Right = Zerox + width / 2;
				_Bottom = Zeroy - height / 2;
				_Top = Zeroy + height / 2;

				//绕着鼠标中心缩放后的视口的边界坐标（最终）

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

				//缩放过后鼠标点的视口坐标
				ZoomAtPointXv_end = ZoomAtPointXv * sensitivity;
				ZoomAtPointYv_end = ZoomAtPointYv * sensitivity;
				//缩放
				width *= sensitivity;
				height *= sensitivity;
				//绕着视口中心缩放后的视口的边界坐标
				_Left = Zerox - width / 2;
				_Right = Zerox + width / 2;
				_Bottom = Zeroy - height / 2;
				_Top = Zeroy + height / 2;

				//绕着鼠标中心缩放后的视口的边界坐标（最终）

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


		//还原通过屏幕大小设置正交投影前的参数，用于保存模型当前的变换位置。
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mViewBase::ZoomAtViewCenter_ByMove(int FirstPosx, int FirstPosy, int nowPosx, int nowPosy, float sensitivity)
	{
		if (IfFirstMouse == true)
		{
			width_firstmouse = _Right - _Left;
			height_firstmouse = _Top - _Bottom;
			IfFirstMouse = false;
		}
		QVector2D ViewCenter = QVector2D((_Left + _Right) / 2, (_Top + _Bottom) / 2);//视图中心的视图坐标
		int offset = (nowPosx - FirstPosx)+ (FirstPosy -nowPosy);
		float width,height;
		float Sens;//最终灵敏度
		if (offset >= 0)
		{
			float value = offset / (SCR_WIDTH + SCR_HEIGHT) * 9+1 ;//将offset值的范围映射到(1-10)
			Sens = value * sensitivity;
			width = width_firstmouse / Sens;
			height = height_firstmouse/ Sens;
			_ScaleProportion /= sensitivity;
			
		}
		else if (offset < 0)
		{
			float value = -offset / (SCR_WIDTH + SCR_HEIGHT) * 9+1 ;//将offset值的范围映射到(1-10)
			Sens = value * sensitivity;
			width = width_firstmouse * Sens;
			height = height_firstmouse * Sens;
			_ScaleProportion *= sensitivity;

		}
		_Left = ViewCenter.x() - width / 2;
		_Right = ViewCenter.x() + width / 2;
		_Bottom = ViewCenter.y() - height / 2;
		_Top = ViewCenter.y() + height / 2;

		//还原通过屏幕大小设置正交投影前的参数，用于保存模型当前的变换位置。
		ReturnOrthoByRatio();
		SetPVMValue();
	}
	void mViewBase::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
	{
	}
	void mViewBase::ResetOrthoAndCamera(QVector3D center, float maxRadius)
	{

	}
}
