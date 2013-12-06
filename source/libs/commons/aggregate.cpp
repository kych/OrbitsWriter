/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2012 devbean@galaxyworld.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *-------------------------------------------------*/

#include "aggregate.h"

#include <QWriteLocker>
#include <QDebug>

using namespace Commons::Aggregation;

Aggregate::Aggregate(QObject *parent) :
    QObject(parent)
{
    QWriteLocker locker(&lock());
    aggregateMap().insert(this, this);
}

Aggregate::~Aggregate()
{
    QWriteLocker locker(&lock());
    foreach (QObject *component, m_components) {
        disconnect(component, SIGNAL(destroyed(QObject*)), this, SLOT(deleteSelf(QObject*)));
        aggregateMap().remove(component);
    }
    qDeleteAll(m_components);
    m_components.clear();
    aggregateMap().remove(this);
}

void Aggregate::deleteSelf(QObject *obj)
{
    {
        QWriteLocker locker(&lock());
        aggregateMap().remove(obj);
        m_components.removeAll(obj);
    }
    delete this;
}

void Aggregate::add(QObject *component)
{
    if (!component) {
        return;
    }
    {
        QWriteLocker locker(&lock());
        Aggregate *parentAggregation = aggregateMap().value(component);
        if (parentAggregation == this) {
            return;
        }
        if (parentAggregation) {
            qWarning() << "Cannot add a component that belongs to a different aggregate" << component;
            return;
        }
        m_components.append(component);
        connect(component, SIGNAL(destroyed(QObject*)), this, SLOT(deleteSelf(QObject*)));
        aggregateMap().insert(component, this);
    }
    emit changed();
}

void Aggregate::remove(QObject *component)
{
    if (!component) {
        return;
    }
    {
        QWriteLocker locker(&lock());
        aggregateMap().remove(component);
        m_components.removeAll(component);
        disconnect(component, SIGNAL(destroyed(QObject*)), this, SLOT(deleteSelf(QObject*)));
    }
    emit changed();
}

Aggregate * Aggregate::parentAggregate(QObject *obj)
{
    QReadLocker locker(&lock());
    return aggregateMap().value(obj);
}

QHash<QObject *, Aggregate *> & Aggregate::aggregateMap()
{
    static QHash<QObject *, Aggregate *> map;
    return map;
}

QReadWriteLock & Aggregate::lock()
{
    static QReadWriteLock lock;
    return lock;
}
