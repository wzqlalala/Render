#pragma once

#include "renddata_global.h"
#include "mResultInfo.h"

//MBasicFunction
#include "mPostEnum.h"

//Qt
#include <QList>
#include <QHash>
#include <set>
#include <QVector3D>

namespace MDataPost
{
	class mOneFrameData1;
	class mPostSetBase;
	class mPostRigidWallDataBase;
	class RENDDATA_EXPORT mDataPost1
	{
	public:
		/*
		 * Ĭ�Ϲ��캯��
		 */
		mDataPost1();

		/*
		 * Ĭ����������
		 */
		~mDataPost1();

		/*
		* ����ģ����
		*/
		void setModelName(const QString& name);

		/*
		* ��ȡģ����
		*/
		QString getModelName();

		/*
		* �洢������ͣ�ʱ����ػ���Ƶ����أ�
		*/
		void setResultType(int type);

		/*
		* ��ȡ�������
		*/
		int getResultType();

		/*
		 * ɾ������֡������
		 */
		void deleteAllFrameData();

		/*
		 * ���ĳ֡������
		 */
		void appendFrameData(int frameid,double time, mOneFrameData1 *frameData);

		/*
		* ���ĳ֡��ĳ֡ģ������һ��
		*/
		void appendSameFrame(int frame1, int frame2);

		/*
		 * ��ȡ��0֡�ĺ�������
		 */
		mOneFrameData1 *getFirstFrameData();

		/*
		 * ��ȡ���һ֡�ĺ�������
		 */
		mOneFrameData1 *getLastFrameData();

		/*
		 * ��ȡ����֡��id
		 */
		void getAllFrameId(std::set<int> &frameids);

		/*
		 * ��ȡĳһ֡��Ӧ�ĺ���ģ������
		 */
		mOneFrameData1 *getOneFrameData(int frameid);

		/*
		 * ��ȡĳһ֡��Ӧ��ʱ��
		 */
		double getOneFrameTime(int frameid);

		/*
		* ��ȡ��֡��
		*/
		int getTotalFrameCount();

		//�������񲿼�����ɫ
		void setMeshPartColor(QString partName, QVector3D color);

		//�������񲿼�������
		void setMeshPartVisual(QString partName, bool isShow);

		//��ȡ���񲿼�������
		bool getMeshPartVisual(QString partName);

		//����ȫ�����񲿼�������
		void setAllMeshPartVisual(bool isShow);

		//����ȫ�����񲿼�����ɫ
		void setAllMeshPartColor(QVector3D color);

		//��ȡ���񲿼�����ɫ
		QVector3D getMeshPartColor(QString partName);

		//��ȡ���ļ���������е�֡�洢���е��㲿������
		void updateAllPostPartNames();

		//��ȡ����֡�����в���
		std::set<QString> getAllPostPartNames();

		/*
		* ���֡��Ϣ
		*/
		void appendFrameInfo(FrameInfo fi);

		/*
		* ��ȡ֡��Ϣ
		*/
		FrameInfo getFrameInfo(int f);

		/*
		* ��ȡ֡��Ϣ
		*/
		FrameInfo getFrameInfo(const QString& cname);

		/*
		* ���ݹ�����ȡ֡��Ϣ
		*/
		QList<FrameInfo> getFrameInfos(const QStringList& cnames);

		/*
		* ��ȡ����֡��Ϣ
		*/
		QList<FrameInfo> getFrameInfos();

		/*
		* ��ȡ��֡��
		*/
		int getFrameAmount();

		/*
		* ��ȡ���й�����
		*/
		QStringList getAllCaseNames();

		/*
		* ��ȡĳ�����µ�֡����
		*/
		QStringList getCaseFrameNames(const QString& cname = QString("All"));

		/*
		* ��ȡĳ֡�ı�����
		*/
		QStringList getFrameVariables(int f);

		/*
		* ��ȡĳ֡��ʸ����
		*/
		QStringList getFrameVectors(int f);

		/*
		* ��ȡ���б�����
		*/
		QStringList getAllVariableNames();

		/*
		* ��ȡĳ�����ķ���
		*/
		QStringList getVariableCompos(const QString& var);

		/*
		* ��ȡĳ�����ķ����Ƿ�Ϊ����
		*/
		bool isVarCompoComplex(const QString& var, const QString& compo);

