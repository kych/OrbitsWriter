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

#ifndef ID_H
#define ID_H

#include <QMetaType>
#include <QString>

#include "core_global.h"

namespace Core {

class CORE_EXPORT Id
{
public:
    static Id fromUniqueIdentifier(int uid)
    {
        return Id(uid, uid);
    }

    Id() : m_id(0) {}
    Id(const char *name);
    explicit Id(const QString &name);

    QByteArray name() const;
    QString toString() const;

    bool isValid() const
    {
        return m_id;
    }

    int uniqueIdentifier() const
    {
        return m_id;
    }

    bool operator==(Id id) const
    {
        return m_id == id.m_id;
    }

    bool operator==(const char *name) const;

    bool operator!=(Id id) const
    {
        return m_id != id.m_id;
    }

    bool operator!=(const char *name) const
    {
        return !operator==(name);
    }

    bool operator<(Id id) const
    {
        return m_id < id.m_id;
    }

    bool operator>(Id id) const
    {
        return m_id > id.m_id;
    }

private:
    int m_id;

    Id(int uid, int) : m_id(uid) {}
    Id(const QLatin1String &); // Intentionally unimplemented
}; // end of class Core::Id

inline uint qHash(const Id &id) { return id.uniqueIdentifier(); }

} // end of namespace Core

Q_DECLARE_METATYPE(Core::Id)
Q_DECLARE_METATYPE(QList<Core::Id>)

#endif // ID_H
