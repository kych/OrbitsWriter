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

#include <QDebug>

#include "objectpool.h"

/*!
  \class Commons::ObjectPool
  The ObjectPool class is the objects pool used in gloabl.
 */

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

/*!
 Constructs an instance of ObjectPool.
 */
ObjectPool::ObjectPool(QObject *parent) :
    QObject(parent),
    d(new Internal::ObjectPoolPrivate(this))
{
}

/*!
  Destroys the instance of this class.
 */
ObjectPool::~ObjectPool()
{
    delete d;
}

/*!
  Add the given object \a obj to the object pool, so it can be retrieved again from the pool by type.

  This is a objects pool and does not do any memory management - added objects
  must be removed from the pool and deleted manually by whoever is responsible for the object.

  Emits the objectAdded() signal.

  \sa removeObject()
  \sa getObject()
  \sa getObjects()
 */
void ObjectPool::addObject(QObject *obj)
{
    d->addObject(obj);
}

/*!
  Removes the object \a obj from the object pool.

  Emits aboutToRemoveObject().

  \sa PluginManager::addObject()
 */
void ObjectPool::removeObject(QObject *obj)
{
    d->removeObject(obj);
}

/*!
  Gets the list of all objects in the pool, unfiltered.

  Usually clients do not need to call this.

  \sa getObject()
  \sa getObjects()
 */
QList<QObject *> ObjectPool::objectsInPool() const
{
    return d->objectsPool;
}

/*!
  Gets one object with a given name from the object pool.

  \sa addObject()
 */
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

/*!
  Gets one object inheriting a class with a given name from the object pool.

  \sa addObject()
 */
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

/*!
  \fn QList<T *> ObjectPool::getObjects() const
  Gets all objects of a given type from the object pool.

  This method is aware of Aggregation::Aggregate, i.e. it uses
  the Aggregation::query methods instead of qobject_cast to
  determine the type of an object.

  \sa addObject()
 */

/*!
  \fn T * ObjectPool::getObject() const
  Gets the object of a given type from the object pool.

  This method is aware of Aggregation::Aggregate, i.e. it uses
  the Aggregation::query methods instead of qobject_cast to
  determine the type of an object.

  If there are more than one object of the given type in
  the object pool, this method will choose an arbitrary one of them.

  \sa addObject()
 */

/*!
  \fn void ObjectPool::objectAdded(QObject * obj)
  Emits when \a obj has been added to objects pool.
 */

/*!
  \fn void ObjectPool::aboutToRemoveObject(QObject * obj)
  Emits when \a obj will be removed from the object pool.
 */
