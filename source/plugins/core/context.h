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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QDebug>
#include <QList>
#include <QPointer>
#include <QWidget>

#include "core/core_global.h"

namespace Core
{

class CORE_EXPORT Context
{
public:
    Context() {}
    explicit Context(const char *c1) { add(c1); }
    Context(const char *c1, const char *c2) { add(c1); add(c2); }
    Context(const char *c1, const char *c2, const char *c3) { add(c1); add(c2); add(c3); }
    Context(const char *base, int offset);

    void add(const char *id);
    bool contains(const char *id) const;
    bool contains(int c) const { return d.contains(c); }
    int size() const { return d.size(); }
    bool isEmpty() const { return d.isEmpty(); }
    int at(int i) const { return d.at(i); }
    int indexOf(int c) const { return d.indexOf(c); }
    void removeAt(int i) { d.removeAt(i); }
    void prepend(int c) { d.prepend(c); }

    typedef QList<int>::const_iterator const_iterator;
    const_iterator begin() const { return d.begin(); }
    const_iterator end() const { return d.end(); }

    void add(const Context &c) { d += c.d; }
    void add(int c) { d.append(c); }
    bool operator ==(const Context &c) const { return d == c.d; }

private:
    QList<int> d;
}; // end of class Core::Context

class CORE_EXPORT WidgetContext : public QObject
{
    Q_OBJECT
public:
    explicit WidgetContext(QObject *parent = 0) : QObject(parent) {}

    virtual Context context() const { return m_context; }
    virtual QWidget *widget() const { return m_widget; }

    virtual void setContext(const Context &context) { m_context = context; }
    virtual void setWidget(QWidget *widget) { m_widget = widget; }

protected:
    Context m_context;
    QPointer<QWidget> m_widget;
}; // end of class Core::WidgetContext

} // end of namespace Core

#endif // CONTEXT_H
