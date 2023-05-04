#include "MxRendTest.h"
#include <QtWidgets/QApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

	/*
	// ����һ�� QOpenGLContext ���󲢳��Գ�ʼ�� OpenGL ������
	QScopedPointer<QOpenGLContext> context(new QOpenGLContext);
	context->create();

	QSurfaceFormat form = context->format();

	// ��ѯ OpenGL �汾
	int major = form.majorVersion();
	int minor = form.minorVersion();
	*/
	/*
	QOffscreenSurface surf;
	surf.create();

	QOpenGLContext ctx;
	ctx.create();
	ctx.makeCurrent(&surf);

	qDebug() << (const char *)ctx.functions()->glGetString(GL_VERSION);
	qDebug() << (const char *)ctx.functions()->glGetString(GL_EXTENSIONS);
	*/
	QSurfaceFormat format;
	format.setMajorVersion(4);
	format.setMinorVersion(5);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);
	
	//setFormat(format);

	//QOpenGLContext *context1 = new QOpenGLContext;
	//context1->setFormat(format);
    //MxRendTest w(1);
    //w.show();

	//QOpenGLContext *context2 = new QOpenGLContext;
	//context2->setFormat(format);
	//context2->setShareContext(context1);
	//if (!context2->create()) {
	//	// context2����ʧ��
	//}
	//context2->setFormat(format);
	MxRendTest w0(0);
	w0.show();
	//MxRendTest w1(1);
	//w1.show();

	//MxRendTest w2(2);
	//w2.show();
    return a.exec();
}
