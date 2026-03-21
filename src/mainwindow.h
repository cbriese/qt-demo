#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QString>
#include <iostream>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void openDbConnectionDialog();

private slots:
	void about();
	void quit();
	void createOrEditContact();
	void deleteContact();
	void notImplemented();

private:
	bool connectToDatabase();
	void createStatusBar();
	void createActions();
	void createToolbars();
	void createMenus();
	void setupTableView();
	void refreshContactsView();
	void disconnectDb();

	QStatusBar *mainStatusBar;
	QToolBar *fileToolBar;
	QMenu *fileMenu;
	QMenu *helpMenu;
	QAction *aboutAct;
	QAction *quitAct;
	QAction *connectAct;
	QAction *disconnectAct;
	QAction *newContactAct;
	QAction *deleteContactAct;
	QDialog *loginDialog;
	QWidget *centralWidget;
	QSqlDatabase db;
	QTableView *contactsView;
	QVBoxLayout *mainLayout;
	QSqlTableModel *contactsModel;
};

#endif // MAINWINDOW_H

