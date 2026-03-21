#include "logindialog.h"

// Constructor for the main window
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
	setWindowTitle(tr("Enter DB Credentials"));
	setupUI();
	setModal(true);
}

// Destructor
LoginDialog::~LoginDialog()
{
}

void LoginDialog::setupUI()
{
	// Create labels
	labelHostname = new QLabel("Hostname:", this);
	labelDatabaseName = new QLabel("Database Name:", this);
	labelUsername = new QLabel("Username:", this);
	labelPassword = new QLabel("Password:", this);

	// Create edit widgets
	editHostname = new QLineEdit(this);
	editDatabaseName = new QLineEdit(this);
	editUsername = new QLineEdit(this);
	editPassword = new QLineEdit(this);

	// Configure Widgets
	editPassword->setEchoMode(QLineEdit::Password);
	editHostname->setText("localhost");
	editDatabaseName->setText("contacts");

	// Create "Ok" and "Cancel" buttons
	buttons = new QDialogButtonBox(this);
	buttons->addButton( QDialogButtonBox::Ok );
	buttons->addButton( QDialogButtonBox::Cancel );
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	// Create a layout
	QGridLayout *formLayout = new QGridLayout(this);

	// Add widgets to the layout
	formLayout->addWidget(labelHostname, 0, 0);
	formLayout->addWidget(editHostname, 0, 1);
	formLayout->addWidget(labelDatabaseName, 1, 0);
	formLayout->addWidget(editDatabaseName, 1, 1);
	formLayout->addWidget(labelUsername, 2, 0);
	formLayout->addWidget(editUsername, 2, 1);
	formLayout->addWidget(labelPassword, 3, 0);
	formLayout->addWidget(editPassword, 3, 1);
	formLayout->addWidget(buttons, 4, 0, 1, 2, Qt::AlignCenter);

	// Ensure the "Username" field has focus
	editUsername->setFocus();
}

QString LoginDialog::hostname() {
	return(this->editHostname->text());
}

QString LoginDialog::databaseName() {
	return(this->editDatabaseName->text());
}

QString LoginDialog::username() {
	return(this->editUsername->text());
}

QString LoginDialog::password() {
	return(this->editPassword->text());
}

