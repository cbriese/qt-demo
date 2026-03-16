#include <QApplication>
#include "mainwindow.h"

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	createStatusBar();
	createActions();
	createMenus();
	setupTableView();
	setupDataEntryFrame();
	mainStatusBar->showMessage("Ready. Not connected.");
	centralWidget = new QWidget;
	mainLayout = new QVBoxLayout(centralWidget);
	setCentralWidget(centralWidget);
	// Add data entry and display widgets to the central widget layout
	mainLayout->addWidget(dataEntryFrame);
	mainLayout->addWidget(contactsView);

	resize(800, 600);
}

// Destructor
MainWindow::~MainWindow()
{
}

void MainWindow::createStatusBar()
{
	mainStatusBar = new QStatusBar(this);
	this->setStatusBar(mainStatusBar);
}

void MainWindow::openDbConnectionDialog()
{
	loginDialog = new QDialog(this);
	loginDialog->setWindowTitle("Enter Database Credentials");
	// loginDialog->resize(400,300);

	// Create some widgets to put into the dialog?
	QGridLayout *dbConnectLayout = new QGridLayout(loginDialog);

	// Set up widgets to collect database login credentials
	QLabel *dbHostnameLabel = new QLabel(tr("DB Hostname:"));
	QLabel *dbNameLabel = new QLabel(tr("DB Name:"));
	QLabel *dbUsernameLabel = new QLabel(tr("DB Username:"));
	QLabel *dbPasswordLabel = new QLabel(tr("DB Password:"));
	QLineEdit *dbHostnameEntry = new QLineEdit();
	QLineEdit *dbNameEntry = new QLineEdit();
	QLineEdit *dbUsernameEntry = new QLineEdit();
	QLineEdit *dbPasswordEntry = new QLineEdit();
	dbPasswordEntry->setEchoMode(QLineEdit::Password);

	QDialogButtonBox *dbConnectButtonBox = new QDialogButtonBox(
		QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
		Qt::Horizontal, loginDialog
	);
	connect(dbConnectButtonBox, &QDialogButtonBox::accepted, loginDialog, &QDialog::accept);
	connect(dbConnectButtonBox, &QDialogButtonBox::rejected, loginDialog, &QDialog::reject);

	// Add widgets to the layout
	dbConnectLayout->addWidget(dbHostnameLabel, 0, 0);
	dbConnectLayout->addWidget(dbNameLabel, 1, 0);
	dbConnectLayout->addWidget(dbUsernameLabel, 2, 0);
	dbConnectLayout->addWidget(dbPasswordLabel, 3, 0);
	dbConnectLayout->addWidget(dbHostnameEntry, 0, 1);
	dbConnectLayout->addWidget(dbNameEntry, 1, 1);
	dbConnectLayout->addWidget(dbUsernameEntry, 2, 1);
	dbConnectLayout->addWidget(dbPasswordEntry, 3, 1);
	dbConnectLayout->addWidget(dbConnectButtonBox, 4, 0, 1, 2, Qt::AlignCenter);

	loginDialog->setLayout(dbConnectLayout);

	while (1) {
		int value = loginDialog->exec();

		std::cerr << "The value returned from the dialog was " << value << std::endl;

		if (value == 1)
		{
			// How do i get the values from these form fields?
			if (dbHostnameEntry->text().isEmpty()) {
				continue;
			}

			if (dbNameEntry->text().isEmpty()) {
				continue;
			}

			if (dbUsernameEntry->text().isEmpty()) {
				continue;
			}

			if (dbPasswordEntry->text().isEmpty()) {
				continue;
			}
 
			std::cerr << "The DB hostname entered was " << dbHostnameEntry->text().toStdString() << std::endl;
			std::cerr << "The DB name entered was " << dbNameEntry->text().toStdString() << std::endl;
			std::cerr << "The DB username entered was " << dbUsernameEntry->text().toStdString() << std::endl;
			std::cerr << "The DB password entered was " << dbPasswordEntry->text().toStdString() << std::endl;
			break;
		}
		else
		{
			break;
		}
	}
}

// Method to create the actions
void MainWindow::createActions()
{
	// Create the "About" action, connect its triggered() signal to the about() slot
	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

	// Create the "Quit" action with a standard shortcut, connect to the quit() slot
	quitAct = new QAction(tr("&Quit"), this);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Exit the application"));
	connect(quitAct, &QAction::triggered, this, &MainWindow::quit);

	// Create another action
	connectAct = new QAction(tr("&Connect..."), this);
	connectAct->setStatusTip(tr("Connect to the database"));
	connect(connectAct, &QAction::triggered, this, &MainWindow::openDbConnectionDialog);

}

