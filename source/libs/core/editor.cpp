/*-------------------------------------------------
 *
 * OrbitsWriter - An Offline Blog Writer
 *
 * Copyright (C) 2012 devbean@galaxyworld.org
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

/*!
  \enum GOW::TextAlignment

  Text alignment.
 */

/*!
  \var GOW::TextAlignment GOW::AlignJustify

  Text alignment jistify.
 */

/*!
  \var GOW::TextAlignment GOW::AlignLeft

  Text alignment left.
 */

/*!
  \var GOW::TextAlignment GOW::AlignRight

  Text alignment right.
 */

/*!
  \var GOW::TextAlignment GOW::AlignCenter

  Text alignment center.
 */

/*!
  \class GOW::Editor

  The editor interface for editors.
 */

/*!
  \fn void GOW::Editor::setTextBold(bool bold)

  Sets text is bold or not.
 */

/*!
  \fn void GOW::Editor::setTextItalic(bool bold)

  Sets text is italic or not.
 */

/*!
  \fn void GOW::Editor::setTextUnderline(bool bold)

  Sets text is underline or not.
 */

/*!
  \fn void GOW::Editor::setTextStrikeOut(bool bold)

  Sets text is strike out or not.
 */

/*!
  \fn void GOW::Editor::setTextAlign(TextAlignment alignment)

  Sets text alignment to \a alignment.
 */

/*!
  \fn void GOW::Editor::setTextColor(const QColor &color)

  Sets text color to \a color.
 */

/*!
  \fn void GOW::Editor::setTextBackgroundColor(const QColor &color)

  Sets text background color to \a color.
 */
