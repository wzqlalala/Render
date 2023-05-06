#include"mLableDataController_common.h"
namespace MDataLable
{
	template<class T>
	QHash<QString, T>& mLableDataController_common::getLableData() { return QHash<QString, T>(); }
	//Õ®”√
	template<> QHash<QString, Fixed_Font_Common>& mLableDataController_common::getLableData<Fixed_Font_Common>() { return _lableData_common->_font_Fixed_Common_Data; }
	template<> QHash<QString, Common_Font_Common>& mLableDataController_common::getLableData<Common_Font_Common>() { return _lableData_common->_font_Common_Common_Data; }
	template<> QHash<QString, X_Point_Common>& mLableDataController_common::getLableData<X_Point_Common>() { return _lableData_common->_XPoint_Common_Data; }
	template<> QHash<QString, TempPoint_Common>& mLableDataController_common::getLableData<TempPoint_Common>() { return _lableData_common->_tempPoint_Common_Data; }
	template<> QHash<QString, TempMesh_Common>& mLableDataController_common::getLableData<TempMesh_Common>() { return _lableData_common->_tempMesh_Common_Data; }
	template<> QHash<QString, Arrow_Font_Axis_Common>& mLableDataController_common::getLableData<Arrow_Font_Axis_Common>() { return _lableData_common->_globalAxis_Common_Data; }
	template<> QHash<QString, Arrow_Common>& mLableDataController_common::getLableData<Arrow_Common>() { return _lableData_common->_arrow_Common_Data; }
}

