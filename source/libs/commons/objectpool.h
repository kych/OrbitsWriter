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

#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <QObject>
#include <QReadWriteLock>

#include "commons/aggregate.h"
#include "commons/commons_global.h"
#include "commons/singleton.h"

#define appObjectPool (Commons::ObjectPool::instance())

namespace Commons
{

namespace Internal
{
class ObjectPoolPrivate;
}

/*!
  The ObjectPool class is the objects pool used in gloabl.
 */
class COMMONS_EXPORT ObjectPool : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(ObjectPool)
public:
    /*!
      Add the given object \a obj to the object pool, so it can be retrieved again from the pool by type.

      This is a objects pool and does not do any memory management - added objects
      must be removed from the pool and deleted manually by whoever is responsible for the object.

      Emits the objectAdded() signal.

      \sa removeObject()
      \sa getObject()
      \sa getObjects()
     */
    void addObject(QObject *obj);

    /*!
      Removes the object \a obj from the object pool.

      Emits aboutToRemoveObject().

      \sa PluginManager::addObject()
     */
    void removeObject(QObject *obj);

    /*!
      Gets the list of all objects in the pool, unfiltered.

      Usually clients do not need to call this.

      \sa getObject()
      \sa getObjects()
     */
    QList<QObject *> objectsInPool() const;

    /*!
      Gets one object with a given name from the object pool.

      \sa addObject()
     */
    QObject *getObjectByName(const QString &name) const;

    /*!
      Gets one object inheriting a class with a given name from the object pool.

      \sa addObject()
     */
    QObject *getObjectByClassName(const QString &className) const;

    /*!
      Gets all objects of a given type from the object pool.

      This method is aware of Aggregation::Aggregate, i.e. it uses
      the Aggregation::query methods instead of qobject_cast to
      determine the type of an object.

      \sa addObject()
     */
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

    /*!
      Gets the object of a given type from the object pool.

      This method is aware of Aggregation::Aggregate, i.e. it uses
      the Aggregation::query methods instead of qobject_cast to
      determine the type of an object.

      If there are more than one object of the given type in
      the object pool, this method will choose an arbitrary one of them.

      \sa addObject()
     */
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
    /*!
      Emits when \a obj has been added to objects pool.
     */
    void objectAdded(QObject * obj);

    /*!
      Emits when \a obj will be removed from the object pool.
     */
    void aboutToRemoveObject(QObject * obj);

private:
    //! Constructs an instance of ObjectPool.
    explicit ObjectPool(QObject *parent = 0);
    //! Destroys the instance of this class.
    ~ObjectPool();

    mutable QReadWriteLock m_lock;

    Internal::ObjectPoolPrivate *d;
    friend class Internal::ObjectPoolPrivate;
}; // end of class Commons::ObjectPool

} // end of namespace Commons

#endif // OBJECTPOOL_H
