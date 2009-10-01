
// Copyright (c) 2008-2009, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include "mainwindow.h"


MainWindow::MainWindow(char* argv[], QWidget *parent) : QWidget(parent) {
    int sampleSize = -1;
    int require_security = 0;
    QString security_dir;

    setAttribute(Qt::WA_QuitOnClose);
    argv ++;
    setWindowTitle(QString("BioNet Monitor"));

    defaultPreferencesIsOpen = false;

    // 
    // Parsing the Command Line Args
    //

    for ( ; *argv != NULL; argv ++) {
        if (strcmp(*argv, "--sample-size") == 0) {
            argv ++;
            sampleSize = atoi(*argv);
        } else if ((strcmp(*argv, "-s") == 0) || (strcmp(*argv, "--security-dir") == 0)) {
            argv++;
            security_dir = QString(*argv);
        } else if ((strcmp(*argv, "-e") == 0) || (strcmp(*argv, "--require-security") == 0)) {
            require_security = 1;
        } else {
            usage();
            exit(1);
        }
    }

    if ((require_security == 1) && ( security_dir.isEmpty() )) {
        qWarning("Security required but no security directory specified, continuing without security");
    }

    if ( !security_dir.isEmpty() ) {
        if (bionet_init_security(qPrintable(security_dir), require_security)) {
            qWarning("Failed to initialize security.");
        }
    }
    
    if (sampleSize < 0)
        sampleSize = 10000;
    

    setupBionetIO();
    setupBionetModel();
    setupBDM();
    setupTreeView();
    setupResourceView();
    setupArchive();
    
    menuBar = new QMenuBar(this);
    createActions();
    createMenus();

    setupWindow();

    bionet->setup();
    bdmio->setup();

    connect(view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
        liveModel, SLOT(lineActivated(QModelIndex)));
    connect(liveModel, SIGNAL(resourceSelected(bionet_resource_t*)), 
        resourceView, SLOT(newResourceSelected(bionet_resource_t*)));
    connect(liveModel, SIGNAL(habSelected(bionet_hab_t*)), 
        resourceView, SLOT(clearView()));
    connect(liveModel, SIGNAL(nodeSelected(bionet_node_t*)), 
        resourceView, SLOT(clearView()));
    connect(liveModel, SIGNAL(streamSelected(bionet_stream_t*)), 
        resourceView, SLOT(newStreamSelected(bionet_stream_t*)));
    connect(bionet, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
        this, SLOT(updatePlot(bionet_datapoint_t*)));

    scaleInfoTemplate = new ScaleInfo;
}


void MainWindow::setupBionetModel() {
    liveModel = new BionetModel(this);

    liveModel->setColumnCount(5);
    liveModel->setRowCount(0);

    liveModel->setHeaderData(0, Qt::Horizontal, QString("Habs.Nodes.Resources"));
    liveModel->setHeaderData(1, Qt::Horizontal, QString("Flavor"));
    liveModel->setHeaderData(2, Qt::Horizontal, QString("Type"));
    liveModel->setHeaderData(3, Qt::Horizontal, QString("Timestamp"));
    liveModel->setHeaderData(4, Qt::Horizontal, QString("Value"));

    // Connects from Bionet to the liveModel
    connect(bionet, SIGNAL(newHab(bionet_hab_t*)), 
        liveModel, SLOT(newHab(bionet_hab_t*)));
    connect(bionet, SIGNAL(lostHab(bionet_hab_t*)), 
        liveModel, SLOT(lostHab(bionet_hab_t*)));
    connect(bionet, SIGNAL(newNode(bionet_node_t*)), 
        liveModel, SLOT(newNode(bionet_node_t*)));
    connect(bionet, SIGNAL(lostNode(bionet_node_t*)), 
        liveModel, SLOT(lostNode(bionet_node_t*)));
    connect(bionet, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
        liveModel, SLOT(newDatapoint(bionet_datapoint_t*)));
    connect(liveModel, SIGNAL(lostResource(QString)), 
        this, SLOT(lostPlot(QString)));
    return;
}


