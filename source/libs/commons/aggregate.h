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

#ifndef AGGREGATE_H
#define AGGREGATE_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QReadWriteLock>

#include "commons/commons_global.h"

namespace Commons
{

/*!
  The Aggregation namespace contains support for bundling related components,
  such that each component exposes the properties and behavior of the
  other components to the outside.

  Components that are bundled to an Aggregate can be "cast" to each other
  and have a coupled life cycle. See the documentation of Aggregation::Aggregate for
  details and examples.
 */
namespace Aggregation
{

/*!
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
class COMMONS_EXPORT Aggregate : public QObject
{
    Q_OBJECT
public:
    /*!
      Constructs a new Aggregate with the given \a parent.

      The \a parent is passed directly passed to the QObject part
      of the class and is not used beside that.
     */
    Aggregate(QObject *parent = 0);

    /*!
      Destroys the aggregate automatically deletes all its components.
     */
    virtual ~Aggregate();

    /*!
      Adds the \a component to the aggregate.

      You can't add a component that is part of a different aggregate
      or an aggregate itself.

      Emits changed() signal.

      \sa Aggregate::remove()
     */
    void add(QObject *component);

    /*!
      Removes the \a component from the aggregate.

      Emits changed() signal.

      \sa Aggregate::add()
     */
    void remove(QObject *component);

    /*!
      Gets the component with the given type, if there is one.

      If there are multiple components with that type a random one is returned.

      \sa Aggregate::components()
      \sa Aggregate::add()
     */
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

    /*!
      Gets all components with the given type, if there are any.

      \sa Aggregate::component()
      \sa Aggregate::add()
     */
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

    /*!
      Gets the Aggregate object of \a obj if there is one. Otherwise returns 0.
     */
    static Aggregate *parentAggregate(QObject *obj);

    /*!
      Returns a read-write lock for Aggregate operations.
     */
    static QReadWriteLock &lock();

signals:
    /*!
      Emits when any conponents adds or removes from this aggregate.
     */
    void changed();

private slots:
    void deleteSelf(QObject *obj);

private:
    //! Returns the aggregate map.
    static QHash<QObject *, Aggregate *> &aggregateMap();

    QList<QObject *> m_components;
}; // end of class Commons::Aggregate

//! Get a component via global template function.
//! Internal used.
template <typename T>
T * query(Aggregate *obj)
{
    if (!obj) {
        return (T *)0;
    }
    return obj->template component<T>();
}

/*!
  Performs a dynamic cast that is aware of a possible Aggregate that \a obj
  might belong to.

  If \a obj itself is of the requested type then it is simply cast
  and returned. Otherwise, if \a obj belongs to an Aggregate all its components are
  checked, or if it doesn't belong to an Aggregate null is returned.

  \sa Aggregate::component()
 */
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

//! Get all components of a specific type via template function.
//! Internal used.
template <typename T>
QList<T *> query_all(Aggregate *obj)
{
    if (!obj) {
        return QList<T *>();
    }
    return obj->template components<T>();
}

/*!
  If \a obj belongs to an Aggregate, all components that can be cast to the given
  type are returned. Otherwise, \a obj is returned if it is of the requested type.

  \sa Aggregate::components()
 */
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
