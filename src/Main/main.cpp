//解决中文乱码
//#pragma execution_character_set("utf-8")

#include <iostream>
#include <sstream>
#include <fstream>
#include <QCoreApplication>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setMajorVersion(4);
	format.setMinorVersion(5);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	return a.exec();
}