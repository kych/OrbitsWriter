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

#include "corelistener.h"

using namespace Core;

/*!
  \class Core::CoreListener
  The CoreListener class provides a hook for plugins to veto on certain events
  emitted from the core plugin.

  You implement this interface if you want to prevent certain events from
  occurring, e.g. if you want to prevent the closing of the whole application
  or to prevent the closing of an editor window under certain conditions.

  If e.g. the application window requests a close, then first
  CoreListener::coreAboutToClose() is called (in arbitrary order) on all
  registered objects implementing this interface. If one of these calls returns
  false, the process is aborted and the event is ignored. If all calls return
  true, the corresponding signal is emitted and the event is accepted/performed.

  In order to notice application about this interface, you should add your
  implementing object to object pool using
  gCore->objectPool()->addObject(yourImplementingObject);
  and you must remove the object at deconstruction
  (the best place is in the destructor of your plugin).
  If you try to prevent the event, just return false from the override functions.
 */

/*!
  \fn CoreListener::editorAboutToClose(Editor *)
  Callbacks when editor is about to close.
 */

/*!
  \fn CoreListener::mainWindowAboutToClose()
  Callbacks when main window is about to close.
 */
