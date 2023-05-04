#pragma once
#include "renddata_global.h"

#include <QObject>
#include <QHash>
#include <set>

namespace MDataPost
{
	class mPostOneFrameRendData;
	class RENDDATA_EXPORT mPostAnimationRendData :public QObject
	{
	public:

		/*
		 * ���캯��
		 */
		mPostAnimationRendData();

		/*
		 * ��������
		 */
		~mPostAnimationRendData();

		/*
		 * Ϊ������ȡ����֡��id
		 */
		std::set<int> getRendAnimationIds();

		/*
		 * Ϊ�������ĳһ֡������
		 */
		void appendRendOneFrameData(int id, mPostOneFrameRendData *postOneFrameData);

		/*
		 * Ϊ������ȡĳһ֡������
		 */
		mPostOneFrameRendData* getRendOneFrameData(int id);

		/*
		* ɾ������֡������
		*/
		void deleteAnimationRendData();

		/*
		 * ���ö�������Ⱦ��Χ,��Ļ�����ͳһ����Ⱦ��Χ����Ļ����ÿ֡�ķ�Χ��ԭΪ��ʼֵ
		 */
		void setAnimationFrameRange(bool isAgreement);

		/*
		* ���ö������Զ�����Ⱦ��Χ
		*/
		void setAnimationCustomFrameRange(float minData, float maxData);

	private:
		//ÿһ֡��Ӧ������
		QHash<int, mPostOneFrameRendData*> _rendFrameid_RendFrameData;

		//����֡������
		std::set<int> _allRendFrameids;

		//����֡������Сֵ
		float _allMinData;

		//����֡�������ֵ
		float _allMaxData;
	};
}
