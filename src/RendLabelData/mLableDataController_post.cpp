#include"mLableDataController_post.h"
namespace MDataLable
{
	//∫Û¥¶¿Ì
	template<class T>
	QHash<int, QHash<QString, T>>& getLableData()
	{
		return QHash<int, QHash<QString, T>>();
	}
	template<> QHash<int, QHash<QString, VectorArrow_Post>>& mLableDataController_post::getLableData<VectorArrow_Post>()
	{
		return _lableData_post->_vecArrow_Post_Data;
	}
	template<> QHash<int, QHash<QString, Fixed_Font_Post>>& mLableDataController_post::getLableData<Fixed_Font_Post>()
	{
		return _lableData_post->_font_Fixed_Post_Data;
	}
}