/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2013 devbean@galaxyworld.org
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

#ifndef AGGREGATE_H
#define AGGREGATE_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QReadWriteLock>

#include "commons/commons_global.h"

namespace Commons
{

namespace Aggregation
{

class COMMONS_EXPORT Aggregate : public QObject
{
    Q_OBJECT
public:
    Aggregate(QObject *parent = 0);
    virtual ~Aggregate();

    void add(QObject *component);
    void remove(QObject *component);

    template <typename T>
    T *component() const
    {
        QReadLocker(&lock());
        foreach (QObject *component, m_components) {
            if (T *result = qobject_cast<T *>(component)) {
                return result;
            }
        }
        return (T *)0;
    }

    template <typename T>
    QList<T *> components() const
    {
        QReadLocker(&lock());
        QList<T *> results;
        foreach (QObject *component, m_components) {
            if (T *result = qobject_cast<T *>(component)) {
                results << result;
            }
        }
        return results;
    }

    static Aggregate *parentAggregate(QObject *obj);

    static QReadWriteLock &lock();

signals:
    void changed();

private slots:
    void deleteSelf(QObject *obj);

private:
    //! Returns the aggregate map.
    static QHash<QObject *, Aggregate *> &aggregateMap();

    QList<QObject *> m_components;
}; // end of class Commons::Aggregate

template <typename T>
T * query(Aggregate *obj)
{
    if (!obj) {
        return (T *)0;
    }
    return obj->template component<T>();
}

template <typename T>
T * query(QObject *obj)
{
    if (!obj) {
        return (T *)0;
    }
    T *result = qobject_cast<T *>(obj);
    if (!result) {
        QReadLocker(&lock());
        Aggregate *parentAggregation = Aggregate::parentAggregate(obj);
        result = (parentAggregation ? query<T>(parentAggregation) : 0);
    }
    return result;
}

template <typename T>
QList<T *> query_all(Aggregate *obj)
{
    if (!obj) {
        return QList<T *>();
    }
    return obj->template components<T>();
}

template <typename T>
QList<T *> query_all(QObject *obj)
{
    if (!obj) {
        return QList<T *>();
    }
    QReadLocker(&lock());
    Aggregate *parentAggregation = Aggregate::parentAggregate(obj);
    QList<T *> results;
    if (parentAggregation) {
        results = query_all<T>(parentAggregation);
    } else if (T *result = qobject_cast<T *>(obj)) {
        results.append(result);
    }
    return results;
}

} // namespace Commons::Aggregation

} // namespace Commons

#endif // AGGREGATE_H
