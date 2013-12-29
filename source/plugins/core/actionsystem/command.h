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

#ifndef COMMAND_H
#define COMMAND_H

#include "core/id.h"

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

#include <QObject>

namespace Core {

class Context;

class Command : public QObject
{
    Q_OBJECT
public:
    enum CommandAttribute {
        CA_Hide = 1,
        CA_UpdateText = 2,
        CA_UpdateIcon = 4,
        CA_NonConfigurable = 8
    };
    Q_DECLARE_FLAGS(CommandAttributes, CommandAttribute)

    virtual void setAttribute(CommandAttribute attr) = 0;
    virtual void removeAttribute(CommandAttribute attr) = 0;
    virtual bool hasAttribute(CommandAttribute attr) const = 0;

    virtual Id id() const = 0;

    virtual void setDescription(const QString &text) = 0;
    virtual QString description() const = 0;

    virtual QAction *action() const = 0;
    virtual QWidget *widget() const = 0;
    virtual Context context() const = 0;

    virtual void setDefaultKeySequence(const QKeySequence &key) = 0;
    virtual QKeySequence defaultKeySequence() const = 0;

    virtual QKeySequence keySequence() const = 0;
    virtual void setKeySequence(const QKeySequence &key) = 0;

    virtual bool isActive() const = 0;

signals:
    void keySequenceChanged();
    void activeStateChanged();
}; // end of class Core::Command

} // end of namespace Core

#endif // COMMAND_H
