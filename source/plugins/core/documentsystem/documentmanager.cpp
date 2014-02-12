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

#include <QTextDocument>

#include "documentmanager.h"
#include "documentmanager_p.h"

using namespace Core;
using namespace Core::Internal;

// ========== Core::DocumentManager ========== //

GET_INSTANCE(DocumentManager)

/*!
  Creates a new document then return it.
 */
QTextDocument *DocumentManager::createDocument()
{
    QTextDocument *doc = new QTextDocument(this);
    d->docList.append(doc);
    emit documentCreated(doc);
    return doc;
}

/*!
  Return current document.
 */
QTextDocument *DocumentManager::currentDocument() const
{
    return d->docList.first();
}

DocumentManager::DocumentManager(QObject *parent) :
    QObject(parent),
    d(new Internal::DocumentManagerPrivate(this))
{
}

DocumentManager::~DocumentManager()
{
    delete d;
}

// ========== Core::Internal::DocumentManagerPrivate ========== //

DocumentManagerPrivate::DocumentManagerPrivate(DocumentManager *documentManager) :
    q(documentManager)
{
}

DocumentManagerPrivate::~DocumentManagerPrivate()
{
}

void DocumentManagerPrivate::initialize()
{
    q->createDocument();
}
