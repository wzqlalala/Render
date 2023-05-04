#include "mIMxdbFile1.h"

//MBasicFunction
#include "mCommonFunctions.h"
#include "mBasicEnum.h"

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostRigidWallData1.h"
#include "mPostOneFrameRendData.h"

//Qt
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QVector>
#include <QVector3D>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <math.h>

#define optimazation false

using namespace MBasicFunction;
QMutex postmutex;
namespace MIOFile
{
	mIMxdbFile1::mIMxdbFile1():QRunnable()
	{
		_oneFrameRender = nullptr;
		//_animateRender = nullptr;
		setAutoDelete(true);
	}

	mIMxdbFile1::mIMxdbFile1(const QString& initfile, const QStringList& filenames):QRunnable()
	{
		_initFileName = initfile;
		_frameFiles = filenames;

		_oneFrameRender = nullptr;
		setAutoDelete(true);
		//_animateRender = nullptr;
	}

	mIMxdbFile1::~mIMxdbFile1()
	{
	}

	void mIMxdbFile1::setOneFrameRenderData(mPostOneFrameRendData * oneframe)
	{
		_oneFrameRender = oneframe;
	}

	mPostOneFrameRendData * mIMxdbFile1::getOneFrameRenderData()
	{
		return _oneFrameRender;
	}

	//void mIMxdbFile1::setAnimateRenderData(mPostAnimationRendData1 * aniframe)
	//{
	//	_animateRender = aniframe;
	//}

