#include "mIFluentBCMeshFile.h"

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <qdebug.h>

//BasicData
#include "MeshMessage.h"

//MxFunctions
#include "mGlobalSignals.h"
#include "mGlobalVarible.h"

using namespace MxFunctions;
namespace MIOFile
{
	bool mIFluentBCMeshFile::ReadBCMeshFile(QString fileName)
	{
		// ��ȡfluent�ļ�
		//string filename= "2700w.cas";
		_fileName = fileName;
		vector<string> strVec;
		string filename = fileName.toLocal8Bit().toStdString();
		ifstream file_fp(filename, ios::in);
		if (!file_fp.is_open())
		{
			mGlobalSignals::getInstance()->outputMessageSig(2, QString("�ļ�������"));
			return false;
		}
		string buffer;
		int line = 0;
		while (true) {
			getline(file_fp, buffer);
			line = line + 1;
			if (buffer[0] == '(') { strVec.push_back(buffer.substr(0, 80)); }
			if (buffer.substr(0, 5) == "(2 3)") {
				break;
			}

			if (line > 30)
			{
				mGlobalSignals::getInstance()->outputMessageSig(2, QString("�����ļ����ݲ���ȷ���߷�3D����"));
				return false;
			}
		}

		//mDatabaseSingleton::getInstance()->getDatabase()->getFluidCommonSettingData()->_casfileName = _fileName;

		ReadFile1(file_fp, strVec, buffer);

		// _meshFace.clear();
		// _frequencyName_caseName.clear();
		//�洢�ļ����������������
		//mDatabaseSingleton::getInstance()->getMeshData()->_fileNames.append(_fileName);
		//mDatabaseSingleton::getInstance()->getMeshData()->_fileParts.insert(_fileName, _filepartnames);

		return true;
	}
	void mIFluentBCMeshFile::ReadFile1(ifstream &file_fp, vector<string> strVec, string buffer)
	{
		//mDatabase *db = mDatabaseSingleton::getInstance()->getDatabase();
		//if (db == nullptr)
		//{
		//	return;
		//}

		char* mark_xyz = (char*)malloc(sizeof(char) * 2);  //��Ǳ߽��
		vector<int> Faceinfo;

		int find_xyz = 0;  //�������� ͳ�� ��=1�����쳣
		int nodeNum = 0;


		int nbc = -1;        //����߽����
		int faceNum = 0;
		int find_Face = 0;     //�������� ͳ��  ��=1�����쳣
		int find_interior = 0; //�ڲ�������� ͳ��  ��=1�����쳣
		int bcface_number = 0;
		int FaceSum = 0;
		int NodeSum = 0;

		//��ʱ����
		QStringList linelist{};
		//QString partName = "fluentbc";
		////��������
		//mMeshPartData1 *partData = mMeshModelData1::getInstance()->getMeshPartDataByPartName(partName);
		//if (partData == nullptr)
		//{
		//	_globalPartId++;
		//	partData = new mMeshPartData1(partName, _globalPartId);

		//	//�洢��ǰ�����ļ��а����������
		//	_filepartnames.append(partName);
		//}

		//int startMeshID = _globalMeshId;
		//int startGeoFaceID = _globalFaceId;

		//QVector<mMeshPartData1*> partDatas;

		int partnum = 0;
		vector<string> strVecpartelem;  //ÿ��part��Ӧ�ĵ�Ԫ��Ϣ

		vector<string> strVeczone;  //�洢������Ϣ

		int NBc = 0;
		vector<string> bcVec;  //�洢����Ϣ
		while (!file_fp.eof()) {
			getline(file_fp, buffer);
			if (buffer.substr(0, 3) == "(39" || buffer.substr(0, 3) == "(45") { strVeczone.push_back(buffer.substr(0, 80)); }
			if (buffer.substr(0, 3) == "(12" && buffer[5] != '0')
			{
				partnum++;
				strVecpartelem.push_back(buffer.substr(0, 80));
			}
			if (buffer.substr(0, 3) == "(13" && buffer[5] != '0')
			{
				NBc++;
				bcVec.push_back(buffer.substr(0, 80));
			}
		}

		file_fp.close();

		vector<int> Bctype(NBc);
		vector<int> bcid(NBc);
		vector<string> BcName(NBc);
		for (int i = 0; i < NBc; i++)
		{
			char temp[80];
			for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
			for (int isize = 5; isize < bcVec[i].size(); isize++) { temp[isize - 5] = bcVec[i][isize]; }
			int zoneID, rdin1, rdin2, bctype, facetype;
                        sscanf(temp, "%x%x%x%x%x", &zoneID, &rdin1, &rdin2, &bctype, &facetype);
			bcid[i] = zoneID;
			Bctype[i] = bctype;
		}

		vector<int> partID(partnum);
		vector<int>parttype(partnum);
		vector<string> partname(partnum);
		vector<int> partfirstelemid(partnum);
		vector<int> partlastelemid(partnum);
		for (int i = 0; i < partnum; i++)
		{
			char temp[80];
			for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
			for (int isize = 5; isize < strVecpartelem[i].size(); isize++) { temp[isize - 5] = strVecpartelem[i][isize]; }
			int zoneID, rdin1, rdin2, parttype, elemtype;
                        sscanf(temp, "%x%x%x%x%x", &zoneID, &rdin1, &rdin2, &parttype, &elemtype);
			partID[i] = zoneID;
			partfirstelemid[i] = rdin1;
			partlastelemid[i] = rdin2;

		}

		for (int i = 0; i < strVeczone.size(); i++) {
			if (strVeczone[i].substr(0, 5) == "(39 (" || strVeczone[i].substr(0, 5) == "(45 (") {
				int isize = strVeczone[i].size();
				string stemp = strVeczone[i].substr(5, isize);
				stemp.erase(0, stemp.find_first_not_of(' '));
				string fnumber = stemp.substr(0, stemp.find_first_of(' '));
				int zoneID;
				try {
					zoneID = stoi(fnumber);  //id����
				}
				catch (...) {
					cout << " Error @ read in boundary name." << endl;
					fflush(stdout);
					exit(-1);
				}
				stemp.erase(0, stemp.find_first_of(' '));  //ȥ����һ������
				stemp.erase(0, stemp.find_first_not_of(' ')); //ȥ�����ֺ���Ŀո�
				string zonetype = stemp.substr(0, stemp.find_first_of(' '));
				//zoneΪpart����
				if (strcmp(zonetype.c_str(), "fluid") == 0)
				{
					stemp.erase(0, stemp.find_first_of(' '));  //ȥ���߽��������
					stemp.erase(0, stemp.find_first_not_of(' ')); //ȥ���������ƺ���Ŀո�
					int name_end = stemp.find_first_of(')');  //�ҵ��߽����ƽ�β
					//ѭ������ID
					for (int ipart = 0; ipart < partnum; ipart++) {
						if (partID[ipart] == zoneID) {
							parttype[ipart] = 0;
							partname[ipart] = stemp.substr(0, name_end);
							/*for (int j = 0; j < name_end; j++) {
								partname[ipart][j] = stemp[j];
							}*/
							break;
						}
					}
				}
				else if (strcmp(zonetype.c_str(), "solid") == 0)
				{
					stemp.erase(0, stemp.find_first_of(' '));  //ȥ���߽��������
					stemp.erase(0, stemp.find_first_not_of(' ')); //ȥ���������ƺ���Ŀո�
					int name_end = stemp.find_first_of(')');  //�ҵ��߽����ƽ�β
					//ѭ������ID
					for (int ipart = 0; ipart < partnum; ipart++)
					{
						if (partID[ipart] == zoneID)
						{
							parttype[ipart] = 0;
							partname[ipart] = stemp.substr(0, name_end);
							/*for (int j = 0; j < name_end; j++)
							{
								partname[ipart][j] = stemp[j];
							}*/
							break;
						}
					}
				}
				else
				{
					stemp.erase(0, stemp.find_first_of(' '));  //ȥ���߽��������
					stemp.erase(0, stemp.find_first_not_of(' ')); //ȥ���������ƺ���Ŀո�
					int name_end = stemp.find_first_of(')');  //�ҵ��߽����ƽ�β
					//ѭ������ID
					for (int ibc = 0; ibc < NBc; ibc++) {
						if (bcid[ibc] == zoneID) {
							BcName[ibc] = stemp.substr(0, name_end);
							/*for (int j = 0; j < name_end; j++) {
								BcName[ibc][j] = stemp[j];
							}*/
							break;
						}
					}
				}
				//ѭ������ID is ok
			}
		}

		
		//����part
		//QHash<QString, int> partDatas;
		for (int i = 0; i < partnum; i++)
		{
			QString partName =  QString::fromStdString(partname[i]);

			//mMeshPartData1* partData = mMeshModelData1::getInstance()->getMeshPartDataByPartName(partName);
			//if (partData == nullptr)
			//{
			//	partData = new mMeshPartData1(partName, partID[i]);
			//	//mMeshModelData1::getInstance()->appendMeshPartData()
			//	partDatas.insert(partName, partData);
			//	//�洢��ǰ�����ļ��а����������
			//	_filepartnames.append(partName);
			//	//partDatas.append(partData);
			//}

			//if (!db->isSolveDomainIDExist(partID[i]))
			//{
			//	//��Ҫ���������
			//	mSolveDomainFluid *solveDomainFluid = new mSolveDomainFluid(partName);
			//	solveDomainFluid->setSolveDomainID(partID[i]);
			//	db->appendSolveDomainData(solveDomainFluid);

			//	//��Ҫ������ʼ����
			//	mFluidInitialSetting *initialSetting = new mFluidInitialSetting(partName);
			//	db->appendFluidInitialSettingData(initialSetting);
			//}


		}

		file_fp = ifstream(_fileName.toLocal8Bit(), ios::in);
		if (!file_fp.is_open())
		{
			mGlobalSignals::getInstance()->outputMessageSig(2, QString("�ļ�������"));
			return;
		}

		QHash<int,QPair<QVector<MXMeshTriangle*>, QVector<MXMeshQuadrangle*>>> geoFaceData;
		//QHash<int, QVector<MXMeshQuadrangle*>> geoFaceData;
		QHash<QString, set<int>> geoFacePartName;
		QHash<int, MXMeshVertex*> allVertexData;

		while (!file_fp.eof()) {

			getline(file_fp, buffer);
			if (buffer.substr(0, 3) == "(39" || buffer.substr(0, 3) == "(45") { strVec.push_back(buffer.substr(0, 80)); }
			//�����ռ�
			if (buffer.substr(0, 3) == "(10" && buffer[5] == '0') {
				strVec.push_back(buffer.substr(0, 80));
				char temp[80];
				for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
				for (int isize = 5; isize < buffer.size(); isize++) { temp[isize - 5] = buffer[isize]; }
				int rdin0, rdin1, rdin2, rdin3;
				sscanf(temp, "%x%x%x%x", &rdin0, &rdin1, &rdin2, &rdin3);
				//����xyz�ռ�
				find_xyz = find_xyz + 1;
				if (rdin1 != 1 || find_xyz != 1) { cout << "�����ļ����ݲ���ȷ" << endl; fflush(stdout); exit(-1); }
				std::free(mark_xyz);
				mark_xyz = (char*)malloc(sizeof(char) * (rdin2 + 1)); if (mark_xyz == NULL) { cout << " Allocate host meory failed." << endl; fflush(stdout); exit(-1); }
				memset(mark_xyz, 0, sizeof(char) * (rdin2 + 1));

				nodeNum = rdin2;
			}




			//����Face�ռ�
			if (buffer.substr(0, 3) == "(13" && buffer[5] == '0') {
				strVec.push_back(buffer.substr(0, 80));
				char temp[80];
				for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
				for (int isize = 5; isize < buffer.size(); isize++) { temp[isize - 5] = buffer[isize]; }
				int rdin0, rdin1, rdin2, rdin3;
				sscanf(temp, "%x%x%x%x", &rdin0, &rdin1, &rdin2, &rdin3);

				find_Face = find_Face + 1;
				if (rdin1 != 1 || find_Face != 1) { cout << "�����ļ����ݲ���ȷ" << endl; exit(-1); }
				faceNum = rdin2;
				cout << " faceNum = " << faceNum << endl;
				fflush(stdout);


			}

			//���������
			if (buffer.substr(0, 3) == "(10" && buffer[5] != '0') {
				strVec.push_back(buffer.substr(0, 80));
				char temp[80];
				for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
				for (int isize = 5; isize < buffer.size(); isize++) { temp[isize - 5] = buffer[isize]; }
				int zoneID, rdin1, rdin2, rdin3, rdim;
				sscanf(temp, "%x%x%x%x%x", &zoneID, &rdin1, &rdin2, &rdin3, &rdim);
				if (rdim != 3) { cout << "��3D����" << endl; fflush(stdout); exit(-1); }
				NodeSum = NodeSum + rdin2 - rdin1 + 1;
				rdin1 = rdin1 - 1; //��0��ʼ

				//file_fp.seekg(rdin2 - rdin1 + 1, ios::cur);

				getline(file_fp, buffer);
				if (buffer.size() > 2) {  //����֮ǰû��'('
					for (int iL = rdin1 + 1; iL < rdin2; iL++) {
						file_fp.getline(temp, 80);
						//sscanf(temp, "%lf%lf%lf", &node_h.x[iL], &node_h.y[iL], &node_h.z[iL]);
					}
				}
				else {  //����֮ǰ��'('
					for (int iL = rdin1; iL < rdin2; iL++) {
						file_fp.getline(temp, 80);
						//sscanf(temp, "%lf%lf%lf", &node_h.x[iL], &node_h.y[iL], &node_h.z[iL]);
					}
				}


			} //����� ok


			//����faceinfo
			if (buffer.substr(0, 3) == "(13" && buffer[5] != '0') {
				strVec.push_back(buffer.substr(0, 80));
				char temp[80];
				for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
				for (int isize = 5; isize < buffer.size(); isize++) { temp[isize - 5] = buffer[isize]; }
				int zoneID, rdin1, rdin2, bctype, facetype;
				sscanf(temp, "%x%x%x%x%x", &zoneID, &rdin1, &rdin2, &bctype, &facetype);
				nbc += 1;

				FaceSum = FaceSum + rdin2 - rdin1 + 1;


				rdin2 = rdin2 - rdin1 + 1;  //�ܸ���

				if (bctype == 2) { //�ڲ���
					for (int iL = 0; iL < rdin2; iL++) {
						file_fp.getline(temp, 80);
					}
				}
				else
				{  //�߽���
					bcface_number += rdin2;
					int faceLR[2];
					int face_point[5];
					string partname1;
					string partname2;
					file_fp.getline(temp, 80);
					//mMeshGeoFaceData1 *geoFaceData;
					QString partName;
					if (facetype == 3)
					{
						face_point[0] = 3;
						sscanf(temp, "%x%x%x%x%x", &face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
						if (faceLR[1] > 0) //�����,��Ԫ����������part
						{
							for (int i = 0; i < NBc; i++)
							{
								if (zoneID == bcid[i])
								{
									Bctype[i] = -3;
								}
							}
							for (int i = 0; i < partnum; i++)
							{
								if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
								{
									partname1 = partname[i];
								}
							}
							for (int i = 0; i < partnum; i++)
							{
								if (faceLR[1] >= partfirstelemid[i] && faceLR[1] <= partlastelemid[i])
								{
									partname2 = partname[i];
								}
							}

							//������
							QString partName1 = QString::fromStdString(partname1);
							QString partName2 = QString::fromStdString(partname2);
							_globalMeshId++;
							geoFacePartName[partName1].insert(zoneID);
							MXMeshTriangle * mesh = new MXMeshTriangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), _globalMeshId);						
							geoFaceData[zoneID].first.append(mesh);
						}
						else
						{
							for (int i = 0; i < partnum; i++)
							{
								if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
								{
									partname1 = partname[i];
								}
							}
							//������
							QString partName = QString::fromStdString(partname1);
							_globalMeshId++;
							geoFacePartName[partName].insert(zoneID);
							MXMeshTriangle * mesh = new MXMeshTriangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), _globalMeshId);
							geoFaceData[zoneID].first.append(mesh);
						}


					}
					else if(facetype == 4)
					{
						face_point[0] = 4;
						sscanf(temp, "%x%x%x%x%x%x", &face_point[4], &face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
						if (faceLR[1] > 0) //�����,��Ԫ����������part
						{
							for (int i = 0; i < NBc; i++)
							{
								if (zoneID == bcid[i])
								{
									Bctype[i] = -3;
								}
							}
							for (int i = 0; i < partnum; i++)
							{
								if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
								{
									partname1 = partname[i];
								}
							}
							for (int i = 0; i < partnum; i++)
							{
								if (faceLR[1] >= partfirstelemid[i] && faceLR[1] <= partlastelemid[i])
								{
									partname2 = partname[i];
								}
							}
							//������
							QString partName1 = QString::fromStdString(partname1);
							QString partName2 = QString::fromStdString(partname2);
							_globalMeshId++;
							geoFacePartName[partName1].insert(zoneID);
							MXMeshQuadrangle * mesh = new MXMeshQuadrangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), this->getVertex(face_point[4], allVertexData), _globalMeshId);
							geoFaceData[zoneID].second.append(mesh);
						}
						else
						{
							for (int i = 0; i < partnum; i++)
							{
								if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
								{
									partname1 = partname[i];
								}
							}
							//������
							QString partName = QString::fromStdString(partname1);
							_globalMeshId++;
							geoFacePartName[partName].insert(zoneID);
							MXMeshQuadrangle * mesh = new MXMeshQuadrangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), this->getVertex(face_point[4], allVertexData), _globalMeshId);
							geoFaceData[zoneID].second.append(mesh);
						}
					}
					else if(facetype == 0)
					{
						int ftype, ftemp;
						sscanf(temp, "%x", &ftype);
						if (ftype == 3) {
							face_point[0] = 3;
							sscanf(temp, "%x%x%x%x%x%x", &ftemp,&face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
							if (faceLR[1] > 0) //�����,��Ԫ����������part
							{
								for (int i = 0; i < NBc; i++)
								{
									if (zoneID == bcid[i])
									{
										Bctype[i] = -3;
									}
								}
								for (int i = 0; i < partnum; i++)
								{
									if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
									{
										partname1 = partname[i];
									}
								}
								for (int i = 0; i < partnum; i++)
								{
									if (faceLR[1] >= partfirstelemid[i] && faceLR[1] <= partlastelemid[i])
									{
										partname2 = partname[i];
									}
								}

								//������
								QString partName1 = QString::fromStdString(partname1);
								QString partName2 = QString::fromStdString(partname2);
								geoFacePartName[partName1].insert(zoneID);
								_globalMeshId++;
								MXMeshTriangle * mesh = new MXMeshTriangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), _globalMeshId);
								geoFaceData[zoneID].first.append(mesh);
							}
							else
							{
								for (int i = 0; i < partnum; i++)
								{
									if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
									{
										partname1 = partname[i];
									}
								}
								//������
								QString partName = QString::fromStdString(partname1);
								geoFacePartName[partName].insert(zoneID);
								_globalMeshId++;
								MXMeshTriangle * mesh = new MXMeshTriangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), _globalMeshId);
								geoFaceData[zoneID].first.append(mesh);
							}
						}
						else if (ftype == 4)
						{
							face_point[0] = 4;
							sscanf(temp, "%x%x%x%x%x%x%x", &ftemp,&face_point[4], &face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
							if (faceLR[1] > 0) //�����,��Ԫ����������part
							{
								for (int i = 0; i < NBc; i++)
								{
									if (zoneID == bcid[i])
									{
										Bctype[i] = -3;
									}
								}
								for (int i = 0; i < partnum; i++)
								{
									if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
									{
										partname1 = partname[i];
									}
								}
								for (int i = 0; i < partnum; i++)
								{
									if (faceLR[1] >= partfirstelemid[i] && faceLR[1] <= partlastelemid[i])
									{
										partname2 = partname[i];
									}
								}

								//������
								QString partName1 = QString::fromStdString(partname1);
								QString partName2 = QString::fromStdString(partname2);
								_globalMeshId++;
								geoFacePartName[partName1].insert(zoneID);
								MXMeshQuadrangle * mesh = new MXMeshQuadrangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), this->getVertex(face_point[4], allVertexData), _globalMeshId);
								geoFaceData[zoneID].second.append(mesh);
							}
							else
							{
								for (int i = 0; i < partnum; i++)
								{
									if (faceLR[0] >= partfirstelemid[i] && faceLR[0] <= partlastelemid[i])
									{
										partname1 = partname[i];
									}
								}
								//������
								QString partName = QString::fromStdString(partname1);
								_globalMeshId++;
								geoFacePartName[partName].insert(zoneID);
								MXMeshQuadrangle * mesh = new MXMeshQuadrangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), this->getVertex(face_point[4], allVertexData), _globalMeshId);
								geoFaceData[zoneID].second.append(mesh);
							}
						}
					}
					partName = QString::fromStdString(partname1);
					//��ʼ����߽���faceinfo
					if (facetype == 3) {   //������
						for (int iL = 0; iL < rdin2 - 1; iL++) {
							face_point[0] = 3;
							file_fp.getline(temp, 80);
							sscanf(temp, "%x%x%x%x%x", &face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
							if (faceLR[1] > 0)
							{

							}
							else
							{

							}
							_globalMeshId++;
							MXMeshTriangle * mesh = new MXMeshTriangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData),  _globalMeshId);
							geoFaceData[zoneID].first.append(mesh);
							//Faceinfo.push_back(face_point[0]);
							//Faceinfo.push_back(face_point[1]);
							//Faceinfo.push_back(face_point[2]);
							//Faceinfo.push_back(face_point[3]);
							//Faceinfo.push_back(0);
						}
					}
					else if (facetype == 4) { //�ı���
						for (int iL = 0; iL < rdin2 - 1; iL++) {
							face_point[0] = 4;
							file_fp.getline(temp, 80);
							sscanf(temp, "%x%x%x%x%x%x", &face_point[4], &face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
							if (faceLR[1] > 0)
							{

							}
							else
							{

							}
							_globalMeshId++;
							MXMeshQuadrangle * mesh = new MXMeshQuadrangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), this->getVertex(face_point[4], allVertexData), _globalMeshId);
							geoFaceData[zoneID].second.append(mesh);
							for (int i = 0; i < 4; i++)
							{
								qDebug() << mesh->getVertex(i)->getNum();
							}
							//Faceinfo.push_back(face_point[0]);
							//Faceinfo.push_back(face_point[1]);
							//Faceinfo.push_back(face_point[2]);
							//Faceinfo.push_back(face_point[3]);
							//Faceinfo.push_back(face_point[4]);
						}
					}
					else if (facetype == 0) { //�����
						for (int iL = 0; iL < rdin2 - 1; iL++) {
							file_fp.getline(temp, 80);
							int ftype, ftemp;
							sscanf(temp, "%x", &ftype);
							if (ftype == 3) {
								face_point[0] = 3;
								sscanf(temp, "%x%x%x%x%x%x", &ftemp,&face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
								if (faceLR[1] > 0)
								{

								}
								else
								{

								}
								_globalMeshId++;
								MXMeshTriangle * mesh = new MXMeshTriangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), _globalMeshId);
								geoFaceData[zoneID].first.append(mesh);
								//Faceinfo.push_back(face_point[0]);
								//Faceinfo.push_back(face_point[1]);
								//Faceinfo.push_back(face_point[2]);
								//Faceinfo.push_back(face_point[3]);
								//Faceinfo.push_back(0);
							}
							else if (ftype == 4) {
								face_point[0] = 4;
								sscanf(temp, "%x%x%x%x%x%x%x", &ftemp,&face_point[4], &face_point[3], &face_point[2], &face_point[1], &faceLR[0], &faceLR[1]);
								if (faceLR[1] > 0)
								{

								}
								else
								{

								}
								_globalMeshId++;
								MXMeshQuadrangle * mesh = new MXMeshQuadrangle(this->getVertex(face_point[1], allVertexData), this->getVertex(face_point[2], allVertexData), this->getVertex(face_point[3], allVertexData), this->getVertex(face_point[4], allVertexData), _globalMeshId);
								geoFaceData[zoneID].second.append(mesh);
							}
							else {
								cout << "������Ͳ���ȷ��" << endl; fflush(stdout); exit(-1);
							}
						}
					}
					else {
						cout << "������Ͳ���ȷ��" << endl; fflush(stdout); exit(-1);
					}

				}//�ڲ�����߽����ж�

			} //����faceinfo ok

		}//�ļ���ѭ������

		mGlobalSignals::getInstance()->outputMessageSig(0, QString("�����ȡ�ɹ���"));

		//���FaceSum
		/*
		for (int i = 0; i < NBc; i++)
		{
			if (db->isFluidBCIDExist(bcid.at(i)))
			{
				continue;
			}
			if (Bctype[i] != 2)
			{
				mFluidBCBase* bcBase = nullptr;
				if (Bctype[i] == 3)   //����
				{
					bcBase = new mFluidBCWall(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == -3)  //��ϱ���
				{
					bcBase = new mFluidBCCouplingWall(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == 5)  //ѹ������
				{
					bcBase = new mFluidBCPressureOutlet(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == 7)  //�Գ�
				{
					bcBase = new mFluidBCSymmetric(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == 4)  //ѹ������
				{
					bcBase = new mFluidBCPressureInlet(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == 9)  //�����߽�
				{
					bcBase = new mFluidBCOutFlow(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == 10)  //�ٶ����
				{
					bcBase = new mFluidBCVelocityInlet(QString::fromStdString(BcName.at(i)));
				}
				else if (Bctype[i] == 20)  //�����������
				{
					bcBase = new mFluidBCMassRateInlet(QString::fromStdString(BcName.at(i)));
				}
				//else if(bctype == 3 ) { bc_h_kind[iloc] = 201; }
				else
				{
					mGlobalSignals::getInstance()->outputMessageSig(1, QString("�߽�IDΪ%1�ı߽����ʹ���").arg(bcid.at(i)));
				}
				if (bcBase != nullptr)
				{
					bcBase->setFluidBCID(bcid.at(i));
					db->appendFluidBCData(bcBase);
				}
			}
		}
		*/

		mGlobalSignals::getInstance()->outputMessageSig(0, QString("�߽��ȡ�ɹ���"));

		strVec.clear();         //���vector�ռ�
		strVec.shrink_to_fit(); //���vector�ռ�


		//��cas�ļ��ٴζ���
		for (auto meshs : geoFaceData)
		{
			for (auto mesh : meshs.first)
			{
				for (int i = 0;i < 3 ;++i)
				{
					int pntid = mesh->getVertex(i)->getNum();
					mark_xyz[pntid] = 1;
				}
			}
		}
		for (auto meshs : geoFaceData)
		{
			for (auto mesh : meshs.second)
			{
				for (int i = 0; i < 4; ++i)
				{
					int pntid = mesh->getVertex(i)->getNum();
					mark_xyz[pntid] = 1;
				}
			}
		}

		int bc_point_num = 0;

		for (int i = 0; i < nodeNum; i++) {
			if (mark_xyz[i] == 1) bc_point_num += 1;
		}

		//cout << "bc_point_num=" << bc_point_num << endl;
		double xyz[3];

		file_fp.close();

		file_fp = ifstream(_fileName.toLocal8Bit(), ios::in);
		if (!file_fp.is_open())
		{
			mGlobalSignals::getInstance()->outputMessageSig(2, QString("�ļ�������"));
			return;
		}
		int nodenum = 0;

		while (!file_fp.eof()) {
			getline(file_fp, buffer);

			if (buffer.substr(0, 3) == "(10" && buffer[5] != '0') {
				cout << buffer << endl;
				char temp[80];
				for (int isize = 0; isize < 80; isize++) { temp[isize] = 0; }
				for (int isize = 5; isize < buffer.size(); isize++) { temp[isize - 5] = buffer[isize]; }
				int zoneID, rdin1, rdin2, rdin3, rdim;
				sscanf(temp, "%x%x%x%x%x", &zoneID, &rdin1, &rdin2, &rdin3, &rdim);
				if (rdim != 3) { cout << "��3D����" << endl; fflush(stdout); exit(-1); }
				//rdin1 = rdin1 - 1; //��0��ʼ

				//��ʼ����xyz����
				getline(file_fp, buffer);
				if (buffer.size() > 2) {  //����֮ǰû��'('
					memset(temp, 0, 80);
					for (int isize = 0; isize < buffer.size(); isize++) { temp[isize] = buffer[isize]; }
					if (mark_xyz[rdin1] == 1)
					{
						++nodenum;
						sscanf(temp, "%lf%lf%lf", &xyz[0], &xyz[1], &xyz[2]);
						MXMeshVertex *vertex = getVertex(rdin1, allVertexData);
						vertex->set_xyz(xyz[0], xyz[1], xyz[2]);
					}

					for (int iL = rdin1 + 1; iL <= rdin2; iL++) {
						file_fp.getline(temp, 80);
						if (mark_xyz[iL] == 1)
						{
							++nodenum;
							sscanf(temp, "%lf%lf%lf", &xyz[0], &xyz[1], &xyz[2]);
							MXMeshVertex *vertex = getVertex(iL, allVertexData);
							vertex->set_xyz(xyz[0], xyz[1], xyz[2]);
						}

					}
				}
				else {  //����֮ǰ��'('
					for (int iL = rdin1; iL <= rdin2; iL++) {
						file_fp.getline(temp, 80);
						if (mark_xyz[iL] == 1)
						{
							++nodenum;
							sscanf(temp, "%lf%lf%lf", &xyz[0], &xyz[1], &xyz[2]);
							MXMeshVertex *vertex = getVertex(iL, allVertexData);
							vertex->set_xyz(xyz[0], xyz[1], xyz[2]);
						}
					}
				}
				if (nodenum == bc_point_num) break;
			} //����� ok

		}


		mGlobalSignals::getInstance()->outputMessageSig(0, QString("�ڵ��ȡ�ɹ���"));

		MXReadData *readData=MeshMessage::getInstance()->getReadData();
		readData->CreateVertex(allVertexData);
		QHashIterator<QString,set<int>> iter(geoFacePartName);
		while (iter.hasNext())
		{
			iter.next();
			QHash<int, QPair<QVector<MXMeshTriangle*>, QVector<MXMeshQuadrangle*>>> values;
			for (auto id : iter.value())
			{
				values.insert(id, geoFaceData.value(id));
			}
			readData->CreateGeoFace(iter.key(), values);
		}

		file_fp.close();
		//�ٴζ���ok
		std::free(mark_xyz);

		//db->getFluidCommonSettingData()->_partNum = partnum;
		//db->getFluidCommonSettingData()->_nodeNum = nodenum;

		//int nodeID = 1;
		//MXReadData *readData = MeshMessage::getInstance()->getReadData();
		//QString partName = "part1";
		//MXMeshTriangle *tri;
		//MXMeshQuadrangle *quad;
		//QVector<MXMeshTriangle*> tris;
		//QVector<MXMeshQuadrangle*> quads;
		/////*******************��**************/
		//_globalFaceId++;
		//tri = new MXMeshTriangle(new MXMeshVertex(0, 0, 0, ++nodeID), new MXMeshVertex(0, 1, 0, ++nodeID), new MXMeshVertex(1, 0, 0, ++nodeID), ++_globalMeshId);tris.append(tri);
		//for (int i = 0; i < 3; ++i)
		//{
		//	int pntid = tri->getVertex(i)->getNum();
		//	qDebug() << pntid;
		//}

		//tri = new MXMeshTriangle(new MXMeshVertex(1, 1, 0, ++nodeID), new MXMeshVertex(0, 1, 0, ++nodeID), new MXMeshVertex(1, 0, 0, ++nodeID), ++_globalMeshId); tris.append(tri);
		//quad = new MXMeshQuadrangle(new MXMeshVertex(0, 0, 1, ++nodeID), new MXMeshVertex(1, 0, 1, ++nodeID), new MXMeshVertex(1, 1, 1, ++nodeID), new MXMeshVertex(0, 1, 1, ++nodeID), ++_globalMeshId);quads.append(quad);
		//readData->CreateGeoFace(partName, _globalFaceId, tris, quads);

	}
	MXMeshVertex * mIFluentBCMeshFile::getVertex(int id, QHash<int, MXMeshVertex*> &vertexs)
	{
		MXMeshVertex *vertex = vertexs[id];
		if (vertex == nullptr)
		{
			vertex = new MXMeshVertex(0, 0, 0, id);
			vertexs[id] = vertex;
		}
		return vertex;
	}
}