		/*
		* ��ȡĳ�����Ĳ�
		*/
		QStringList getVariableLayer(const QString& var);

		/*
		 * ��ȡĳ֡�����������
		 */
		QStringList getFrameParts(int f);

		/*
		 * ��ȡĳ֡��ʱ��
		 */
		double getFrameTime(int f);

		/*
		 * ��ȡĳ֡���ڵĹ�����
		 */
		QString getFrameCase(int f);

		/*
		* ��ȡĳ֡ģ̬Ƶ�ʽ��
		*/
		bool getFrameFreqValue(int f, double* val);

		/*
		* ��ȡĳ֡��ģ̬���
		*/
		bool getFrameComplexMode(int f, double* val);

		/*
		* ��ȡ�������ͣ�0-�ڵ��1-��Ԫ��
		*/
		int getVariableType(const QString& var);

		/*
		* ����غ���ȡ��Ӧ��/Ӧ�䵥Ԫ����
		*/
		void appendShellLayer(const QString& name, int nlayer);

		/*
		* ��ȡ�غ���ȡ��Ӧ��/Ӧ�����
		*/
		int getShellLayerCount(const QString& name);

		/*********************************************************************************����******************************************************************************************/
		/*
		* �������
		*/
		void appendSetData(mPostSetBase* setData);

		/*
		* ��ȡ���и���
		*/
		int getSetDataCount();

		/*
		* ��ȡ���ݣ���������
		*/
		mPostSetBase* getSetData(int i);

		/*
		* ��ȡ���ݣ���������
		*/
		mPostSetBase* getSetData(const QString& name);

		/*
		* ��ȡ����ͨ�ü��ϵ����ƣ��������غɺͱ߽缯�ϵ����⼯�ϣ�
		*/
		QStringList getSetNames();

		/*
		* �滻�������ݣ���������
		*/
		void replaceSetData(mPostSetBase* sData);

		/*
		* ɾ����������
		*/
		void deleteSetData(const QString& name);

		/*
		* ɾ����������
		*/
		void deleteSetData(int i);

		/*
		* ɾ����������
		*/
		void deleteSetData();

		/*
		* ����������
		*/
		void resetSetName(const QString& oldname, const QString& newname);

		/*
		* �ж����Ƿ����
		*/
		bool isSetNameExist(const QString& name);


		/*
		* ��ȡĳ�����͵����ݣ��������غɺͱ߽缯�ϵ����⼯�ϣ�
		*/
		QList<mPostSetBase*> getTypeSetDatas(PostSetType type);

		/*
		* ��ȡĳ������ͨ�ü��ϵ��������ƣ��������غɺͱ߽缯�ϵ����⼯�ϣ�
		*/
		QStringList getTypeSetNames(PostSetType type);


		/*********************************************************************************����ǽ******************************************************************************************/
		/*
		* �������
		*/
		void appendRigidWallData(mPostRigidWallDataBase* data);

		/*
		* ��ȡ���и���
		*/
		int getRigidWallDataCount();

		/*
		* ��ȡ���ݣ���������
		*/
		mPostRigidWallDataBase* getRigidWallData(QString name);

		/*
		* ��ȡ���еĸ���ǽID
		*/
		QList<QString> getAllRigidWallNames();

	private:
		//����
		QString _modelName;

		//�����ʱ����ػ���Ƶ�����  0-ʱ�䣬1-Ƶ��
		int _resultType;

		//ÿһ֡��Ӧ��ģ������
		QHash<int, mOneFrameData1*> _frameid_frameData;

		//��ǰ֡����һ֡��ģ��һ��
		QHash<int, int> _frameSameFrame;

		//ÿһ֡��Ӧ��ʱ��
		QHash<int, double> _frameid_Time;

		//����֡������
		std::set<int> _allFrameids;

		//����֡�����в���
		std::set<QString> _allPartNames;

		//����֡��Ϣ
		QList<FrameInfo> _frameInfo;

		//�غ���ȡ���ݣ���Ӧ����Ӧ��Ĳ���
		QHash<QString, int> _shellLayers;

		//ĳһ֡�ļ������ݣ��л�֡��ɾ�����ݣ�
		QList<mPostSetBase*> _setData;

		//��ʼ֡�ĸ���ǽ
		QHash<QString, mPostRigidWallDataBase*> _rigidWallDatas;//����ǽ����_����ָ��
	};
}