	void mIMxdbFile1::slot_importAllMxDbFile()
	{
		//�������ݿ�
		//mDataPost1* datapost = new mDataPost1;
		
		//��ȡģ����
		QFileInfo inifinfo(_initFileName);
		QString inifname = inifinfo.baseName();
		if (inifname.isEmpty())
		{
			_dp->setModelName(QString("PostModel"));
			_postDataName = QString("PostModel");
		}
		else
		{
			_dp->setModelName(inifname);
			_postDataName = inifname;
		}

		//ȥ�������ظ��ĺ���
		//mDatabaseSingleton::getInstance()->appendDataPost(datapost);

		//��ȡ��ʼ���������ļ�
		bool openinit = this->openFileStatus(_initFileName);
		if (openinit)
		{
			mOneFrameData1* initFrame = new mOneFrameData1;
			this->readInitModelDatas(_dp, initFrame);
			_readf.close();

			emit readFileSuccessSig(_initFileName);

			//�洢
			_dp->appendFrameData(0, 0.0, initFrame);
		}
		else
		{
			//�����Ϣ
			//mGlobalSignals::getInstance()->outputMessageSig(0, QString("û�г�ʼ֡�ļ�..."));
			_readf.close();
			return;
		}

		//��ȡ֡�ļ�
		const int nframe = _frameFiles.size();
		for (int fi = 0; fi < nframe; ++fi)
		{
			//�ж��ļ���״̬
			QString framefile = _frameFiles.at(fi);
			bool isopen = this->openFileStatus(framefile);
			if (!isopen)
			{
				//emit readFileStatusSig(FileNotOpen);
				continue;
			}

			FrameInfo frameInfo;
			frameInfo._frameFile = framefile;
			frameInfo._frameNum = fi + 1;

			//�洢֡����
			QFileInfo finfo(framefile);
			QString fname = finfo.baseName();
			QString caseinfo = fname.remove(inifname + "_");
			QString framename = QString("%1.%2").arg(fi + 1).arg(caseinfo);
			frameInfo._frameName = framename;

			//��ȡ������
			char caseName[64];
			_readf.read(caseName, 64 * sizeof(char));
			//_byteAmount += 64*sizeof(char);
			frameInfo._frameCase = QString(caseName).simplified();

			//��ȡʱ��/Ƶ�����ͣ�����0xA1B1C103����֮��İ汾��
			if (_version >= 0xA1B1C103)
			{
				int typeval;
				_readf.read((char*)&typeval, sizeof(int));
				_dp->setResultType(typeval);
			}

			//��ȡ���洢ʱ��/Ƶ��
			double timeval;
			_readf.read((char*)&timeval, sizeof(double));
			//_byteAmount += sizeof(double);
			frameInfo._frameTime = timeval;

			//��ȡģ���Ƿ��Ѵ��ڵı�ʶ
			int sameframe;
			_readf.read((char*)&sameframe, sizeof(int));
			//_byteAmount += sizeof(int);
			frameInfo._frameModel = sameframe;
			_dp->appendSameFrame(fi + 1, sameframe);

			mOneFrameData1* framedata = nullptr;
			if (sameframe == -1)
			{
				//֡����
				framedata = new mOneFrameData1;

				//�����Ϣ
				//mGlobalSignals::getInstance()->outputMessageSig(0, QString("���ڶ�ȡģ������..."));
				this->readModelDatas(framedata);

				_dp->appendFrameData(fi + 1, timeval, framedata);
			}
			else
			{
				//��ȡ��ͬ��ģ������
				framedata = _dp->getOneFrameData(sameframe);
				_dp->appendFrameData(fi + 1, timeval, framedata);
			}
			if (framedata == nullptr)
			{
				continue;
			}

			//�����Ϣ
			//mGlobalSignals::getInstance()->outputMessageSig(0, QString("���ڶ�ȡ������Ϣ..."));

			//��ȡ��������
			int varCount;
			_readf.read((char*)&varCount, sizeof(int));
			//_byteAmount += sizeof(int);

			for (int i = 0; i < varCount; ++i)
			{
				//��ȡ�������
				int restype;
				_readf.read((char*)&restype, sizeof(int));
				//_byteAmount += sizeof(int);

				if (restype == 0)
				{
					VariableInfo varinfo;
					varinfo._resultType = restype;

					//��ȡ������
					char varname[64];
					_readf.read(varname, 64 * sizeof(char));
					//_byteAmount += 64 * sizeof(char);
					varinfo._varName = QString(varname);

					//��ȡ����
					int vartype;
					_readf.read((char*)&vartype, sizeof(int));
					//_byteAmount += sizeof(int);
					varinfo._varType = vartype;

					//��ȡ��������
					int varcount;
					_readf.read((char*)&varcount, sizeof(int));
					//_byteAmount += sizeof(int);
					varinfo._varCount = varcount;

					//��ȡÿ�������Ĳ�����������
					QList<int> nlayerlist;
					for (int j = 0; j < varcount; ++j)
					{
						int nlayer;
						_readf.read((char*)&nlayer, sizeof(int));
						//_byteAmount += sizeof(int);
						nlayerlist.append(nlayer);

						QStringList lnames;
						for (int k = 0; k < nlayer; ++k)
						{
							char cstr[32];
							_readf.read(cstr, 32 * sizeof(char));
							//_byteAmount += 32 * sizeof(char);
							lnames.append(QString(cstr));
						}
						varinfo._varLayerNames.append(lnames);
					}
					varinfo._varLayer = nlayerlist;

					//��ȡ������
					int ncompo;
					_readf.read((char*)&ncompo, sizeof(int));
					//_byteAmount += sizeof(int);
					varinfo._nCompo = ncompo;

					//��ȡ�Ƿ����ʸ��ģ
					bool ismag;
					_readf.read((char*)&ismag, sizeof(bool));
					//_byteAmount += sizeof(bool);
					varinfo._isMag = ismag;

					//��ȡ������
					QStringList componames;
					QHash<QString, bool> compoplex;
					for (int j = 0; j < ncompo; ++j)
					{
						//������
						char cstr[32];
						_readf.read(cstr, 32 * sizeof(char));
						//_byteAmount += 32 * sizeof(char);
						componames.append(QString(cstr));

						//�Ƿ�Ϊ����
						bool bpara = false;
						_readf.read((char*)&bpara, sizeof(bool));
						//_byteAmount += sizeof(bool);
						compoplex.insert(QString(cstr), bpara);
					}
					varinfo._compoNames = componames;
					varinfo._compoComplex = compoplex;

					//��ȡ�ñ�����ʼ���ֽ�λ��
					long long int varbyte;
					_readf.read((char*)&varbyte, sizeof(long long int));
					//_byteAmount += sizeof(long long int);
					varinfo._varByte = varbyte;

					//�洢
					frameInfo._frameVars.append(varinfo);

					//����������ֵ
					for (int j = 0; j < varcount; ++j)
					{
						//��ȡ�ڵ��Ԫ��
						int nloop;
						_readf.read((char*)&nloop, sizeof(int));

						//������Ҫ�������ֽ���������
						int ndouble = ncompo;
						if (ncompo == 0)
						{
							ndouble = 1;
						}
						int nlayer = nlayerlist.at(j);
						if (nlayer != 0)
						{
							ndouble = ncompo * nlayer;
						}
						int skipbyte = nloop * (sizeof(int) + 2 * ndouble * sizeof(double));
						_readf.seekg(skipbyte, ios::cur);
						//_byteAmount += skipbyte;
					}
				}
				else if (restype == 1)
				{
					//��ȡ������
					char namestr[64];
					_readf.read(namestr, 64 * sizeof(char));
					//_byteAmount += 64 * sizeof(char);
					QString name = QString(namestr);

					if (name == QString("special_frequency"))			//Ƶ��ֵ
					{
						SpecialFrequency freq;

						double val[2];
						_readf.read((char*)val, 2 * sizeof(double));
						//_byteAmount += 2 * sizeof(double);
						freq._modalFreq = val[0];
						freq._modalEigen = val[1];

						//�洢
						frameInfo._isFrequency = true;
						frameInfo._frequency = freq;
					}
					if (name == QString("complex_modes"))			//Ƶ��ֵ
					{
						ComplexModes cmode;

						double val[4];
						_readf.read((char*)val, 4 * sizeof(double));
						//_byteAmount += 2 * sizeof(double);
						cmode._realVal = val[0];
						cmode._virtualVal = val[1];
						cmode._freqVal = val[2];
						cmode._dampVal = val[3];

						//�洢
						frameInfo._isComplexMode = true;
						frameInfo._compMode = cmode;
					}
					else if (name == QString("EM_force"))
					{
						EMForce emf;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emf._name = QString(fnamechar);

						//ֵ
						_readf.read((char*)(emf.val), 3 * sizeof(double));

						//�洢
						frameInfo._isEMForce = true;
						frameInfo._emforce.append(emf);
					}
					else if (name == QString("EM_torque"))
					{
						EMTorque emt;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emt._name = QString(fnamechar);

						//ֵ
						_readf.read((char*)(emt.val), 3 * sizeof(double));

						//�洢
						frameInfo._isEMTorque = true;
						frameInfo._emtorque.append(emt);
					}
					else if (name == QString("EM_eddycurrentloss"))
					{
						EMEddyCurrentLoss emecl;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emecl._name = QString(fnamechar);

						//ֵ
						_readf.read((char*)&(emecl.val), sizeof(double));

						//�洢
						frameInfo._isEMEddyCurrentLoss = true;
						frameInfo._emeddyloss.append(emecl);
					}
					else if (name == QString("EM_energy"))
					{
						EMEnergy eme;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						eme._name = QString(fnamechar);

						//ֵ
						_readf.read((char*)&(eme.val), sizeof(double));

						//�洢
						frameInfo._isEMEnergy = true;
						frameInfo._emenergy.append(eme);
					}
					else if (name == QString("EM_inductance"))
					{
						EMInductance emi;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emi._name = QString(fnamechar);

						//�����������
						int nrow;
						_readf.read((char*)&nrow, sizeof(int));

						//������������
						for (int mi = 0; mi < nrow; ++mi)
						{
							char mchar[64];
							_readf.read(mchar, 64 * sizeof(char));
							emi._matrixtitle.append(QString(mchar));
						}

						//ֵ
						double* vals = new double[nrow*nrow];
						_readf.read((char*)vals, nrow*nrow * sizeof(double));
						emi.val = vals;

						//�洢
						frameInfo._isEMInductance = true;
						frameInfo._eminductance.append(emi);
					}
					else if (name == QString("EM_capacitance"))
					{
						EMCapacitance emc;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emc._name = QString(fnamechar);

						//�����������
						int nrow;
						_readf.read((char*)&nrow, sizeof(int));

						//������������
						for (int mi = 0; mi < nrow; ++mi)
						{
							char mchar[64];
							_readf.read(mchar, 64 * sizeof(char));
							emc._matrixtitle.append(QString(mchar));
						}

						//ֵ
						double* vals = new double[nrow*nrow];
						_readf.read((char*)vals, nrow*nrow * sizeof(double));
						emc.val = vals;

						//qDebug() << vals[0] << " " <<  vals[1] << " " << vals[2] << endl;
						//qDebug() << vals[3] << " " << vals[4] << " " << vals[5] << endl;
						//qDebug() << vals[6] << " " << vals[7] << " " << vals[8] << endl;

						//�洢
						frameInfo._isEMCapacitance = true;
						frameInfo._emcapacitance.append(emc);
					}
					else if (name == QString("EM_ohmicloss"))
					{
						EMOhmicLoss eme;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						eme._name = QString(fnamechar);

						//ֵ
						_readf.read((char*)&(eme.val), sizeof(double));

						//�洢
						frameInfo._isEMOhmicLoss = true;
						frameInfo._emohmicloss.append(eme);
					}
					else if (name == QString("EM_hysteresisloss"))
					{
						EMHysteresisLoss eme;

						//����
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						eme._name = QString(fnamechar);

						//ֵ
						_readf.read((char*)&(eme.val), sizeof(double));

						//�洢
						frameInfo._isEMHysteresisLoss = true;
						frameInfo._emhysteresisloss.append(eme);
					}
					else if (name == QString("RigidWallPos"))
					{
						int rigidWallNum = _dp->getRigidWallDataCount();
						for (int j = 0; j < rigidWallNum; j++)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							//��ǰλ��
							double endpos[3];
							_readf.read((char*)endpos, 3 * sizeof(double));

							//framedata->setRigidWallEndPos(id, QVector3D(endpos[0], endpos[1], endpos[2]));
						}
					}
				}
				else if (restype == 2)
				{
					//��ȡ�غ���
					char loadname[64];
					_readf.read(loadname, 64 * sizeof(char));
					QString namestr = QString(loadname);

					//��ȡ�ֽ�λ��
					long long int varbyte;
					_readf.read((char*)&varbyte, sizeof(long long int));

					//�洢
					frameInfo._loadResults.insert(namestr, varbyte);

					//�����غ����������ֽ���
					if (namestr.compare("ShellStrain", Qt::CaseInsensitive) == 0 ||
						namestr.compare("ShellStress", Qt::CaseInsensitive) == 0)		//�ǵ�ԪӦ���Ӧ��
					{
						//��ȡ�ǵ�Ԫ����
						int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4)+
							framedata->getNumByElementType(TRIA3)+ framedata->getNumByElementType(QPH);

						int nlayer = 0;
						for (int ish = 0; ish < nshell; ++ish)
						{
							//����ID
							_readf.seekg(sizeof(int), ios::cur);

							//��ȡ����
							int shelllayer;
							_readf.read((char*)&shelllayer, sizeof(int));

							//ȡ�������ֵ������֡�Ŀǲ�����һ�£����ֻͳ�Ƶ�һ֡��
							if (fi == 0)
							{
								nlayer = max(nlayer, shelllayer);
							}

							//����ֵ
							_readf.seekg(shelllayer* 9 * sizeof(double), ios::cur);
						}
						if (fi == 0)
						{
							_dp->appendShellLayer(namestr, nlayer);
						}
					}
					else if (namestr.compare("BarEndLoad", Qt::CaseInsensitive) == 0)		//������-����Ԫ�͸˵�Ԫ
					{
						//��ȡ�˵�Ԫ������Ԫ����
						int nbar = framedata->getNumByElementType(D3R2) + framedata->getNumByElementType(D3B2);

						//�����ֽ���
						_readf.seekg(nbar * (sizeof(int) + 4 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("BarStress", Qt::CaseInsensitive) == 0)	//����ԪӦ��
					{
						//��ȡ����Ԫ����
						int nbar = framedata->getNumByElementType(D3B2);

						//�����ֽ���
						_readf.seekg(nbar * (sizeof(int) + 13 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShearStress", Qt::CaseInsensitive) == 0)	//���а�Ӧ��
					{
						//��ȡ����Ԫ����
						int nshear = framedata->getNumByElementType(SPQ4);

						//�����ֽ���
						_readf.seekg(nshear * (sizeof(int) + 3 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShearLoad", Qt::CaseInsensitive) == 0)	//���а��غ�
					{
						//��ȡ����Ԫ����
						int nshear = framedata->getNumByElementType(SPQ4);

						//�����ֽ���
						_readf.seekg(nshear * (sizeof(int) + 4 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShellForceResultant", Qt::CaseInsensitive) == 0)	//�ǵ�Ԫ�غ�
					{
						//��ȡ�ǵ�Ԫ����
						int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4)+
							framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);

						//�����ֽ���
						_readf.seekg(nshell * (sizeof(int) + 18 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShellForceMoment", Qt::CaseInsensitive) == 0)	//�ǵ�Ԫ����
					{
						//��ȡ�ǵ�Ԫ����
						int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4)+
							framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);

						//�����ֽ���
						_readf.seekg(nshell * (sizeof(int) + 18 * sizeof(double)), ios::cur);
					}
				}
			}

			//�洢
			_dp->appendFrameInfo(frameInfo);

			//�������
			int endtag;
			_readf.read((char*)&endtag, sizeof(int));
			//_byteAmount += sizeof(int);
			if (endtag == 0xA1B1C1D2)
			{
				emit readFileSuccessSig(framefile);
			}

			//�ر��ļ�
			_readf.close();
		}

		//�������ݿ����в�������
		_dp->updateAllPostPartNames();

		emit finishedImportAllMxDbFileSig();
	}

	//void mIMxdbFile1::slot_importOneFrameData(mPostAnimationRendData1 *animationData, int f, int threadNum, const QString & postname, const QString & var, const QString & compo, const QString & layer, const QString & complex, bool hasstreamLine, QString streamLineVectorName, bool hasvectorArrowVector, QString vectorArrowVectorName)
	//{
	//	_animationData = animationData; 
	//	_threadNum = threadNum; 
	//	_frame = f;
	//	_pname = postname, _var = var, _compo = compo, _layer = layer, _complex = complex; _hasstreamLine = hasstreamLine; _streamLineVectorName = streamLineVectorName;  _hasvectorArrowVector = hasvectorArrowVector; _vectorArrowVectorName = vectorArrowVectorName;
	//}

	bool mIMxdbFile1::readCloudData()
	{
		QString complex = "ʵ��";
		int layer = 0;
		int f = 1;
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return false;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return false;
		}

		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			//if (varinfo._varName == var)
			{
				//���ļ�
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return false;
				}

				//��ȡ�������ֽ�λ��
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//��ȡ������
				const int ncompo = varinfo._nCompo;

				//��ȡ��ǰ����������
				const int compoi = 0;

				//�洢ֵ
				double maxval = DBL_MIN, minval = DBL_MAX;
				QHash<int, float> compovalues;

				//��ȡ��ǰ�����еı�������
				const int nresult = varinfo._varCount;
				for (int k = 0; k < nresult; ++k)
				{
					//��ȡ������������
					const int nlayer = varinfo._varLayer.at(k);
					const int layerindex = 0;
					//�������ֵ����������ֵ������
					int nval, valindex;
					if (nlayer == 0)
					{
						if (ncompo == 0)
						{
							nval = 1;
							valindex = 0;
						}
						else
						{
							nval = ncompo;
							valindex = compoi;
						}
					}
					else
					{
						if (ncompo == 0)
						{
							nval = nlayer;
							valindex = layerindex + compoi;
						}
						else
						{
							nval = ncompo * nlayer;
							valindex = ncompo * layerindex + compoi;
						}
					}

					//��ȡ����
					int loopcount;
					_readf.read((char*)&loopcount, sizeof(int));

					//��ȡ����ֵ
					for (int j = 0; j < loopcount; ++j)
					{
						//ID
						int id;
						_readf.read((char*)&id, sizeof(int));

						//ֵ��ʵ����
						double* val = new double[nval];
						_readf.read((char*)val, nval * sizeof(double));

						//ֵ���鲿��
						double* val1 = new double[nval];
						_readf.read((char*)val1, nval * sizeof(double));

						double tempval;
						if (complex == QString("ʵ��"))
						{
							tempval = val[valindex];
						}
						else if (complex == QString("�鲿"))
						{
							tempval = val1[valindex];
						}
						else if (complex == QString("��ֵ"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = sqrt(v0*v0 + v1 * v1);
						}
						else if (complex == QString("��λ"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
						}
						else
						{
							if (valindex == -1 && nval == 3)
							{
								tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
									val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
							}
							else
							{
								tempval = val[valindex];
							}
						}
						compovalues.insert(id, tempval);

						//ȡ��ֵ
						//if (j == 0)
						//{
							//maxval = tempval;
							//minval = tempval;
						//}
						//else
						//{
							maxval = (tempval > maxval) ? tempval : maxval;
							minval = (tempval < minval) ? tempval : minval;
						//}

						delete val;
						delete val1;
					}
				}

				//��ɫ������
				//QString titlestr = var;
				//if (ncompo > 0)
				//{
				//	if (compoi == -1)
				//	{
				//		titlestr = titlestr + "-Mag";
				//	}
				//	else
				//	{
				//		titlestr = titlestr + "-" + compo;
				//	}
				//}
				//if (!layer.isEmpty())
				//{
				//	titlestr = titlestr + ":" + layer;
				//}

				//compovalues.clear();

				//�洢����
				//if (isani)
				//{
				//	if (_animateRender != nullptr)
				//	{
				//		mPostOneFrameData* oneframe = new mPostOneFrameData;
				//		oneframe->setOrignalMaxMinData(maxval, minval);
				//		oneframe->setRendData(f, compovalues, titlestr);
				//		_animateRender->appendRendOneFrameData(f, oneframe);
				//	}
				//}
				//else
				//{
				if (_oneFrameRender != nullptr)
				{
					_oneFrameRender->setOrignalMaxMinData(maxval, minval);
					_oneFrameRender->setRendData(1, compovalues, "");
					_oneFrameRender->setNodeOrElement(MViewBasic::NodeOrElement(varinfo._varType));
				}
				//}

				compovalues.clear();

				_readf.close();

				//�����Ϣ
				//mOutputMessage tepmsg(NormalMessage, QString("��%1֡�ļ����ݶ�ȡ�ɹ���").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				QList<FrameInfo>().swap(frameInfo);
				frameInfo.clear();
				return true;
			}
		}
		return false;
	}

	void mIMxdbFile1::readDisplacementData()
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		int f = 1;
		QString complexstr = "ʵ��";
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}
		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == QString("Displacement"))
			{
				//���ļ�
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//��ȡ�������ֽ�λ��
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//��ȡ�ڵ���
				int nodecount;
				_readf.read((char*)&nodecount, sizeof(int));
				
				QHash<int, QVector3D> dispvals;
				dispvals.reserve(nodecount);
				for (int j = 0; j < nodecount; ++j)
				{
					//�ڵ�ID
					int nid;
					_readf.read((char*)&nid, sizeof(int));

					//λ��ֵ��ʵ����
					double val[3];
					_readf.read((char*)val, 3 * sizeof(double));

					//λ��ֵ���鲿��
					double val1[3];
					_readf.read((char*)val1, 3 * sizeof(double));

					QVector3D valvec(val[0], val[1], val[2]);
					if (complexstr == QString("ʵ��"))
					{
						valvec = QVector3D(val[0], val[1], val[2]);
					}
					else if (complexstr == QString("�鲿"))
					{
						valvec = QVector3D(val1[0], val1[1], val1[2]);
					}
					else if (complexstr == QString("��ֵ"))
					{
						double magx = sqrt(val[0] * val[0] + val1[0] * val1[0]);
						double magy = sqrt(val[1] * val[1] + val1[1] * val1[1]);
						double magz = sqrt(val[2] * val[2] + val1[2] * val1[2]);
						valvec = QVector3D(magx, magy, magz);
					}
					else if (complexstr == QString("��λ"))
					{
						double magx = atan(val[0]/val1[0]);
						double magy = atan(val[1]/val1[1]);
						double magz = atan(val[2]/val1[2]);
						valvec = QVector3D(magx, magy, magz);
					}

					dispvals.insert(nid, valvec);
				}

				//�洢����
				//if (isani)
				//{
				//	if (_animateRender != nullptr)
				//	{
				//		mPostOneFrameData* oneframe = _animateRender->getRendOneFrameData(f);
				//		if (oneframe != nullptr)
				//		{
				//			oneframe->setNodeDisplacementData(dispvals);
				//		}
				//	}
				//}
				//else
				{
					if (_oneFrameRender != nullptr)
					{
						_oneFrameRender->setNodeDisplacementData(dispvals);
					}
				}

				dispvals.clear();

				//�����Ϣ
				//mOutputMessage tepmsg(NormalMessage, QString("��%1֡�ļ�λ�����ݶ�ȡ�ɹ���").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				_readf.close();
				return;
			}
		}
	}

	void mIMxdbFile1::readVectorData(const QString& pname, int f, const QString & var)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}
		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == var)
			{
				//��ȡ������
				const int ncompo = varinfo._nCompo;
				if (ncompo != 3)
				{
					return;
				}

				//���ļ�
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//��ȡ�������ֽ�λ��
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//��ȡ�ڵ�/��Ԫ��
				int nodecount;
				_readf.read((char*)&nodecount, sizeof(int));

				QHash<int, QVector3D> vecvals;
				vecvals.reserve(nodecount);
				for (int j = 0; j < nodecount; ++j)
				{
					//�ڵ�ID
					int nid;
					_readf.read((char*)&nid, sizeof(int));

					//ʸ��ֵ��ʵ����
					double val[3];
					_readf.read((char*)val, 3 * sizeof(double));

					//ʸ��ֵ���鲿��
					double val1[3];
					_readf.read((char*)val1, 3 * sizeof(double));

					vecvals.insert(nid, QVector3D(val[0], val[1], val[2]));
				}

				//��ȡ��������
				MViewBasic::NodeOrElement vartype = MViewBasic::NodeOrElement(varinfo._varType);

				//�洢
				if (_oneFrameRender != nullptr)
				{
					_oneFrameRender->setRendData(vecvals, vartype);
				}
				//else if (isani&&_animateRender != nullptr)
				//{
				//	mPostOneFrameData* oneframe = _animateRender->getRendOneFrameData(f);
				//	if (oneframe != nullptr)
				//	{
				//		oneframe->setRendData(vecvals, vartype);
				//	}
				//}

				//�����Ϣ
				//mOutputMessage tepmsg(NormalMessage, QString("��%1֡�ļ�ʸ�����ݶ�ȡ�ɹ���").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				_readf.close();
				return;
			}
		}
	}

	void mIMxdbFile1::readCurveData1(const QString& pname, const QString & var, const QStringList & compos, const QString & layer, const QString & complex,
		const set<int>& ids, QStringList & cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		const int nframe = frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int ii = 0; ii < nvar; ++ii)
			{
				VariableInfo varinfo = finfo._frameVars.at(ii);
				if (varinfo._varName == var)
				{
					//���ļ�
					bool isopen = openFileStatus(finfo._frameFile);
					if (!isopen)
					{
						return;
					}

					//��ȡ�������ֽ�λ��
					long long int varbyte = varinfo._varByte;
					_readf.seekg(varbyte, ios::beg);

					//��ȡ������
					const int ncompo = varinfo._nCompo;

					//����X����
					xvalues.append(finfo._frameTime);

					//��ȡ��ǰ�����еı�������
					const int nresult = varinfo._varCount;
					for (int k = 0; k < nresult; ++k)
					{
						//��ȡ������������
						const int nlayer = varinfo._varLayer.at(k);
						const int layerindex = varinfo._varLayerNames.at(k).indexOf(layer);
						//�������ֵ����
						int nval;
						if (nlayer == 0)
						{
							if (ncompo == 0)
							{
								nval = 1;
							}
							else
							{
								nval = ncompo;
							}
						}
						else
						{
							if (ncompo == 0)
							{
								nval = nlayer;
							}
							else
							{
								nval = ncompo * nlayer;
							}
						}

						//��ȡ����
						int loopcount;
						_readf.read((char*)&loopcount, sizeof(int));

						//��ȡ����ֵ
						for (int j = 0; j < loopcount; ++j)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							if (ids.find(id) != ids.end())
							{
								//ֵ��ʵ����
								double* val = new double[nval];
								_readf.read((char*)val, nval * sizeof(double));

								//ֵ���鲿��
								double* val1 = new double[nval];
								_readf.read((char*)val1, nval * sizeof(double));

								for (QString compo : compos)
								{
									//��������
									QString cname = QString("Time-%1_%2_%3_%4-ID%5").arg(var).arg(compo).arg(layer).arg(complex).arg(id);
									if (!cnames.contains(cname))
									{
										cnames.append(cname);
									}

									//��ȡ��ǰ����������
									const int compoi = varinfo._compoNames.indexOf(compo);
									//�������������
									int valindex;
									if (nlayer == 0)
									{
										if (ncompo == 0)
										{
											valindex = 0;
										}
										else
										{
											valindex = compoi;
										}
									}
									else
									{
										if (ncompo == 0)
										{
											valindex = layerindex + compoi;
										}
										else
										{
											valindex = ncompo * layerindex + compoi;
										}
									}
									//ȡֵ
									double tempval;
									if (complex == QString("ʵ��"))
									{
										tempval = val[valindex];
									}
									else if (complex == QString("�鲿"))
									{
										tempval = val1[valindex];
									}
									else if (complex == QString("��ֵ"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = sqrt(v0*v0 + v1 * v1);
									}
									else if (complex == QString("��λ"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
									}
									else
									{
										if (valindex == -1 && nval == 3)
										{
											tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
												val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
										}
										else
										{
											tempval = val[valindex];
										}
									}
									//�洢
									QVector<double> tempvec;
									if (yvalues.contains(cname))
									{
										tempvec = yvalues.value(cname);
									}
									tempvec.append(tempval);
									yvalues.insert(cname, tempvec);
								}
							}
							else
							{
								_readf.seekg(2 * nval * sizeof(double), ios::cur);
							}
						}
					}

					//�ر��ļ�
					_readf.close();
				}
			}
		}
	}

	void mIMxdbFile1::readCurveData2(const QString& pname, const QString & var, const QStringList & compos, const QString & layer, const QString & complex, const QVector<int>& ids,
		const set<int>& frameIds, QStringList & cnames, QHash<QString, QHash<int, double>>& yvalues)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		const int nframe = frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = frameInfo.at(i);
			if (frameIds.find(finfo._frameNum) != frameIds.end())
			{
				const int nvar = finfo._frameVars.size();
				for (int ii = 0; ii < nvar; ++ii)
				{
					VariableInfo varinfo = finfo._frameVars.at(ii);
					if (varinfo._varName == var)
					{
						//���ļ�
						bool isopen = openFileStatus(finfo._frameFile);
						if (!isopen)
						{
							return;
						}

						//��ȡ�������ֽ�λ��
						long long int varbyte = varinfo._varByte;
						_readf.seekg(varbyte, ios::beg);

						//��ȡ������
						const int ncompo = varinfo._nCompo;

						//��ȡ��ǰ�����еı�������
						const int nresult = varinfo._varCount;
						for (int k = 0; k < nresult; ++k)
						{
							//��ȡ������������
							const int nlayer = varinfo._varLayer.at(k);
							const int layerindex = varinfo._varLayerNames.at(k).indexOf(layer);
							//�������ֵ����������ֵ������
							int nval;
							if (nlayer == 0)
							{
								if (ncompo == 0)
								{
									nval = 1;
								}
								else
								{
									nval = ncompo;
								}
							}
							else
							{
								if (ncompo == 0)
								{
									nval = nlayer;
								}
								else
								{
									nval = ncompo * nlayer;
								}
							}

							//��ȡ����
							int loopcount;
							_readf.read((char*)&loopcount, sizeof(int));

							//��ȡ����ֵ
							for (int j = 0; j < loopcount; ++j)
							{
								//ID
								int id;
								_readf.read((char*)&id, sizeof(int));

								if (ids.contains(id))
								{
									//ֵ��ʵ����
									double* val = new double[nval];
									_readf.read((char*)val, nval * sizeof(double));

									//ֵ���鲿��
									double* val1 = new double[nval];
									_readf.read((char*)val1, nval * sizeof(double));

									for (QString compo : compos)
									{
										//��������
										QString cname = QString("ID-%1_%2_%3_%4-f%5").arg(var).arg(compo).arg(layer).arg(complex).arg(finfo._frameNum);
										if (!cnames.contains(cname))
										{
											cnames.append(cname);
										}

										//��ȡ��ǰ����������
										const int compoi = varinfo._compoNames.indexOf(compo);
										//�������������
										int valindex;
										if (nlayer == 0)
										{
											if (ncompo == 0)
											{
												valindex = 0;
											}
											else
											{
												valindex = compoi;
											}
										}
										else
										{
											if (ncompo == 0)
											{
												valindex = layerindex + compoi;
											}
											else
											{
												valindex = ncompo * layerindex + compoi;
											}
										}
										//ȡֵ
										double tempval;
										if (complex == QString("ʵ��"))
										{
											tempval = val[valindex];
										}
										else if (complex == QString("�鲿"))
										{
											tempval = val1[valindex];
										}
										else if (complex == QString("��ֵ"))
										{
											double v0 = val[valindex];
											double v1 = val1[valindex];
											tempval = sqrt(v0*v0 + v1 * v1);
										}
										else if (complex == QString("��λ"))
										{
											double v0 = val[valindex];
											double v1 = val1[valindex];
											tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
										}
										else
										{
											if (valindex == -1 && nval == 3)
											{
												tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
													val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
											}
											else
											{
												tempval = val[valindex];
											}
										}
										//�洢
										QHash<int, double> tempvec;
										if (yvalues.contains(cname))
										{
											tempvec = yvalues.value(cname);
										}
										tempvec.insert(id, tempval);
										yvalues.insert(cname, tempvec);
									}
								}
								else
								{
									_readf.seekg(2 * nval * sizeof(double), ios::cur);
								}
							}
						}

						//�ر��ļ�
						_readf.close();
					}
				}
			}
		}
	}

	void mIMxdbFile1::readCurveData3(const QString& pname, const QString & varx, const QString & compox, const QString & layerx, const QString & complexx,
		const QString & vary, const QStringList & composy, const QString & layery, const QString & complexy, 
		const set<int>& ids, QStringList & cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		const int nframe = frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int ii = 0; ii < nvar; ++ii)
			{
				VariableInfo varinfo = finfo._frameVars.at(ii);

				//Xֵ
				if (varinfo._varName == varx)
				{
					//���ļ�
					bool isopen = openFileStatus(finfo._frameFile);
					if (!isopen)
					{
						return;
					}

					//��ȡ�������ֽ�λ��
					long long int varbyte = varinfo._varByte;
					_readf.seekg(varbyte, ios::beg);

					//��ȡ������
					const int ncompo = varinfo._nCompo;

					//��ȡ��ǰ�����еı�������
					const int nresult = varinfo._varCount;
					for (int k = 0; k < nresult; ++k)
					{
						//��ȡ������������
						const int nlayer = varinfo._varLayer.at(k);
						const int layerindex = varinfo._varLayerNames.at(k).indexOf(layerx);
						//�������ֵ����������ֵ������
						int nval;
						if (nlayer == 0)
						{
							if (ncompo == 0)
							{
								nval = 1;
							}
							else
							{
								nval = ncompo;
							}
						}
						else
						{
							if (ncompo == 0)
							{
								nval = nlayer;
							}
							else
							{
								nval = ncompo * nlayer;
							}
						}

						//��ȡ����
						int loopcount;
						_readf.read((char*)&loopcount, sizeof(int));

						//��ȡ����ֵ
						for (int j = 0; j < loopcount; ++j)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							if (ids.find(id) != ids.end())
							{
								//ֵ��ʵ����
								double* val = new double[nval];
								_readf.read((char*)val, nval * sizeof(double));

								//ֵ���鲿��
								double* val1 = new double[nval];
								_readf.read((char*)val1, nval * sizeof(double));

								//��ȡ��ǰ����������
								const int compoi = varinfo._compoNames.indexOf(compox);
								//�������������
								int valindex;
								if (nlayer == 0)
								{
									if (ncompo == 0)
									{
										valindex = 0;
									}
									else
									{
										valindex = compoi;
									}
								}
								else
								{
									if (ncompo == 0)
									{
										valindex = layerindex + compoi;
									}
									else
									{
										valindex = ncompo * layerindex + compoi;
									}
								}
								//ȡֵ
								double tempval;
								if (complexx == QString("ʵ��"))
								{
									tempval = val[valindex];
								}
								else if (complexx == QString("�鲿"))
								{
									tempval = val1[valindex];
								}
								else if (complexx == QString("��ֵ"))
								{
									double v0 = val[valindex];
									double v1 = val1[valindex];
									tempval = sqrt(v0*v0 + v1 * v1);
								}
								else if (complexx == QString("��λ"))
								{
									double v0 = val[valindex];
									double v1 = val1[valindex];
									tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
								}
								else
								{
									if (valindex == -1 && nval == 3)
									{
										tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
											val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
									}
									else
									{
										tempval = val[valindex];
									}
								}
								//�洢
								xvalues.append(tempval);
							}
							else
							{
								_readf.seekg(2 * nval * sizeof(double), ios::cur);
							}
						}
					}

					//�ر��ļ�
					_readf.close();
				}

				//Yֵ
				if (varinfo._varName == vary)
				{
					//���ļ�
					bool isopen = openFileStatus(finfo._frameFile);
					if (!isopen)
					{
						return;
					}

					//��ȡ�������ֽ�λ��
					long long int varbyte = varinfo._varByte;
					_readf.seekg(varbyte, ios::beg);

					//��ȡ������
					const int ncompo = varinfo._nCompo;

					//��ȡ��ǰ�����еı�������
					const int nresult = varinfo._varCount;
					for (int k = 0; k < nresult; ++k)
					{
						//��ȡ������������
						const int nlayer = varinfo._varLayer.at(k);
						const int layerindex = varinfo._varLayerNames.at(k).indexOf(layery);
						//�������ֵ����������ֵ������
						int nval;
						if (nlayer == 0)
						{
							if (ncompo == 0)
							{
								nval = 1;
							}
							else
							{
								nval = ncompo;
							}
						}
						else
						{
							if (ncompo == 0)
							{
								nval = nlayer;
							}
							else
							{
								nval = ncompo * nlayer;
							}
						}

						//��ȡ����
						int loopcount;
						_readf.read((char*)&loopcount, sizeof(int));

						//��ȡ����ֵ
						for (int j = 0; j < loopcount; ++j)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							if (ids.find(id) != ids.end())
							{
								//ֵ��ʵ����
								double* val = new double[nval];
								_readf.read((char*)val, nval * sizeof(double));

								//ֵ���鲿��
								double* val1 = new double[nval];
								_readf.read((char*)val1, nval * sizeof(double));

								for (QString compo : composy)
								{
									//��������
									QString cname = QString("%1_%2_%3_%4-%5_%6_%7_%8-ID%9").arg(varx).arg(compox).arg(layerx).arg(complexx)
										.arg(vary).arg(compo).arg(layery).arg(complexy).arg(id);
									if (!cnames.contains(cname))
									{
										cnames.append(cname);
									}

									//��ȡ��ǰ����������
									const int compoi = varinfo._compoNames.indexOf(compo);
									//�������������
									int valindex;
									if (nlayer == 0)
									{
										if (ncompo == 0)
										{
											valindex = 0;
										}
										else
										{
											valindex = compoi;
										}
									}
									else
									{
										if (ncompo == 0)
										{
											valindex = layerindex + compoi;
										}
										else
										{
											valindex = ncompo * layerindex + compoi;
										}
									}
									//ȡֵ
									double tempval;
									if (complexy == QString("ʵ��"))
									{
										tempval = val[valindex];
									}
									else if (complexy == QString("�鲿"))
									{
										tempval = val1[valindex];
									}
									else if (complexy == QString("��ֵ"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = sqrt(v0*v0 + v1 * v1);
									}
									else if (complexy == QString("��λ"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
									}
									else
									{
										if (valindex == -1 && nval == 3)
										{
											tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
												val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
										}
										else
										{
											tempval = val[valindex];
										}
									}
									//�洢
									QVector<double> tempvec;
									if (yvalues.contains(cname))
									{
										tempvec = yvalues.value(cname);
									}
									tempvec.append(tempval);
									yvalues.insert(cname, tempvec);
								}
							}
							else
							{
								_readf.seekg(2 * nval * sizeof(double), ios::cur);
							}
						}
					}

					//�ر��ļ�
					_readf.close();
				}
			}
		}
	}

	void mIMxdbFile1::readLoadData(const QString& pname, const QStringList & casenames, std::set<int>& ids, const QString& type)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		//���ݹ�������ȡ֡��Ϣ
		for (QString cname : casenames)
		{
			int firstbar = cname.indexOf("-");
			QString tempname = cname.right(cname.size() - firstbar - 1);
			FrameInfo fi = _dp->getFrameInfo(tempname);

			//���ļ�
			bool isopen = openFileStatus(fi._frameFile);
			if (isopen)
			{
				//��ȡ֡����
				mOneFrameData1* framedata = _dp->getOneFrameData(fi._frameNum);
				if (framedata == nullptr)
				{
					_readf.close();
					continue;
				}

				//��ȡ�غ����ݵ��ֽ�λ�ò���ת
				if (!fi._loadResults.contains(type))
				{
					_readf.close();
					continue;
				}
				long long int loadbyte = fi._loadResults.value(type);
				_readf.seekg(loadbyte, ios::beg);

				//��ȡ����
				std::set<int> eleIds = ids;
				if (type.compare("BarEndLoad", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mBarEndLoad* bel = new mBarEndLoad;
					//mSourceData::getInstance()->appendBarEndLoad(cname, bel);

					//��ȡ����
					const int nbar = framedata->getNumByElementType(D3R2) + framedata->getNumByElementType(D3B2);
					for (int i = 0; i < nbar; ++i)
					{
						//��ԪID
						int barid;
						_readf.read((char*)&barid, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(barid) == 1)
						{
							//ֵ
							double endload[4];
							_readf.read((char*)endload, 4 * sizeof(double));

							//bel->appendBarId(barid);
							//bel->appendEndLoad(barid, endload[0], endload[1]);
							//bel->appendEndLoad1(barid, endload[2], endload[3]);

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(barid);
						}
						else
						{
							_readf.seekg(4 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("BarStress", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcBarS* sbs = new mSrcBarS;
					//mSourceData::getInstance()->appendBarStress(cname, sbs);

					//��ȡ����
					const int nbar = framedata->getNumByElementType(D3B2);
					for (int i = 0; i < nbar; ++i)
					{
						//��ԪID
						int barid;
						_readf.read((char*)&barid, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(barid) == 1)
						{
							//ֵ
							double vals[13];
							_readf.read((char*)vals, 13 * sizeof(double));

							//sbs->appendBarId(barid);
							//sbs->appendPointC(barid, vals[0], vals[1]);
							//sbs->appendPointD(barid, vals[2], vals[3]);
							//sbs->appendPointE(barid, vals[4], vals[5]);
							//sbs->appendPointF(barid, vals[6], vals[7]);
							//sbs->appendAxial(barid, vals[8]);
							//sbs->appendMaximum(barid, vals[9], vals[10]);
							//sbs->appendMinimum(barid, vals[11], vals[12]);

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(barid);
						}
						else
						{
							_readf.seekg(13 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShearStress", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcShearS* sss = new mSrcShearS;
					//mSourceData::getInstance()->appendShearStress(cname, sss);

					//��ȡ����
					const int nshear = framedata->getNumByElementType(SPQ4);
					for (int i = 0; i < nshear; ++i)
					{
						//��ԪID
						int shearid;
						_readf.read((char*)&shearid, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(shearid) == 1)
						{
							//ֵ
							double vals[3];
							_readf.read((char*)vals, 3 * sizeof(double));

							//sss->appendShearId(shearid);
							//sss->appendShearMax(shearid, vals[0]);
							//sss->appendShearAvg(shearid, vals[1]);
							//sss->appendShearThickness(shearid, vals[2]);

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(shearid);
						}
						else
						{
							_readf.seekg(3 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShearLoad", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcShearF* ssf = new mSrcShearF;
					//mSourceData::getInstance()->appendShearForce(cname, ssf);

					//��ȡ����
					const int nshear = framedata->getNumByElementType(SPQ4);
					for (int i = 0; i < nshear; ++i)
					{
						//��ԪID
						int shearid;
						_readf.read((char*)&shearid, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(shearid) == 1)
						{
							//ֵ
							double vals[4];
							_readf.read((char*)vals, 4 * sizeof(double));

							//ssf->appendShearId(shearid);
							//ssf->appendShearValues(shearid, vals[0], vals[1], vals[2], vals[3]);

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(shearid);
						}
						else
						{
							_readf.seekg(4 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellStrain", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcShellSE* ssse = new mSrcShellSE;
					//mSourceData::getInstance()->appendShellStrain(cname, ssse);

					//��ȡ����
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//��ԪID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//��ȡ����
						int nlayer;
						_readf.read((char*)&nlayer, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(shellid) == 1)
						{
							//ֵ
							for (int j = 0; j < nlayer; ++j)
							{
								double vals[9];
								_readf.read((char*)vals, 9 * sizeof(double));

								QString layername;
								if (j == 0)
								{
									layername = QString("At Center");
								}
								else
								{
									layername = QString("Layer %1").arg(j);
								}
								//ssse->appendShellId(shellid);
								//ssse->appendShellPly(shellid, layername, vals);
							}

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(nlayer * 9 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellStress", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcShellSE* ssse = new mSrcShellSE;
					//mSourceData::getInstance()->appendShellStress(cname, ssse);

					//��ȡ����
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//��ԪID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//��ȡ����
						int nlayer;
						_readf.read((char*)&nlayer, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(shellid) == 1)
						{
							//ssse->appendShellNPly(shellid, nlayer);
							//ֵ
							for (int j = 0; j < nlayer; ++j)
							{
								double vals[9];
								_readf.read((char*)vals, 9 * sizeof(double));

								QString layername;
								if (j == 0)
								{
									layername = QString("At Z1");
								}
								else if (j == 1)
								{
									layername = QString("At Center");
								}
								else if (j == 2)
								{
									layername = QString("At Z2");
								}
								else
								{
									layername = QString("Layer %1").arg(j - 2);
								}
								//ssse->appendShellId(shellid);
								//ssse->appendShellPly(shellid, layername, vals);
							}

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(nlayer * 9 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellForceResultant", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcShellRF* ssrf = new mSrcShellRF;
					//mSourceData::getInstance()->appendShellRForce(cname, ssrf);

					//��ȡ����
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//��ԪID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(shellid) == 1)
						{
							//ֵ
							double* vals = new double[18];
							_readf.read((char*)vals, 18 * sizeof(double));

							//ssrf->appendShellId(shellid);
							//ssrf->appendShellValues(shellid, vals);

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(18 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellForceMoment", Qt::CaseInsensitive) == 0)
				{
					//�洢����
					//mSrcShellMF* ssmf = new mSrcShellMF;
					//mSourceData::getInstance()->appendShellMForce(cname, ssmf);

					//��ȡ����
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//��ԪID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//�ж��Ƿ�Ϊ���赥Ԫ
						if (eleIds.count(shellid) == 1)
						{
							//ֵ
							double* vals = new double[18];
							_readf.read((char*)vals, 18 * sizeof(double));

							//ssmf->appendShellId(shellid);
							//ssmf->appendShellValues(shellid, vals);

							//�Ѵ洢�ĵ�Ԫ���б����Ƴ�
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(18 * sizeof(double), ios::cur);
						}

						//�ж�����Ҫ��ID�Ƿ���ȫ����ȡ��������ֹ��ȡ
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
			}
			_readf.close();
		}
	}

	void mIMxdbFile1::readAllResults(const QString & pname, QList<QStringList>& vals)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();

		QHash<QString, QStringList> maxinfo;	//key-������-������value-���ֵ�����ֵ֡�����ֵλ��
		QHash<QString, QStringList> mininfo;	//key-������-������value-���ֵ�����ֵ֡�����ֵλ��
		for (FrameInfo finfo : frameInfo)
		{
			QList<VariableInfo> varInfo = finfo._frameVars;
			for (VariableInfo vinfo : varInfo)
			{
				//���ļ�
				bool isopen = openFileStatus(finfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//��ȡ�������ֽ�λ��
				long long int varbyte = vinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//��ȡ������
				const int ncompo = vinfo._nCompo;

				QStringList groupnames;
				if (vinfo._compoNames.size() > 0)
				{
					for (QString coname : vinfo._compoNames)
					{
						QString name = vinfo._varName + "-" + coname;
						groupnames.append(name);
					}
					if (vinfo._compoNames.size() == 3 && vinfo._isMag)
					{
						groupnames.append(vinfo._varName + "-Mag");
					}
				}
				else
				{
					groupnames.append(vinfo._varName);
				}

				//��ȡ��ǰ�����еı�������
				const int nresult = vinfo._varCount;
				for (int k = 0; k < nresult; ++k)
				{
					//��ȡ������������
					const int nlayer = vinfo._varLayer.at(k);
					//�������ֵ����������ֵ������
					int nval;
					if (nlayer == 0)
					{
						if (ncompo == 0)
						{
							nval = 1;
						}
						else
						{
							nval = ncompo;
						}
					}
					else
					{
						if (ncompo == 0)
						{
							nval = nlayer;
						}
						else
						{
							nval = ncompo * nlayer;
						}
					}

					//��ȡ����
					int loopcount;
					_readf.read((char*)&loopcount, sizeof(int));

					//��ȡ����ֵ
					for (int j = 0; j < loopcount; ++j)
					{
						//ID
						int id;
						_readf.read((char*)&id, sizeof(int));

						//ֵ��ʵ����
						double* val = new double[nval];
						_readf.read((char*)val, nval * sizeof(double));

						//ֵ���鲿��
						double* val1 = new double[nval];
						_readf.read((char*)val1, nval * sizeof(double));

						//ȡ��ֵ
						for (int jj = 0; jj < nval; ++jj)
						{
							if (j == 0)
							{
								maxinfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
								mininfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
							}
							else
							{
								double tempval1 = maxinfo.value(groupnames.at(jj)).at(0).toDouble();
								if (val[jj] > tempval1)
								{
									maxinfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
								}
								double tempval2 = mininfo.value(groupnames.at(jj)).at(0).toDouble();
								if (val[jj] < tempval2)
								{
									mininfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
								}
							}
						}

						//�������
						if (nval == 3 && vinfo._isMag)
						{
							double magval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
								val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
							if (j == 0)
							{
								maxinfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
								mininfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
							}
							else
							{
								double tempval1 = maxinfo.value(groupnames.at(3)).at(0).toDouble();
								if (magval > tempval1)
								{
									maxinfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
								}
								double tempval2 = mininfo.value(groupnames.at(3)).at(0).toDouble();
								if (magval < tempval2)
								{
									mininfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
								}
							}
						}

						//ɾ��
						delete val;
						delete val1;
					}
				}

				_readf.close();
			}
		}

		//����
		for (FrameInfo finfo : frameInfo)
		{
			QList<VariableInfo> varInfo = finfo._frameVars;
			for (VariableInfo vinfo : varInfo)
			{
				if (vinfo._compoNames.size() > 0)
				{
					for (QString cname : vinfo._compoNames)
					{
						QString gname = vinfo._varName + "-" + cname;
						QStringList templist = { gname };
						templist.append(maxinfo.value(gname));
						templist.append(mininfo.value(gname));
						if (!vals.contains(templist))
						{
							vals.append(templist);
						}
					}
					if (vinfo._compoNames.size() == 3 && vinfo._isMag)
					{
						QString gname = vinfo._varName + "-Mag";
						QStringList templist = { gname };
						templist.append(maxinfo.value(gname));
						templist.append(mininfo.value(gname));
						if (!vals.contains(templist))
						{
							vals.append(templist);
						}
					}
				}
				else
				{
					QStringList templist = { vinfo._varName };
					templist.append(maxinfo.value(vinfo._varName));
					templist.append(mininfo.value(vinfo._varName));
					if (!vals.contains(templist))
					{
						vals.append(templist);
					}
				}
			}
		}
	}

	void mIMxdbFile1::readScalarData(const QString & pname, int f, const QString & var, QHash<int, double>& vals, MViewBasic::NodeOrElement &nodeOrElement)
	{
		if (var == "")
		{
			return;
		}
		QString compo, layer, complex;

		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}

		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == var)
			{
				//���ļ�
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//��ȡ�������ֽ�λ��
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//��ȡ������
				const int ncompo = varinfo._nCompo;

				//��ȡ��ǰ����������
				const int compoi = varinfo._compoNames.indexOf(compo);

				//�洢ֵ
				double maxval = 0, minval = 0;
				QHash<int, float> compovalues;

				//��ȡ��ǰ�����еı�������
				const int nresult = varinfo._varCount;
				for (int k = 0; k < nresult; ++k)
				{
					//��ȡ������������
					const int nlayer = varinfo._varLayer.at(k);
					const int layerindex = varinfo._varLayerNames.at(k).indexOf(layer);
					//�������ֵ����������ֵ������
					int nval, valindex;
					if (nlayer == 0)
					{
						if (ncompo == 0)
						{
							nval = 1;
							valindex = 0;
						}
						else
						{
							nval = ncompo;
							valindex = compoi;
						}
					}
					else
					{
						if (ncompo == 0)
						{
							nval = nlayer;
							valindex = layerindex + compoi;
						}
						else
						{
							nval = ncompo * nlayer;
							valindex = ncompo * layerindex + compoi;
						}
					}

					//��ȡ����
					int loopcount;
					_readf.read((char*)&loopcount, sizeof(int));

					//��ȡ����ֵ
					for (int j = 0; j < loopcount; ++j)
					{
						//ID
						int id;
						_readf.read((char*)&id, sizeof(int));

						//ֵ��ʵ����
						double* val = new double[nval];
						_readf.read((char*)val, nval * sizeof(double));

						//ֵ���鲿��
						double* val1 = new double[nval];
						_readf.read((char*)val1, nval * sizeof(double));

						double tempval;
						if (complex == QString("ʵ��"))
						{
							tempval = val[valindex];
						}
						else if (complex == QString("�鲿"))
						{
							tempval = val1[valindex];
						}
						else if (complex == QString("��ֵ"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = sqrt(v0*v0 + v1 * v1);
						}
						else if (complex == QString("��λ"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
						}
						else
						{
							if (valindex == -1 && nval == 3)
							{
								tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
									val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
							}
							else
							{
								tempval = val[valindex];
							}
						}
						vals.insert(id, tempval);

						//ȡ��ֵ
						if (j == 0)
						{
							maxval = tempval;
							minval = tempval;
						}
						else
						{
							maxval = (tempval > maxval) ? tempval : maxval;
							minval = (tempval < minval) ? tempval : minval;
						}

						delete val;
						delete val1;
					}
				}

				compovalues.clear();

				_readf.close();

				//��ȡ��������
				nodeOrElement = MViewBasic::NodeOrElement(varinfo._varType);

				//�����Ϣ
				//mOutputMessage tepmsg(NormalMessage, QString("��%1֡�ļ����ݶ�ȡ�ɹ���").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				QList<FrameInfo>().swap(frameInfo);
				frameInfo.clear();
				return;
			}
		}
	}

	void mIMxdbFile1::readVectorData(const QString & pname, int f, const QString & var, QHash<int, QVector3D> &vals, MViewBasic::NodeOrElement &nodeOrElement)
	{
		if (var == "")
		{
			return;
		}
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);

		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}
		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == var)
			{
				//��ȡ������
				const int ncompo = varinfo._nCompo;
				if (ncompo != 3)
				{
					return;
				}

				//���ļ�
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//��ȡ�������ֽ�λ��
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//��ȡ�ڵ�/��Ԫ��
				int nodecount;
				_readf.read((char*)&nodecount, sizeof(int));

				//QHash<int, QVector3D> vecvals;

				vals.reserve(nodecount);

				for (int j = 0; j < nodecount; ++j)
				{
					//�ڵ�ID
					int nid;
					_readf.read((char*)&nid, sizeof(int));

					//ʸ��ֵ��ʵ����
					double val[3];
					_readf.read((char*)val, 3 * sizeof(double));

					//ʸ��ֵ���鲿��
					double val1[3];
					_readf.read((char*)val1, 3 * sizeof(double));

					vals.insert(nid, QVector3D(val[0], val[1], val[2]));
				}

				//��ȡ��������
				nodeOrElement = MViewBasic::NodeOrElement(varinfo._varType);

				//�����Ϣ
				//mOutputMessage tepmsg(NormalMessage, QString("��%1֡�ļ�ʸ�����ݶ�ȡ�ɹ���").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				_readf.close();
				return;

			}
		}
	}

	void mIMxdbFile1::run()
	{
		//bool isread = this->readCloudData(_pname, _frame, _var, _compo, _layer, _complex);
		//if (isread)
		//{
		//	//��ȡλ��
		//	this->readDisplacementData(_pname, _frame, _complex);

		//	//�ж��Ƿ��ȡ����ͼʸ��
		//	if (_hasstreamLine)
		//	{
		//		this->readVectorData(_pname, _frame, _streamLineVectorName);
		//	}
		//	else
		//	{
		//		//�ж��Ƿ��ȡʸ��ͼʸ��
		//		if (_hasvectorArrowVector)
		//		{
		//			this->readVectorData(_pname, _frame, _vectorArrowVectorName);
		//		}
		//	}
		//}
		////postmutex.lock();
		//QMutexLocker locker(&postmutex);
		//_animationData->appendRendOneFrameData(_frame, _oneFrameRender);
		//if (_animationData->getRendAnimationIds().size() == _threadNum)
		//{
		//	emit finishedImportAllMxDbFileSig();
		//}
		//postmutex.unlock();
	}

	bool mIMxdbFile1::openFileStatus(const QString & fname)
	{
		//_byteAmount = 0;

		//�ж��ļ��Ƿ����
		QFile dbFile(fname);
		if (!dbFile.exists())
		{
			//emit readFileStatusSig(FileNotExist);
			//mGlobalSignals::getInstance()->outputMessageSig(1, QString("�ļ�%1�����ڣ�").arg(fname));
			return false;
		}
		//�ж��ļ��Ƿ��
		_readf.open(fname.toLocal8Bit(), ios::in | ios::binary);
		if (!_readf)
		{
			//emit readFileStatusSig(FileNotOpen);
			//mGlobalSignals::getInstance()->outputMessageSig(2, QString("�ļ�%1��ʧ�ܣ�").arg(fname));
			return false;
		}

		//��ȡ�汾��
		int v = 0;
		_readf.read((char*)&v, sizeof(int));
		//_byteAmount += sizeof(int);
		_version = (unsigned int)v;
		if (_version > 0xA1B1C104)			//-1582186239 == 0xA1B1C102��-1582186238 == 0xA1B1C103
		{
			//emit readFileStatusSig(FileVersionFalse);
			//mGlobalSignals::getInstance()->outputMessageSig(2, QString("�ļ�%1�汾����").arg(fname));
			return false;
		}

		return true;
	}

	void mIMxdbFile1::readInitModelDatas(shared_ptr<mDataPost1> dataPost, mOneFrameData1 * fd)
	{
		/*
		//��ȡ�ڵ�����
		int nodeamount;
		_readf.read((char*)&nodeamount, sizeof(int));
		//_byteAmount += sizeof(int);
		fd->resizeNodeSize(nodeamount);

		//�洢�ڵ�����
		for (int i = 0; i < nodeamount; ++i)
		{
			//��ȡ�ڵ�ID
			int nodeid;
			_readf.read((char*)&nodeid, sizeof(int));
			//_byteAmount += sizeof(int);

			//��ȡ�ڵ�����
			double nodecoord[3];
			_readf.read((char*)nodecoord, 3 * sizeof(double));
			//_byteAmount += 3* sizeof(double);

			//�洢�ڵ�ID������
			mPostMeshNodeData1 *nodeData = new mPostMeshNodeData1(nodeid, QVector3D(nodecoord[0], nodecoord[1], nodecoord[2]));
			fd->appendNodeData(nodeid, nodeData);
		}

		//�����Ϣ
		mGlobalSignals::getInstance()->outputMessageSig(0, QString("ģ�ͽڵ����ݶ�ȡ�ɹ�..."));

		//��ȡPart��
		int partcount;
		_readf.read((char*)&partcount, sizeof(int));

		//��ȡpart��Ϣ
		for (int i = 0; i < partcount; ++i)
		{
			//��ȡPart��
			char partname[64];
			_readf.read(partname, 64 * sizeof(char));
			//_byteAmount += 64*sizeof(char);

			mPostMeshPartData1 *partData = new mPostMeshPartData1(QString(partname), i + 1);

			//��ȡ��Ԫ����
			int elecount;
			_readf.read((char*)&elecount, sizeof(int));
			//_byteAmount += sizeof(int);

			QHash<QVector<int>, mPostMeshFaceData1*> _meshFace;

			for (int j = 0; j < elecount; ++j)
			{
				//��ȡ��ԪID
				int eleid;
				_readf.read((char*)&eleid, sizeof(int));
				//_byteAmount += sizeof(int);

				//��ȡ��Ԫ����
				char eletype[32];
				_readf.read(eletype, 32 * sizeof(char));
				//_byteAmount += 32* sizeof(char);

				//�洢
				ElementType elementtype = mCommonFunctions::getElementType(QString(eletype).toUpper());
				MeshType meshtype = mCommonFunctions::transformTypeStrToEnum(elementtype);
				//�������ͻ�ȡ��Ԫ�����Ľڵ����
				int elenode = mCommonFunctions::elementNodeCount(elementtype);


				//��ȡ��Ԫ�����Ľڵ�ID
				int *ids = new int[elenode];
				_readf.read((char*)ids, elenode * sizeof(int));
				//_byteAmount += elenode*sizeof(int);				

				QVector<int> nodeids;
				nodeids.reserve(elenode);
				for (int k = 0; k < elenode; ++k)
				{
					nodeids.append(ids[k]);
				}
				delete ids;
				postmutex.lock();

				fd->createMesh(eleid, meshtype, elementtype, nodeids, partData, _meshFace);
				postmutex.unlock();
			}

			fd->appendMeshPartData(partname, partData);
			//�洢��ʾ����
			partData->setMeshFaceData(_meshFace.values().toVector());
			//�����Ϣ
			mGlobalSignals::getInstance()->outputMessageSig(0, QString("%1�������ݶ�ȡ�ɹ���").arg(QString(partname)));
			_meshFace.clear();
			QHash<QVector<int>, mPostMeshFaceData1*>().swap(_meshFace);
		}
		//����Ԫ���ݷ��䵽hash��vector
		fd->calculateCellData();
		fd->calculatePointCell();
		//���ɱ߽���
		fd->caculateMeshLine();

		//�����Ϣ
		mGlobalSignals::getInstance()->outputMessageSig(0, QString("�߽������ɳɹ���"));

		*/

		this->readModelDatas(fd);

		//��ȡ�������������ǽ������
		if (_version >= 0xA1B1C104)
		{
			//��ȡ����
			int rwcount;
			_readf.read((char*)&rwcount, sizeof(int));

			for (int i = 0; i < rwcount; ++i)
			{
				//��ȡID
				int ID;
				_readf.read((char*)&ID, sizeof(int));

				//��ȡ����
				int rwtype;
				_readf.read((char*)&rwtype, sizeof(int));

				//��ȡ������ʼ����ֹ����
				double normaldirect[6];
				_readf.read((char*)normaldirect, 6 * sizeof(double));
				if (rwtype == 0)//�����޴�ƽ�����ǽ
				{
					mPostInfinitelyLargePlaneRigidWall *rigidWall = new mPostInfinitelyLargePlaneRigidWall(ID, normaldirect);
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 1)					//�����޴�ƽ�����ǽ
				{
					//��������ֹ����
					double edgecoord[3];
					_readf.read((char*)edgecoord, 3 * sizeof(double));

					//�߳�L
					double edgeL;
					_readf.read((char*)&edgeL, sizeof(double));

					//�߳�M
					double edgeM;
					_readf.read((char*)&edgeM, sizeof(double));

					mPostLimitedLargePlaneRigidWall *rigidWall = new mPostLimitedLargePlaneRigidWall(ID, normaldirect);
					memcpy(rigidWall->_lineEndVector, edgecoord, 3 * sizeof(double));
					rigidWall->_L = edgeL;
					rigidWall->_M = edgeM;
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 2)				//�����������ǽ
				{
					//��������ֹ����
					double edgecoord[3];
					_readf.read((char*)edgecoord, 3 * sizeof(double));

					//�߳�L
					double edgeL;
					_readf.read((char*)&edgeL, sizeof(double));

					//�߳�M
					double edgeM;
					_readf.read((char*)&edgeM, sizeof(double));

					//�߳�P
					double edgeP;
					_readf.read((char*)&edgeP, sizeof(double));

					mPostRectangularRigidWall *rigidWall = new mPostRectangularRigidWall(ID, normaldirect);
					memcpy(rigidWall->_lineEndVector, edgecoord, 3 * sizeof(double));
					rigidWall->_L = edgeL;
					rigidWall->_M = edgeM;
					rigidWall->_P = edgeP;
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 3)					//��Բ��״����ǽ
				{
					//�뾶
					double radius;
					_readf.read((char*)&radius, sizeof(double));

					//���򳤶�
					double length;
					_readf.read((char*)&length, sizeof(double));

					mPostCylindricalRigidWall *rigidWall = new mPostCylindricalRigidWall(ID, normaldirect);
					rigidWall->_radius = radius;
					rigidWall->_length = length;
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 4)//�������ǽ
				{
					//�뾶
					double radius;
					_readf.read((char*)&radius, sizeof(double));

					mPostSphericalRigidWall *rigidWall = new mPostSphericalRigidWall(ID, normaldirect);
					rigidWall->_radius = radius;
					dataPost->appendRigidWallData(rigidWall);
				}
			}
		}
	}

	void mIMxdbFile1::readModelDatas(mOneFrameData1* fd)
	{
		//��ȡ�ڵ�����
		int nodeamount;
		_readf.read((char*)&nodeamount, sizeof(int));
		//_byteAmount += sizeof(int);
		fd->resizeNodeSize(nodeamount);

		int maxNodeID = 0;
		//�洢�ڵ�����
		for (int i = 0; i < nodeamount; ++i)
		{
			//��ȡ�ڵ�ID
			int nodeid;
			_readf.read((char*)&nodeid, sizeof(int));
			//_byteAmount += sizeof(int);

			//��ȡ�ڵ�����
			double nodecoord[3];
			_readf.read((char*)nodecoord, 3 * sizeof(double));
			//_byteAmount += 3* sizeof(double);

			maxNodeID = max(maxNodeID, nodeid);

			//�洢�ڵ�ID������
			mPostMeshNodeData1 *nodeData = new mPostMeshNodeData1(nodeid, QVector3D(nodecoord[0], nodecoord[1], nodecoord[2]));
			fd->appendNodeData(nodeid, nodeData);
		}

		qDebug() << "�ڵ�����Ϊ" << nodeamount;

		//�����Ϣ
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("ģ�ͽڵ����ݶ�ȡ�ɹ�..."));

		//��ȡPart��
		int partcount;
		_readf.read((char*)&partcount, sizeof(int));
		
		//��ȡpart��Ϣ
		for (int i = 0; i < partcount; ++i)
		{
			//��ȡPart��
			char partname[64];
			_readf.read(partname, 64 * sizeof(char));
			//_byteAmount += 64*sizeof(char);

			mPostMeshPartData1 *partData = new mPostMeshPartData1(QString(partname), i + 1);

			//��ȡ��Ԫ����
			int elecount;
			_readf.read((char*)&elecount, sizeof(int));
			//_byteAmount += sizeof(int);

			qDebug() << "��Ԫ����Ϊ" << elecount;
	
			//QHash<QVector<int>, mPostMeshFaceData1*> _meshFace;
			QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>> meshFaces(maxNodeID);
#if optimazation

			int minnodeidpart = maxNodeID;
			int maxnodeidpart = 0;
#endif // optimazation
			for (int j = 0; j < elecount; ++j)
			{
				//��ȡ��ԪID
				int eleid;
				_readf.read((char*)&eleid, sizeof(int));
				//_byteAmount += sizeof(int);

				//��ȡ��Ԫ����
				char eletype[32];
				_readf.read(eletype, 32 * sizeof(char));
				//_byteAmount += 32* sizeof(char);

				//�洢
				ElementType elementtype = mCommonFunctions::getElementType(QString(eletype).toUpper());
				MeshType meshtype = mCommonFunctions::transformTypeStrToEnum(elementtype);
				//�������ͻ�ȡ��Ԫ�����Ľڵ����
				int elenode = mCommonFunctions::elementNodeCount(elementtype);


				//��ȡ��Ԫ�����Ľڵ�ID
				int *ids = new int[elenode];
				_readf.read((char*)ids, elenode * sizeof(int));
				//_byteAmount += elenode*sizeof(int);				
				
				QVector<int> nodeids;
				nodeids.reserve(elenode);
				for (int k = 0; k < elenode; ++k)
				{
					nodeids.append(ids[k]);
#if optimazation
					minnodeidpart = min(minnodeidpart, ids[k]);
					maxnodeidpart = max(maxnodeidpart, ids[k]);
#endif // optimazation
				}
				delete ids;
				postmutex.lock();
		
				fd->createMesh(eleid, meshtype, elementtype, nodeids, partData, meshFaces);
				postmutex.unlock();
			}

			fd->appendMeshPartData(partname, partData);
			//�洢��ʾ����
			QVector<mPostMeshFaceData1*> faceData;
#if optimazation
			for (int j = minnodeidpart;j <= maxnodeidpart;++j)
			{
				for (auto iter : meshFaces.at(j))
				{
					faceData.append(iter.second);
				}
			}
#else
			for (auto face : meshFaces)
			{
				for (auto iter : face)
				{
					faceData.append(iter.second);
				}
			}
#endif // optimazation
			partData->setMeshFaceData(faceData);
			//partData->setMeshFaceData(_meshFace.values().toVector());
			//�����Ϣ
			//mGlobalSignals::getInstance()->outputMessageSig(0, QString("%1�������ݶ�ȡ�ɹ���").arg(QString(partname)));
			//meshFaces.clear();
			QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>>().swap(meshFaces);
			//QHash<QVector<int>, mPostMeshFaceData1*>().swap(_meshFace);
		}

		//����Ԫ���ݷ��䵽hash��vector
		fd->calculateCellData();
		//fd->calculatePointCell();
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("�������ݣ�"));
		//���ɱ߽���
		fd->caculateMeshLine();

		//�����Ϣ
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("�߽������ɳɹ���"));

		QtConcurrent::run(fd, &mOneFrameData1::calculatePointCell);
	}
}