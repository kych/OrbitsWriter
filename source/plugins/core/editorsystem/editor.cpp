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

#include "editor.h"

/*!
  \class Core::Editor
  Editor for the application.

  Editors are used for editing document of Orbits Writer. Editors could
  be provided by plugins. Editor plugins should have following classes:

  \li \c Editor editor instance.
  \li \c EditorFactory factory to create this editor.

  Editors provide document editing operations, such as text format,
  text alignments, color and font, etc. Editor actions will be callback
  through editActionEvent() function.

  Editor widget should be returned by widget() function.
 */
