#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlField>

class ContactDialog : public QDialog
{
	Q_OBJECT

private slots:
	void validateInput();

private:
	QLabel *labelFirstName;
	QLabel *labelLastName;
	QLabel *labelAddress1;
	QLabel *labelAddress2;
	QLabel *labelCity;
	QLabel *labelState;
	QLabel *labelZipCode;
	QLabel *labelPhone1;
	QLabel *labelPhone2;
	QLabel *labelEmail1;
	QLabel *labelEmail2;
	QLabel *labelBirthday;
	QLabel *labelSaveBirthday;
	QDialogButtonBox *buttons;

	QLineEdit *editFirstName;
	QLineEdit *editLastName;
	QLineEdit *editAddress1;
	QLineEdit *editAddress2;
	QLineEdit *editCity;
	QLineEdit *editState;
	QLineEdit *editZipCode;
	QLineEdit *editPhone1;
	QLineEdit *editPhone2;
	QLineEdit *editEmail1;
	QLineEdit *editEmail2;
	QDateEdit *editBirthday;
	QCheckBox *checkSaveBirthday;

	QSqlRecord *contactRec;

	void setupUI();

public:
	ContactDialog(QWidget *parent = nullptr, QSqlRecord *rec = nullptr);
	~ContactDialog();
	void setContact(QSqlRecord *);
	QString firstName();
	QString lastName();
	QString address1();
	QString address2();
	QString city();
	QString state();
	QString zipCode();
	QString phone1();
	QString phone2();
	QString email1();
	QString email2();
	QDate birthday();
	bool saveBirthday();

};

#endif // CONTACTDIALOG_H

