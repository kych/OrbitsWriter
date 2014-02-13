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
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QStaticText>
#include <QTextCharFormat>

#include <core/actionsystem/actioncontainer.h>
#include <core/actionsystem/actionmanager.h>
#include <core/coreconstants.h>
#include <core/context.h>

#include <htmledit/editoraction.h>
#include <htmledit/htmleditconstants.h>
#include <htmledit/htmleditor.h>
#include <htmledit/htmlsourceedit.h>
#include <htmledit/htmlvisualedit.h>

#include "formattingconstants.h"
#include "formattingplugin.h"

namespace Formatting
{

// Icons
static const char ICON_TEXTBOLD[]      = ":/img/bold";
static const char ICON_TEXTITALIC[]    = ":/img/italic";
static const char ICON_TEXTSTRIKE[]    = ":/img/strike";
static const char ICON_TEXTUNDERLINE[] = ":/img/underline";
static const char ICON_TEXTFONT[]      = ":/img/font";
static const char ICON_ALIGNCENTER[]   = ":/img/align_center";
static const char ICON_ALIGNLEFT[]     = ":/img/align_left";
static const char ICON_ALIGNRIGHT[]    = ":/img/align_right";
static const char ICON_ALIGNJUSTIFY[]  = ":/img/align_justify";

namespace Internal
{

class TextFormatAction : public HtmlEdit::EditorAction
{
public:
    enum TextFormatType {
        Bold,
        Italic,
        Underline,
        Strike
    };

    TextFormatAction(TextFormatType type, bool enable)
    {
        m_type = type;
        m_enable = enable;
    }

    bool hasVisualEditorAction()
    {
        return true;
    }

    void doActionOnVisualEditor(HtmlEdit::HtmlVisualEdit *visualEdit)
    {
        QTextCharFormat fmt;
        switch (m_type) {
        case Bold:
        {
            fmt.setFontWeight(m_enable ? QFont::Bold : QFont::Normal);
            break;
        }
        case Italic:
        {
            fmt.setFontItalic(m_enable);
            break;
        }
        case Underline:
        {
            fmt.setFontUnderline(m_enable);
            break;
        }
        case Strike:
        {
            fmt.setFontStrikeOut(m_enable);
            break;
        }
        }
        //visualEdit->mergeFormatOnWordOrSelection(fmt);
    }

    bool hasSourceEditorAction()
    {
        return false;
    }

private:
    TextFormatType m_type;
    bool m_enable;
}; // end of class Formatting::Internal::TextFormatAction

class TextAlignmentAction : public HtmlEdit::EditorAction
{
public:
    TextAlignmentAction(Qt::Alignment alignment)
    {
        m_alignment = alignment;
    }

    bool hasVisualEditorAction()
    {
        return true;
    }

    void doActionOnVisualEditor(HtmlEdit::HtmlVisualEdit *visualEdit)
    {
        //visualEdit->setAlignment(m_alignment);
    }

    bool hasSourceEditorAction()
    {
        return false;
    }

private:
    Qt::Alignment m_alignment;
}; // end of class Formatting::Internal::TextAlignmentAction

class IconUtil
{
public:
    static QPixmap textColorIcon()
    {
        QPixmap pixmap(32, 32);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QColor::fromRgb(38, 115, 238));
        QFont font;
        font.setWeight(QFont::Black);
        font.setPointSizeF(28);
        painter.setFont(font);
        painter.drawText(0, 0, 32, 32, Qt::AlignCenter, QLatin1String("A"));
        return pixmap;
    }

    static QPixmap textBackgroundColorIcon()
    {
        QPixmap pixmap(32, 32);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.fillRect(0, 0, 32, 32, QColor::fromRgb(255, 245, 0));
        QFont font;
        font.setWeight(QFont::Black);
        font.setPointSizeF(28);
        painter.setFont(font);
        painter.drawText(0, 0, 32, 32, Qt::AlignCenter, QLatin1String("A"));
        return pixmap;
    }
}; // end of class Formatting::Internal::IconUtil

class FormattingPluginPrivate : public QObject
{
    Q_OBJECT
public:
    FormattingPluginPrivate() :
        m_textBoldAction(0),
        m_textItalicAction(0),
        m_textStrikeAction(0),
        m_textUnderlineAction(0),
        m_textFontAction(0),
        m_textColorAction(0),
        m_textBackgroundAction(0),
        m_alignLeftAction(0),
        m_alignCenterAction(0),
        m_alignRightAction(0),
        m_alignJustifyAction(0),
        m_alignActionGroup(0)
    {
    }

    void init();

    QAction      *m_textBoldAction;
    QAction      *m_textItalicAction;
    QAction      *m_textStrikeAction;
    QAction      *m_textUnderlineAction;
    QAction      *m_textFontAction;
    QAction      *m_textColorAction;
    QAction      *m_textBackgroundAction;
    QAction      *m_alignLeftAction;
    QAction      *m_alignCenterAction;
    QAction      *m_alignRightAction;
    QAction      *m_alignJustifyAction;
    QActionGroup *m_alignActionGroup;

public slots:
    void setTextBold(bool bold);
    void setTextItalic(bool italic);
    void setTextStrike(bool strike);
    void setTextUnderline(bool underline);

