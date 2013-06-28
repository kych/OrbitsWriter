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

#include <QAction>
#include <QApplication>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>

#include "colorbutton.h"
#include "fontchooser.h"
#include "fontsizechooser.h"
#include "mainwindow.h"
#include "previewer.h"
#include "sourceeditor.h"
#include "visualeditor.h"

namespace GOW {

class MainWindow::Private : public QObject
{
    Q_OBJECT
    PRIVATE_Q_POINTER(MainWindow)
public:
    Private(MainWindow *win);

    void setupMenus();
    void setupToolBars();
    void setupStatusBar();
    void setupEditors();

    QAction *newDocAction;
    QAction *openDocAction;
    QAction *closeDocAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;

    QAction *textBoldAction;
    QAction *textItalicAction;
    QAction *textUnderlineAction;
    QAction *textStrikeoutAction;
    QAction *textFontAction;
    QAction *textColorAction;
    QAction *textBackgroundColorAction;
    QAction *alignCenterAction;
    QAction *alignJustifyAction;
    QAction *alignLeftAction;
    QAction *alignRightAction;

    QAction *helpContentAction;
    QAction *aboutAction;

    FontChooser *fontChooser;
    FontSizeChooser *fontSizeChooser;
    ColorButton *textColorButton;
    ColorButton *textBackgroundColorButton;

