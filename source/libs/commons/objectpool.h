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

#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <QObject>
#include <QReadWriteLock>

#include "commons/aggregate.h"
#include "commons/commons_export.h"
#include "commons/singleton.h"

#define gObjectPool (Commons::ObjectPool::instance())

namespace Commons
{

namespace Internal
{
class ObjectPoolPrivate;
}

class COMMONS_EXPORT ObjectPool : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(ObjectPool)
public:
    void addObject(QObject *obj);
    void removeObject(QObject *obj);

    QList<QObject *> objectsInPool() const;

    QObject *getObjectByName(const QString &name) const;
    QObject *getObjectByClassName(const QString &className) const;

    template <typename T>
    QList<T *> getObjects() const
    {
        QReadLocker lock(&m_lock);
        QList<T *> results;
        QList<QObject *> all = objectsInPool();
        QList<T *> result;
        foreach (QObject *obj, all) {
            result = Aggregation::query_all<T>(obj);
            if (!result.isEmpty()) {
                results += result;
            }
        }
        return results;
    }

    template <typename T>
    T *getObject() const
    {
        QReadLocker lock(&m_lock);
        QList<QObject *> all = objectsInPool();
        T *result = 0;
        foreach (QObject *obj, all) {
            if ((result = Aggregation::query<T>(obj)) != 0) {
                break;
            }
        }
        return result;
    }

signals:
    void objectAdded(QObject * obj);
    void aboutToRemoveObject(QObject * obj);

private:
    explicit ObjectPool(QObject *parent = 0);
    ~ObjectPool();

    mutable QReadWriteLock m_lock;

    Internal::ObjectPoolPrivate *d;
    friend class Internal::ObjectPoolPrivate;
}; // end of class Commons::ObjectPool

} // end of namespace Commons

#endif // OBJECTPOOL_H
