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
    QTabWidget *editorArea;

    HtmlSourceEdit *sourceEdit;
    HtmlVisualEdit *visualEdit;
    QLineEdit  *titleEdit;

public slots:
    void editWidgetChanged(int index);
};

void HtmlEditWidgetPrivate::editWidgetChanged(int index)
{
    switch (index) {
    case IDX_SOURCEEDIT:
    {
        gCore->updateAdditionalContexts(
                    Core::Context(Constants::CONTEXT_HTMLVISUALEDITOR),
                    Core::Context(Constants::CONTEXT_HTMLSOURCEEDITOR));
        break;
    }
    case IDX_VISUALEDIT:
    {
        gCore->updateAdditionalContexts
                (Core::Context(Constants::CONTEXT_HTMLSOURCEEDITOR),
                 Core::Context(Constants::CONTEXT_HTMLVISUALEDITOR));
        break;
    }
    }
}

// end of class HtmlEdit::Internal::HtmlEditWidgetPrivate
} // end of namespace HtmlEdit::Internal

HtmlEditWidget::HtmlEditWidget(QWidget *parent) :
    QWidget(parent),
    d(new Internal::HtmlEditWidgetPrivate)
{
    d->sourceEdit = new HtmlSourceEdit(this);
    d->sourceEdit->setStyleSheet("border: 0");

    d->visualEdit = new HtmlVisualEdit(this);
    d->visualEdit->setStyleSheet("border: 0");

    d->editorArea = new QTabWidget(this);
    d->editorArea->setTabPosition(QTabWidget::South);
    d->editorArea->insertTab(IDX_VISUALEDIT, d->visualEdit, tr("Visual"));
    d->editorArea->insertTab(IDX_SOURCEEDIT, d->sourceEdit, tr("Source"));
    connect(d->editorArea, SIGNAL(currentChanged(int)),
            d, SLOT(editWidgetChanged(int)));

    d->titleEdit = new QLineEdit(this);
    d->titleEdit->setFixedHeight(40);
    QFont defaultFont;
    defaultFont.setPointSize(24);
    d->titleEdit->setFont(defaultFont);
    d->titleEdit->setStyleSheet("border:2px solid gray;"
                                "border-radius: 10px;"
                                "padding:0 8px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 6, 4, 0);
    mainLayout->setMargin(4);
    mainLayout->addWidget(d->titleEdit);
    mainLayout->addWidget(d->editorArea);

    d->visualEdit->setFocus();
}

HtmlEditWidget::~HtmlEditWidget()
{
    delete d;
}

} // end of namespace HtmlEdit

#include "htmleditwidget.moc"