void MainWindow::setupBDM() {
    bdmModel = new BDMModel(this);
    bdmModel->setColumnCount(5);
    bdmModel->setRowCount(0);

    bdmModel->setHeaderData(0, Qt::Horizontal, QString("Habs.Nodes.Resources"));
    bdmModel->setHeaderData(1, Qt::Horizontal, QString("Flavor"));
    bdmModel->setHeaderData(2, Qt::Horizontal, QString("Type"));
    bdmModel->setHeaderData(3, Qt::Horizontal, QString("Timestamp"));
    bdmModel->setHeaderData(4, Qt::Horizontal, QString("Value"));

    bdmView = new Tree;

    bdmView->setAlternatingRowColors(TRUE);
    bdmView->setDragEnabled(FALSE);
    bdmView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(bdmModel, SIGNAL(layoutChanged()), 
        bdmView, SLOT(repaint()));

    bdmView->setModel(bdmModel);
    bdmView->selectAll();

    // Edit/adjust the header
    QHeaderView* header;
    header = bdmView->header();
    header->setMovable(FALSE);
    header->resizeSection(0,200); // resizing the columns
    header->resizeSection(1,75); 
    header->resizeSection(2,50); 
    bdmView->setHeader(header);

    // Edit the size of the bdmView frame
    bdmView->setMinimumHeight(100);
    bdmView->setMinimumWidth(100);

    // create the bdmio
    bdmio = new BDMIO();

    // connect bdm interface with the model
    connect(bdmio, SIGNAL(newHab(bionet_hab_t*)), 
        bdmModel, SLOT(newHab(bionet_hab_t*)));
    connect(bdmio, SIGNAL(lostHab(bionet_hab_t*)), 
        bdmModel, SLOT(lostHab(bionet_hab_t*)));
    connect(bdmio, SIGNAL(newNode(bionet_node_t*)), 
        bdmModel, SLOT(newNode(bionet_node_t*)));
    connect(bdmio, SIGNAL(lostNode(bionet_node_t*)), 
        bdmModel, SLOT(lostNode(bionet_node_t*)));
    connect(bdmio, SIGNAL(newResource(bionet_resource_t*)),
        bdmModel, SLOT(addResource(bionet_resource_t*)));
    connect(bdmio, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
        bdmModel, SLOT(newDatapoint(bionet_datapoint_t*)));

    connect(bdmio, SIGNAL(enableTab(bool)),
        this, SLOT(enableTab(bool)));
}


void MainWindow::setupTreeView() {
    view = new Tree(this);

    view->setAlternatingRowColors(TRUE);
    view->setDragEnabled(FALSE);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    connect(liveModel, SIGNAL(layoutChanged()), 
        view, SLOT(repaint()));
    
    view->setModel(liveModel);
    view->selectAll();
    
    // Edit/adjust the header
    QHeaderView* header;
    header = view->header();
    header->setMovable(FALSE);
    header->resizeSection(0,200); // resizing the columns
    header->resizeSection(1,75); 
    header->resizeSection(2,50); 
    view->setHeader(header);
    
    // Edit the size of the view frame
    view->setMinimumHeight(100);
    view->setMinimumWidth(100);

    //view->show();
    return;
}


