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

#ifndef SINGLETON_H
#define SINGLETON_H

/*!
  This macto expands the singleton instance getter for
  \a ClassName and the instance pointer name should be
  declared by DECLARE_SINGLETON(ClassName).

  You must include QMutex in order to use this macro
  meanwhile the class use it must have a default constructor.
 */
#define GET_INSTANCE(ClassName)                \
ClassName *ClassName::m_instance = 0;          \
ClassName *ClassName::instance()               \
{                                              \
    static QMutex mutex;                       \
    if (!m_instance) {                         \
        mutex.lock();                          \
        if (!m_instance)                       \
            m_instance = new ClassName;        \
        mutex.unlock();                        \
    }                                          \
    return m_instance;                         \
}

/*!
  This macto expands a singleton instance with \a ClassName and
  a getter function which should be implemented
  by GET_INSTANCE(ClassName).

  It will also disable the copy constructor and assignment operator.
 */
#define DECLARE_SINGLETON(ClassName)           \
public:                                        \
    static ClassName *instance();              \
private:                                       \
    Q_DISABLE_COPY(ClassName)                  \
    static ClassName *m_instance;

#endif // SINGLETON_H
