#pragma once

#include "rendbasic_global.h"

//Qt
#include <QList>
#include <QVector4D>

namespace MBasicFunction
{
	class RENDBASIC_EXPORT mColor
	{
	public:
		/*
		* ��ȡ����ָ��
		*/
		static mColor* getInstance();

		/*
		 * Ĭ�Ϲ��캯��
		 */
		mColor();

		/*
		* ��������
		*/
		~mColor();

		/*
		 * ͨ������ID����ȡ��ɫ
		 */
		QVector4D getPartColor(int partID);

		/*
		 * ��ȡ�ڵ����ɫ
		 */
		QVector4D getNodeColor();

	private:
		//����
		static mColor *_instance;

		//ģ����ɫ���б�����partID��ѡ��ɫ
		QList<QVector4D> _partColor;

		//�ڵ����ɫ
		QVector4D _nodeColor;
	};
}
