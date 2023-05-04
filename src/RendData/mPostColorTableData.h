#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"
#include <QOpenGLVersionFunctions>
#include <QVector2D>
#include "mBasicStruct.h"


namespace MDataPost
{
	class RENDDATA_EXPORT mPostColorTableData
	{
	public:

		/*
		 * ���캯��
		 */
		mPostColorTableData(float max, float min, bool isEquivariance = true, int num = 12);

		/*
		 * ��������
		 */
		~mPostColorTableData();

		/*
		 * ������ɫ��һ�еı����߶�
		 */
		void setPostColorRowRatio(float ratio);

		/*
		 * ������ɫ�Ĳ��
		 */
		void setPostColorTableNum(int num);

		/*
		 * ����ֵ�������Сֵ
		 */
		void setPostColorTableValue(float max, float min);

		/*
		 * ������ɫ�Ĳ���Ƿ�Ȳ�
		 */
		void setPostColorTableIsEquivariance(bool isEquivariance);

		/*
		* ��ȡ��ɫ���Ƿ�Ȳ�
		*/
		bool getPostColorTableIsEquivariance();

		/*
		 * ��ȡ��ɫ�Ĳ��
		 */
		int getPostColorTableNum();

		/*
		 * ��ȡ��ɫ��
		 */
		GLubyte* getColorTable();

		/*
		 * ��ȡ��ɫ���λ������ֵ
		 */
		QVector<QVector2D> getColorTableVertex();

		/*
		 * ��ȡ��ɫ�����������ֵ
		 */
		QVector<float> getColorTableTexture();

		/*
		 * ��ȡ��ɫ��Ŀ̶��ߵ�λ������ֵ
		 */
		QVector<QVector2D> getColorTableLineMarkVertex();

		/*
		 * ��ȡ��ɫ�������
		 */
		QVector<MViewBasic::FontText> getColorTableNumVertex();

		/*
		 * ��ȡ��ɫ���mx���ֵ�����
		 */
		QVector2D getColorTableMXVertex();

		/*
		 * ��ȡ��ɫ��ı������ֵ�����
		 */
		QVector2D getColorTableTitleVertex();

		/*
		*��ȡ��ɫ�����Сֵ��������
		*/
		QVector2D getColorTableMinVertex();

		/*
		*��ȡ��ɫ������ֵ��������
		*/
		QVector2D getColorTableMaxVertex();

	private:
		/*
		 * ���¶�������
		 */
		void UpdateVertex();

		/*
		* ����������ϵ���ı�ʱ���¶��������
		*/
		void UpdateFontData();

	private:
		/*
		 * ��ʼ����ɫ��
		 */
		void initialColorTable();

	private:
		//��ɫ�Ĳ��
		int _colortablenum{ 12 };

		//��ɫ��
		GLubyte _colortable[90];

		//���ڴ�С
		int Win_WIDTH, Win_HEIGHT;

		//һ�еĸ߶ȱ���
		float _ratio;

		//��ɫ��ľ���λ������
		QVector<QVector2D> _colorTableVertex;

		//��ɫ��ľ�����������
		QVector<float> _colorTableTexture;

		//��ɫ��Ķ���λ��
		QVector2D _topleft,_topright;

		//��ɫ��ĵײ�λ��
		QVector2D _bottomleft, _bottomright;

		//��ɫ��Ŀ̶���
		QVector<QVector2D> _lineMarkVertex;

		//��ɫ��Ŀ̶�����ֵ��������
		QVector<MViewBasic::FontText> _fontVertex;

		//��ɫ����Ⱦ��ֵ�������Сֵ
		float _max, _min;

		//�Ƿ�Ȳ�
		bool _isEquivariance;

		//mx�ı�־
		QVector2D _mxVertex;

		//�����ı�����������
		QVector2D _titleVertex;

		//���ֵ����������
		QVector2D _maxVertex;

		//��Сֵ����������
		QVector2D _minVertex;
	};
}
