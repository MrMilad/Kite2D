#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);

	a.setOrganizationName("Kite2D");
	a.setApplicationName("Kite2D Editor");

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(34, 34, 34));
	palette.setColor(QPalette::AlternateBase, QColor(63, 63, 63));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Link, QColor(42, 130, 218));

	palette.setColor(QPalette::Highlight, QColor(0, 150, 215));
	palette.setColor(QPalette::HighlightedText, Qt::white);

    a.setPalette(palette);
    MainWindow w;
    w.show();

    return a.exec();
}
