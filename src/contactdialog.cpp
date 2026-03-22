#include "contactdialog.h"

// Constructor for the main window
ContactDialog::ContactDialog(QWidget *parent, QSqlRecord *rec)
    : QDialog(parent)
{
	setContact(rec);
	setWindowTitle(tr("New Contact"));
	setupUI();
	setModal(true);
}

// Destructor
ContactDialog::~ContactDialog()
{
}

void ContactDialog::setupUI()
{
	// Create labels
	labelFirstName = new QLabel("First Name:", this);
	labelLastName = new QLabel("Last Name:", this);
	labelBirthday = new QLabel("Birthday:", this);
	labelSaveBirthday = new QLabel("Save Birthday?", this);
	labelAddress1 = new QLabel("Address 1:", this);
	labelAddress2 = new QLabel("Address 2:", this);
	labelCity = new QLabel("City:", this);
	labelState = new QLabel("State:", this);
	labelZipCode = new QLabel("Zip Code:", this);
	labelPhone1 = new QLabel("Phone #1:", this);
	labelPhone2 = new QLabel("Phone #2:", this);
	labelEmail1 = new QLabel("E-mail #1:", this);
	labelEmail2 = new QLabel("E-mail #2:", this);

	// Create edit widgets
	editFirstName = new QLineEdit(this);
	editLastName = new QLineEdit(this);
	editBirthday = new QDateEdit(this);
	checkSaveBirthday = new QCheckBox(this);
	editAddress1 = new QLineEdit(this);
	editAddress2 = new QLineEdit(this);
	editCity = new QLineEdit(this);
	editState = new QLineEdit(this);
	editZipCode = new QLineEdit(this);
	editPhone1 = new QLineEdit(this);
	editPhone2 = new QLineEdit(this);
	editEmail1 = new QLineEdit(this);
	editEmail2 = new QLineEdit(this);

	// Create "Ok" and "Cancel" buttons
	buttons = new QDialogButtonBox(this);
	buttons->addButton( QDialogButtonBox::Ok );
	buttons->addButton( QDialogButtonBox::Cancel );
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	// If we have a contact record, try to fill in the fields
	if (contactRec != nullptr)
	{
		setWindowTitle("Modify Contact");

		if (!contactRec->field("first_name").value().toString().isEmpty()) {
			editFirstName->setText(contactRec->field("first_name").value().toString());
		}

		if (!contactRec->field("last_name").value().toString().isEmpty()) {
			editLastName->setText(contactRec->field("last_name").value().toString());
		}

		if (!contactRec->field("address_1").value().toString().isEmpty()) {
			editAddress1->setText(contactRec->field("address_1").value().toString());
		}

		if (!contactRec->field("address_2").value().toString().isEmpty()) {
			editAddress2->setText(contactRec->field("address_2").value().toString());
		}

		if (!contactRec->field("city").value().toString().isEmpty()) {
			editCity->setText(contactRec->field("city").value().toString());
		}

		if (!contactRec->field("state").value().toString().isEmpty()) {
			editState->setText(contactRec->field("state").value().toString());
		}

		if (!contactRec->field("zipcode").value().toString().isEmpty()) {
			editZipCode->setText(contactRec->field("zipcode").value().toString());
		}

		if (!contactRec->field("phone_1").value().toString().isEmpty()) {
			editPhone1->setText(contactRec->field("phone_1").value().toString());
		}

		if (!contactRec->field("phone_2").value().toString().isEmpty()) {
			editPhone2->setText(contactRec->field("phone_2").value().toString());
		}

		if (!contactRec->field("email_1").value().toString().isEmpty()) {
			editEmail1->setText(contactRec->field("email_1").value().toString());
		}

		if (!contactRec->field("email_2").value().toString().isEmpty()) {
			editEmail2->setText(contactRec->field("email_2").value().toString());
		}

		if (!contactRec->field("birthday").isNull()) {
			editBirthday->setDate(contactRec->field("birthday").value().toDate());
		}
	}

	// Apply input masks
	editState->setInputMask(">AA");
	editZipCode->setInputMask("99999");

	editBirthday->setDisplayFormat("MM-dd-yyyy");
	editBirthday->clear();

	// Apply input validators
	QRegularExpression rx1("^\\d{3}-\\d{3}-\\d{4}$");
	QRegularExpressionValidator *phoneNumberValidator = new QRegularExpressionValidator(rx1, this);
	editPhone1->setValidator(phoneNumberValidator);
	editPhone2->setValidator(phoneNumberValidator);

	// Connect input widgets to validation routine
	connect(editFirstName, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editLastName, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editAddress1, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editAddress2, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editCity, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editState, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editZipCode, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editPhone1, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editPhone2, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editEmail1, &QLineEdit::textChanged, this, &ContactDialog::validateInput);
	connect(editEmail2, &QLineEdit::textChanged, this, &ContactDialog::validateInput);

	// Create a layout
	QGridLayout *formLayout = new QGridLayout(this);

	// Add first logical column of widgets to the layout
	formLayout->addWidget(labelFirstName, 0, 0);
	formLayout->addWidget(editFirstName, 1, 0);
	formLayout->addWidget(labelLastName, 2, 0); 
	formLayout->addWidget(editLastName, 3, 0);
	formLayout->addWidget(labelBirthday, 4, 0);
	formLayout->addWidget(editBirthday, 5, 0);
	formLayout->addWidget(labelSaveBirthday, 6, 0);
	formLayout->addWidget(checkSaveBirthday, 7, 0);

	// Add second logical column (comprised of 3 columns)
	formLayout->addWidget(labelAddress1, 0, 2, 1, 3);
	formLayout->addWidget(editAddress1, 1, 2, 1, 3);
	formLayout->addWidget(labelAddress2, 2, 2, 1, 3);
	formLayout->addWidget(editAddress2, 3, 2, 1, 3);
	formLayout->addWidget(labelCity, 4, 2);
	formLayout->addWidget(editCity, 5, 2);
	formLayout->addWidget(labelState, 4, 4);
	formLayout->addWidget(editState, 5, 4);
	formLayout->addWidget(labelZipCode, 6, 2);
	formLayout->addWidget(editZipCode, 7, 2);

	// Add third logical column
	formLayout->addWidget(labelPhone1, 0, 6);
	formLayout->addWidget(editPhone1, 1, 6);
	formLayout->addWidget(labelPhone2, 2, 6);
	formLayout->addWidget(editPhone2, 3, 6);
	formLayout->addWidget(labelEmail1, 4, 6);
	formLayout->addWidget(editEmail1, 5, 6);
	formLayout->addWidget(labelEmail2, 6, 6);
	formLayout->addWidget(editEmail2, 7, 6);

	// Add the buttons at the bottom
	formLayout->addWidget(buttons, 10, 0, 1, 7, Qt::AlignCenter);

	// Set minimum widths for the columns of the layout
	formLayout->setColumnMinimumWidth(0, 150);
	formLayout->setColumnMinimumWidth(1, 5);
	formLayout->setColumnMinimumWidth(2, 125);
	formLayout->setColumnMinimumWidth(3, 15);
	formLayout->setColumnMinimumWidth(4, 125);
	formLayout->setColumnMinimumWidth(5, 5);
	formLayout->setColumnMinimumWidth(6, 300);

	// Ensure the "First Name" field has focus
	editFirstName->setFocus();

	// Disable the "Ok" button if we are creating a contact
	// and allow the input validation to enable it
	if (contactRec == nullptr) {
		buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
	}

}

