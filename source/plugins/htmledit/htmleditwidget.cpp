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
#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>

#include <core/appcore.h>
#include <core/context.h>
#include <core/documentsystem/documentmanager.h>

#include "htmleditconstants.h"
#include "htmleditwidget.h"
#include "htmlsourceedit.h"
#include "htmlvisualedit.h"

namespace HtmlEdit
{

static const int IDX_VISUALEDIT = 0;
static const int IDX_SOURCEEDIT = 1;

namespace Internal
{
class HtmlEditWidgetPrivate : public QObject
{
    Q_OBJECT
public:
    QTabWidget     *m_editorArea;

    HtmlSourceEdit *m_sourceEdit;
    HtmlVisualEdit *m_visualEdit;
    QLineEdit      *m_titleEdit;

public slots:
    void editWidgetChanged(int index);
}; // end of class HtmlEdit::Internal::HtmlEditWidgetPrivate

void HtmlEditWidgetPrivate::editWidgetChanged(int index)
{
    switch (index) {
    case IDX_SOURCEEDIT:
    {
        gCore->updateAdditionalContexts(
                    Core::Context(Constants::CONTEXT_HTMLVISUALEDITOR),
                    Core::Context(Constants::CONTEXT_HTMLSOURCEEDITOR));
        m_sourceEdit->setPlainText(gDocumentManager->toHtmlSource());
        break;
    }
    case IDX_VISUALEDIT:
    {
        gCore->updateAdditionalContexts
                (Core::Context(Constants::CONTEXT_HTMLSOURCEEDITOR),
                 Core::Context(Constants::CONTEXT_HTMLVISUALEDITOR));
        m_visualEdit->setHtml(m_sourceEdit->toPlainText());
        break;
    }
    }
}

} // end of namespace HtmlEdit::Internal

HtmlEditWidget::HtmlEditWidget(QWidget *parent) :
    QWidget(parent),
    d(new Internal::HtmlEditWidgetPrivate)
{
    d->m_sourceEdit = new HtmlSourceEdit(this);
    d->m_sourceEdit->setStyleSheet("border: 0");

    d->m_visualEdit = new HtmlVisualEdit(this);
    d->m_visualEdit->setStyleSheet("border: 0");

    d->m_editorArea = new QTabWidget(this);
    d->m_editorArea->setTabPosition(QTabWidget::South);
    d->m_editorArea->insertTab(IDX_VISUALEDIT, d->m_visualEdit, tr("Visual"));
    d->m_editorArea->insertTab(IDX_SOURCEEDIT, d->m_sourceEdit, tr("Source"));
    connect(d->m_editorArea, SIGNAL(currentChanged(int)),
            d, SLOT(editWidgetChanged(int)));

    d->m_titleEdit = new QLineEdit(this);
    d->m_titleEdit->setFixedHeight(40);
    QFont defaultFont;
    defaultFont.setPointSize(24);
    d->m_titleEdit->setFont(defaultFont);
    d->m_titleEdit->setStyleSheet("border:2px solid gray;"
                                "border-radius: 10px;"
                                "padding:0 8px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 6, 4, 0);
    mainLayout->setMargin(4);
    mainLayout->addWidget(d->m_titleEdit);
    mainLayout->addWidget(d->m_editorArea);

    d->m_visualEdit->setFocus();
}

HtmlEditWidget::~HtmlEditWidget()
{
    delete d;
}

HtmlSourceEdit *HtmlEditWidget::sourceEdit() const
{
    return d->m_sourceEdit;
}

HtmlVisualEdit *HtmlEditWidget::visualEdit() const
{
    return d->m_visualEdit;
}

} // end of namespace HtmlEdit

#include "htmleditwidget.moc"
