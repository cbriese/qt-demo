#include <QApplication>
#include "mainwindow.h"

// Constructor for the main window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	// Create status bar
	createStatusBar();

	// Create actions
	createActions();

	// Create the menus
	createMenus();

	// Prepare data table
	setupTableView();

	// Display a message in the status bar
	mainStatusBar->showMessage("Ready. Not connected.");

	// Create a central widget for the main window
	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	// Create a vertical box layout for the main window contents
	mainLayout = new QVBoxLayout(centralWidget);

	// Set database driver for default database connection
	db = QSqlDatabase::addDatabase("QPSQL");

	// Add data table widget to the central widget layout
	mainLayout->addWidget(contactsView);

	resize(1024, 768);
}

// Destructor
MainWindow::~MainWindow()
{
}

/*
 * Creates a status bar for the main window
 */
void MainWindow::createStatusBar()
{
	mainStatusBar = new QStatusBar(this);
	this->setStatusBar(mainStatusBar);
}

/*
 * This method will display a dialog box to allow the user
 * to enter the hostname, database name, username,
 * and password for the database connection
 */
void MainWindow::openDbConnectionDialog()
{
	if (db.isOpen()) {
		// Tell user the DB is already connected and return
		QString msg = "Already connected to database ";
		QMessageBox::information(this, tr("Connection Status"), msg.append(db.databaseName()));
		mainStatusBar->showMessage("Connected");
		return;
	}

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
	QLineEdit *dbHostnameEntry = new QLineEdit(tr("localhost"));
	QLineEdit *dbNameEntry = new QLineEdit(tr("contacts"));
	QLineEdit *dbUsernameEntry = new QLineEdit();
	QLineEdit *dbPasswordEntry = new QLineEdit();

	// Configure text entry fields
	dbPasswordEntry->setEchoMode(QLineEdit::Password);

	// Set up some buttons for the dialog box (Ok & Cancel)
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

	// Ensure username field has the keyboard focus
	dbUsernameEntry->setFocus();

	loginDialog->setLayout(dbConnectLayout);

	while (!db.isOpen()) {
		int value = loginDialog->exec();
#ifdef DEBUG

		std::cerr << "The value returned from the dialog was " << value << std::endl;
#endif

		// User clicked "Ok"
		if (value == 1)
		{
			// How do i get the values from these form fields?
			if (dbHostnameEntry->text().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database hostname required")
				);
				continue;
			}

			if (dbNameEntry->text().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database name required")
				);
				continue;
			}

			if (dbUsernameEntry->text().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database username required")
				);
				continue;
			}

			if (dbPasswordEntry->text().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database password required")
				);
				continue;
			}
 
			// db.setHostName(std::getenv("CONTACTS_DB_HOSTNAME"));
			// db.setDatabaseName(std::getenv("CONTACTS_DB_NAME"));
			// db.setUserName(std::getenv("CONTACTS_DB_USERNAME"));
			// db.setPassword(std::getenv("CONTACTS_DB_PASSWORD"));

			db.setHostName(dbHostnameEntry->text());
			db.setDatabaseName(dbNameEntry->text());
			db.setUserName(dbUsernameEntry->text());
			db.setPassword(dbPasswordEntry->text());
#ifdef DEBUG

			std::cerr << "The DB hostname entered was " <<
				dbHostnameEntry->text().toStdString() << std::endl;
			std::cerr << "The DB name entered was "<<
				dbNameEntry->text().toStdString() << std::endl;
			std::cerr << "The DB username entered was " <<
				dbUsernameEntry->text().toStdString() << std::endl;
			std::cerr << "The DB password entered was " <<
				dbPasswordEntry->text().toStdString() << std::endl;
