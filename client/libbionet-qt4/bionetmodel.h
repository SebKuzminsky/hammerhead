
// Copyright (c) 2008-2009, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#ifndef BIONETMODEL_H
#define BIONETMODEL_H

#include <QIcon>
#include <QModelIndex>
#include <QRegExp>
#include <QResource>
#include <QStandardItemModel>
#include <QString>
#include <QtDebug>

#include <iostream>

#define FULLNAMEROLE            (Qt::UserRole)
#define HABNAMELENGTH           (2*BIONET_NAME_COMPONENT_MAX_LEN)
#define NODENAMELENGTH          (3*BIONET_NAME_COMPONENT_MAX_LEN)
#define RESOURCENAMELENGTH      (4*BIONET_NAME_COMPONENT_MAX_LEN)

extern "C" {
#include "bionet.h"
#include "bionet-util.h"
};

using namespace std;

class BionetModel : public QStandardItemModel {
    Q_OBJECT

    public:
        BionetModel(QObject* parent=0);

        QString getName(const QModelIndex &index) const;
        QString getID(const QModelIndex &index) const;

    public slots:
        void newHab(bionet_hab_t* hab);
        void lostHab(bionet_hab_t* hab);
        void newNode(bionet_node_t* node);
        void lostNode(bionet_node_t* node);
        void newDatapoint(bionet_datapoint_t* datapoint);
        void lineActivated(QModelIndex current);

    signals:
        void resourceChanged(bionet_resource_t* resource);
        void habSelected(bionet_hab_t* hab);
        void nodeSelected(bionet_node_t* node);
        void resourceSelected(bionet_resource_t* resource);
        void streamSelected(bionet_stream_t* stream);
        void newResource(QString res);
        void lostResource(QString res);
        void newStream(QString stream);
        void lostStream(QString stream);

    private:
};

#endif
