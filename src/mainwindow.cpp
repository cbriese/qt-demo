#include <QApplication>
#include "mainwindow.h"
#include "logindialog.h"
#include "contactdialog.h"

// Constructor for the main window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	// Create status bar
	createStatusBar();

	// Create actions
	createActions();

	// Create toolbars
	createToolbars();

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

	resize(1920, 1080);
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
 * Create a dialog box to create or modify a contact.
 */
void MainWindow::createOrEditContact()
{
	ContactDialog *contactDialog = new ContactDialog(this);

	// To store the result of the dialog box
	int value;

	// Show the dialog and try to get the information from the user
	while (value = contactDialog->exec())
	{
		// The user clicked "Ok"
		if (value == 1)
		{
			// Check if the user provided a first name
			if (contactDialog->firstName().isEmpty())
			{
				// No? Try again.
				QMessageBox::warning(this, "Required Field Missing",
					"First Name is a required field"
				);
				continue;
			}

			// Check if the user provided a last name
			if (contactDialog->lastName().isEmpty())
			{
				// No? Try again.
				QMessageBox::warning(this, "Required Field Missing",
					"Last Name is a required field"
				);
				continue;
			}

			// Get a new empty record from the data model
			QSqlRecord rec = contactsModel->record();

			// Remove the 0th field since the driver tries to assign a NULL value
			// This shifts all other column numbers as a result.
			rec.remove(0);

			// Pretty blindly assign values to the fields from the form data
			rec.setValue(0, contactDialog->firstName());
			rec.setValue(1, contactDialog->lastName());
			rec.setValue(2, contactDialog->address1());
			rec.setValue(3, contactDialog->address2());
			rec.setValue(4, contactDialog->city());
			rec.setValue(5, contactDialog->state());
			rec.setValue(6, contactDialog->zipCode());
			if (contactDialog->phone1() != "--") {
				rec.setValue(7, contactDialog->phone1());
			}
			if (contactDialog->phone2() != "--") {
				rec.setValue(8, contactDialog->phone2());
			}
			rec.setValue(9, contactDialog->email1());
			rec.setValue(10, contactDialog->email2());
			if (contactDialog->saveBirthday()) {
				// Save the birthday
				rec.setValue(11, contactDialog->birthday());
			} else {
				rec.remove(11);
			}
			

			// Try to insert the new row
			bool record_inserted = contactsModel->insertRecord(-1, rec);

			if (record_inserted)
			{
				// Commit
				bool submitted = contactsModel->submitAll();

				if (!submitted) {
					QMessageBox::warning(
						this,
						contactsModel->lastError().driverText(),
						contactsModel->lastError().databaseText()
					);
					break;
				}
			}
			else
			{
				// Put a message box here!
				QMessageBox::warning(
					this,
					contactsModel->lastError().driverText(),
					contactsModel->lastError().databaseText()
				);
			}
		}
		else
		{
			// User clicked "Cancel"
		}

		break;
	}
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

	LoginDialog *loginDialog = new LoginDialog(this);

	while (!db.isOpen()) {
		int value = loginDialog->exec();
#ifdef DEBUG

		std::cerr << "The value returned from the dialog was " << value << std::endl;
#endif

		// User clicked "Ok"
		if (value == 1)
		{
			// How do i get the values from these form fields?
			if (loginDialog->hostname().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database hostname required")
				);
				continue;
			}

			if (loginDialog->databaseName().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database name required")
				);
				continue;
			}

			if (loginDialog->username().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database username required")
				);
				continue;
			}

			if (loginDialog->password().isEmpty()) {
				QMessageBox::warning(this, "Connection Status",
					tr("Database password required")
				);
				continue;
			}
 
			db.setHostName(loginDialog->hostname());
			db.setDatabaseName(loginDialog->databaseName());
			db.setUserName(loginDialog->username());
			db.setPassword(loginDialog->password());
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

// Method to create toolbar
void MainWindow::createToolbars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newContactAct);
	fileToolBar->addAction(deleteContactAct);
	fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
	fileToolBar->setFloatable(false);
	fileToolBar->setMovable(false);
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

	// Create "New Contact" action
	newContactAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew), tr("&New"), this);
	newContactAct->setShortcuts(QKeySequence::New);
	newContactAct->setStatusTip(tr("Create a new contact"));
	connect(newContactAct, &QAction::triggered, this, &MainWindow::createOrEditContact);

	// Create "Delete Contact" action
	deleteContactAct = new QAction(
		QIcon::fromTheme(QIcon::ThemeIcon::EditDelete),
		tr("&Delete"),
		this
	);
	deleteContactAct->setStatusTip(tr("Delete selected contact"));
	connect(deleteContactAct, &QAction::triggered, this, &MainWindow::deleteContact);
}