#endif

			if (!db.open()) {
				qDebug() << "Database connection failed:" << db.lastError().text();
				QMessageBox::warning(this, db.lastError().driverText(),
					db.lastError().databaseText()
				);
				continue;
			} else {
				this->mainStatusBar->showMessage("Connected");
			}

			break;
		}
		// User clicked "Cancel" or hit <Esc>
		else
		{
			break;
		}
	}

	if (db.isOpen()) {
		// Update the status bar
		this->mainStatusBar->showMessage("Connected");

		// Refresh the data in the table view
		this->refreshContactsView();
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

	// Create the "Connect..." action 
	connectAct = new QAction(tr("&Connect..."), this);
	connectAct->setStatusTip(tr("Connect to the database"));
	connect(connectAct, &QAction::triggered, this, &MainWindow::openDbConnectionDialog);

	// Create "Disconnect" action
	disconnectAct = new QAction(tr("&Disconnect"), this);
	disconnectAct->setStatusTip(tr("Disconnect from the database"));
	connect(disconnectAct, &QAction::triggered, this, &MainWindow::disconnectDb);
}

void MainWindow::disconnectDb()
{
	if (db.isOpen()) {
		// QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(contactsView->model());
		contactsView->setModel(nullptr);
		db.close();
	}
	else
	{
		QMessageBox::information(this, "Connection Status", tr("Not connected"));
	}
	mainStatusBar->showMessage("Disconnected");
}

// Method to create the menus
void MainWindow::createMenus()
{
	// Get the main window's menu bar and add menus
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(connectAct);
	fileMenu->addAction(disconnectAct);
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

// Callback to quit the application
void MainWindow::quit()
{
	QApplication::quit();
}

/*
 * Prepare the area of the main window that allows the user
 * to enter data for a new contact.
 */
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
	connect(addContactButton, &QPushButton::clicked, this, &MainWindow::notImplemented);

	// Add labels to data entry layout
	dataEntryLayout->addWidget(firstLabel, 0, 0);
	dataEntryLayout->addWidget(firstEdit, 0, 1);
	dataEntryLayout->addWidget(lastLabel, 1, 0);
	dataEntryLayout->addWidget(lastEdit, 1, 1);
	dataEntryLayout->addWidget(addContactButton, 2, 1, 1, 2);

}

void MainWindow::notImplemented() {
	QMessageBox::warning(this, "Oops!", "This function is not implemented");
}

void MainWindow::refreshContactsView()
{
	QSqlTableModel *model = new QSqlTableModel();
	model->setTable("contacts");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->select();

	if (model->lastError().isValid())
	{
		QMessageBox::warning(this, model->lastError().driverText(),
			model->lastError().databaseText()
		);
		db.close();
		mainStatusBar->showMessage(tr("Disconnected"));
		return;
	}
	// Set human-readable headers (optional)
	model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
	model->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
	model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
	model->setHeaderData(3, Qt::Horizontal, QObject::tr("Address 1"));
	model->setHeaderData(4, Qt::Horizontal, QObject::tr("Address 2"));
	model->setHeaderData(5, Qt::Horizontal, QObject::tr("City"));
	model->setHeaderData(6, Qt::Horizontal, QObject::tr("State"));
	model->setHeaderData(7, Qt::Horizontal, QObject::tr("Zip Code"));
	model->setHeaderData(8, Qt::Horizontal, QObject::tr("Phone #1"));
	model->setHeaderData(9, Qt::Horizontal, QObject::tr("Phone #2"));
	model->setHeaderData(10, Qt::Horizontal, QObject::tr("E-mail #1"));
	model->setHeaderData(11, Qt::Horizontal, QObject::tr("E-mail #2"));
	model->setHeaderData(12, Qt::Horizontal, QObject::tr("Birthday"));

	contactsView->setModel(model);
	contactsView->hideColumn(0);
	contactsView->resizeColumnsToContents();
	contactsView->horizontalHeader()->setStretchLastSection(true);
}

/*
 * Prepare the area of the main window that will display the contacts
 */
void MainWindow::setupTableView()
{
	// Create a QTableView and set the model
	contactsView = new QTableView();
	contactsView->horizontalHeader()->setStretchLastSection(true);

	// Hide the vertical header of the table
	contactsView->verticalHeader()->hide();

	if (db.isOpen())
	{
		this->refreshContactsView();
	}

#ifdef DEBUG

	std::cerr << "Made it here" << std::endl;
#endif
}

