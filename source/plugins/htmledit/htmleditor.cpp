/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2014 devbean@galaxyworld.org
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

#include <core/context.h>

#include "htmleditconstants.h"
#include "htmleditor.h"
#include "htmleditwidget.h"

namespace HtmlEdit
{

namespace Internal
{

class HtmlEditorPrivate
{
public:
    HtmlEditorPrivate() : editWidget(0) {}

    HtmlEditWidget *editWidget;
}; // end of class HtmlEdit::Internal::HtmlSourceEditorPrivate
} // end of namespace HtmlEdit::Internal

HtmlEditor::HtmlEditor(HtmlEditWidget *editWidget) :
    Editor(editWidget),
    d(new Internal::HtmlEditorPrivate)
{
    d->editWidget = editWidget;

    m_widget = d->editWidget;
    m_context = Core::Context(Constants::CONTEXT_HTMLSOURCEEDITOR);
}

HtmlEditor::~HtmlEditor()
{
    delete d;
}

} // end of namespace HtmlEdit
