#include "gmusicplayermainwidget.h"
#include <QApplication>
#include "gmusicplayservice.h"
#include "gfuncprovider.h"
#include "gmusicplayservice.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GMusicPlayerMainWidget w(dynamic_cast<IUnknown *>(GFuncProvider::getInstance()));
	w.show();
	int ret = a.exec();

	return ret;
}
