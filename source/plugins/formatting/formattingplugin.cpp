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

#include <QAction>

#include <core/actionsystem/actioncontainer.h>
#include <core/actionsystem/actionmanager.h>
#include <core/coreconstants.h>
#include <core/context.h>

#include <htmledit/htmleditconstants.h>

#include "formattingconstants.h"
#include "formattingplugin.h"

namespace Formatting
{

// Icons
static const char ICON_TEXTBOLD[]                   = ":/img/bold";
static const char ICON_TEXTITALIC[]                 = ":/img/italic";
static const char ICON_TEXTSTRIKE[]                 = ":/img/strike";
static const char ICON_TEXTUNDERLINE[]              = ":/img/underline";

namespace Internal
{
class FormattingPluginPrivate : public QObject
{
    Q_OBJECT
public:
    FormattingPluginPrivate() :
        textBoldAction(0),
        textItalicAction(0),
        textStrikeAction(0),
        textUnderlineAction(0)
    {
    }

    void init();

    QAction      *textBoldAction;
    QAction      *textItalicAction;
    QAction      *textStrikeAction;
    QAction      *textUnderlineAction;
};

void FormattingPluginPrivate::init()
{
    Core::Context ctx(Constants::CONTEXT_FORMATTING, HtmlEdit::Constants::CONTEXT_HTMLVISUALEDITOR);
    Core::ActionContainer *formatMenu = gActionManager->actionContainer(Core::Constants::M_FORMAT);
    Core::ActionContainer *toolBar = gActionManager->actionContainer(Core::Constants::DEFAULT_TOOL_BAR);

    // Text Bold Action
    QIcon icon = QIcon(QLatin1String(ICON_TEXTBOLD));
    textBoldAction = new QAction(icon, tr("&Bold"), this);
    textBoldAction->setCheckable(true);
    textBoldAction->setShortcut(Qt::CTRL + Qt::Key_B);
    Core::Command *cmdTextBold = gActionManager->registerAction(textBoldAction,
                                                                Constants::ID_TEXTBOLD,
                                                                ctx);
    formatMenu->addAction(cmdTextBold, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextBold, Core::Constants::GT_FORMAT);

    // Text Italic Action
    icon = QIcon(QLatin1String(ICON_TEXTITALIC));
    textItalicAction = new QAction(icon, tr("&Italic"), this);
    textItalicAction->setShortcut(Qt::CTRL + Qt::Key_I);
    textItalicAction->setCheckable(true);
    Core::Command *cmdTextItalic = gActionManager->registerAction(textItalicAction,
                                                                  Constants::ID_TEXTITALIC,
                                                                  ctx);
    formatMenu->addAction(cmdTextItalic, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextItalic, Core::Constants::GT_FORMAT);

    // Text Strike Action
    icon = QIcon(QLatin1String(ICON_TEXTSTRIKE));
    textStrikeAction = new QAction(icon, tr("Strike"), this);
    textStrikeAction->setShortcut(Qt::CTRL + Qt::Key_D);
    textStrikeAction->setCheckable(true);
    Core::Command *cmdTextStrike = gActionManager->registerAction(textStrikeAction,
                                                                  Constants::ID_TEXTSTRIKE,
                                                                  ctx);
    formatMenu->addAction(cmdTextStrike, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextStrike, Core::Constants::GT_FORMAT);

    // Text Underline Action
    icon = QIcon(QLatin1String(ICON_TEXTUNDERLINE));
    textUnderlineAction = new QAction(icon, tr("&Underline"), this);
    textUnderlineAction->setShortcut(Qt::CTRL + Qt::Key_U);
    textUnderlineAction->setCheckable(true);
    Core::Command *cmdTextUnderline = gActionManager->registerAction(textUnderlineAction,
                                                                     Constants::ID_TEXTUNDERLINE,
                                                                     ctx);
    formatMenu->addAction(cmdTextUnderline, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextUnderline, Core::Constants::GT_FORMAT);
}

// end of class Formatting::Internal::FormattingPluginPrivate
} // end of namespace Formatting::Internal

FormattingPlugin::FormattingPlugin() :
    d(new Internal::FormattingPluginPrivate)
{
}

FormattingPlugin::~FormattingPlugin()
{
    delete d;
}

bool FormattingPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    d->init();
    return true;
}

void FormattingPlugin::dependenciesInitialized()
{

}

} // end of namespace Formatting

#include "formattingplugin.moc"

Q_EXPORT_PLUGIN2(FormattingPlugin, Formatting::FormattingPlugin)
