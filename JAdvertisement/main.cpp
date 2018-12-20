#include "JAdWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JAdWidget w;

    for(int i = 0; i < 6; i++)
    {
        QPixmap pic(QString(":/res/jpg/%1.jpg").arg(i+1));
        w.addPage(pic);
    }
    w.show();

    return a.exec();
}
