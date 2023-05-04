#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "rendbasic_global.h"

//QT
#include <QObject>


namespace MAppConfiguration
{
	class RENDBASIC_EXPORT mFileConfiguration :public QObject
	{
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mFileConfiguration();

		/*
		* ��������
		*/
		~mFileConfiguration();

		/**********************************RecentOpenMxCaeFileDir**************************************/

		/*
		 * �����ʼ����ʱ����ļ���ȡ����򿪵Ĺ����ļ���·��
		 */
		void setRecentOpenMxCaeFileDir(const QList<QString> &dir);

		/*
		 * ��Ӵ򿪵Ĺ����ļ���·��
		 */
		void appendMxCaeFileDir(const QString &fileDir);

		/*
		 * ��ȡ����򿪵Ĺ����ļ���·��(�ϴδ򿪵�)
		 */
		QString getMxCaeFileDir();

		/*
		* ��ȡ����򿪵Ĺ����ļ���·��(������ε�)
		*/
		QList<QString> getMxCaeFileDirData();

		/**********************************RecentInDir**************************************/

		/*
		 * �����ʼ����ʱ����ļ���ȡ����򿪵�·��
		 */
		void setRecentInDir(const QString &dir);

		/*
		 * ��ȡ����򿪵ĵ�·��
		 */
		QString getRecentInDir();


		/**********************************RecentOutDir**************************************/

		/*
		 * �����ʼ����ʱ����ļ���ȡ���������·��
		 */
		void setRecentOutDir(const QString &dir);

		/*
		 * ��ȡ��������ĵ�·��
		 */
		QString getRecentOutDir();

		/**********************************WorkingDir**************************************/

		/*
		 * ���ù���Ŀ¼
		 */
		void setWorkingDir(const QString &dir);

		/*
		 * ��ȡ����Ŀ¼
		 */
		QString getWorkingDir();

	private:

		//����򿪵Ĺ����ļ���·��������
		QList<QString> _recentOpenMxCaeFileDir;

		//���������ߴ򿪣����ļ�����·��
		QString _recentInDir;

		//�����������д����д�ļ�����·����������Ŀ¼��
		QString _recentOutDir;

		//����Ŀ¼
		QString _workingDir;

	public:

		//ǰ���������ڵ��ϵ�ļ���
		QByteArray _prePointCellFileName;//��ȥdat��׺
		QByteArray _prePointLineFileName;//��ȥdat��׺
		QByteArray _postPointCellFileName;//��ȥdat��׺
		QByteArray _postPointLineFileName;//��ȥdat��׺
	};
}