void MainWindow::deleteContact() {
	QItemSelectionModel *selectionModel = contactsView->selectionModel();

	QString firstName;
	QString lastName;
	QModelIndex index;

	if (selectionModel->currentIndex().isValid()) {
		// Get the highlighted row
		int highlightedRow = selectionModel->currentIndex().row();

		// Get the current index
		index = selectionModel->currentIndex();

		// A place to temporarily store data
		QVariant data;

		// Retrieve data in column #1
		data = index.sibling(highlightedRow, 1).data();

		// Save to first name
		firstName = data.toString();

		// Retrieve data in colume #2
		data = index.sibling(highlightedRow, 2).data();

		// Save to last name
		lastName = data.toString();
	}

	// Display the message box to confirm deletion and store which button was pressed
	int pressed = QMessageBox::question(this,
		"Are You Sure?", "Delete contact " + firstName + " " + lastName + "?");


	// If the "Yes" button was pressed...
	if (pressed == QMessageBox::Yes) {
		// Delete the row from the data model
		bool row_deleted = contactsModel->removeRow(index.row(), index.parent());

		if (row_deleted) {
			// Presist the change
			bool committed = contactsModel->submitAll();

			// Display a message box if something went wrong
			if (!committed) {
				QMessageBox *box;
				QMessageBox::warning(this, contactsModel->lastError().driverText(),
					contactsModel->lastError().databaseText()
				);
			}
		} else {
			// Display a message box if something went wrong
			QMessageBox::warning(this, contactsModel->lastError().driverText(),
				contactsModel->lastError().databaseText()
			);
		}
	}
}
	

void MainWindow::disconnectDb()
{
	if (db.isOpen()) {
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

void MainWindow::notImplemented() {
	QMessageBox::warning(this, "Oops!", "This feature is not implemented");
}

void MainWindow::refreshContactsView()
{
	if (contactsModel != nullptr)
	{
		contactsModel = new QSqlTableModel();
		contactsModel->setTable("contacts");
		contactsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
		contactsModel->select();

		if (contactsModel->lastError().isValid())
		{
			QMessageBox::warning(this, contactsModel->lastError().driverText(),
				contactsModel->lastError().databaseText()
			);
			db.close();
			mainStatusBar->showMessage(tr("Disconnected"));
			return;
		}
		// Set human-readable headers (optional)
		contactsModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
		contactsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
		contactsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
		contactsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Address 1"));
		contactsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Address 2"));
		contactsModel->setHeaderData(5, Qt::Horizontal, QObject::tr("City"));
		contactsModel->setHeaderData(6, Qt::Horizontal, QObject::tr("State"));
		contactsModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Zip Code"));
		contactsModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Phone #1"));
		contactsModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Phone #2"));
		contactsModel->setHeaderData(10, Qt::Horizontal, QObject::tr("E-mail #1"));
		contactsModel->setHeaderData(11, Qt::Horizontal, QObject::tr("E-mail #2"));
		contactsModel->setHeaderData(12, Qt::Horizontal, QObject::tr("Birthday"));

		contactsView->setModel(contactsModel);
		contactsView->hideColumn(0);
		contactsView->setColumnWidth(1, 150);
		contactsView->setColumnWidth(2, 150);
		contactsView->setColumnWidth(3, 200);
		contactsView->setColumnWidth(4, 200);
		contactsView->setColumnWidth(5, 150);
		contactsView->setColumnWidth(6, 50);
		contactsView->setColumnWidth(7, 75);
		contactsView->setColumnWidth(8, 150);
		contactsView->setColumnWidth(9, 150);
		contactsView->setColumnWidth(10, 200);
		contactsView->setColumnWidth(11, 200);
		contactsView->setColumnWidth(12, 150);

		contactsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		contactsView->setSelectionBehavior(QAbstractItemView::SelectRows);
		contactsView->setSelectionMode(QAbstractItemView::SingleSelection);
		// contactsView->horizontalHeader()->setStretchLastSection(true);
		// contactsView->resizeColumnsToContents();
	}
}

/*
 * Prepare the area of the main window that will display the contacts
 */
void MainWindow::setupTableView()
{
	// Create a QTableView and set the model
	contactsView = new QTableView();
	// contactsView->horizontalHeader()->setStretchLastSection(true);

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

