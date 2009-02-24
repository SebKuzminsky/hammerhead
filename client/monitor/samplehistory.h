
// Copyright (c) 2008-2009, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#ifndef SAMPLEHISTORY_H
#define SAMPLEHISTORY_H

#include <QHash>
#include <QList>
#include <QString>
#include <QPointer>
#include <QAction>

#include <cstring>
#include <cstdio>
#include <iostream>
#include <ctime>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include "bionetmodel.h"

extern "C" {
#include "bionet.h"
#include "bionet-util.h"
};

class SampleHistory : public QObject {
    Q_OBJECT

    public:
        SampleHistory(int size, QObject* parent);
        void clear();
        double* doubleListToArray(QList<double> *listData);
        time_t* time_tListToArray(QList<time_t> *listData);
        int getSampleSize() { return sampleSize; }
        time_t* getTimes(QString key);
        double* getValues(QString key);
        int getSize(QString key);
        bool isEmpty(QString key);

    public slots:
        void addResource(QString id);
        void recordSample(bionet_datapoint_t* datapoint);
        void removeResource(QString id);
        void setSampleSize(int newSize);

    signals:
        void newSample(QString key);

    private:
        int sampleSize;
        struct sampleData {
            QList<time_t> *times;
            QList<double> *values;
        };
        QHash<QString, sampleData*> *samples;
};

#endif
