#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

//MViewBasic
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include <QVector4D>
#include <QVector>

using namespace MViewBasic;
namespace MPreRend
{
	class RENDVIEW_EXPORT mPreRendStatus
	{

	public:
		//��ʾģʽ
		ShowFuntion _showFunction = SmoothShaded;

		//ʰȡģʽ
		PickFilter _pickFilter;

		//ʰȡ�ĵ�Ԫ���͹�����
		std::set<int> _pickElementTypeFilter;

		//�Ƿ�������
		bool _lightIsOn = true;

		//����λ�ø������λ��
		bool _lightIsDependOnCamera = true;

		//���ղ���
		PostLight _postLight;

		//�ڵ�Ĵ�С
		int _nodeSize = 1;

		//��Ĵ�С
		int _pointSize = 1;

		//�߿�
		float _lineWidth = 1.0;

		//�������ߵ���ɫ
		QVector4D _faceLineColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
	};
}