void MainWindow::switchViews(int index) {
    // When switching between views, We don't want to:
    //  * disconnect from bionet, since when we switch back we still want to be updated
    //  * disconnect recording the datapoint signals
    //  * Don't disconnect updating plots
    // But we do want to:
    //  * Switch the resource view to the active model

    if (index == 0) { // bionet live
        resourceView->clearView();

        // disconnect selecting habs/nodes/resources/streams to the resourceview
        disconnect(bdmModel, SIGNAL(resourceSelected(bionet_resource_t*)), 
            resourceView, SLOT(newResourceSelected(bionet_resource_t*)));
        disconnect(bdmModel, SIGNAL(habSelected(bionet_hab_t*)), 
            resourceView, SLOT(clearView()));
        disconnect(bdmModel, SIGNAL(nodeSelected(bionet_node_t*)), 
            resourceView, SLOT(clearView()));
        disconnect(bdmModel, SIGNAL(streamSelected(bionet_stream_t*)), 
            resourceView, SLOT(newStreamSelected(bionet_stream_t*)));
        
        // disconnect the row highlighting action
        disconnect(bdmView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
            bdmModel, SLOT(lineActivated(QModelIndex)));

        // connecting the hab/node/datapoint updates
        connect(bionet, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
            resourceView, SLOT(resourceValueChanged(bionet_datapoint_t*)));
        connect(bionet, SIGNAL(lostHab(bionet_hab_t*)), 
            resourceView, SLOT(lostHab(bionet_hab_t*)));
        connect(bionet, SIGNAL(lostNode(bionet_node_t*)), 
            resourceView, SLOT(lostNode(bionet_node_t*)));

        // reconnect the row highlighting action
        connect(view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
            liveModel, SLOT(lineActivated(QModelIndex)));

        // disconnect the plot button from make bdm plot to make plot
        disconnect(resourceView, SIGNAL(plotResource(QString)), 
            this, SLOT(makeBDMPlot(QString)));
        connect(resourceView, SIGNAL(plotResource(QString)), 
            this, SLOT(makePlot(QString)));

    } else if (index == 1) { // bdm
        resourceView->clearView();

        // Disconnecting the hab/node/datapoint updates
        disconnect(bionet, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
            resourceView, SLOT(resourceValueChanged(bionet_datapoint_t*)));
        disconnect(bionet, SIGNAL(lostHab(bionet_hab_t*)), 
            resourceView, SLOT(lostHab(bionet_hab_t*)));
        disconnect(bionet, SIGNAL(lostNode(bionet_node_t*)), 
            resourceView, SLOT(lostNode(bionet_node_t*)));

        // disconnect the row highlighting action
        disconnect(view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
            liveModel, SLOT(lineActivated(QModelIndex)));

        // connect selecting habs/nodes/resources/streams to the resourceview
        connect(bdmModel, SIGNAL(resourceSelected(bionet_resource_t*)), 
            resourceView, SLOT(newResourceSelected(bionet_resource_t*)));
        connect(bdmModel, SIGNAL(habSelected(bionet_hab_t*)), 
            resourceView, SLOT(clearView()));
        connect(bdmModel, SIGNAL(nodeSelected(bionet_node_t*)), 
            resourceView, SLOT(clearView()));
        connect(bdmModel, SIGNAL(streamSelected(bionet_stream_t*)), 
            resourceView, SLOT(newStreamSelected(bionet_stream_t*)));

        // connect the row highlighting action
        connect(bdmView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
            bdmModel, SLOT(lineActivated(QModelIndex)));

        // disconnect the plot button from make plot to make bdm plot
        disconnect(resourceView, SIGNAL(plotResource(QString)), 
            this, SLOT(makePlot(QString)));
        connect(resourceView, SIGNAL(plotResource(QString)), 
            this, SLOT(makeBDMPlot(QString)));
    }
}


void MainWindow::enableTab(bool enable) {
    tabs->setTabEnabled(1, enable);
}


void MainWindow::setupResourceView() {
    resourceView = new ResourceView();

    // (for losing habs & updating the pane)
    connect(bionet, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
        resourceView, SLOT(resourceValueChanged(bionet_datapoint_t*)));
    connect(bionet, SIGNAL(lostHab(bionet_hab_t*)), 
        resourceView, SLOT(lostHab(bionet_hab_t*)));
    connect(bionet, SIGNAL(lostNode(bionet_node_t*)), 
        resourceView, SLOT(lostNode(bionet_node_t*)));

    // connecting the plot button
    connect(resourceView, SIGNAL(plotResource(QString)), 
        this, SLOT(makePlot(QString)));

    return;
}


void MainWindow::setupArchive() {
    archive = new Archive(this);

    // Connecting everything to the sample archive
    connect(liveModel, SIGNAL(newResource(QString)), 
        archive, SLOT(addResource(QString)));
    connect(liveModel, SIGNAL(lostResource(QString)), 
        archive, SLOT(removeResource(QString)));
    connect(bionet, SIGNAL(newDatapoint(bionet_datapoint_t*)), 
        archive, SLOT(recordSample(bionet_datapoint_t*)));

    return;
}


