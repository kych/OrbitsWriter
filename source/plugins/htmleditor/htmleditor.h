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

#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include <core/editorsystem/editor.h>

namespace HtmlEditor {

namespace Internal {
class HTMLEditorPirvate;
}

class HTMLEditor : public Core::Editor
{
    Q_OBJECT
public:
    explicit HTMLEditor(QObject *parent = 0);
    ~HTMLEditor();
private:
    Internal::HTMLEditorPirvate *d;
    friend class Internal::HTMLEditorPirvate;
}; // end of class HtmlEditor::HTMLEditor

} // end of namespace HtmlEditor

#endif // HTMLEDITOR_H
