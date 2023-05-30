#pragma once
#include <QString>
#include <string>
#include <vector>
#include <QStringList>

using namespace std;
class MXMeshVertex;
namespace MIOFile
{
	class mIFluentBCMeshFile
	{
	public:
		bool ReadBCMeshFile(QString filename);

		void ReadFile1(ifstream &file_fp, vector<string> strVec, string buffer);

		MXMeshVertex *getVertex(int id, QHash<int, MXMeshVertex*> &vertexs);

	private:
		//当前文件下面所有的部件名字
		QStringList _filepartnames;

		//文件路径
		QString _fileName;
	};
}
