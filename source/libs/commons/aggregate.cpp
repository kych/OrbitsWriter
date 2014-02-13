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

#include "aggregate.h"

#include <QWriteLocker>
#include <QDebug>

using namespace Commons::Aggregation;

/*!
  \namespace Commons::Aggregation
  The Aggregation namespace contains support for bundling related components,
  such that each component exposes the properties and behavior of the
  other components to the outside.

  Components that are bundled to an Aggregate can be "cast" to each other
  and have a coupled life cycle. See the documentation of Aggregation::Aggregate for
  details and examples.
 */

/*!
  \class Aggregate
  Defines a collection of related components that can be viewed as a unit.

  An Aggregate is a collection of components that are handled as a unit,
  such that each component exposes the properties and behavior of the
  other components in the Aggregate to the outside.

  Specifically that means:
  -# They can be "cast" to each other (using query and query_all methods).
  -# Their life cycle is coupled, i.e. whenever one is deleted all of them are.
  Components can be of any QObject derived type.

  You can use an Aggregate to simulate multiple inheritance by aggregation. Assume we have
  \code
      using namespace Commons::Aggregation;
      class MyInterface : public QObject { ........ };
      class MyInterfaceEx : public QObject { ........ };
      [...]
      MyInterface *object = new MyInterface; // this is single inheritance
  \endcode
  The query method works like a qobject_cast with normal objects:
  \code
      Q_ASSERT(query<MyInterface>(object) == object);
      Q_ASSERT(query<MyInterfaceEx>(object) == 0);
  \endcode
  If we want 'object' to also implement the class MyInterfaceEx,
  but don't want to or cannot use multiple inheritance, we can do it
  at any point using an Aggregate:
  \code
      MyInterfaceEx *objectEx = new MyInterfaceEx;
      Aggregate *aggregate = new Aggregate;
      aggregate->add(object);
      aggregate->add(objectEx);
  \endcode
  The Aggregate bundles the two objects together.
  If we have any part of the collection we get all parts:
  \code
      Q_ASSERT(query<MyInterface>(object) == object);
      Q_ASSERT(query<MyInterfaceEx>(object) == objectEx);
      Q_ASSERT(query<MyInterface>(objectEx) == object);
      Q_ASSERT(query<MyInterfaceEx>(objectEx) == objectEx);
  \endcode
  The following deletes all three: object, objectEx and aggregate:
  \code
      delete objectEx;
      // or delete object;
      // or delete aggregate;
  \endcode

  Aggregation aware code never uses qobject_cast, but always uses
  Commons::Aggregation::query which behaves like a qobject_cast as a fallback.
 */

/*!
  Constructs a new Aggregate with the given \a parent.

  The \a parent is passed directly passed to the QObject part
  of the class and is not used beside that.
 */
Aggregate::Aggregate(QObject *parent) :
    QObject(parent)
{
    QWriteLocker locker(&lock());
    aggregateMap().insert(this, this);
}

/*!
  Destroys the aggregate automatically deletes all its components.
 */
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

/*!
  Adds the \a component to the aggregate.

  You can't add a component that is part of a different aggregate
  or an aggregate itself.

  Emits changed() signal.

  \sa Aggregate::remove()
 */
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

/*!
  Removes the \a component from the aggregate.

  Emits changed() signal.

  \sa Aggregate::add()
 */
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

/*!
  Gets the Aggregate object of \a obj if there is one. Otherwise returns 0.
 */
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

/*!
  Returns a read-write lock for Aggregate operations.
 */
QReadWriteLock & Aggregate::lock()
{
    static QReadWriteLock lock;
    return lock;
}

/*!
  \fn T * Aggregate::component() const
  Gets the component with the given type, if there is one.

  If there are multiple components with that type a random one is returned.

  \sa Aggregate::components()
  \sa Aggregate::add()
 */

/*!
  \fn QList<T *> Aggregate::components() const
  Gets all components with the given type, if there are any.

  \sa Aggregate::component()
  \sa Aggregate::add()
 */

/*!
  \fn void Aggregate::changed()
  Emits when any conponents adds or removes from this aggregate.
 */

/*!
  \fn T * Aggregate::query(QObject *obj)
  Performs a dynamic cast that is aware of a possible Aggregate that \a obj
  might belong to.

  If \a obj itself is of the requested type then it is simply cast
  and returned. Otherwise, if \a obj belongs to an Aggregate all its components are
  checked, or if it doesn't belong to an Aggregate null is returned.

  \sa Aggregate::component()
 */

/*!
  \fn QList<T *> Aggregate::query_all(QObject *obj)
  If \a obj belongs to an Aggregate, all components that can be cast to the given
  type are returned. Otherwise, \a obj is returned if it is of the requested type.

  \sa Aggregate::components()
 */

/*!
  \fn QList<T *> Aggregate::query_all(Aggregate *obj)
  Get all components of a specific type via template function.
  \note Internal used.
 */

/*!
  \fn T * Aggregate::query(Aggregate *obj)
  Get a component via global template function.
  \note Internal used.
 */