void ContactDialog::validateInput() {
	bool allInputValid;

	allInputValid = !editFirstName->text().isEmpty() &&
		!editLastName->text().isEmpty() &&
		(editPhone1->hasAcceptableInput() || editPhone1->text().isEmpty()) &&
		(editPhone2->hasAcceptableInput() || editPhone2->text().isEmpty());
	buttons->button(QDialogButtonBox::Ok)->setEnabled(allInputValid);
}

void ContactDialog::setContact(QSqlRecord *record) {

	contactRec = record;
}

QString ContactDialog::firstName() {
	return(this->editFirstName->text());
}

QString ContactDialog::lastName() {
	return(this->editLastName->text());
}

QString ContactDialog::address1() {
	return(this->editAddress1->text());
}

QString ContactDialog::address2() {
	return(this->editAddress2->text());
}

QString ContactDialog::city() {
	return(this->editCity->text());
}

QString ContactDialog::state() {
	return(this->editState->text());
}

QString ContactDialog::zipCode() {
	return(this->editZipCode->text());
}

QString ContactDialog::phone1() {
	return(this->editPhone1->text());
}

QString ContactDialog::phone2() {
	return(this->editPhone2->text());
}

QString ContactDialog::email1() {
	return(this->editEmail1->text());
}

QString ContactDialog::email2() {
	return(this->editEmail2->text());
}

QDate ContactDialog::birthday() {
	return(this->editBirthday->date());
}

bool ContactDialog::saveBirthday() {
	if (this->checkSaveBirthday->checkState() == Qt::Checked) {
		return(true);
	} else {
		return(false);
	}
}
