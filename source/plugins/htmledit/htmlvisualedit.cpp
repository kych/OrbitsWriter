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

#include <QDebug>

#include <core/documentsystem/document.h>
#include <core/documentsystem/documentmanager.h>

#include "htmlvisualedit.h"

namespace HtmlEdit
{

HtmlVisualEdit::HtmlVisualEdit(QWidget *parent) :
    QTextEdit(parent)
{
    // document
    connect(gDocumentManager, SIGNAL(documentCreated(Core::Document*)),
            this, SLOT(onDocumentCreated(Core::Document*)));
}

void HtmlVisualEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

void HtmlVisualEdit::setHtmlSource(const QString &source)
{
    setHtml(source);
}

QString HtmlVisualEdit::toHtmlSource() const
{
    return gDocumentManager->currentDocument()->toHtmlSource();
}

void HtmlVisualEdit::onDocumentCreated(Core::Document *document)
{
    setDocument(document->document());

    // UI
    QFont font;
    font.setPointSizeF(11);
    setFont(font);
}

} // end of namespace HtmlEdit
