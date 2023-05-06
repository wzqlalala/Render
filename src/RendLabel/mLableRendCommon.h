#pragma once

//////////////////////////////////////////////////////////////////////
//此类用于渲染 不同形状类型 并放置在对应 模型坐标系下 坐标位置的图标//
//////////////////////////////////////////////////////////////////////

#include <QObject>
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
using namespace MDataLable;
namespace MLableRend
{
	//通用类型图标绘制
	template<class T>
	class mLableRendCommon : public mLableRendBase<mDrawArray>
	{
	public:
		explicit mLableRendCommon(const QHash<QString, T> &data, const QVector<QVector3D> &shapeVertex, const GLenum drawMode)
            :mLableRendBase<mDrawArray>(7, drawMode), _shapeVertex(shapeVertex), _data(data){}
        ~mLableRendCommon(){}
	private:
		//渲染数据
		const QHash<QString, T> &_data;
		//形状数据
		QVector<QVector3D> _shapeVertex; 
		//顶点数据
		vector<float> _vertex_Pos;  //需要移动的坐标
	};
}
