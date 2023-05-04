#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

//MViewBasic
#include "mMeshViewEnum.h"

//MBasicFunction
#include "mBasicEnum.h"
#include "mBasicStructor.h"

#include <QVector4D>
#include <QVector>

using namespace MViewBasic;
using namespace MBasicFunction;
namespace MPreRend
{
	class RENDVIEW_EXPORT mPreRendStatus
	{

	public:
		//��ʾģʽ
		ShowFuntion _showFunction = ElementFace;

		//ʰȡģʽ
		MViewBasic::PickFilter _pickFilter;

		//ʰȡ�ĵ�Ԫ���͹�����
		std::set<int> _pickElementTypeFilter;

		//�Ƿ�������
		bool _lightIsOn = true;

		//����λ�ø������λ��
		bool _lightIsDependOnCamera = true;

		//���ղ���
		PostLight _postLight;

		//��Ĵ�С
		int _pointSize = 1;

		//�߿�
		float _lineWidth = 1.0;

		//�������ߵ���ɫ
		QVector4D _faceLineColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
	};
}

