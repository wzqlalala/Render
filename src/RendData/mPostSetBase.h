#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include "mPostEnum.h"
//QT
#include <QObject>
#include <set>

namespace MDataPost
{
	class RENDDATA_EXPORT mPostSetBase
	{
	
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mPostSetBase();

		/*
		* ���ع��캯��
		*/
		mPostSetBase(const QString& name, PostSetType type);

		/*
		* ��������
		*/
		~mPostSetBase();

		/*
		* ������
		*/
		void setSetName(const QString& name);

		/*
		* ��ȡ��
		*/
		QString getSetName();

		/*
		* ��������
		*/
		void setSetType(PostSetType type);

		/*
		* ��ȡ����
		*/
		PostSetType getSetType();

		/*
		* ��ȡ���������ַ���
		*/
		QString getSetTypeStr();
	
	private:

		//��
		QString _setName;

		//����
		PostSetType _setType;
	};

	class RENDDATA_EXPORT mPostNodeSet : public mPostSetBase
	{
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mPostNodeSet();

		/*
		* ���ع��캯��
		*/
		mPostNodeSet(const QString& name);

		/*
		* ��������
		*/
		~mPostNodeSet();

	public:
		//�ڵ�ID�б���˳��
		std::set<int> _ids;

	};

	class RENDDATA_EXPORT mPostElementSet : public mPostSetBase
	{
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mPostElementSet();

		/*
		* ���ع��캯��
		*/
		mPostElementSet(const QString& name);

		/*
		* ��������
		*/
		~mPostElementSet();

	public:
		//��ԪID�б���˳��
		std::set<int> _ids;

	};

	class RENDDATA_EXPORT mPostElementSurfaceSet: public mPostSetBase
	{
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mPostElementSurfaceSet();

		/*
		* ���ع��캯��
		*/
		mPostElementSurfaceSet(const QString& name);

		/*
		* ��������
		*/
		~mPostElementSurfaceSet();

	public:
		//��Ԫ��ID�б���˳��
		std::set<int> _ids;

	};

	class RENDDATA_EXPORT mPostPartSet : public mPostSetBase
	{
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mPostPartSet();

		/*
		* ���ع��캯��
		*/
		mPostPartSet(const QString& name);

		/*
		* ��������
		*/
		~mPostPartSet();

	public:
		//�����б���˳��
		std::set<QString> _names;

	};

	class RENDDATA_EXPORT mPostCuttingPlaneSet : public mPostSetBase
	{
	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mPostCuttingPlaneSet();

		/*
		* ���ع��캯��
		*/
		mPostCuttingPlaneSet(const QString& name);

		/*
		* ��������
		*/
		~mPostCuttingPlaneSet();

	public:
		//�����б���˳��
		std::set<QString> _names;

	};
}
