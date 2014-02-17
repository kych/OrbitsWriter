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

#ifndef DOCUMENT_P_H
#define DOCUMENT_P_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QTextBlock;
class QTextDocument;
class QTextFragment;
class QTextFrame;
QT_END_NAMESPACE

namespace Core {

namespace Internal {

class DocumentPrivate
{
public:
    DocumentPrivate();
    ~DocumentPrivate();

    QString toHtml() const;
    QString formatHtml(const QString &html) const;

    void processFrame(QTextFrame *parentFrame, QTextFrame *childFrame) const;
    QString processBlock(QTextFrame *parentFrame, QTextBlock currentBlock) const;
    QString processFragment(QTextFragment fragment) const;

    QTextDocument *textDocument;
    QString html;
}; // end of class Core::Internal::DocumentPrivate

} // end of namespace Core::Internal

} // end of namespace Core

#endif // DOCUMENT_P_H
