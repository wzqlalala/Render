#pragma once

//Qt
#include <QString>
#include <QStringList>
#include <QHash>
#include <QVector3D>

/*
* ����������Ϣ
*/
struct VariableInfo
{
	//������ͣ��������/�ڵ㵥Ԫ����
	int _resultType;

	//������
	QString _varName;

	//�������ͣ��ڵ�/��Ԫ
	int _varType;

	//��������
	int _varCount;

	//ÿ�������Ĳ���
	QList<int> _varLayer;

	//ÿ�������Ĳ�����
	QList<QStringList> _varLayerNames;

	//������
	int _nCompo;

	//�Ƿ����ʸ��ģ
	bool _isMag;

	//������
	QStringList _compoNames;

	//�÷����Ƿ�Ϊ����
	QHash<QString, bool> _compoComplex;

	//�ֽڿ�ʼλ��
	long long int _varByte;
};

/*
* Ƶ�ʽ��
*/
struct SpecialFrequency
{
	//ģ̬Ƶ��
	double _modalFreq;

	//ģ̬����ֵ
	double _modalEigen;
};

/*
* ��ģ̬���
*/
struct ComplexModes
{
	//ģֵ̬ʵ��
	double _realVal;

	//ģֵ̬�鲿
	double _virtualVal;

	//Ƶ��
	double _freqVal;

	//����
	double _dampVal;
};

/*
* ���-�����
*/
struct EMForce
{
	//����
	QString _name;

	//ֵ
	double val[3];
};

/*
* ���-�������
*/
struct EMTorque
{
	//����
	QString _name;

	//ֵ
	double val[3];
};

/*
* ���-�������
*/
struct EMEddyCurrentLoss
{
	//����
	QString _name;

	//ֵ
	double val;
};

/*
* ���-����
*/
struct EMEnergy
{
	//����
	QString _name;

	//ֵ
	double val;
};

/*
* ���-���
*/
struct EMInductance
{
	//����
	QString _name;

	//�������
	QStringList _matrixtitle;

	//ֵ
	double* val;
};

/*
* ���-����
*/
struct EMCapacitance
{
	//����
	QString _name;

	//�������
	QStringList _matrixtitle;

	//ֵ
	double* val;
};

/*
* ���-ŷķ���
*/
struct EMOhmicLoss
{
	//����
	QString _name;

	//ֵ
	double val;
};

/*
* ���-�������
*/
struct EMHysteresisLoss
{
	//����
	QString _name;

	//ֵ
	double val;
};

/*
* ֡��Ϣ
*/
struct FrameInfo
{
	void setRigidWallEndPos(int ID, QVector3D pos)
	{
		_rigidWallEndPos.insert(QString("rigidwall_%1").arg(ID), pos);
	};
	QHash<QString, QVector3D> getRigidWallEndPos()
	{
		return _rigidWallEndPos;
	};

	//��Ӧ���ļ���
	QString _frameFile;

	//֡��
	int _frameNum;

	//֡����
	QString _frameName;

	//֡��������
	QString _frameCase;

	//֡ʱ��
	double _frameTime;

	//֡ģ����Ϣ
	int _frameModel;

	//֡�����Ϣ
	QList<VariableInfo> _frameVars;

	//�Ƿ�������ֵ���
	bool _isFrequency = false;

	//����ֵ���
	SpecialFrequency _frequency;

	//�Ƿ��и�ģ̬���
	bool _isComplexMode = false;

	//��ģ̬���
	ComplexModes _compMode;

	//�Ƿ��е��-��������
	bool _isEMForce = false;

	//���-��������
	QList<EMForce> _emforce;

	//�Ƿ��е��-������ؽ��
	bool _isEMTorque = false;

	//���-������ؽ��
	QList<EMTorque> _emtorque;

	//�Ƿ��е��-������Ľ��
	bool _isEMEddyCurrentLoss = false;

	//���-������Ľ��
	QList<EMEddyCurrentLoss> _emeddyloss;

	//�Ƿ��е��-����
	bool _isEMEnergy = false;

	//���-�������
	QList<EMEnergy> _emenergy;

	//�Ƿ��е��-��н��
	bool _isEMInductance = false;

	//���-��н��
	QList<EMInductance> _eminductance;

	//�Ƿ��е��-���ݽ��
	bool _isEMCapacitance = false;

	//���-���ݽ��
	QList<EMCapacitance> _emcapacitance;

	//�Ƿ��е��-ŷķ���
	bool _isEMOhmicLoss = false;

	//���-ŷķ���
	QList<EMOhmicLoss> _emohmicloss;

	//�Ƿ��е��-�������
	bool _isEMHysteresisLoss = false;

	//���-�������
	QList<EMHysteresisLoss> _emhysteresisloss;

	//�غɽ��
	QHash<QString, long long int> _loadResults;

	//����ǽ��ֹ����
	QHash<QString, QVector3D> _rigidWallEndPos;
};