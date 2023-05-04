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
namespace MPostRend
{
	class RENDVIEW_EXPORT mPostRendStatus
	{

	public:
		//��Ⱦģʽ
		PostMode _postMode = OneFrame;

		//�Ƿ���ʾ����ǰ��ͼ
		bool _isShowInitialShape{false};

		//�ڵ��������
		NodeOrElement _nodeOrElement = PostNode;

		//��ʾģʽ
		ShowFuntion _showFunction = ElementFace;

		//ʰȡģʽ
		MViewBasic::PickFilter _pickFilter;

		//ʰȡ�ĵ�Ԫ���͹�����
		std::set<int> _pickElementTypeFilter;

		//��ѡ���ǵ�ѡ
		//MViewBasic::PickSoloOrMutiply _pickSoloOrMutiply;

		//��Ⱦ��Χ��0-ȫ��������Χ���Զ��壩��1-��Ⱦ������Χ��
		int _rangeType = 0;

		//�Ƿ���ɢ
		bool _isdispersed = true;

		//�Ƿ�Ȳ�
		bool _isEquivariance = true;

		//�Ƿ���Ⱦ��ɫ��
		bool _isShowPostColorTable = false;

		//������ɫ�����ִ�С
		float _postColorTableFontSize;

		//������ɫ��λ���и߶ȱ���
		float _postColorTableRatio;

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

		/*************************************����ͼ********************************/

		//����ͼ-����ϵ��
		QVector3D _deformFactor = QVector3D(1,1,1);

		/*************************************����********************************/

		//��ƽ��Ĳ���
		QVector<QVector4D> _cuttingPlanes;

		//�Ƿ�ʵʱ��ʾƽ��
		bool _isShowTransparentPlane = true;

		//�Ƿ�ֻ��ʾ���е��棬����ʾģ��
		bool _isOnlyShowCuttingPlane = false;

		/**************************************����**********************************/

		//����֡��
		//����-�Ƿ�����
		bool _generateAnimate;

		//����-���ɵĶ���֡��
		int _aniFrameAmount;

		//����-����
		QString _aniType;

		//����-��Χ
		QString _aniRange;

		//����-���Ʊ���
		QString _aniControl;

		//����-��Χ��ʼֵ
		QString _aniRangeVal1;

		//����-��Χ����ֵ
		QString _aniRangeVal2;

		//����-������֡��
		int _aniLinearCount = 20;

		//����-��ǰ֡
		int _aniCurrentFrame = 1;

		//����-��ʼ֡
		int _aniStartFrame = 1;

		//����-����֡
		int _aniEndFrame = 20;

		//����-֡���
		int _aniFrameInterval = 1;

		//����-֡����
		int _aniFrameRate = 40;

		//����-�Ƿ�ѭ������
		bool _aniLoopPlay = true;

		//����-��Ⱦ��Χ
		QString _aniEveryRange;

		//����-��Сֵ�����Զ��巶Χ���ã�
		float _aniRangeMin;

		//����-���ֵ�����Զ��巶Χ���ã�
		float _aniRangeMax;

		//����-�Ƿ���
		bool _aniIsStart;
	};
}

