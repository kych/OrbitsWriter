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

#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include <core/editorsystem/editor.h>

#include "htmledit_global.h"

namespace HtmlEdit {

namespace Internal {
class HtmlEditorPrivate;
}

class EditorAction;
class HtmlEditWidget;
class HtmlSourceEdit;
class HtmlVisualEdit;

class HTMLEDIT_EXPORT HtmlEditor : public Core::Editor
{
    Q_OBJECT
public:
    static void mergeFormat(EditorAction *editorAction);

    explicit HtmlEditor(HtmlEditWidget *editWidget = 0);
    ~HtmlEditor();

    Core::Id id() const;

    HtmlSourceEdit * sourceEdit() const;
    HtmlVisualEdit * visualEdit() const;

private:
    Internal::HtmlEditorPrivate *d;
    friend class Internal::HtmlEditorPrivate;
}; // end of class HtmlEdit::HtmlEditor

} // end of namespace HtmlEdit

#endif // HTMLEDITOR_H