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
#include <QDebug>
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

#define currentEditor (dynamic_cast<GOW::Editor *>(editorTabs->currentWidget()))

class MainWindow::Private : public QObject
{
    Q_OBJECT
    Q_POINTER(MainWindow)
public:
    Private(MainWindow *q_ptr);

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
    QAction *textStrikeOutAction;
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

    QTabWidget *editorTabs;
    QLineEdit *titleEditor;
    VisualEditor *visualEditor;
    SourceEditor *sourceEditor;
    Previewer *previewer;

private slots:
    void textBold();
    void textItalic();
    void textStrikeOut();
    void textUnderline();
    void textAlign(QAction *action);

    void cursorPositionChanged();
    void currentCharFormatChanged(const QTextCharFormat &format);

    void fontFamilyActivated(const QString &family);
    void fontSizeActivated(int size);

private:
    void createActions();
    void alignmentChanged(Qt::Alignment align);
    void fontChanged(const QFont &font);
}; // end of class GOW::MainWindow::Private

MainWindow::Private::Private(MainWindow *q_ptr) :
    QObject(q_ptr),
    q(q_ptr)
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
    formatMenu->addAction(textStrikeOutAction);
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
    formatBar->addAction(textStrikeOutAction);
    fontChooser = new FontChooser(q);
    formatBar->addWidget(fontChooser);
    connect(fontChooser, SIGNAL(fontFamilyActivated(QString)),
            this, SLOT(fontFamilyActivated(QString)));
    fontSizeChooser = new FontSizeChooser(q);
    formatBar->addWidget(fontSizeChooser);
    connect(fontSizeChooser, SIGNAL(fontSizeActivated(int)),
            this, SLOT(fontSizeActivated(int)));
    textColorButton = new ColorButton(q);
    textColorButton->setStandardColors();
    textColorButton->setTipIcon(QIcon(":/image/text_color"));
    textColorButton->setToolTip(tr("Text Color"));
    formatBar->addWidget(textColorButton);
//    m_textColorAction->setIcon(m_textColorButton->icon());
    textBackgroundColorButton = new ColorButton(q);
    textBackgroundColorButton->setStandardColors();
    textBackgroundColorButton->setCurrentColor(Qt::white);
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
    editorTabs = new QTabWidget(q);
    editorTabs->setTabPosition(QTabWidget::South);

    visualEditor = new VisualEditor(editorTabs);
    visualEditor->setStyleSheet("border: 0");
    editorTabs->addTab(visualEditor, tr("Visual"));
    connect(visualEditor, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
    connect(visualEditor, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));

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

#define FORMAT_FUNC(ACTION) \
    void MainWindow::Private::ACTION() \
    { \
        currentEditor->ACTION(ACTION##Action->isChecked()); \
    }

FORMAT_FUNC(textBold)
FORMAT_FUNC(textItalic)
FORMAT_FUNC(textStrikeOut)
FORMAT_FUNC(textUnderline)

void MainWindow::Private::textAlign(QAction *action)
{
    if (action == alignCenterAction) {
        currentEditor->textAlign(AlignCenter);
    } else if (action == alignLeftAction) {
        currentEditor->textAlign(AlignLeft);
    } else if (action == alignRightAction) {
        currentEditor->textAlign(AlignRight);
    } else if (action == alignJustifyAction) {
        currentEditor->textAlign(AlignJustify);
    }
}

void MainWindow::Private::cursorPositionChanged()
{
    alignmentChanged(visualEditor->alignment());
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
    connect(textBoldAction, SIGNAL(triggered()), this, SLOT(textBold()));

    textItalicAction = new QAction(QIcon::fromTheme("format-text-italic", QIcon(":/image/italic")), tr("Italic"), this);
    textItalicAction->setShortcut(Qt::CTRL + Qt::Key_I);
    textItalicAction->setStatusTip(tr("Set text italic."));
    textItalicAction->setCheckable(true);
    connect(textItalicAction, SIGNAL(triggered()), this, SLOT(textItalic()));

    textUnderlineAction = new QAction(QIcon::fromTheme("format-text-underline", QIcon(":/image/underline")), tr("Underline"), this);
    textUnderlineAction->setShortcut(Qt::CTRL + Qt::Key_U);
    textUnderlineAction->setStatusTip(tr("Add underline."));
    textUnderlineAction->setCheckable(true);
    connect(textUnderlineAction, SIGNAL(triggered()), this, SLOT(textUnderline()));

    textStrikeOutAction = new QAction(QIcon::fromTheme("format-text-strikethrough", QIcon(":/image/strike")), tr("Strike"), this);
    textStrikeOutAction->setShortcut(Qt::CTRL + Qt::Key_D);
    textStrikeOutAction->setStatusTip(tr("Strike out."));
    textStrikeOutAction->setCheckable(true);
    connect(textStrikeOutAction, SIGNAL(triggered()), this, SLOT(textStrikeOut()));

    textFontAction = new QAction(QIcon(":/image/font"), tr("Font..."), this);
    textFontAction->setStatusTip(tr("Set font."));

    textColorAction = new QAction(QIcon(":/image/text_color"), tr("Text Color..."), this);
    textColorAction->setStatusTip(tr("Text Color."));

    textBackgroundColorAction = new QAction(QIcon(":/image/text_background_color"), tr("Text Background Color..."), this);
    textBackgroundColorAction->setStatusTip(tr("Text background color."));

    QActionGroup *alignGroup = new QActionGroup(this);
    connect(alignGroup, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));
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

void MainWindow::Private::alignmentChanged(Qt::Alignment align)
{
    if (align & Qt::AlignLeft) {
        alignLeftAction->setChecked(true);
    } else if (align & Qt::AlignHCenter) {
        alignCenterAction->setChecked(true);
    } else if (align & Qt::AlignRight) {
        alignRightAction->setChecked(true);
    } else if (align & Qt::AlignJustify) {
        alignJustifyAction->setChecked(true);
    }
}

void MainWindow::Private::fontChanged(const QFont &font)
{
    fontChooser->setCurrentIndex(fontChooser->findData(QFontInfo(font).family()));
    fontSizeChooser->setCurrentIndex(fontSizeChooser->findData(QString::number(font.pointSize())));
    textBoldAction->setChecked(font.bold());
    textItalicAction->setChecked(font.italic());
    textStrikeOutAction->setChecked(font.strikeOut());
    textUnderlineAction->setChecked(font.underline());
}

void MainWindow::Private::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
}

void MainWindow::Private::fontFamilyActivated(const QString &family)
{
    currentEditor->textFontFamily(family);
}

void MainWindow::Private::fontSizeActivated(int size)
{
    currentEditor->textFontSize(size);
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
