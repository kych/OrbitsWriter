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

#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>

#include <commons/singleton.h>

#include "core/core_global.h"

#define gDocumentManager (Core::DocumentManager::instance())

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

namespace Core {

class MainWindow;

namespace Internal {
class DocumentManagerPrivate;
}

class CORE_EXPORT DocumentManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(DocumentManager)
public:
    QTextDocument * createDocument();
    QTextDocument * currentDocument() const;

signals:
    void documentCreated(QTextDocument *document);

private:
    explicit DocumentManager(QObject *parent = 0);
    ~DocumentManager();

    Internal::DocumentManagerPrivate *d;
    friend class Internal::DocumentManagerPrivate;
    friend class MainWindow;
}; // end of class Core::DocumentManager

} // end of namespace Core

#endif // DOCUMENTMANAGER_H
