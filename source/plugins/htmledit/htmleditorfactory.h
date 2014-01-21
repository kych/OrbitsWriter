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

#ifndef HTMLEDITORFACTORY_H
#define HTMLEDITORFACTORY_H

#include <core/editorsystem/editorfactory.h>

namespace HtmlEdit {

namespace Internal
{
class HtmlEditorFactoryPrivate;
}

class HtmlEditorFactory : public Core::EditorFactory
{
    Q_OBJECT
public:
    explicit HtmlEditorFactory(QObject *parent = 0);
    ~HtmlEditorFactory();

    Core::Editor *createEditor(QWidget *parent);
    Core::Id id() const;

private:
    Internal::HtmlEditorFactoryPrivate *d;
    friend class Internal::HtmlEditorFactoryPrivate;
}; // end of class HtmlEdit::HtmlEditorFactory

} // end of namespace HtmlEdit

#endif // HTMLEDITORFACTORY_H
