#include "radarmapcreator.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RadarMapCreator w;
	w.show();
	return a.exec();
}
