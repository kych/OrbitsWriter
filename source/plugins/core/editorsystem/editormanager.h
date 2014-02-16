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

#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QObject>

#include <commons/singleton.h>

#include "core/core_export.h"

#define gEditorManager (Core::EditorManager::instance())

namespace Core {

class Editor;

namespace Internal
{
class EditorManagerPrivate;
}

class CORE_EXPORT EditorManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(EditorManager)
public:
    void initialize();

    Editor * currentEditor();

signals:
    void currentEditorChanged(Core::Editor *editor);

private:
    explicit EditorManager(QObject *parent = 0);
    ~EditorManager();

    Internal::EditorManagerPrivate *d;
    friend class Internal::EditorManagerPrivate;
}; // end of class Core::EditorManager

} // end of namespace Core

#endif // EDITORMANAGER_H
