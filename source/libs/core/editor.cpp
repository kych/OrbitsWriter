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
  \enum Orbits::TextAlignment

  Text alignment.
 */

/*!
  \var Orbits::TextAlignment Orbits::AlignJustify

  Text alignment jistify.
 */

/*!
  \var Orbits::TextAlignment Orbits::AlignLeft

  Text alignment left.
 */

/*!
  \var Orbits::TextAlignment Orbits::AlignRight

  Text alignment right.
 */

/*!
  \var Orbits::TextAlignment Orbits::AlignCenter

  Text alignment center.
 */

/*!
  \class Orbits::Editor

  The editor interface for editors.
 */

/*!
  \fn void Orbits::Editor::setTextBold(bool bold)

  Sets text is bold or not.
 */

/*!
  \fn void Orbits::Editor::setTextItalic(bool bold)

  Sets text is italic or not.
 */

/*!
  \fn void Orbits::Editor::setTextUnderline(bool bold)

  Sets text is underline or not.
 */

/*!
  \fn void Orbits::Editor::setTextStrikeOut(bool bold)

  Sets text is strike out or not.
 */

/*!
  \fn void Orbits::Editor::setTextAlign(TextAlignment alignment)

  Sets text alignment to \a alignment.
 */

/*!
  \fn void Orbits::Editor::setTextColor(const QColor &color)

  Sets text color to \a color.
 */

/*!
  \fn void Orbits::Editor::setTextBackgroundColor(const QColor &color)

  Sets text background color to \a color.
 */
