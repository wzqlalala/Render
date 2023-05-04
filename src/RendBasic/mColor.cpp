#include "mColor.h"

//QVector4D
#include <QVector4D>

namespace MBasicFunction
{
	mColor* mColor::_instance = nullptr;

	mColor* mColor::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new mColor;
		}
		return _instance;
	}

	mColor::mColor()
	{
		//��ʼ��������ɫ�б�
		_partColor = QList<QVector4D>{ QVector4D(1.00f, 0.00f, 0.00f, 1.00f),
									   QVector4D(0.00f, 0.00f, 1.00f, 1.00f),
									  QVector4D(0.65f, 0.59f, 0.0f, 1.00f),
									  QVector4D(0.84f, 0.19f, 0.46f, 1.00f),
									  QVector4D(0.57f, 0.43f, 0.85f, 1.00f),
									  QVector4D(0.91f, 0.67f, 0.65f, 1.00f),
									  QVector4D(0.50f, 0.24f, 0.12f, 1.00f),
									  QVector4D(0.05f, 0.6f, 0.3f, 1.00f) };

		//��ʼ���ڵ���ɫ
		_nodeColor = QVector4D{ 0.20f, 1.00f, 0.20f, 1.00f };


	}

	mColor::~mColor()
	{

	}

	QVector4D mColor::getPartColor(int partID)
	{
		//ͨ��������ID����ɫ�б�Ĵ�Сȡ������Ȼ���ȡ����ɫֵ
		int num = (partID-1) % _partColor.size();
		return _partColor.at(num);
	}

	QVector4D mColor::getNodeColor()
	{
		return _nodeColor;
	}
}
