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

#ifndef DOCUMENTMANAGER_P_H
#define DOCUMENTMANAGER_P_H

#include <QObject>
#include <QList>
#include <QTextDocument>

namespace Core {

class DocumentManager;

namespace Internal {

class DocumentManagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit DocumentManagerPrivate(Core::DocumentManager * documentManager);
    ~DocumentManagerPrivate();

    void initialize();

    QList<QTextDocument *> docList;

private:
    Core::DocumentManager *q;
}; // end of class Core::Internal::DocumentManagerPrivate
} // end of namespace Core::Internal
} // end of namespace Core

#endif // DOCUMENTMANAGER_P_H
