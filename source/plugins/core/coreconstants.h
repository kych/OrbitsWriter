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

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Core {

/*!
  The Constants namespace contains constants using in Core library and
  maybe need for other libraries and plugins.
 */
namespace Constants {

// Context
//! Global context.
const char CONTEXT_GLOBAL[]                       = "Global Context";
//! Editor manager context.
const char CONTEXT_EDITORMANAGER[]                = "Editor Manager Context";
//! Editor context.
const char CONTEXT_EDITOR[]                       = "Editor Context";

// Menu Bar
//! Unique identifier name for menu bar.
const char MENU_BAR[]                             = "OrbitsWriter.MenuBar";

// Menus
//! Unique identifier name for file menu on menu bar.
const char M_FILE[]                               = "OrbitsWriter.Menu.File";
//! Unique identifier name for edit menu on menu bar.
const char M_EDIT[]                               = "OrbitsWriter.Menu.Edit";
//! Unique identifier name for format menu on menu bar.
const char M_FORMAT[]                             = "OrbitsWriter.Menu.Format";
//! Unique identifier name for insert menu on menu bar.
const char M_INSERT[]                             = "OrbitsWriter.Menu.Insert";
//! Unique identifier name for tools menu on menu bar.
const char M_TOOLS[]                              = "OrbitsWriter.Menu.Tools";
//! Unique identifier name for help menu on menu bar.
const char M_HELP[]                               = "OrbitsWriter.Menu.Help";

// Tool Bar
//! Unique identifier name for default tool bar.
const char DEFAULT_TOOL_BAR[]                     = "OrbitsWriter.DefaultToolBar";

// Default Action Group
//! Unique identifier name for default action group level 1.
const char G_DEFAULT_ACTIONGROUP_1[]              = "OrbitsWriter.ActionGroup.Default.One";
//! Unique identifier name for default action group level 2.
const char G_DEFAULT_ACTIONGROUP_2[]              = "OrbitsWriter.ActionGroup.Default.Two";
//! Unique identifier name for default action group level 3.
const char G_DEFAULT_ACTIONGROUP_3[]              = "OrbitsWriter.ActionGroup.Default.Three";

// Action Groups
//! Unique identifier name for file action group.
const char GM_FILE[]                              = "OrbitsWriter.ActionGroup.File";
//! Unique identifier name for edit action group.
const char GM_EDIT[]                              = "OrbitsWriter.ActionGroup.Edit";
//! Unique identifier name for format action group.
const char GM_FORMAT[]                            = "OrbitsWriter.ActionGroup.Format";
//! Unique identifier name for insert action group.
const char GM_INSERT[]                            = "OrbitsWriter.ActionGroup.Insert";
//! Unique identifier name for tools action group.
const char GM_TOOLS[]                             = "OrbitsWriter.ActionGroup.Tools";
//! Unique identifier name for help action group.
const char GM_HELP[]                              = "OrbitsWriter.ActionGroup.Help";

// Tool Bar Groups
//! Unique identifier name for file action group on tool bar.
const char GT_FILE[]                              = "OrbitsWriter.ActionGroup.ToolBar.File";
//! Unique identifier name for format action group on tool bar.
const char GT_FORMAT[]                            = "OrbitsWriter.ActionGroup.ToolBar.Format";
//! Unique identifier name for undo/redo action group on tool bar.
const char GT_UNDOREDO[]                          = "OrbitsWriter.ActionGroup.ToolBar.UndoRedo";
//! Unique identifier name for copy/paste action group on tool bar.
const char GT_COPYPASTE[]                         = "OrbitsWriter.ActionGroup.ToolBar.CopyPaste";

// Menu Groups
//! Unique identifier name for creating new document under file action group.
const char G_FILE_NEW[]                           = "OrbitsWriter.ActionGroup.File.New";
//! Unique identifier name for opening a document under file action group.
const char G_FILE_OPEN[]                          = "OrbitsWriter.ActionGroup.File.Open";
//! Unique identifier name for closing a document under file action group.
const char G_FILE_CLOSE[]                         = "OrbitsWriter.ActionGroup.File.Close";
//! Unique identifier name for saving a document under file action group.
const char G_FILE_SAVE[]                          = "OrbitsWriter.ActionGroup.File.Save";
//! Unique identifier name for other actions under file action group.
const char G_FILE_OTHER[]                         = "OrbitsWriter.ActionGroup.File.Other";

//! Unique identifier name for undo/redo under edit action group.
const char G_EDIT_UNDOREDO[]                      = "OrbitsWriter.ActionGroup.Edit.UndoRedo";
//! Unique identifier name for cut/copy/paste under edit action group.
const char G_EDIT_COPYPASTE[]                     = "OrbitsWriter.ActionGroup.Edit.CopyPaste";
//! Unique identifier name for other actions under edit action group.
const char G_EDIT_OTHER[]                         = "OrbitsWriter.ActionGroup.Edit.Other";

//! Unique identifier name for main tools under format action group.
const char G_FORMAT_MAIN[]                        = "OrbitsWriter.ActionGroup.Format.Main";
//! Unique identifier name for text style under format action group.
const char G_FORMAT_TEXTSTYLE[]                   = "OrbitsWriter.ActionGroup.Format.TextStyle";
//! Unique identifier name for font/color under format action group.
const char G_FORMAT_FONTCOLOR[]                   = "OrbitsWriter.ActionGroup.Format.FontColor";
//! Unique identifier name for alignment under format action group.
const char G_FORMAT_ALIGNMENT[]                   = "OrbitsWriter.ActionGroup.Format.Alignment";
//! Unique identifier name for other actions under format action group.
const char G_FORMAT_OTHER[]                       = "OrbitsWriter.ActionGroup.Format.Other";

//! Unique identifier name for other actions under insert action group.
const char G_INSERT_MAIN[]                        = "OrbitsWriter.ActionGroup.Insert.Main";

//! Unique identifier name for option under tools action group.
const char G_TOOLS_OPTION[]                       = "OrbitsWriter.ActionGroup.Tools.Option";

//! Unique identifier name for help contents under help action group.
const char G_HELP_HELPCONTENTS[]                  = "OrbitsWriter.ActionGroup.Help.HelpContents";
//! Unique identifier name for showing about information under help action group.
const char G_HELP_ABOUT[]                         = "OrbitsWriter.ActionGroup.Help.About";

// Action Unique Identifiers
//! Unique identifier name for creating document action.
const char ID_NEWDOC[]                            = "OrbitsWriter.NewDoc";
//! Unique identifier name for opening document action.
const char ID_OPENDOC[]                           = "OrbitsWriter.OpenDoc";
//! Unique identifier name for saving document action.
const char ID_SAVEDOC[]                           = "OrbitsWriter.SaveDoc";
//! Unique identifier name for saving as document action.
const char ID_SAVEASDOC[]                         = "OrbitsWriter.SaveAsDoc";
//! Unique identifier name for exiting application.
const char ID_EXIT[]                              = "OrbitsWriter.Exit";
//! Unique identifier name for undo.
const char ID_UNDO[]                              = "OrbitsWriter.Undo";
//! Unique identifier name for redo.
const char ID_REDO[]                              = "OrbitsWriter.Redo";
//! Unique identifier name for cutting.
const char ID_CUT[]                               = "OrbitsWriter.Cut";
//! Unique identifier name for copying.
const char ID_COPY[]                              = "OrbitsWriter.Copy";
//! Unique identifier name for pasting.
const char ID_PASTE[]                             = "OrbitsWriter.Paste";
//! Unique identifier name for option.
const char ID_OPTION[]                            = "OrbitsWriter.Option";
//! Unique identifier name for help contents.
const char ID_HELPCONTENTS[]                      = "OrbitsWriter.HelpContents";
//! Unique identifier name for about.
const char ID_ABOUT[]                             = "OrbitsWriter.About";
//! Unique identifier name for about plugins.
const char ID_ABOUTPLUGINS[]                      = "OrbitsWriter.AboutPlugins";

} // end of namespace Core::Constants
} // end of namespace Core

#endif // CONSTANTS_H
