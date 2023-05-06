#include"mLableDataController_pre.h"
namespace MDataLable
{
	//gcc中模板特化在头文件中声明 在cpp中实现，不然会报重定义错误
	template<class T>
	shared_ptr<T> mLableDataController_pre::getLableDataBase()
	{
		return shared_ptr<T>();
	}
	template<> shared_ptr<mLableData_implicit> mLableDataController_pre::getLableDataBase<mLableData_implicit>()
	{
		return _lableData_implicit;
	}
	template<> shared_ptr<mLableData_explicit> mLableDataController_pre::getLableDataBase<mLableData_explicit>()
	{
		return _lableData_explicit;
	}
	template<> shared_ptr<mLableData_fluid> mLableDataController_pre::getLableDataBase<mLableData_fluid>()
	{
		return _lableData_fluid;
	}
	template<> shared_ptr<mLableData_magnet> mLableDataController_pre::getLableDataBase<mLableData_magnet>()
	{
		return _lableData_magnet;
	}


	template<class T>
	QHash<QString, T>& mLableDataController_pre::getLableData() { return QHash<QString, T>(); }
	//隐式
	template<> QHash<QString, Arrow_Font_Axis_Implicit>& mLableDataController_pre::getLableData<Arrow_Font_Axis_Implicit>() { return _lableData_implicit->_arrow_Font_Axis_Implicit_Data; }
	template<> QHash<QString, Arrow_Font_Load_Implicit>& mLableDataController_pre::getLableData<Arrow_Font_Load_Implicit>() { return _lableData_implicit->_arrow_Font_Load_Implicit_Data; }
	template<> QHash<QString, Triangle_Font_Load_Implicit>& mLableDataController_pre::getLableData<Triangle_Font_Load_Implicit>() { return _lableData_implicit->_triangle_Font_Load_Implicit_Data; }
	template<> QHash<QString, Rhombus_Font_Load_Implicit>& mLableDataController_pre::getLableData<Rhombus_Font_Load_Implicit>() { return _lableData_implicit->_rhombus_Font_Load_Implicit_Data; }
	template<> QHash<QString, X_Font_Load_Implicit>& mLableDataController_pre::getLableData<X_Font_Load_Implicit>() { return _lableData_implicit->_X_Font_Load_Implicit_Data; }
	template<> QHash<QString, Lightning_Font_Load_Implicit>& mLableDataController_pre::getLableData<Lightning_Font_Load_Implicit>() { return _lableData_implicit->_lightning_Font_Load_Implicit_Data; }
	template<> QHash<QString, DF_BC_Implicit>& mLableDataController_pre::getLableData<DF_BC_Implicit>() { return _lableData_implicit->_df_BC_Implicit_Data; }
	template<> QHash<QString, Home_BC_Implicit>& mLableDataController_pre::getLableData<Home_BC_Implicit>() { return _lableData_implicit->_home_BC_Implicit_Data; }
	template<> QHash<QString, Line_Connect_Implicit>& mLableDataController_pre::getLableData<Line_Connect_Implicit>() { return _lableData_implicit->_line_Connect_Implicit_Data; }
	template<> QHash<QString, FaceToFace_Connect_Implicit>& mLableDataController_pre::getLableData<FaceToFace_Connect_Implicit>() { return _lableData_implicit->_faceToFace_Connect_Implicit_Data; }
	template<> QHash<QString, FaceToFace_Contact_Implicit>& mLableDataController_pre::getLableData<FaceToFace_Contact_Implicit>() { return _lableData_implicit->_faceToFace_Contact_Implicit_Data; }
	template<> QHash<QString, Type1_RC_Implicit>& mLableDataController_pre::getLableData<Type1_RC_Implicit>() { return _lableData_implicit->_Type1_RC_Implicit_Data; }
	template<> QHash<QString, Type2_RC_Implicit>& mLableDataController_pre::getLableData<Type2_RC_Implicit>() { return _lableData_implicit->_Type2_RC_Implicit_Data; }
	template<> QHash<QString, Line_RC_Implicit>& mLableDataController_pre::getLableData<Line_RC_Implicit>() { return _lableData_implicit->_line_RC_Implicit_Data; }
	template<> QHash<QString, Sphere_RC_Implicit>& mLableDataController_pre::getLableData<Sphere_RC_Implicit>() { return _lableData_implicit->_sphere_RC_Implicit_Data; }
	template<> QHash<QString, DF_RBC_Implicit>& mLableDataController_pre::getLableData<DF_RBC_Implicit>() { return _lableData_implicit->_df_RBC_Implicit_Data; }
	template<> QHash<QString, Arrow_PlyCoordinate_Implicit>& mLableDataController_pre::getLableData<Arrow_PlyCoordinate_Implicit>() { return _lableData_implicit->_arrow_PlyCoordinate_Implicit_Data; }
	//显式
	template<> QHash<QString, Line_RW_Explicit>& mLableDataController_pre::getLableData<Line_RW_Explicit>() { return _lableData_explicit->_line_RW_Explicit_Data; }
	template<> QHash<QString, Face_RW_Explicit>& mLableDataController_pre::getLableData<Face_RW_Explicit>() { return _lableData_explicit->_face_RW_Explicit_Data; }
	template<> QHash<QString, Axis_RW_Explicit>& mLableDataController_pre::getLableData<Axis_RW_Explicit>() { return _lableData_explicit->_axis_RW_Explicit_Data; }
	template<> QHash<QString, Arrow_Font_Axis_Explicit>& mLableDataController_pre::getLableData<Arrow_Font_Axis_Explicit>() { return _lableData_explicit->_arrow_Font_Axis_Explicit_Data; }
	template<> QHash<QString, Arrow_Font_Load_Explicit>& mLableDataController_pre::getLableData<Arrow_Font_Load_Explicit>() { return _lableData_explicit->_arrow_Font_Load_Explicit_Data; }
	template<> QHash<QString, DF_Load_Explicit>& mLableDataController_pre::getLableData<DF_Load_Explicit>() { return _lableData_explicit->_df_Load_Explicit_Data; }
	template<> QHash<QString, DF_BC_Explicit>& mLableDataController_pre::getLableData<DF_BC_Explicit>() { return _lableData_explicit->_df_BC_Explicit_Data; }
	template<> QHash<QString, Arrow_BC_Explicit>& mLableDataController_pre::getLableData<Arrow_BC_Explicit>() { return _lableData_explicit->_arrow_BC_Explicit_Data; }
	template<> QHash<QString, Line_Connect_Explicit>& mLableDataController_pre::getLableData<Line_Connect_Explicit>() { return _lableData_explicit->_line_Connect_Explicit_Data; }
	template<> QHash<QString, FaceToFace_Connect_Explicit>& mLableDataController_pre::getLableData<FaceToFace_Connect_Explicit>() { return _lableData_explicit->_faceToFace_Connect_Explicit_Data; }
	template<> QHash<QString, DF_Connect_Explicit>& mLableDataController_pre::getLableData<DF_Connect_Explicit>() { return _lableData_explicit->_df_Connect_Explicit_Data; }
	template<> QHash<QString, Arrow_Font_InitCondition_Explicit>& mLableDataController_pre::getLableData<Arrow_Font_InitCondition_Explicit>() { return _lableData_explicit->_arrow_Font_InitCondition_Explicit_Data; }
	template<> QHash<QString, Triangle_Font_InitCondition_Explicit>& mLableDataController_pre::getLableData<Triangle_Font_InitCondition_Explicit>() { return _lableData_explicit->_triangle_Font_InitCondition_Explicit_Data; }
	template<> QHash<QString, FaceToFace_Contact_Explicit>& mLableDataController_pre::getLableData<FaceToFace_Contact_Explicit>() { return _lableData_explicit->_faceToFace_Contact_Explicit_Data; }
	template<> QHash<QString, Face_Box_Explicit>& mLableDataController_pre::getLableData<Face_Box_Explicit>() { return _lableData_explicit->_face_Box_Explicit_Data; }
	template<> QHash<QString, Line_Box_Explicit>& mLableDataController_pre::getLableData<Line_Box_Explicit>() { return _lableData_explicit->_line_Box_Explicit_Data; }
	template<> QHash<QString, Arrow_Vector_Explicit>& mLableDataController_pre::getLableData<Arrow_Vector_Explicit>() { return _lableData_explicit->_arrow_Vector_Explicit_Data; }
	//电磁
	template<> QHash<QString, Arrow_Font_Axis_Magnet>& mLableDataController_pre::getLableData<Arrow_Font_Axis_Magnet>() { return _lableData_magnet->_arrow_Font_Axis_Magnet_Data; }
	template<> QHash<QString, Arrow_Font_Load_Magnet>& mLableDataController_pre::getLableData<Arrow_Font_Load_Magnet>() { return _lableData_magnet->_arrow_Font_Load_Magnet_Data; }
	template<> QHash<QString, Triangle_Font_Load_Magnet>& mLableDataController_pre::getLableData<Triangle_Font_Load_Magnet>() { return _lableData_magnet->_triangle_Font_Load_Magnet_Data; }
	template<> QHash<QString, Lightning_Font_Load_Magnet>& mLableDataController_pre::getLableData<Lightning_Font_Load_Magnet>() { return _lableData_magnet->_lightning_Font_Load_Magnet_Data; }
	template<> QHash<QString, Arrow_BC_Magnet>& mLableDataController_pre::getLableData<Arrow_BC_Magnet>() { return _lableData_magnet->_arrow_BC_Magnet_Data; }
	template<> QHash<QString, Home_BC_Magnet>& mLableDataController_pre::getLableData<Home_BC_Magnet>() { return _lableData_magnet->_home_BC_Magnet_Data; }


}

