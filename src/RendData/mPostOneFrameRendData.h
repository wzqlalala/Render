#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QHash>
#include <QPair>
#include <QVector3D>
#include <QVector2D>
#include <set>

#include "mMeshViewEnum.h"
#include "mBasicEnum.h"

namespace MDataPost
{
	class mPostColorTableData;
	class RENDDATA_EXPORT mPostOneFrameRendData
	{
	public:
		/*
		 * Ĭ�Ϲ��캯��
		 */
		mPostOneFrameRendData();

		/*
		 * ���ƹ��캯��
		 */
		mPostOneFrameRendData(int id);

		/*
		 * ��������
		 */
		~mPostOneFrameRendData();

		/*
		* �������캯�����������Զ�������
		*/
		mPostOneFrameRendData(const mPostOneFrameRendData& oneframe);

		/*
		 * ���ýڵ㻹������
		 */
		void setNodeOrElement(MViewBasic::NodeOrElement nodeOrElement);

		/*
		 * ��ȡ�ڵ㻹������
		 */
		MViewBasic::NodeOrElement getNodeOrElement();

		/*
		 * ����λ�����ݣ��ڵ�λ��ʸ��
		 */
		void setNodeDisplacementData(const QHash<int, QVector3D>& dis);

		/*
		 * ������Ⱦ���ݣ��ڵ����
		 */
		void setRendData(int id,const QHash<int, float>& data, const QString & text);

		/*
		 * ������Ⱦ���ݣ�ʸ��
		 */
		void setRendData(const QHash<int, QVector3D>& data, MViewBasic::NodeOrElement type);

		/*
		 * ��ȡ����������
		 */
		QHash<int,float> getRendValue();

		/*
		 * ��ȡ�ڵ��Ӧ��λ������
		 */
		QHash<int, QVector3D> getNodeDisplacementData();

		/*
		 * ��ȡ��Ӧ��ʸ������
		 */
		QHash<int, QVector3D> getVectorRendData();

		/*
		* ��ȡʸ����������
		*/
		MViewBasic::NodeOrElement getVectorValueType();

		/*
		 * ��ȡ��ǰ��Ⱦ�ı���
		 */
		QString getRendTitle();

		/*
		 * ��ԭ��Ⱦ��С���ֵ
		 */
		void resetMinMaxData();

		/*
		 * ������Ⱦ���ݵ���Сֵ
		 */
		void setMinData(float value);

		/*
		 * ������Ⱦ���ݵ����ֵ
		 */
		void setMaxData(float value);

		/*
		 * ������Ⱦ���ݵ���ֵ
		 */
		void setMinMaxData(float maxValue, float minValue);

		/*
		* ���õ�ǰ��Ⱦ���ݵ���ֵ
		*/
		void setCurrentMinMaxData(float maxValue, float minValue);

		/*
		 * ����ԭʼ���ݵ���С���ֵ
		 */
		void setOrignalMaxMinData(double max, double min);

		/*
		* ���ò�������ֵ
		*/
		void setPartMinMaxData(QString partName, float min, float max);

		/*
		* ͨ����ֵ������ɫ��
		*/
		void updateColorTableMinMaxData();

		/*
		* ���ݲ�����ֵ��ȡ��ֵ
		*/
		QHash<QString, QVector2D> getPartMinMaxData();

		/*
		* ��ȡ�������ݵ���Сֵ
		*/
		float getMinData();

		/*
		* ��ȡ�������ݵ����ֵ
		*/
		float getMaxData();

		/*
		* ��ȡ��ǰ��Ⱦ���ݵ���Сֵ
		*/
		float getCurrentMinData();

		/*
		* ��ȡ��ǰ��Ⱦ���ݵ����ֵ
		*/
		float getCurrentMaxData();

		/*
		* ��ȡԭʼ���ݵ���Сֵ
		*/
		float getOriginalMinData();

		/*
		* ��ȡԭʼ���ݵ����ֵ
		*/
		float getOriginalMaxData();

		/*
		 * ��ȡ��ǰ֡��Ⱦ
		 */
		int getRendID();

		/*
		 * ��ȡ��ǰ֡����ɫ��
		 */
		mPostColorTableData *getRendColorTable();

		///*
		//*��ȡ��Сֵ������
		//*/
		//QString getMinText();

		///*
		//*��ȡ���ֵ������
		//*/
		//QString getMaxText();

		/*
		 * ���ñ��ηŴ���
		 */
		void setDeformationScale(QVector3D scale);

		/*
		 * ��ȡ���ηŴ���
		 */
		QVector3D getDeformationScale();

		/*
		 * ������������ϵ��
		 */
		void setTextureCoordScale(float scale);

		/*
		 * ��ȡ��������ϵ��
		 */
		float getTextureCoordScale();

		/*
		* ɾ�������������ݺͺ�ʸ�����ݺ�λ������
		*/
		void deleteValueAndDisplacementData();
	private:
		//��ɫ�����
		mPostColorTableData *_postColorTable;

		//�����Ǹ��ݽڵ㻹�ǵ�Ԫ
		MViewBasic::NodeOrElement _nodeOrElement;

		//��ǰ֡��Ⱦ�ı���
		QString _title;

		//��ǰ֡��id
		int _id;

		//��������ϵ��
		float _textureCoordScale;

		//���ηŴ���
		QVector3D _deformationScale;

		//��������
		QHash<int, float> _value;

		//ԭʼ������Сֵ
		float _constMinData;

		//ԭʼ�������ֵ
		float _constMaxData;

		//��Ⱦ����Сֵ
		float _minData;

		//��Ⱦ�����ֵ
		float _maxData;

		//��ǰ��Ⱦ����Сֵ
		float _currentMinData;

		//��ǰ��Ⱦ�����ֵ
		float _currentMaxData;

		//����ʸ��
		QHash<int,QVector3D> _valueVector;

		//ʸ����������
		MViewBasic::NodeOrElement _vectorValueType;

		//�ڵ�λ��ʸ������
		QHash<int, QVector3D> _nodeDisplacement;

		//����������ֵ
		QHash<QString, QVector2D> _partMinMaxValue;
	};

}
