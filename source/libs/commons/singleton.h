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

#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>

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

#define DECLARE_SINGLETON(ClassName)           \
public:                                        \
    static ClassName *instance();              \
private:                                       \
    Q_DISABLE_COPY(ClassName)                  \
    static ClassName *m_instance;

#endif // SINGLETON_H