// Method to create the menus
void MainWindow::createMenus()
{
	// Get the main window's menu bar and add menus
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(connectAct);
	fileMenu->addAction(quitAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
}

// Callback to display "about" information
void MainWindow::about()
{
	QMessageBox::about(this, tr("About Menu"),
		tr("The <b>Qt</b> example demonstrates how to "
		"use various Qt features."));
}

void MainWindow::quit()
{
	QApplication::quit();
}

bool MainWindow::connectToDatabase() {
	db = QSqlDatabase::addDatabase("QPSQL");
	// NEED to replace with environment variables
	if (std::getenv("CONTACTS_DB_HOSTNAME") != nullptr) {
		db.setHostName(std::getenv("CONTACTS_DB_HOSTNAME"));
	} else {
		qDebug() << "Environment variable CONTACTS_DB_HOSTNAME not set";
		std::cout << "Environment variable CONTACTS_DB_HOSTNAME not set" << std::endl;
		return false;
	}

	if (std::getenv("CONTACTS_DB_NAME") != nullptr) {
		db.setDatabaseName(std::getenv("CONTACTS_DB_NAME"));
	} else {
		qDebug() << "Environment variable CONTACTS_DB_NAME not set";
		std::cout << "Environment variable CONTACTS_DB_NAME not set" << std::endl;
		return false;
	}

	if (std::getenv("CONTACTS_DB_USERNAME") != nullptr) {
		db.setUserName(std::getenv("CONTACTS_DB_USERNAME"));
	} else {
		qDebug() << "Environment variable CONTACTS_DB_USERNAME not set";
		std::cout << "Environment variable CONTACTS_DB_USERNAME not set" << std::endl;
		return false;
	}

	if (std::getenv("CONTACTS_DB_PASSWORD") != nullptr) {
		db.setPassword(std::getenv("CONTACTS_DB_PASSWORD"));
	} else {
		qDebug() << "Environment variable CONTACTS_DB_PASSWORD not set";
		std::cout << "Environment variable CONTACTS_DB_PASSWORD not set" << std::endl;
		return false;
	}

	if (!db.open()) {
		qDebug() << "Database connection failed:" << db.lastError().text();
		return false;
	} else {
		qDebug() << "Database connected successfully.";
		return true;
	}
}

void MainWindow::setupDataEntryFrame()
{
	// Create a frame to old the data entry widgets
	dataEntryFrame = new QFrame();

	// Create a layout for the data entry widgets
	QGridLayout *dataEntryLayout = new QGridLayout(dataEntryFrame);

	// Create labels for data entry
	QLabel *firstLabel = new QLabel(QObject::tr("First Name"));
	QLabel *lastLabel = new QLabel(QObject::tr("Last Name"));

	// Create widgets for data entry
	QLineEdit *firstEdit = new QLineEdit();
	QLineEdit *lastEdit = new QLineEdit();

	// Create a button
	QPushButton *addContactButton = new QPushButton("&Add Contact");

	// Add labels to data entry layout
	dataEntryLayout->addWidget(firstLabel, 0, 0);
	dataEntryLayout->addWidget(firstEdit, 0, 1);
	dataEntryLayout->addWidget(lastLabel, 1, 0);
	dataEntryLayout->addWidget(lastEdit, 1, 1);
	dataEntryLayout->addWidget(addContactButton, 2, 1, 1, 2);

}

void MainWindow::setupTableView()
{
	// Create a QSqlQueryModel to hold the data
	QSqlQueryModel *model = new QSqlQueryModel();
	if (db.isOpen())
	{
		model->setQuery("SELECT id, first_name, last_name FROM public.contacts ORDER BY id ASC");
	}

	// Check for query errors
	if (model->lastError().isValid()) {
		qDebug() << "Query failed:" << model->lastError().text();
		this->quit();
	}

	// Set human-readable headers (optional)
	model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
	model->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
	model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));

	// Create a QTableView and set the model
	contactsView = new QTableView();
	contactsView->setModel(model);
	contactsView->horizontalHeader()->setStretchLastSection(true);
	contactsView->resizeColumnsToContents();

	// Hide the vertical header of the table
	contactsView->verticalHeader()->hide();

	std::cerr << "Made it here" << std::endl;
}

