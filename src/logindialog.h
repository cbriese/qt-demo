#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGridLayout>

class LoginDialog : public QDialog
{
	Q_OBJECT

private slots:

private:
	QLabel *labelHostname;
	QLabel *labelDatabaseName;
	QLabel *labelUsername;
	QLabel *labelPassword;

	QLineEdit *editHostname;
	QLineEdit *editDatabaseName;
	QLineEdit *editUsername;
	QLineEdit *editPassword;
	QDialogButtonBox *buttons;

	void setupUI();

public:
	LoginDialog(QWidget *parent = nullptr);
	~LoginDialog();
	QString hostname();
	QString databaseName();
	QString username();
	QString password();

};

#endif // LOGINDIALOG_H

