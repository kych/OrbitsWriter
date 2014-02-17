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
#include <QTextDocument>
#include <QTextFrame>

#include "document.h"
#include "document_p.h"

using namespace Core;
using namespace Core::Internal;

// ========== Core::Internal::Document ========== //

Document::Document(QObject *parent) :
    QObject(parent),
    d(new Internal::DocumentPrivate)
{
}

Document::~Document()
{
    delete d;
}

QString Document::toHtmlSource(bool format) const
{
    if (!format) {
        return d->toHtml();
    } else {
        return d->formatHtml(d->toHtml());
    }
}

QTextDocument *Document::document() const
{
    return d->textDocument;
}

// ========== Core::Internal::DocumentPrivate ========== //

DocumentPrivate::DocumentPrivate() :
    textDocument(new QTextDocument)
{
}

DocumentPrivate::~DocumentPrivate()
{
    delete textDocument;
}

QString DocumentPrivate::toHtml() const
{
    QString html;
    QTextFrame *root = textDocument->rootFrame();
    for (QTextFrame::iterator it = root->begin(); !(it.atEnd()); ++it) {
        QTextFrame *childFrame = it.currentFrame();
        QTextBlock childBlock = it.currentBlock();
        if (childFrame) {
            processFrame(root, childFrame);
            qDebug() << childFrame;
        } else if (childBlock.isValid()) {
            qDebug() << processBlock(root, childBlock);
        }
    }
    return textDocument->toHtml();
}

QString DocumentPrivate::formatHtml(const QString &html) const
{
    return html;
}

void DocumentPrivate::processFrame(QTextFrame *parentFrame, QTextFrame *childFrame) const
{
//    for (QTextFrame::iterator it = childFrame->begin(); !(it.atEnd()); ++it) {
//        QTextFrame *frame = it.currentFrame();
//        QTextBlock block = it.currentBlock();
//        if (childFrame) {
//            processFrame(childFrame, frame);
//        } else if (childBlock.isValid()) {
//            processBlock(childFrame, block);
//        }
//    }
}

QString DocumentPrivate::processBlock(QTextFrame *parentFrame, QTextBlock currentBlock) const
{
    Q_UNUSED(parentFrame);

    QString currentBlockHtml;
    QTextBlock::iterator it;
    for (it = currentBlock.begin(); !(it.atEnd()); ++it) {
        QTextFragment currentFragment = it.fragment();
        if (currentFragment.isValid()) {
            currentBlockHtml = currentBlockHtml.append(processFragment(currentFragment));
        }
    }
    return currentBlockHtml;
}

QString DocumentPrivate::processFragment(QTextFragment fragment) const
{
    QString fragmentHtml = fragment.text();
    QTextCharFormat textCharFormat = fragment.charFormat();
    if (textCharFormat.fontWeight() == QFont::Bold) {
        fragmentHtml = QLatin1String("<b>") + fragmentHtml + QLatin1String("</b>");
    }
    if (textCharFormat.fontItalic()) {
        fragmentHtml = QLatin1String("<i>") + fragmentHtml + QLatin1String("</i>");
    }
    if (textCharFormat.fontUnderline()) {
        fragmentHtml = QLatin1String("<u>") + fragmentHtml + QLatin1String("</u>");
    }
    if (textCharFormat.fontStrikeOut()) {
        fragmentHtml = QLatin1String("<del>") + fragmentHtml + QLatin1String("</del>");
    }
    return fragmentHtml;
}