void MainWindow::setupBionetIO() {
    bionet = new BionetIO(this);

    bionet->addHabSubscription("*.*");
    bionet->addNodeSubscription("*.*.*");
    bionet->addResourceSubscription("*.*.*:*");

    return;
}


void MainWindow::setupWindow() {
    tabs = new QTabWidget(this);
    tabs->addTab(view, "Live");
    tabs->addTab(bdmView, "History");

    connect(tabs, SIGNAL(currentChanged(int)), 
        this, SLOT(switchViews(int)));

    //view->setParent(views);

    resViewHolder = new QWidget(this);
    resViewHolder->setLayout(resourceView);

    splitter = new QSplitter(this);
    splitter->addWidget(tabs);
    splitter->addWidget(resViewHolder);


    layout = new QHBoxLayout(this);
    layout->setMenuBar(menuBar);
    layout->addWidget(splitter);
}


void MainWindow::closeEvent(QCloseEvent* event) {

    archive->disconnect();
    resourceView->disconnect();
    view->disconnect();
    liveModel->disconnect();
    bionet->disconnect();

    foreach(PlotWindow* p, plots) {
        delete p;
    }
    
    delete archive;

    event->accept();
}


void MainWindow::changeHostname() {
    bool ok;
    
    QString name = QInputDialog::getText(this, "BioNet Monitor", 
            "Input new NAG Hostname:", QLineEdit::Normal, 
            QString(), &ok);
    if ( ok && !name.isEmpty() ) {
        bionet->setup();
    }
}