    void setAlignLeft();
    void setAlignCenter();
    void setAlignRight();
    void setAlignJustify();
}; // end of class Formatting::Internal::FormattingPluginPrivate

void FormattingPluginPrivate::init()
{
    Core::Context ctx(Constants::CONTEXT_FORMATTING, HtmlEdit::Constants::CONTEXT_HTMLVISUALEDITOR);
    Core::ActionContainer *formatMenu = gActionManager->actionContainer(Core::Constants::M_FORMAT);
    Core::ActionContainer *toolBar = gActionManager->actionContainer(Core::Constants::DEFAULT_TOOL_BAR);

    // Text Bold Action
    QIcon icon = QIcon(QLatin1String(ICON_TEXTBOLD));
    m_textBoldAction = new QAction(icon, tr("&Bold"), this);
    m_textBoldAction->setCheckable(true);
    m_textBoldAction->setShortcut(Qt::CTRL + Qt::Key_B);
    Core::Command *cmdTextBold = gActionManager->registerAction(m_textBoldAction,
                                                                Constants::ID_TEXTBOLD,
                                                                ctx);
    formatMenu->addAction(cmdTextBold, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextBold, Core::Constants::GT_FORMAT);
    connect(m_textBoldAction, SIGNAL(toggled(bool)),
            this, SLOT(setTextBold(bool)));

    // Text Italic Action
    icon = QIcon(QLatin1String(ICON_TEXTITALIC));
    m_textItalicAction = new QAction(icon, tr("&Italic"), this);
    m_textItalicAction->setShortcut(Qt::CTRL + Qt::Key_I);
    m_textItalicAction->setCheckable(true);
    Core::Command *cmdTextItalic = gActionManager->registerAction(m_textItalicAction,
                                                                  Constants::ID_TEXTITALIC,
                                                                  ctx);
    formatMenu->addAction(cmdTextItalic, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextItalic, Core::Constants::GT_FORMAT);
    connect(m_textItalicAction, SIGNAL(toggled(bool)),
            this, SLOT(setTextItalic(bool)));

    // Text Strike Action
    icon = QIcon(QLatin1String(ICON_TEXTSTRIKE));
    m_textStrikeAction = new QAction(icon, tr("Strike"), this);
    m_textStrikeAction->setShortcut(Qt::CTRL + Qt::Key_D);
    m_textStrikeAction->setCheckable(true);
    Core::Command *cmdTextStrike = gActionManager->registerAction(m_textStrikeAction,
                                                                  Constants::ID_TEXTSTRIKE,
                                                                  ctx);
    formatMenu->addAction(cmdTextStrike, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextStrike, Core::Constants::GT_FORMAT);
    connect(m_textStrikeAction, SIGNAL(toggled(bool)),
            this, SLOT(setTextStrike(bool)));

    // Text Underline Action
    icon = QIcon(QLatin1String(ICON_TEXTUNDERLINE));
    m_textUnderlineAction = new QAction(icon, tr("&Underline"), this);
    m_textUnderlineAction->setShortcut(Qt::CTRL + Qt::Key_U);
    m_textUnderlineAction->setCheckable(true);
    Core::Command *cmdTextUnderline = gActionManager->registerAction(m_textUnderlineAction,
                                                                     Constants::ID_TEXTUNDERLINE,
                                                                     ctx);
    formatMenu->addAction(cmdTextUnderline, Core::Constants::G_FORMAT_TEXTSTYLE);
    toolBar->addAction(cmdTextUnderline, Core::Constants::GT_FORMAT);
    connect(m_textUnderlineAction, SIGNAL(toggled(bool)),
            this, SLOT(setTextUnderline(bool)));

    // Text Font Action
    icon = QIcon(QLatin1String(ICON_TEXTFONT));
    m_textFontAction = new QAction(icon, tr("&Font..."), this);
    Core::Command *cmdTextFont = gActionManager->registerAction(m_textFontAction,
                                                                Constants::ID_TEXTFONT,
                                                                ctx);
    formatMenu->addAction(cmdTextFont, Core::Constants::G_FORMAT_FONTCOLOR);
    // Text Font Action on Tool Bar
    // Text Size Action on Tool Bar
    // Text Color Action
    icon = QIcon(IconUtil::textColorIcon());
    m_textColorAction = new QAction(icon, tr("Text Color..."), this);
    Core::Command *cmdTextColor = gActionManager->registerAction(m_textColorAction,
                                                                 Constants::ID_TEXTCOLOR,
                                                                 ctx);
    formatMenu->addAction(cmdTextColor, Core::Constants::G_FORMAT_FONTCOLOR);
    // Text Color Action on Tool Bar
    // Text Background Color Action
    icon = QIcon(IconUtil::textBackgroundColorIcon());
    m_textBackgroundAction = new QAction(icon, tr("Text Background Color..."), this);
    Core::Command *cmdTextBackgroundColor = gActionManager->registerAction(m_textBackgroundAction,
                                                                           Constants::ID_TEXTBACKGROUNDCOLOR,
                                                                           ctx);
    formatMenu->addAction(cmdTextBackgroundColor, Core::Constants::G_FORMAT_FONTCOLOR);
    // Text Background Color Action on Tool Bar

    // Alignment Left Action
    icon = QIcon(QLatin1String(ICON_ALIGNLEFT));
    m_alignLeftAction = new QAction(icon, tr("Align Left"), this);
    m_alignLeftAction->setCheckable(true);
    Core::Command *cmdAlignLeft = gActionManager->registerAction(m_alignLeftAction,
                                                                 Constants::ID_ALIGNLEFT,
                                                                 ctx);
    connect(m_alignLeftAction, SIGNAL(toggled(bool)),
            this, SLOT(setAlignLeft()));

    // Alignment Center Action
    icon = QIcon(QLatin1String(ICON_ALIGNCENTER));
    m_alignCenterAction = new QAction(icon, tr("Align Center"), this);
    m_alignCenterAction->setCheckable(true);
    Core::Command *cmdAlignCenter = gActionManager->registerAction(m_alignCenterAction,
                                                                   Constants::ID_ALIGNCENTER,
                                                                   ctx);
    connect(m_alignCenterAction, SIGNAL(toggled(bool)),
            this, SLOT(setAlignCenter()));

    // Alignment Right Action
    icon = QIcon(QLatin1String(ICON_ALIGNRIGHT));
    m_alignRightAction = new QAction(icon, tr("Align Right"), this);
    m_alignRightAction->setCheckable(true);
    Core::Command *cmdAlignRight = gActionManager->registerAction(m_alignRightAction,
                                                                  Constants::ID_ALIGNRIGHT,
                                                                  ctx);
    connect(m_alignRightAction, SIGNAL(toggled(bool)),
            this, SLOT(setAlignRight()));

    if (QApplication::isLeftToRight()) {
        formatMenu->addAction(cmdAlignLeft, Core::Constants::G_FORMAT_ALIGNMENT);
        toolBar->addAction(cmdAlignLeft, Core::Constants::GT_FORMAT);
        formatMenu->addAction(cmdAlignCenter, Core::Constants::G_FORMAT_ALIGNMENT);
        toolBar->addAction(cmdAlignCenter, Core::Constants::GT_FORMAT);
        formatMenu->addAction(cmdAlignRight, Core::Constants::G_FORMAT_ALIGNMENT);
        toolBar->addAction(cmdAlignRight, Core::Constants::GT_FORMAT);
    } else {
        formatMenu->addAction(cmdAlignRight, Core::Constants::G_FORMAT_ALIGNMENT);
        toolBar->addAction(cmdAlignRight, Core::Constants::GT_FORMAT);
        formatMenu->addAction(cmdAlignCenter, Core::Constants::G_FORMAT_ALIGNMENT);
        toolBar->addAction(cmdAlignCenter, Core::Constants::GT_FORMAT);
        formatMenu->addAction(cmdAlignLeft, Core::Constants::G_FORMAT_ALIGNMENT);
        toolBar->addAction(cmdAlignLeft, Core::Constants::GT_FORMAT);
    }

    // Alignment Fill Action
    icon = QIcon(QLatin1String(ICON_ALIGNJUSTIFY));
    m_alignJustifyAction = new QAction(icon, tr("Align Justify"), this);
    m_alignJustifyAction->setCheckable(true);
    Core::Command *cmdAlignFill = gActionManager->registerAction(m_alignJustifyAction,
                                                                 Constants::ID_ALIGNFILL,
                                                                 ctx);
    formatMenu->addAction(cmdAlignFill, Core::Constants::G_FORMAT_ALIGNMENT);
    toolBar->addAction(cmdAlignFill, Core::Constants::GT_FORMAT);
    connect(m_alignJustifyAction, SIGNAL(toggled(bool)),
            this, SLOT(setAlignJustify()));

    m_alignActionGroup = new QActionGroup(this);
    m_alignActionGroup->addAction(m_alignLeftAction);
    m_alignActionGroup->addAction(m_alignCenterAction);
    m_alignActionGroup->addAction(m_alignRightAction);
    m_alignActionGroup->addAction(m_alignJustifyAction);
}

void FormattingPluginPrivate::setTextBold(bool bold)
{
    TextFormatAction action(TextFormatAction::Bold, bold);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setTextItalic(bool italic)
{
    TextFormatAction action(TextFormatAction::Italic, italic);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setTextStrike(bool strike)
{
    TextFormatAction action(TextFormatAction::Strike, strike);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setTextUnderline(bool underline)
{
    TextFormatAction action(TextFormatAction::Underline, underline);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setAlignLeft()
{
    TextAlignmentAction action(Qt::AlignLeft | Qt::AlignAbsolute);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setAlignCenter()
{
    TextAlignmentAction action(Qt::AlignHCenter);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setAlignRight()
{
    TextAlignmentAction action(Qt::AlignRight | Qt::AlignAbsolute);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

void FormattingPluginPrivate::setAlignJustify()
{
    TextAlignmentAction action(Qt::AlignJustify);
    HtmlEdit::HtmlEditor::mergeFormat(&action);
}

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
