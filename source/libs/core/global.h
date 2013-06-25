/*-------------------------------------------------
 *
 * OrbitsWriter - An Offline Blog Writer
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

#ifndef GLOBAL_H
#define GLOBAL_H

/*!
  This macro can be used to expose classes within libraries.
 */
#ifdef SHARED_LIBRARY
#define LIBRARY_EXPORT Q_DECL_EXPORT
#else
#define LIBRARY_EXPORT Q_DECL_IMPORT
#endif

/*!
  This macro can be used to declare a private data class for \a CLASS.

  The data class will be in \c Internal namespace
  and its name will be \c CLASSPrivate.
 */
#define DECLARE_PRIVATE_DATA(CLASS)            \
namespace Internal                             \
{                                              \
    class CLASS##Private;                      \
}

/*!
  This macro can be used to declare a private data class for \a CLASS
  with name \c d .

  This is a D-Pointer declaration. The data class will be a friend class
  and \c d pointer will be \c const .
 */
#define PRIVATE_D_POINTER(CLASS)               \
private:                                       \
    friend class Internal::CLASS##Private;     \
    Internal::CLASS##Private * const d;

/*!
  This macro can be used to declare a private data class for \a CLASS
  with name \c q .

  This is a Q-Pointer declaration used in data classes. The Q-Pointer should
  be the public class to this data.
 */
#define PRIVATE_Q_POINTER(CLASS)               \
private:                                       \
    friend class CLASS;                        \
    CLASS * const q;

/*!
  This macto can be used to generate the singleton instance getter for
  \a CLASS and the instance pointer name should be
  declared by macro \c DECLARE_SINGLETON(CLASS) .

  You must include QMutex in order to use this macro
  meanwhile the class use it must have a default constructor.
 */
#define GET_INSTANCE(CLASS)                    \
CLASS *CLASS::m_instance = 0;                  \
CLASS *CLASS::instance()                       \
{                                              \
    static QMutex mutex;                       \
    if (!m_instance) {                         \
        mutex.lock();                          \
        if (!m_instance)                       \
            m_instance = new CLASS;            \
        mutex.unlock();                        \
    }                                          \
    return m_instance;                         \
}

/*!
  This macro can be used to declare a singleton instance with \a CLASS
  and a getter function which should be implemented
  by macro \c GET_INSTANCE(CLASS).

  It will also disable the copy constructor and assignment operator.
 */
#define DECLARE_SINGLETON(CLASS)               \
public:                                        \
    static CLASS *instance();                  \
private:                                       \
    Q_DISABLE_COPY(CLASS)                      \
    static CLASS *m_instance;

#endif // end of GLOBAL_H
