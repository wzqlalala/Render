#pragma once

//////////////////////////////////////////////////////////////////////
//����������Ⱦ ��ͬ��״���� �������ڶ�Ӧ ģ������ϵ�� ����λ�õ�ͼ��//
//////////////////////////////////////////////////////////////////////

#include <QObject>
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
using namespace MDataLable;
namespace MLableRend
{
	//ͨ������ͼ�����
	template<class T>
	class mLableRendCommon : public mLableRendBase<mDrawArray>
	{
	public:
		explicit mLableRendCommon(const QHash<QString, T> &data, const QVector<QVector3D> &shapeVertex, const GLenum drawMode)
            :mLableRendBase<mDrawArray>(7, drawMode), _shapeVertex(shapeVertex), _data(data){}
        ~mLableRendCommon(){}
	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
		//��״����
		QVector<QVector3D> _shapeVertex; 
		//��������
		vector<float> _vertex_Pos;  //��Ҫ�ƶ�������
	};
}
