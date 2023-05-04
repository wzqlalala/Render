#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QObject>
#include <QVector>
#include <QPair>
#include <QThread>
#include <set>



namespace MDataPost
{
	class mOneFrameData1;
	class mPostMeshVisualThread;
	class RENDDATA_EXPORT mPostMeshVisualControl :public QObject
	{
		Q_OBJECT

	public:
		mPostMeshVisualControl();

		~mPostMeshVisualControl();

		//��������
		void maskMesh(mOneFrameData1 *oneFrameData,std::set<int> meshIDs);

		//��ʾ����
		void showMesh(mOneFrameData1 *oneFrameData,std::set<int> meshIDs);

		//������������
		void reverseMaskMesh(mOneFrameData1 *oneFrameData);

		//��ʾ��������
		void showAllMesh(mOneFrameData1 *oneFrameData);

	private:

		void doSomeWork(mOneFrameData1 * oneFrameData);

	private slots:

		void slot_finishedWork();

	signals:

		void startWorkSig();

		void finishedWorkSig();

	private:

		QVector<QPair<mPostMeshVisualThread*, QThread*>> _threads;
	};
}

