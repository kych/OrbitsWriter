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

#include <commons/objectpool.h>

#include "appcore.h"
#include "context.h"
#include "editorfactory.h"
#include "editormanager.h"
#include "editormanager_p.h"
#include "id.h"

using namespace Core;
using namespace Core::Internal;

template <class EditorFactory>
EditorFactory *findEditorFactoryById(const Id &id)
{
    const QList<EditorFactory *> factories = gObjectPool->getObjects<EditorFactory>();
    foreach (EditorFactory *factory, factories) {
        if (id == factory->id()) {
            return factory;
        }
    }
    return 0;
}

// ========== Core::Internal::EditorManagerPrivate ========== //

EditorManagerPrivate::EditorManagerPrivate(EditorManager *mgr) :
    q(mgr),
    currEditor(0)
{
}

EditorManagerPrivate::~EditorManagerPrivate()
{
}

void EditorManagerPrivate::refreshActions()
{

}

void EditorManagerPrivate::setCurrentEditor(Editor *editor)
{
    if (currEditor == editor) {
        return;
    }
    currEditor = editor;
    refreshActions();
    emit q->currentEditorChanged(editor);
}

Editor *EditorManagerPrivate::createEditor(const Id &id)
{
    EditorFactory *factory = findEditorFactoryById<EditorFactory>(id);
    Q_ASSERT_X(factory != 0, "currentEditor", "No any editor factory registered.");
    currEditor = factory->createEditor(gCore->mainWindow());
//    setEditorActions(currEditor->editorActions());
    return currEditor;
}

void EditorManagerPrivate::contextAboutToChange(WidgetContext *context)
{
    Editor *editor = context ? qobject_cast<Editor *>(context) : 0;
    if (editor) {
        setCurrentEditor(editor);
    } else {
//        updateActions();
    }
}

// ========== Core::EditorManager ========== //

/*!
  \class Core::EditorManager
  The EditorManager class is the manager for editors.

  The main editor is HTML editor provided by HtmlEdit plugin. New editors
  should add itself to object pool in order to create the instance.
 */

GET_INSTANCE(EditorManager)

EditorManager::EditorManager(QObject *parent) :
    QObject(parent),
    d(new Internal::EditorManagerPrivate(this))
{
}

EditorManager::~EditorManager()
{
    delete d;
    d = 0;
}

void EditorManager::initialize()
{
//    d->init();

    connect(gCore, SIGNAL(contextAboutToChange(WidgetContext*)),
            d, SLOT(contextAboutToChange(WidgetContext*)));
}

Editor *EditorManager::currentEditor()
{
    if (!d->currEditor) {
        static const char ID_HTMLVISUALEDITORFACTORY[] = "HtmlVisualEditorFactory ID";
        d->createEditor(Id(ID_HTMLVISUALEDITORFACTORY));
    }
    return d->currEditor;
}
