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

#include <core/appcore.h>
#include <core/context.h>
#include <core/editorsystem/editor.h>
#include <core/editorsystem/editormanager.h>
#include <core/id.h>

#include "editoraction.h"
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
    HtmlEditorPrivate() : m_editWidget(0) {}

    HtmlEditWidget *m_editWidget;
}; // end of class HtmlEdit::Internal::HtmlSourceEditorPrivate
} // end of namespace HtmlEdit::Internal

void HtmlEditor::mergeFormat(EditorAction *editorAction)
{
    Core::Editor *currentEditor = gEditorManager->currentEditor();
    if (currentEditor->id() == HtmlEdit::Constants::ID_HTMLEDITOR) {
        HtmlEdit::HtmlEditor *htmlEditor = qobject_cast<HtmlEdit::HtmlEditor *>(currentEditor);
        if (gCore->hasContext(Core::Id(HtmlEdit::Constants::CONTEXT_HTMLVISUALEDITOR).uniqueIdentifier())) {
            if (editorAction->hasVisualEditorAction()) {
                HtmlEdit::HtmlVisualEdit *visualEdit = htmlEditor->visualEdit();
                editorAction->doActionOnVisualEditor(visualEdit);
            }
        } else if (gCore->hasContext(Core::Id(HtmlEdit::Constants::CONTEXT_HTMLSOURCEEDITOR).uniqueIdentifier())) {
        }
    }
}

HtmlEditor::HtmlEditor(HtmlEditWidget *editWidget) :
    Editor(editWidget),
    d(new Internal::HtmlEditorPrivate)
{
    d->m_editWidget = editWidget;

    m_widget = d->m_editWidget;
    m_context = Core::Context(Constants::CONTEXT_HTMLVISUALEDITOR);
}

HtmlEditor::~HtmlEditor()
{
    delete d;
}

Core::Id HtmlEditor::id() const
{
    return Core::Id(Constants::ID_HTMLEDITOR);
}

HtmlSourceEdit *HtmlEditor::sourceEdit() const
{
    return d->m_editWidget->sourceEdit();
}

HtmlVisualEdit *HtmlEditor::visualEdit() const
{
    return d->m_editWidget->visualEdit();
}

} // end of namespace HtmlEdit