void MainWindow::createActions() {
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    plotAction = new QAction(tr("&Plot"), this);
    plotAction->setShortcut(tr("Ctrl+P"));
    connect(plotAction, SIGNAL(triggered()), resourceView, SLOT(plotClicked()));
    
    //hostnameAction = new QAction(tr("Change Nag Hostname"), this);
    //connect(hostnameAction, SIGNAL(triggered()), this, SLOT(changeHostname()));

    /*
    filterAction = new QAction(tr("Filter"), this);
    filterAction->setShortcut(tr("Ctrl+F"));
    connect(filterAction, SIGNAL(triggered()), this, SLOT(filterDialog()));
    */

    aboutAction = new QAction(tr("&How-To"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    shortcuts = new QAction(tr("&Shortcuts"), this);
    connect(shortcuts, SIGNAL(triggered()), this, SLOT(cuts()));

    preferencesAction = new QAction(tr("&All/Default Plot Preferences"), this);
    connect(preferencesAction, SIGNAL(triggered()), this, SLOT(openDefaultPlotPreferences()));

    updateSubscriptionsAction = new QAction(tr("&Change Subscriptions"), this);
    updateSubscriptionsAction->setShortcut(tr("Ctrl+C"));
    connect(updateSubscriptionsAction, SIGNAL(triggered()), bdmio, SLOT(editSubscriptions()));
    
    pollingFrequencyAction = new QAction(tr("BDM Polling &Frequency"), this);
    pollingFrequencyAction->setShortcut(tr("Ctrl+F"));
    connect(pollingFrequencyAction, SIGNAL(triggered()), bdmio, SLOT(changeFrequency()));

    connectToBDMAction = new QAction(tr("Connect to &BDM"), this);
    connectToBDMAction->setShortcut(tr("Ctrl+B"));
    connect(connectToBDMAction, SIGNAL(triggered()), bdmio, SLOT(promptForConnection()));

    disconnectFromBDMAction = new QAction(tr("&Disconnect from BDM"), this);
    disconnectFromBDMAction->setShortcut(tr("Ctrl+D"));
    connect(disconnectFromBDMAction, SIGNAL(triggered()), bdmio, SLOT(disconnectFromBDM()));
}



void MainWindow::createMenus() {

    fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(plotAction);
    fileMenu->addAction(preferencesAction);
    fileMenu->addAction(connectToBDMAction);
    fileMenu->addAction(disconnectFromBDMAction);
    fileMenu->addAction(updateSubscriptionsAction);
    fileMenu->addAction(pollingFrequencyAction);
    //fileMenu->addAction(sampleAction);
    //fileMenu->addAction(hostnameAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    //actionMenu = menuBar->addMenu(tr("&Actions"));

    helpMenu = menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(shortcuts);

    connect(fileMenu, SIGNAL(aboutToShow()), this, SLOT(updateMenus()));
} 


void MainWindow::updateMenus() {
    bionet_resource_t* resource;

    resource = resourceView->resourceInView();
    if (resource == NULL)
        plotAction->setEnabled(false);
    else if (bionet_resource_get_data_type(resource) == BIONET_RESOURCE_DATA_TYPE_STRING)
        plotAction->setEnabled(false);
    else
        plotAction->setEnabled(true);
}


void MainWindow::usage(void) {
    cout << "usage: bionet-monitor OPTIONS...\n\
\n\
OPTIONS:\n\
\n\
    -e, --require-security\n\
        Require security\n\
\n\
    -s <DIR>, --security-dir <DIR>\n\
        Set the directory containing the security certificates\n\
\n\
    --sample-size\n\
        Set the number of samples to store for plotting (default: 10,000)\n\
\n\
    --help\n\
        Prints this help.\n\
\n\
\n" << endl;
}


void MainWindow::about() {
    QMessageBox::about(this, tr("BioNet Monitor How-To"), 
        "The BioNet-Monitor program is a graphical \n\
user interface client for viewing and \n\
commanding BioNet sensors, parameters, \n\
and actuators.\n\
\n\
For command line options run \n\
    bionet-monitor --help\n\
\n\
Use the keyboard, mouse, or arrow keys to\n\
nagivate the Node Hierarchy on the left.\n\
\n\
To plot resources, select a resource and\n\
click on either plot button.  Only\n\
resources can be plotted.\n\
\n\
Parameter and actuator values can be\n\
changed using the \"Update Value\" \n\
field on the right.");
}


void MainWindow::cuts() {
    QMessageBox::about(this, tr("BioNet Monitor Shortcuts"), "Space \t Plot (when \"Plot\" button is active)\n\
Tab   \t Alternate Panes\n\
Ctrl-P\t Plot active resource\n\
Ctrl-W\t Close active window\n\
Ctrl-Q\t Quit"
    );
}


void MainWindow::makePlot(QString key) {
    
    if (( !archive->contains(key) ) || ( archive->history(key)->size() == 0 ))
        return;

    if ( !plots.contains(key) ) {
        PlotWindow* p = new PlotWindow(key, archive->history(key), 
                scaleInfoTemplate, 
                this);
        connect(p, SIGNAL(newPreferences(PlotWindow*, ScaleInfo*)), this, SLOT(openPrefs(PlotWindow*, ScaleInfo*)));
        plots.insert(key, p);
        connect(p, SIGNAL(destroyed(QObject*)), this, SLOT(destroyPlot(QObject*)));

        /* if default preferences exists, add the plot to the plots it updates */
        if (defaultPreferencesIsOpen)
            defaultPreferences->addPlot(p);
    }
}


void MainWindow::makeBDMPlot(QString key) {
    History *history;
    PlotWindow *p;
    QString finalKey;

    // Get the info from the BDM
    history = bdmio->createHistory(key);

    if ( history->isEmpty() ) {
        qDebug() << "empty history for resource" << qPrintable(key) << "?";
        return;
    }

    // Create the plot & insert it into the plots list
    p = new PlotWindow(key, history, scaleInfoTemplate, this);

    connect(p, SIGNAL(newPreferences(PlotWindow*, ScaleInfo*)), 
        this, SLOT(openPrefs(PlotWindow*, ScaleInfo*)));
    connect(p, SIGNAL(destroyed(QObject*)), 
        this, SLOT(destroyPlot(QObject*)));
    
    // FIXME: overlap with a 'live' bionet existing resource results in ?
    plots.insert(key, p);
}


void MainWindow::updatePlot(bionet_datapoint_t* datapoint) {
    bionet_resource_t* resource;
    const char *resource_name;

    if (datapoint == NULL)
        return;

    resource = bionet_datapoint_get_resource(datapoint);

    resource_name = bionet_resource_get_name(resource);
    if (resource_name == NULL) {
        cout << "updatePlot(): unable to get resource name" << endl;
        return;
    }

    QString key = QString(resource_name);
    PlotWindow* p = plots.value(key);

    if ( p != NULL ) {
        p->updatePlot();
    }
}


void MainWindow::lostPlot(QString key) {
    PlotWindow* p = plots.value(key);

    if ( p != NULL ) {
        delete p;
    }
}


void MainWindow::destroyPlot(QObject* obj) {
    QString key = obj->objectName();
    plots.take(key); // its already going to be deleted so dont worry about it
}


void MainWindow::openDefaultPlotPreferences() {
    if (!defaultPreferencesIsOpen) {
        defaultPreferencesIsOpen = true;
        QList<PlotWindow*> windows = plots.values();
        defaultPreferences = new PlotPreferences(windows, scaleInfoTemplate, QString("All"), this);

        connect(defaultPreferences, SIGNAL(applyChanges(ScaleInfo*)), this, SLOT(updateScaleInfo(ScaleInfo*)));
        connect(defaultPreferences, SIGNAL(destroyed(QObject*)), this, SLOT(closedDefaultPlotPreferences()));

        defaultPreferences->show();
    } else {
        /* Don't open twice! raise it instead */
        defaultPreferences->raise();
    }
}


void MainWindow::closedDefaultPlotPreferences() {
    defaultPreferencesIsOpen = false;
}


void MainWindow::updateScaleInfo(ScaleInfo *si) {
    if (scaleInfoTemplate != NULL)
        delete scaleInfoTemplate;
    scaleInfoTemplate = si->copy();
}


void MainWindow::openPrefs(PlotWindow *pw, ScaleInfo *current) {
    PlotPreferences *pp;

    if (pw == NULL) {
        return;
    }

    QList<PlotWindow*> window;
    QList<QString> keys;
    QString key;

    keys = plots.keys(pw);
    if (keys.isEmpty()) {
        qDebug("Tried to open preferences for a non-existant plot window");
        return;
    }
    key = keys.first();

    if (preferences.contains(key)) {
        /* Don't open twice! raise it instead */
        pp = preferences[key];
        pp->raise();
        return;
    }

    window.append(pw);

    pp = new PlotPreferences(window, current, key, pw);
    pp->setObjectName(key);
    pp->show();
    preferences.insert(key, pp);

    connect(pp, SIGNAL(destroyed(QObject*)), 
            this, SLOT(closedPreferences(QObject*)));
}


void MainWindow::closedPreferences(QObject* obj) {
    QString key = obj->objectName();
    preferences.take(key);
    // PlotPreferences are already being deleted, we just need to remove them from
    // the hash manually
}


Tree::Tree(QWidget *parent) : QTreeView(parent) {
    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(expand(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(collapse(const QModelIndex &)));
};


void Tree::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        QModelIndex current = selectionModel()->currentIndex();
        setExpanded(current, !isExpanded(current));
    }

    QTreeView::keyPressEvent(event);
}


/*
void Tree::collapse(const QModelIndex &index) {
    QRegExp nodeRX(".*\\..*\\..*");     // matches *.*.*
    BionetModel *bm = (BionetModel*)model();

    // is it a hab?
    if (index.parent() == QModelIndex()) {
        QString unsub = bm->name(index) + ".*";
        bionet_unsubscribe_node_list_by_name(qPrintable(unsub));
    }

    // match nodes, not resources/streams
    if ( nodeRX.exactMatch(bm->name(index)) && !bm->name(index).contains(':') ) {
        QString unsub = bm->name(index) + ":*";
        bionet_unsubscribe_datapoints_by_name(qPrintable(unsub));
    }
}


void Tree::expand(const QModelIndex &index) {
    QRegExp nodeRX(".*\\..*\\..*");     // matches *.*.*
    BionetModel *bm = (BionetModel*)model();

    // hab?
    if (index.parent() == QModelIndex()) {
        QString sub = bm->name(index) + QString(".*");
        bionet_subscribe_node_list_by_name(qPrintable(sub));
    }

    // match only nodes, not resources/streams
    if ( nodeRX.exactMatch(bm->name(index)) && !bm->name(index).contains(':') ) {
        QString unsub = bm->name(index) + ":*";
        bionet_subscribe_datapoints_by_name(qPrintable(unsub));
    }
}
*/
