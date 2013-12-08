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

#include <QHash>
#include <QVector>

#include "id.h"

/*!
  \class Core::Id
  The class Id encapsulates an identifier.

  It is used as a type-safe helper class instead of a \c QString
  or \c QByteArray.

  The internal representation of the id is assumed
  to be plain 7-bit-clean ASCII.
 */

namespace Core
{

// ========== Core::StringHashHolder ========== //

/*!
  \brief The StringHashHolder class holds the underlying C-style string
  with its langth and computed hash code.

  The hash code for the string which stored in this holder class is computed as
  <pre>
  s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
  </pre>
  using <code>int</code> arithmetic, where <code>s[i]</code> is the <i>i</i>th
  character of the string, <code>n</code> is the length of the string,
  and <code>^</code> indicates exponentiation.
  (The hash value of the empty string is zero.)

  This algorithm is used by <code>hashCode()</code> method of Java <code>String</code>.
 */
class StringHashHolder
{
public:
    explicit StringHashHolder(const char *s) :
        str(s),
        hashcode(0)
    {
        strlen = qstrlen(str);

        int h = hashcode;
        if (h == 0) {
            for (uint i = 0, offset = 0; i < strlen; i++) {
                h = 31 * h + str[offset++];
            }
            hashcode = h;
        }
    }

    uint strlen;     //!< string length
    const char *str; //!< C-style string
    uint hashcode;   //!< hash code of string
}; // end of class Core::StringHashHolder

/*!
  Returns true if two StringHashHolders are equal.
 */
static bool operator==(const StringHashHolder &sh1, const StringHashHolder &sh2)
{
    // sh.n is unlikely to discriminate better than the hash.
    return sh1.hashcode == sh2.hashcode && qstrcmp(sh1.str, sh1.str) == 0;
}

static uint qHash(const StringHashHolder &sh)
{
    return sh.hashcode;
}


// ========== Core::IdCache ========== //

class IdCache : public QHash<StringHashHolder, int>
{
public:
    ~IdCache()
    {
        for (IdCache::iterator it = begin(); it != end(); ++it) {
            delete[] const_cast<char *>(it.key().str);
        }
    }
};


// global
static int                 lastUid = 0;
static QVector<QByteArray> nameCache;
static IdCache             idCache;

static int createUid(const char *str)
{
    if (str && *str) {
        StringHashHolder sh(str);
        int uid = idCache.value(sh, 0);
        if (uid == 0) {
            if (lastUid == 0) {
                nameCache.append(QByteArray());
            }
            uid = ++lastUid;
            sh.str = qstrdup(sh.str);
            idCache[sh] = uid;
            nameCache.append(QByteArray::fromRawData(sh.str, sh.strlen));
        }
        return uid;
    }
    return 0;
}


// ========== Core::Id ========== //

/*!
  Constants an instance of Id with \a name.
 */
Id::Id(const char *name) : m_id(createUid(name)) {}

/*!
  Constants an instance of Id with \a name.
 */
Id::Id(const QString &name) : m_id(createUid(name.toUtf8())) {}

/*!
  Gets the name of this Id.
 */
QByteArray Id::name() const
{
    return nameCache.at(m_id);
}

/*!
  Returns the string format of this Id.
 */
QString Id::toString() const
{
    return QString::fromUtf8(nameCache.at(m_id));
}

/*!
  Returns true if this Id is equal to the one with \a name.
 */
bool Id::operator==(const char *name) const
{
    return qstrcmp(nameCache.at(m_id).constData(), name) == 0;
}

} // end of namespace Core

/*!
  \fn Id Id::fromUniqueIdentifier(int uid)
  Constructs an instance of Id from
  the unique identifier \a uid.
 */

/*!
  \fn Id::Id()
  Constructs an invalid instance of Id.
 */

/*!
  \fn bool Id::isValid() const
  Returns true if this Id is valid.
 */

/*!
  \fn int Id::uniqueIdentifier() const
  Returns the unique identifier of this Id.
 */

/*!
  \fn bool Id::operator==(Id id) const
  Returns true if this Id is equal to the other one.
 */

/*!
  \fn bool Id::operator!=(Id id) const
  Returns true if this Id is not equal to the other one.
 */

/*!
  \fn bool Id::operator!=(const char *name) const
  Returns true if this Id is not equal to the one with \a name.
 */

/*!
  \fn bool Id::operator<(Id id) const
  Returns true if this string is less than the other one.
 */

/*!
  \fn bool Id::operator>(Id id) const
  Returns true if this string is greater than the other one.
 */

/*!
  \fn inline uint Id::qHash(const Id &id)
  Hash function for Id.
 */
