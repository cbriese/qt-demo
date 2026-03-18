#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Create the main window
	MainWindow w;

	// Set title
	w.setWindowTitle("Qt Demo Application");

	// Show main window
	w.show();

	// Open the database connection dialog
	w.openDbConnectionDialog();

	// Run it
	return a.exec();
}

