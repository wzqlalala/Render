#pragma once
//�����������
#pragma execution_character_set("utf-8")

//Qt
#include <QObject>
#include <QRunnable>
#include <QHash>
#include <iostream>
#include <fstream>
#include <QStringList>
#include <QVector>
#include <QString>
#include <set>

#include "mMeshViewEnum.h"

namespace MDataPost
{
	class mOneFrameData1;
	class mDataPost1;
	class mPostOneFrameRendData;
}

//namespace MPostView
//{
//	class mPostAnimationRendData1;
//}

using namespace std;
using namespace MDataPost;
namespace MIOFile
{
	class mIMxdbFile1 : public QObject, public QRunnable
	{
		Q_OBJECT
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mIMxdbFile1();

		/*
		* ���ع��캯��
		* initfile��ʾ��ʼ���͵��ļ�������ģ������
		* filenames����֡�ļ�
		*/
		mIMxdbFile1(const QString& initfile, const QStringList& filenames);

		/*
		* ��������
		*/
		~mIMxdbFile1();

		void setDataPost(shared_ptr<mDataPost1> dp) { _dp = dp; };

		/*
		* ���õ�֡�������
		*/
		void setOneFrameRenderData(mPostOneFrameRendData* oneframe);

		/*
		* ��ȡ��֡�������
		*/
		mPostOneFrameRendData* getOneFrameRenderData();

		/*
		* ���ö����������
		*/
		//void setAnimateRenderData(mPostAnimationRendData1* aniframe);

	public slots:

		/*
		* ����ȫ���ļ�����ȡģ�����ݺ�֡��Ϣ��
		*/
		void slot_importAllMxDbFile();

		/*
		* ������ȡ
		*/
		//void slot_importOneFrameData(mPostAnimationRendData1 *animationData, int f, int threadNum, const QString& pname, const QString& var, const QString& compo, const QString& layer, const QString& complex,bool hasstreamLine,QString streamLineVectorName, bool hasvectorArrowVector, QString vectorArrowVectorName);

		/*
		* ��ȡ��֡�������ݣ���Ҫ������ͼ�Ͷ�����
		*/
		bool readCloudData();

		/*
		* ��ȡ��֡λ��ʸ�����ݣ���Ҫ���ڱ���ͼ��
		*/
		void readDisplacementData();

		/*
		* ��ȡ��֡ʸ�����ݣ���Ҫ��������ͼ��ʸ��ͼ�ȣ�
		*/
		void readVectorData(const QString& pname, int f, const QString& var);

		/*
		* ��ȡʱ��-������������
		*/
		void readCurveData1(const QString& pname, const QString& var, const QStringList& compos, const QString& layer, const QString& complex, const set<int>& ids,
			QStringList& cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues);

		/*
		* ��ȡID-������������
		*/
		void readCurveData2(const QString& pname, const QString& var, const QStringList& compos, const QString& layer, const QString& complex, const QVector<int>& ids, const set<int>& frameIds,
			QStringList& cnames, QHash<QString, QHash<int, double>>& yvalues);

		/*
		* ��ȡ����-������������
		*/
		void readCurveData3(const QString& pname, const QString& varx, const QString& compox, const QString& layerx, const QString& complexx,
			const QString& vary, const QStringList& composy, const QString& layery, const QString& complexy, 
			const set<int>& ids,QStringList& cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues);

		/*
		* ��ȡ�غ�
		*/
		void readLoadData(const QString& pname, const QStringList& casenames, std::set<int>& ids, const QString& type);

		/*
		* ��ȡ���б�����ֵ��Ϣ
		*/
		void readAllResults(const QString& pname, QList<QStringList>& vals);

		/*
		* ��ȡָ��������������Ϣ
		*/
		void readScalarData(const QString& pname, int f, const QString& var, QHash<int, double> &vals,MViewBasic::NodeOrElement &nodeOrElement);

		/*
		* ��ȡָ��ʸ����������Ϣ
		*/
		void readVectorData(const QString& pname, int f, const QString& var, QHash<int,QVector3D> &vals, MViewBasic::NodeOrElement &nodeOrElement);


	private:

		/*
		* ����run����
		*/
		void run() override;
		/*
		* �ж��ļ�״̬
		*/
		bool openFileStatus(const QString& fname);

		/*
		* ��ȡ��ʼģ������
		*/
		void readInitModelDatas(shared_ptr<mDataPost1> dataPost,mOneFrameData1* fd);

		/*
		* ��ȡģ������
		*/
		void readModelDatas(mOneFrameData1* fd);

	signals:
		/*
		* �����źţ������ļ���ȡ���
		* ����type��ʾ���������
		*/
		//void readFileStatusSig(IOFileErrorType type = UnknownError);

		/*
		* �����źţ���ȡ�ɹ�
		*/
		void readFileSuccessSig(const QString& filename);

		/*
		* �����źţ���ȡȫ��֡�ļ��ɹ�
		*/
		void finishedImportAllMxDbFileSig();


	private:
		//�汾��
		unsigned int _version;

		//��ʼ�����ļ�
		QString _initFileName;

		//֡�ļ�
		QStringList _frameFiles;

		//�������ݿ�����
		QString _postDataName;

		shared_ptr<mDataPost1> _dp;

		//����֡��Ϣ
		//static QList<FrameInfo> _frameInfo;

		//�ļ�������
		ifstream _readf;

		//��֡�������
		mPostOneFrameRendData* _oneFrameRender;

		//�����������
		//mPostAnimationRendData1* _animateRender;
		//���ɶ�����Ҫ�Ĳ���
		int _frame;
		//mPostAnimationRendData1 *_animationData; int _threadNum; QString _pname, _var, _compo, _layer, _complex; bool _hasstreamLine; QString _streamLineVectorName; bool _hasvectorArrowVector; QString _vectorArrowVectorName;
		//long long int _byteAmount;
	};


}