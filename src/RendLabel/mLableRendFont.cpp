#include"mLableRendFont.h"
namespace MLableRend
{
	///*
	//* 开始添加文字所有的顶点数据
	//*/
	//template<class T>
	//template<class T1>
	//void mLableRendFont<T>::appendFontVertex(const QString &name, const QVector<T1>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
	//	const QVector3D color) {};
	////偏特化
	//template<class T>
	//template<>
	//void mLableRendFont<T>::appendFontVertex<POS_TXT_DIR>(const QString &name, const QVector<POS_TXT_DIR>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
	//	const QVector3D color)
	//{
	//	auto tmp = this->_data[name];
	//	float arrowSize = tmp.lableData.getLableSize();
	//	for (auto val : data)
	//	{
	//		QVector3D pos = val.first;
	//		QString txt = val.second.first;
	//		QVector3D dir = val.second.second;
	//		beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos, arrowSize, dir);
	//	}
	//};
	////偏特化
	//template<class T>
	//template<>
	//void mLableRendFont<>::appendFontVertex<POS_TXT>(const QString &name, const QVector<POS_TXT>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
	//	const QVector3D color)
	//{
	//	for (auto val : data)
	//	{
	//		QVector3D pos = val.first;
	//		QString txt = val.second;
	//		beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos);
	//	}
	//};

}
