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

#include <QDebug>
#include <QMutex>

#include "objectpool.h"

namespace Commons
{

namespace Internal
{
class ObjectPoolPrivate
{
public:
    explicit ObjectPoolPrivate(ObjectPool *pool) : q(pool) {}

    void addObject(QObject * obj);
    void removeObject(QObject * obj);
    QList<QObject *> objectsPool;

    ObjectPool *q;
}; // end of class Commons::Internal::ObjectPoolPrivate

void ObjectPoolPrivate::addObject(QObject *obj)
{
    {
        QWriteLocker lock(&(q->m_lock));
        if (!obj) {
            qWarning() << "PluginManagerPrivate::addObject(): trying to add null object";
            return;
        }
        if (objectsPool.contains(obj)) {
            qWarning() << "PluginManagerPrivate::addObject(): trying to add duplicate object";
            return;
        }
        objectsPool.append(obj);
    }
    emit q->objectAdded(obj);
}

void ObjectPoolPrivate::removeObject(QObject *obj)
{
    if (!obj) {
        qWarning() << "PluginManagerPrivate::removeObject(): trying to remove null object";
        return;
    }

    if (!objectsPool.contains(obj)) {
        qWarning() << "PluginManagerPrivate::removeObject(): object not in list:"
                   << obj << obj->objectName();
        return;
    }

    emit q->aboutToRemoveObject(obj);
    QWriteLocker lock(&(q->m_lock));
    objectsPool.removeAll(obj);
}

} // end of namespace Commons::Internal

// ========== Commons::ObjectPool ========== //

GET_INSTANCE(ObjectPool)

ObjectPool::ObjectPool(QObject *parent) :
    QObject(parent),
    d(new Internal::ObjectPoolPrivate(this))
{
}

ObjectPool::~ObjectPool()
{
    delete d;
}

void ObjectPool::addObject(QObject *obj)
{
    d->addObject(obj);
}

void ObjectPool::removeObject(QObject *obj)
{
    d->removeObject(obj);
}

QList<QObject *> ObjectPool::objectsInPool() const
{
    return d->objectsPool;
}

QObject *ObjectPool::getObjectByName(const QString &name) const
{
    QReadLocker lock(&m_lock);
    QList<QObject *> all = objectsInPool();
    foreach (QObject *obj, all) {
        if (obj->objectName() == name) {
            return obj;
        }
    }
    return 0;
}

QObject *ObjectPool::getObjectByClassName(const QString &className) const
{
    const QByteArray ba = className.toUtf8();
    QReadLocker lock(&m_lock);
    QList<QObject *> all = objectsInPool();
    foreach (QObject *obj, all) {
        if (obj->inherits(ba.constData())) {
            return obj;
        }
    }
    return 0;
}

} // end of namespace Commons
