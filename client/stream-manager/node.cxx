
//
// Copyright (C) 2004-2008, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G and NNC06CB40C.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of GNU General Public License version 2, as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
//


#include "node.h"


Node::Node(QString name, QWidget* parent) : 
    QWidget(parent)
{
    this->name = name;

    QRegExp streamRX(".*\\..*\\..*\\:.*"); // matches *.*.*:*
    QRegExp nodeRX(".*\\..*\\..*");     // matches *.*.*
    QRegExp habRX(".*\\..*");           // matches *.*

    if ( streamRX.exactMatch(name) ) {
        id = name.section('.', 2, 2).section(':', 1, 1);
        bionetType = STREAM;

        bionet_stream_t* stream = bionet_cache_lookup_stream(
            qPrintable(name.section('.', 0, 0)),
            qPrintable(name.section('.', 1, 1)),
            qPrintable(name.section('.', 2, 2).section(':', 0, 0)),
            qPrintable(id));

        if (stream->direction == BIONET_STREAM_DIRECTION_CONSUMER)
            endpointType = CONSUMER;
        else if (stream->direction == BIONET_STREAM_DIRECTION_PRODUCER)
            endpointType = PRODUCER;
        else 
            endpointType = NEITHER;

        bionetPtr = (void*)stream;

    } else if ( nodeRX.exactMatch(name) ) {

        bionet_node_t* node = bionet_cache_lookup_node(
            qPrintable(name.section('.', 0, 0)),
            qPrintable(name.section('.', 1, 1)),
            qPrintable(name.section('.', 2, 2)));
        bionetPtr = (void*)node;

        id = name.section('.',2,2);
        bionetType = NODE;
        endpointType = NEITHER;

    } else if ( habRX.exactMatch(name) ) {

        bionet_hab_t* hab = bionet_cache_lookup_hab(
            qPrintable(name.section('.', 0, 0)),
            qPrintable(name.section('.', 1, 1)));
        bionetPtr = (void*)hab;

        id = name;
        bionetType = HAB;
        endpointType = NEITHER;

    } else if ( name.isEmpty() ) {
        id = QString();
        bionetType = ROOT;
        endpointType = NEITHER;
    }

    children = new QList<Node*>;

    /*
    path = new QPainterPath(QPoint(0,0));
    path->addEllipse(area);
    path->addText(area, Qt::AlignHCenter, Qt::AlignVCenter, id);
    */
}


Node::~Node() {
    while ( !children->isEmpty() ) {
        Node* child = children->takeFirst();
        delete child;
    }
    delete children;
}



void Node::addChild(Node* child) {
    children->append(child);
}


void Node::addChildWithoutArea(Node* child) {
    children->append(child);
}


bool Node::removeChild(QString id) {
    int size = children->size();

    for (int i = 0; i < size; i++ ) {
        Node* child = children->at(i);
        if (child->getId() == id) {
            Node* child = children->takeAt(i);
            delete child;
            return true;
        }
    }

    return false;
}


Node* Node::find(QString fullname) {
    if (this->name == fullname)
        return this;

    foreach (Node* node, *children) {
        Node* leaf = node->find(fullname);
        if (leaf != NULL)
            return leaf;
    }

    return NULL;
}


Node* Node::find(QPoint point) {

    if (area.contains(point))
        return this;

    foreach (Node* node, *children) {
        Node *leaf = node->find(point);
        if (leaf != NULL)
            return leaf;
    }

    return NULL;
}


Node* Node::find(QRect rect) {

    if (area.intersects(rect))
        return this;

    foreach (Node* node, *children) {
        Node *leaf = node->find(rect);
        if (leaf != NULL)
            return leaf;
    }

    return NULL;
}


void Node::setArea(QRect newArea) {
    this->area = newArea;
    //cout << "this->area = " << area.x() << ", " << area.y() << endl;
}