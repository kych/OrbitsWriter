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

#include <core/id.h>

#include "htmleditconstants.h"
#include "htmleditor.h"
#include "htmleditorfactory.h"
#include "htmleditwidget.h"

namespace HtmlEdit
{

namespace Internal
{
class HtmlEditorFactoryPrivate
{
public:

}; // end of class HtmlEdit::Internal::HtmlEditorFactoryPrivate
} // end of namespace HtmlEdit::Internal

HtmlEditorFactory::HtmlEditorFactory(QObject *parent) :
    Core::EditorFactory(parent),
    d(new Internal::HtmlEditorFactoryPrivate)
{
}

HtmlEditorFactory::~HtmlEditorFactory()
{
    delete d;
}

Core::Editor *HtmlEditorFactory::createEditor(QWidget *parent)
{
    HtmlEditWidget *editWidget = new HtmlEditWidget(parent);
    return new HtmlEditor(editWidget);
}

Core::Id HtmlEditorFactory::id() const
{
    return Core::Id(Constants::ID_HTMLEDITORFACTORY);
}

} // end of namespace HtmlEdit