    QLineEdit *titleEditor;
    VisualEditor *visualEditor;
    SourceEditor *sourceEditor;
    Previewer *previewer;

private:
    void createActions();
}; // end of class GOW::MainWindow::Private

MainWindow::Private::Private(MainWindow *win) :
    QObject(win),
    q(win)
{
}

void MainWindow::Private::setupMenus()
{
    createActions();

    QMenuBar *bar = q->menuBar();

    // Menu File
    QMenu *fileMenu = new QMenu(tr("&File"), q);
    fileMenu->addAction(newDocAction);
    fileMenu->addAction(openDocAction);
    fileMenu->addAction(closeDocAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    bar->addMenu(fileMenu);

    // Menu Edit
    QMenu *editMenu = new QMenu(tr("&Edit"), q);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    bar->addMenu(editMenu);

    // Menu format
    QMenu *formatMenu = new QMenu(tr("F&ormat"), q);
    formatMenu->addAction(textBoldAction);
    formatMenu->addAction(textItalicAction);
    formatMenu->addAction(textUnderlineAction);
    formatMenu->addAction(textStrikeoutAction);
    formatMenu->addSeparator();
    formatMenu->addAction(textFontAction);
    formatMenu->addAction(textColorAction);
    formatMenu->addAction(textBackgroundColorAction);
    formatMenu->addSeparator();
    QMenu *alignMenu = new QMenu(tr("Alignment"), q);
    if (QApplication::isLeftToRight()) {
        alignMenu->addAction(alignLeftAction);
        alignMenu->addAction(alignCenterAction);
        alignMenu->addAction(alignRightAction);
    } else {
        alignMenu->addAction(alignRightAction);
        alignMenu->addAction(alignCenterAction);
        alignMenu->addAction(alignLeftAction);
    }
    alignMenu->addAction(alignJustifyAction);
    formatMenu->addMenu(alignMenu);
    bar->addMenu(formatMenu);

    bar->addSeparator();
    // Menu Help
    QMenu *helpMenu = new QMenu(tr("&Help"), q);
    helpMenu->addAction(helpContentAction);
    helpMenu->addAction(aboutAction);
    bar->addMenu(helpMenu);
}

void MainWindow::Private::setupToolBars()
{
    const int SPACING_WIDTH = 4;

    QToolBar *editBar = new QToolBar(tr("Edit"), q);
    editBar->addAction(newDocAction);
    editBar->layout()->setSpacing(SPACING_WIDTH);
    editBar->addAction(undoAction);
    editBar->addAction(redoAction);
    q->addToolBar(Qt::TopToolBarArea, editBar);

    QToolBar *formatBar = new QToolBar(tr("Format"), q);
    formatBar->layout()->setSpacing(SPACING_WIDTH);
//    m_headingList = new Widgets::HeadingComboBox(this);
//    formatBar->addWidget(m_headingList);
    formatBar->addAction(textBoldAction);
    formatBar->addAction(textItalicAction);
    formatBar->addAction(textUnderlineAction);
    formatBar->addAction(textStrikeoutAction);
    fontChooser = new FontChooser(q);
    formatBar->addWidget(fontChooser);
    fontSizeChooser = new FontSizeChooser(q);
    formatBar->addWidget(fontSizeChooser);
    textColorButton = new ColorButton(q);
    textColorButton->setStandardColors();
    textColorButton->setTipIcon(QIcon(":/image/text_color"));
    textColorButton->setToolTip(tr("Text Color"));
    formatBar->addWidget(textColorButton);
//    m_textColorAction->setIcon(m_textColorButton->icon());
    textBackgroundColorButton = new ColorButton(q);
    textBackgroundColorButton->setStandardColors();
    textBackgroundColorButton->setTipIcon(QIcon(":/image/text_background_color"));
    textBackgroundColorButton->setToolTip(tr("Text Background Color"));
    formatBar->addWidget(textBackgroundColorButton);
//    m_textBackgroundColorAction->setIcon(m_textBackgroundColorButton->icon());
    formatBar->addSeparator();
    if (QApplication::isLeftToRight()) {
        formatBar->addAction(alignLeftAction);
        formatBar->addAction(alignCenterAction);
        formatBar->addAction(alignRightAction);
    } else {
        formatBar->addAction(alignRightAction);
        formatBar->addAction(alignCenterAction);
        formatBar->addAction(alignLeftAction);
    }
    formatBar->addAction(alignJustifyAction);
    q->addToolBar(Qt::TopToolBarArea, formatBar);
}

void MainWindow::Private::setupStatusBar()
{
    QStatusBar *bar = q->statusBar();
    bar->showMessage(tr("Ready."));
}

void MainWindow::Private::setupEditors()
{
    QTabWidget *editorTabs = new QTabWidget(q);
    editorTabs->setTabPosition(QTabWidget::South);

    visualEditor = new VisualEditor(editorTabs);
    visualEditor->setStyleSheet("border: 0");
    editorTabs->addTab(visualEditor, tr("Visual"));

    previewer = new Previewer(editorTabs);
    previewer->setStyleSheet("border: 0");
    editorTabs->addTab(previewer, tr("Preview"));

    sourceEditor = new SourceEditor(editorTabs);
    sourceEditor->setStyleSheet("border: 0");
    editorTabs->addTab(sourceEditor, tr("Source"));

    titleEditor = new QLineEdit(q);
    titleEditor->setFixedHeight(40);
    QFont defaultFont;
    defaultFont.setPointSize(24);
    titleEditor->setFont(defaultFont);
    titleEditor->setStyleSheet("border:2px solid gray;"
                               "border-radius: 10px;"
                               "padding:0 8px;");

    QWidget *editorArea = new QWidget(q);
    QVBoxLayout *editorAreaLayout = new QVBoxLayout(editorArea);
    editorAreaLayout->setContentsMargins(4, 6, 4, 0);
    editorAreaLayout->addWidget(titleEditor);
    editorAreaLayout->addWidget(editorTabs);
    q->setCentralWidget(editorArea);
}

void MainWindow::Private::createActions()
{
    newDocAction = new QAction(QIcon::fromTheme("document-new", QIcon(":/image/doc_new")), tr("&New"), this);
    newDocAction->setShortcut(QKeySequence::New);
    newDocAction->setStatusTip(tr("Create a new post."));

    openDocAction = new QAction(QIcon::fromTheme("document-open", QIcon(":/image/doc_open")), tr("&Open..."), this);
    openDocAction->setShortcut(QKeySequence::Open);
    openDocAction->setStatusTip(tr("Open a post."));

    closeDocAction = new QAction(QIcon::fromTheme("document-close", QIcon(":/image/doc_close")), tr("&Close..."), this);
    closeDocAction->setShortcut(QKeySequence::Close);
    closeDocAction->setStatusTip(tr("Close a post."));

    saveAction = new QAction(QIcon::fromTheme("document-save", QIcon(":/image/doc_save")), tr("Save"), this);
    saveAction->setEnabled(false);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the post."));

    saveAsAction = new QAction(QIcon::fromTheme("document-save-as", QIcon(":/image/doc_save_as")), tr("Save As"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the post as another one."));

    exitAction = new QAction(QIcon::fromTheme("application-exit", QIcon(":/image/app_exit")), tr("E&xit"), this);
    exitAction->setMenuRole(QAction::QuitRole);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit OrbitsWriter."));

    undoAction = new QAction(QIcon::fromTheme("edit-undo", QIcon(":/image/undo")), tr("Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("Undo."));
    undoAction->setEnabled(false);

    redoAction = new QAction(QIcon::fromTheme("edit-redo", QIcon(":/image/redo")), tr("Redo"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("Redo."));
    redoAction->setEnabled(false);

    cutAction = new QAction(QIcon::fromTheme("edit-cut", QIcon(":/image/cut")), tr("Cut"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut."));
    cutAction->setEnabled(false);

    copyAction = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/image/copy")), tr("Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy."));
    copyAction->setEnabled(false);

    pasteAction = new QAction(QIcon::fromTheme("edit-paste", QIcon(":/image/paste")), tr("Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste."));
    pasteAction->setEnabled(false);

    textBoldAction = new QAction(QIcon::fromTheme("format-text-bold", QIcon(":/image/bold")), tr("Bold"), this);
    textBoldAction->setShortcut(Qt::CTRL + Qt::Key_B);
    textBoldAction->setStatusTip(tr("Set text bold."));
    textBoldAction->setCheckable(true);

    textItalicAction = new QAction(QIcon::fromTheme("format-text-italic", QIcon(":/image/italic")), tr("Italic"), this);
    textItalicAction->setShortcut(Qt::CTRL + Qt::Key_I);
    textItalicAction->setStatusTip(tr("Set text italic."));
    textItalicAction->setCheckable(true);

    textUnderlineAction = new QAction(QIcon::fromTheme("format-text-underline", QIcon(":/image/underline")), tr("Underline"), this);
    textUnderlineAction->setShortcut(Qt::CTRL + Qt::Key_U);
    textUnderlineAction->setStatusTip(tr("Add underline."));
    textUnderlineAction->setCheckable(true);

    textStrikeoutAction = new QAction(QIcon::fromTheme("format-text-strikethrough", QIcon(":/image/strike")), tr("Strike"), this);
    textStrikeoutAction->setShortcut(Qt::CTRL + Qt::Key_D);
    textStrikeoutAction->setStatusTip(tr("Strike out."));
    textStrikeoutAction->setCheckable(true);

    textFontAction = new QAction(QIcon(":/image/font"), tr("Font..."), this);
    textFontAction->setStatusTip(tr("Set font."));

    textColorAction = new QAction(QIcon(":/image/text_color"), tr("Text Color..."), this);
    textColorAction->setStatusTip(tr("Text Color."));

    textBackgroundColorAction = new QAction(QIcon(":/image/text_background_color"), tr("Text Background Color..."), this);
    textBackgroundColorAction->setStatusTip(tr("Text background color."));

    QActionGroup *alignGroup = new QActionGroup(this);
    alignCenterAction = new QAction(QIcon::fromTheme("format-justify-center", QIcon(":/image/align_center")), tr("Center"), this);
    alignCenterAction->setStatusTip(tr("Justify center."));
    alignCenterAction->setShortcut(Qt::CTRL + Qt::Key_E);
    alignCenterAction->setCheckable(true);
    alignCenterAction->setActionGroup(alignGroup);

    alignJustifyAction = new QAction(QIcon::fromTheme("format-justify-fill", QIcon(":/image/align_fill")), tr("Fill"), this);
    alignJustifyAction->setStatusTip(tr("Justify fill."));
    alignJustifyAction->setShortcut(Qt::CTRL + Qt::Key_F);
    alignJustifyAction->setCheckable(true);
    alignJustifyAction->setChecked(true);
    alignJustifyAction->setActionGroup(alignGroup);

    alignLeftAction = new QAction(QIcon::fromTheme("format-justify-left", QIcon(":/image/align_left")), tr("Left"), this);
    alignLeftAction->setStatusTip(tr("Justify left."));
    alignLeftAction->setShortcut(Qt::CTRL + Qt::Key_L);
    alignLeftAction->setCheckable(true);
    alignLeftAction->setActionGroup(alignGroup);

    alignRightAction = new QAction(QIcon::fromTheme("format-justify-right", QIcon(":/image/align_right")), tr("Right"), this);
    alignRightAction->setStatusTip(tr("Justify Right."));
    alignRightAction->setShortcut(Qt::CTRL + Qt::Key_R);
    alignRightAction->setCheckable(true);
    alignRightAction->setActionGroup(alignGroup);

    helpContentAction = new QAction(QIcon::fromTheme("help-contents", QIcon(":/image/help")), tr("Help"), this);
    helpContentAction->setShortcut(QKeySequence::HelpContents);
    helpContentAction->setStatusTip(tr("Open help contents."));

    aboutAction = new QAction(QIcon::fromTheme("help-about", QIcon(":/image/about")), tr("About"), this);
    aboutAction->setMenuRole(QAction::AboutRole);
    aboutAction->setStatusTip(tr("About OrbitsWriter."));
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    d(this)
{
    setWindowTitle(tr("OrbitsWriter [*]"));
    setWindowIcon(QIcon(":/image/app_icon"));
    setUnifiedTitleAndToolBarOnMac(true);

    d->setupMenus();
    d->setupToolBars();
    d->setupStatusBar();
    d->setupEditors();
    d->visualEditor->setFocus();
}

MainWindow::~MainWindow()
{
}

}

#include "mainwindow.moc"
