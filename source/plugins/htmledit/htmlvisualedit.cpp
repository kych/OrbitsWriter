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
#include <QTextFrame>

#include <core/documentsystem/documentmanager.h>

#include "htmlvisualedit.h"

namespace HtmlEdit
{

HtmlVisualEdit::HtmlVisualEdit(QWidget *parent) :
    QTextEdit(parent)
{
    // document
    connect(gDocumentManager, SIGNAL(documentCreated(QTextDocument*)),
            this, SLOT(onDocumentCreated(QTextDocument*)));

    // UI
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSizeF(11);
    setFont(font);
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
    QTextDocument *currentDocument = document();
    QTextFrame *root = currentDocument->rootFrame();
    for (QTextFrame::iterator it = root->begin(); !(it.atEnd()); ++it) {
        QTextFrame *childFrame = it.currentFrame();
        QTextBlock childBlock = it.currentBlock();
        if (childFrame) {
//            processFrame(frameElement, childFrame);
        } else if (childBlock.isValid()) {
//            processBlock(frameElement, childBlock);
        }
    }
    return toHtml();
}

void HtmlVisualEdit::onDocumentCreated(QTextDocument *document)
{
    setDocument(document);
}

} // end of namespace HtmlEdit
