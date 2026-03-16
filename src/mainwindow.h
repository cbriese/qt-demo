#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QHeaderView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <iostream>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void about();
	void quit();

private:
	void openDbConnectionDialog();
	bool connectToDatabase();
	void createStatusBar();
	void createActions();
	void createMenus();
	void setupTableView();
	void setupDataEntryFrame();

	QStatusBar *mainStatusBar;
	QMenu *fileMenu;
	QMenu *helpMenu;
	QAction *aboutAct;
	QAction *quitAct;
	QAction *connectAct;
	QDialog *loginDialog;
	QWidget *centralWidget;
	QFrame *dataEntryFrame;
	QSqlDatabase db;
	QTableView *contactsView;
	QVBoxLayout *mainLayout;
};

#endif // MAINWINDOW_H

