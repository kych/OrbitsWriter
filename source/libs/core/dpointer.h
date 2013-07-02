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

#ifndef DPOINTER_H
#define DPOINTER_H

#include <memory>

namespace GOW
{

template <typename T>
class DPointer
{
public:
    DPointer() : d(new T())
    {
    }

    /*
     * We can implement by variadic templates,
     * but some compilers, such as VC11 does not
     * support it yet. So we have to add these ugly
     * override constructors.
     *
     * template <typename T>
     * template <typename ...Args>
     * DPointer<T>::DPointer(Args && ... args)
     *     : d(new T(std::forward<Args>(args)... ))
     * {
     * }
     */

    template <typename Arg1>
    DPointer(Arg1 && arg1) : d(new T(arg1))
    {
    }

    template <typename Arg1, typename Arg2>
    DPointer(Arg1 && arg1, Arg2 && arg2)
        : d(new T(arg1, arg2))
    {
    }

    template <typename Arg1, typename Arg2, typename Arg3>
    DPointer(Arg1 && arg1, Arg2 && arg2, Arg3 && arg3)
        : d(new T(arg1, arg2, arg3))
    {
    }

    ~DPointer()
    {

    }

    T * operator->() const
    {
        return d.get();
    }

    T * get() const
    {
        return d.get();
    }

private:
    std::unique_ptr<T> d;
}; // end of class GOW::DPointer

#define D_POINTER                   \
    class Private;                  \
    friend class Private;           \
    const GOW::DPointer<Private> d; \

#define Q_POINTER(CLASS)            \
    friend class CLASS;             \
    CLASS * const q;
} // end of namespace GOW

#endif // DPOINTER_H
