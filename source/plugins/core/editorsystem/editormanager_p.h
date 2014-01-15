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

#ifndef EDITORMANAGER_P_H
#define EDITORMANAGER_P_H

#include "editormanager.h"

namespace Core {

class Id;
class WidgetContext;

namespace Internal {

class EditorManagerPrivate : public QObject
{
    Q_OBJECT
public:
    EditorManagerPrivate(EditorManager *mgr);
    ~EditorManagerPrivate();

    EditorManager *q;

//    Editor::EditorActions editorActions;

    Editor *currEditor;

//    void updateEditorActions();
    void refreshActions();

    void setCurrentEditor(Editor *editor);

//    void setEditorActions(Editor::EditorActions actions);

//    void setEditorActionEnable(Editor::EditorActions actions, bool enable);
//    void setEd/itorActionChecked(Editor::EditorActions actions, bool checked);

    Editor * createEditor(const Id &id);

public slots:
    void contextAboutToChange(WidgetContext *context);
}; // end of class Core::Internal::EditorManagerPrivate

} // end of namespace Core::Internal
} // end of namespace Core

#endif // EDITORMANAGER_P_H
