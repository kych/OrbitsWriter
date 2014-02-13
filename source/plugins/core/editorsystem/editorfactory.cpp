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

#include "editorfactory.h"

/*!
  \class Core::EditorFactory
  The EditorFactory class is used for creating editor instances.

  Each type of files may associate to an editor. OrbitsWriter
  will use this factory to create this editor.
 */

/*!
  \fn Editor * EditorFactory::createEditor(QWidget *parent)
  Creates an instance of editor with given \a parent as its parent.
 */

/*!
  \fn Id EditorFactory::id() const
  Gets ID of this factory.
 */

using namespace Core;

EditorFactory::EditorFactory(QObject *parent) :
    QObject(parent)
{
}
