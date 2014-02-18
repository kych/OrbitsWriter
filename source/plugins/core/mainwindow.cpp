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

#include <QCloseEvent>
#include <QDateTime>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>

#include <version.h>
#include <commons/objectpool.h>
#include <commons/settingsmanager.h>

#include "actionsystem/actionmanager.h"
#include "actionsystem/actionmanager_p.h"
#include "appcore.h"
#include "appcore_p.h"
#include "coreconstants.h"
#include "corelistener.h"
#include "documentsystem/documentmanager.h"
#include "documentsystem/documentmanager_p.h"
#include "editorsystem/editor.h"
#include "editorsystem/editormanager.h"
#include "mainwindow.h"
#include "plugindialog.h"

// Icons
static const char ICON_NEWDOC[]       = ":/img/doc_new";      //! Path for new document icon.
static const char ICON_OPENDOC[]      = ":/img/doc_open";     //! Path for open document icon.
static const char ICON_SAVEDOC[]      = ":/img/save";         //! Path for save document icon.
static const char ICON_SAVEASDOC[]    = ":/img/save_as";      //! Path for save as document icon.
static const char ICON_UNDO[]         = ":/img/undo";         //! Path for undo icon.
static const char ICON_REDO[]         = ":/img/redo";         //! Path for redo icon.
static const char ICON_CUT[]          = ":/img/cut";          //! Path for cut icon.
static const char ICON_COPY[]         = ":/img/copy";         //! Path for copy icon.
static const char ICON_PASTE[]        = ":/img/paste";        //! Path for paste icon.
static const char ICON_OPTION[]       = ":/img/options";      //! Path for option icon.
static const char ICON_HELPCONTENTS[] = ":/img/help";         //! Path for help contents icon.
static const char ICON_ABOUT[]        = ":/img/about";        //! Path for about icon.

using namespace Core;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_newDocAction(0),
    m_openDocAction(0),
    m_exitAction(0),
    m_undoAction(0),
    m_redoAction(0),
    m_cutAction(0),
    m_copyAction(0),
    m_pasteAction(0)
{
    gCore->d->mainWindow = this;

    setWindowIcon(QIcon(QLatin1String(":/img/app")));
    setWindowTitle(tr("Orbits Writer [*]"));
    setUnifiedTitleAndToolBarOnMac(true);
}


bool MainWindow::init(QString *errorMessage)
{
    Q_UNUSED(errorMessage)
    registerDefaultContainers();
    registerDefaultActions();
    return true;
}

void MainWindow::prepareToShow()
{
    gActionManager->d->initialize();

    readSettings();
    gCore->d->updateContext();

    gEditorManager->initialize();

    Editor *mainEditor = gEditorManager->currentEditor();
    setCentralWidget(mainEditor->widget());

    //TODO: move these code to editor manager?
    gCore->addWidgetContext(mainEditor);
    gCore->updateAdditionalContexts(Context(), mainEditor->context());

    // initialize document after editor initialied
    gDocumentManager->d->initialize();

    emit gCore->coreAboutToOpen();
    show();
    emit gCore->coreOpened();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    const QList<CoreListener *> listeners = gObjectPool->getObjects<CoreListener>();
    foreach (CoreListener *listener, listeners) {
        if (!listener->mainWindowAboutToClose()) {
            event->ignore();
            return;
        }
    }
    emit gCore->coreAboutToClose();
    writeSettings();
    event->accept();
}

void MainWindow::newDocument()
{
    gDocumentManager->createDocument();
}

void MainWindow::showAboutDialog()
{
    QMessageBox aboutBox;
    aboutBox.setIconPixmap(QPixmap(":/img/app"));
    aboutBox.setText(tr("<h3>Orbits Writer %1</h3>"
                        "<p>Build on Qt %2</p>"
                        "<p>at %3</p>"
                        "<p>from code version %4</p>"
                        "<p>All rights reserved.</p>")
                     .arg(OrbitsWriter::VERSION_LONG)
                     .arg(QT_VERSION_STR)
                     .arg(QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss")))
                     .arg(OrbitsWriter::APPLICATION_REVISION));
    aboutBox.exec();
}

void MainWindow::showAboutPluginDialog()
{
    PluginDialog dialog(this);
    dialog.exec();
}

void MainWindow::registerDefaultContainers()
{
    // tool bar
    ActionContainer *toolBar = gActionManager->createToolBar(Constants::DEFAULT_TOOL_BAR);
    addToolBar(toolBar->toolBar());
    toolBar->toolBar()->setStyleSheet(QLatin1String("QToolBar { spacing:5px }"));
    toolBar->toolBar()->setFloatable(false);
    toolBar->toolBar()->setMovable(false);
    toolBar->appendActionGroup(Constants::GT_FILE);
    toolBar->appendActionGroup(Constants::GT_UNDOREDO);
    toolBar->appendActionGroup(Constants::GT_COPYPASTE);
    toolBar->appendActionGroup(Constants::GT_FORMAT);

    // menu bar
    ActionContainer *menuBar = gActionManager->createMenuBar(Constants::MENU_BAR);
#ifndef Q_OS_MAC // System menu bar on Mac
    setMenuBar(menuBar->menuBar());
#endif
    menuBar->appendActionGroup(Constants::GM_FILE);
    menuBar->appendActionGroup(Constants::GM_EDIT);
    menuBar->appendActionGroup(Constants::GM_FORMAT);
    menuBar->appendActionGroup(Constants::GM_INSERT);
    menuBar->appendActionGroup(Constants::GM_TOOLS);
    menuBar->appendActionGroup(Constants::GM_HELP);

    // File Menu
    ActionContainer *fileMenu = gActionManager->createMenu(Constants::M_FILE);
    menuBar->addMenu(fileMenu, Constants::GM_FILE);
    fileMenu->menu()->setTitle(tr("&File"));
    fileMenu->appendActionGroup(Constants::G_FILE_NEW);
    fileMenu->appendActionGroup(Constants::G_FILE_OPEN);
    fileMenu->appendActionGroup(Constants::G_FILE_SAVE);
    fileMenu->appendActionGroup(Constants::G_FILE_CLOSE);
    fileMenu->appendActionGroup(Constants::G_FILE_OTHER);

    // Edit Menu
    ActionContainer *editMenu = gActionManager->createMenu(Constants::M_EDIT);
    menuBar->addMenu(editMenu, Constants::GM_EDIT);
    editMenu->menu()->setTitle(tr("&Edit"));
    editMenu->appendActionGroup(Constants::G_EDIT_UNDOREDO);
    editMenu->appendActionGroup(Constants::G_EDIT_COPYPASTE);
//    editMenu->appendActionGroup(Constants::P_EDIT_ADVANCED);
//    editMenu->appendActionGroup(Constants::P_EDIT_FIND);
    editMenu->appendActionGroup(Constants::G_EDIT_OTHER);

    // Format Menu
    ActionContainer *formatMenu = gActionManager->createMenu(Constants::M_FORMAT);
    menuBar->addMenu(formatMenu, Constants::GM_FORMAT);
    formatMenu->menu()->setTitle(tr("F&ormat"));
    formatMenu->appendActionGroup(Constants::G_FORMAT_MAIN);
    formatMenu->appendActionGroup(Constants::G_FORMAT_TEXTSTYLE);
    formatMenu->appendActionGroup(Constants::G_FORMAT_FONTCOLOR);
    formatMenu->appendActionGroup(Constants::G_FORMAT_ALIGNMENT);
    formatMenu->appendActionGroup(Constants::G_FORMAT_OTHER);

    // Insert Menu
    ActionContainer *insertMenu = gActionManager->createMenu(Constants::M_INSERT);
    menuBar->addMenu(insertMenu, Constants::GM_INSERT);
    insertMenu->menu()->setTitle(tr("&Insert"));
    insertMenu->appendActionGroup(Constants::G_INSERT_MAIN);

    // Tools Menu
    ActionContainer *toolsMenu = gActionManager->createMenu(Constants::M_TOOLS);
    menuBar->addMenu(toolsMenu, Constants::GM_TOOLS);
    toolsMenu->menu()->setTitle(tr("&Tools"));
    toolsMenu->appendActionGroup(Constants::G_TOOLS_OPTION);

    // Help Menu
    ActionContainer *helpMenu = gActionManager->createMenu(Constants::M_HELP);
    menuBar->addMenu(helpMenu, Constants::GM_HELP);
    helpMenu->menu()->setTitle(tr("&Help"));
    helpMenu->appendActionGroup(Constants::G_HELP_HELPCONTENTS);
    helpMenu->appendActionGroup(Constants::G_HELP_ABOUT);
}

void MainWindow::registerDefaultActions()
{
    ActionContainer *fileMenu = gActionManager->actionContainer(Constants::M_FILE);
    ActionContainer *editMenu = gActionManager->actionContainer(Constants::M_EDIT);
    ActionContainer *formatMenu = gActionManager->actionContainer(Constants::M_FORMAT);
    ActionContainer *insertMenu = gActionManager->actionContainer(Constants::M_INSERT);
    ActionContainer *toolsMenu = gActionManager->actionContainer(Constants::M_TOOLS);
    ActionContainer *helpMenu = gActionManager->actionContainer(Constants::M_HELP);
    ActionContainer *toolBar = gActionManager->actionContainer(Constants::DEFAULT_TOOL_BAR);

    Context globalContext(Constants::CONTEXT_GLOBAL);

    // Tool Bar Separators
    toolBar->addSeparator(globalContext, Constants::GT_FILE);
    toolBar->addSeparator(globalContext, Constants::GT_UNDOREDO);
    toolBar->addSeparator(globalContext, Constants::GT_COPYPASTE);
    toolBar->addSeparator(globalContext, Constants::GT_FORMAT);
    // File Menu Separators
    fileMenu->addSeparator(globalContext, Constants::G_FILE_SAVE);
    fileMenu->addSeparator(globalContext, Constants::G_FILE_OTHER);
    // Edit Menu Separators
    editMenu->addSeparator(globalContext, Constants::G_EDIT_COPYPASTE);
    // Format Menu Separators
    formatMenu->addSeparator(globalContext, Constants::G_FORMAT_FONTCOLOR);
    formatMenu->addSeparator(globalContext, Constants::G_FORMAT_ALIGNMENT);
    formatMenu->addSeparator(globalContext, Constants::G_FORMAT_OTHER);
    // Insert Menu Separators
    insertMenu->addSeparator(globalContext, Constants::G_INSERT_MAIN);
    // Help Menu Separators
    helpMenu->addSeparator(globalContext, Constants::G_HELP_ABOUT);

    // New File Action
    QIcon icon = QIcon::fromTheme(QLatin1String("document-new"), QIcon(QLatin1String(ICON_NEWDOC)));
    m_newDocAction = new QAction(icon, tr("&New"), this);
    Command *cmd = gActionManager->registerAction(m_newDocAction, Constants::ID_NEWDOC, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::New);
    fileMenu->addAction(cmd, Constants::G_FILE_NEW);
    toolBar->addAction(cmd, Constants::GT_FILE);
    connect(m_newDocAction, SIGNAL(triggered()), this, SLOT(newDocument()));

    // Open Action
    icon = QIcon::fromTheme(QLatin1String("document-open"), QIcon(QLatin1String(ICON_OPENDOC)));
    m_openDocAction = new QAction(icon, tr("&Open..."), this);
    cmd = gActionManager->registerAction(m_openDocAction, Constants::ID_OPENDOC, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Open);
    fileMenu->addAction(cmd, Constants::G_FILE_OPEN);
    toolBar->addAction(cmd, Constants::GT_FILE);
    // connect(m_openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    // Save Action
    icon = QIcon::fromTheme(QLatin1String("document-save"), QIcon(QLatin1String(ICON_SAVEDOC)));
    QAction *tmpAction = new QAction(icon, tr("&Save"), this);
    tmpAction->setEnabled(false);
    cmd = gActionManager->registerAction(tmpAction, Constants::ID_SAVEDOC, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Save);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDescription(tr("Save"));
    fileMenu->addAction(cmd, Constants::G_FILE_SAVE);
    toolBar->addAction(cmd, Constants::GT_FILE);

    // Save As Action
    icon = QIcon::fromTheme(QLatin1String("document-save-as"), QIcon(QLatin1String(ICON_SAVEASDOC)));
    tmpAction = new QAction(icon, tr("Save &As..."), this);
    tmpAction->setEnabled(false);
    cmd = gActionManager->registerAction(tmpAction, Constants::ID_SAVEASDOC, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::SaveAs);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDescription(tr("Save As..."));
    fileMenu->addAction(cmd, Constants::G_FILE_SAVE);

    // Exit Action
    icon = QIcon::fromTheme(QLatin1String("application-exit"));
    m_exitAction = new QAction(icon, tr("E&xit"), this);
    m_exitAction->setMenuRole(QAction::QuitRole);
    cmd = gActionManager->registerAction(m_exitAction, Constants::ID_EXIT, globalContext);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Q")));
    fileMenu->addAction(cmd, Constants::G_FILE_OTHER);
    // connect(m_exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    // Undo Action
    icon = QIcon::fromTheme(QLatin1String("edit-undo"), QIcon(QLatin1String(ICON_UNDO)));
    m_undoAction = new QAction(icon, tr("&Undo"), this);
    cmd = gActionManager->registerAction(m_undoAction, Constants::ID_UNDO, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Undo);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDescription(tr("Undo"));
    editMenu->addAction(cmd, Constants::G_EDIT_UNDOREDO);
    toolBar->addAction(cmd, Constants::GT_UNDOREDO);
    m_undoAction->setEnabled(false);

    // Redo Action
    icon = QIcon::fromTheme(QLatin1String("edit-redo"), QIcon(QLatin1String(ICON_REDO)));
    m_redoAction = new QAction(icon, tr("&Redo"), this);
    cmd = gActionManager->registerAction(m_redoAction, Constants::ID_REDO, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Redo);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDescription(tr("Redo"));
    editMenu->addAction(cmd, Constants::G_EDIT_UNDOREDO);
    toolBar->addAction(cmd, Constants::GT_UNDOREDO);
    m_redoAction->setEnabled(false);

    // Cut Action
    icon = QIcon::fromTheme(QLatin1String("edit-cut"), QIcon(QLatin1String(ICON_CUT)));
    m_cutAction = new QAction(icon, tr("Cu&t"), this);
    cmd = gActionManager->registerAction(m_cutAction, Constants::ID_CUT, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Cut);
    editMenu->addAction(cmd, Constants::G_EDIT_COPYPASTE);
    toolBar->addAction(cmd, Constants::GT_COPYPASTE);
    m_cutAction->setEnabled(false);

    // Copy Action
    icon = QIcon::fromTheme(QLatin1String("edit-copy"), QIcon(QLatin1String(ICON_COPY)));
    m_copyAction = new QAction(icon, tr("&Copy"), this);
    cmd = gActionManager->registerAction(m_copyAction, Constants::ID_COPY, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Copy);
    editMenu->addAction(cmd, Constants::G_EDIT_COPYPASTE);
    toolBar->addAction(cmd, Constants::GT_COPYPASTE);
    m_copyAction->setEnabled(false);

    // Paste Action
    icon = QIcon::fromTheme(QLatin1String("edit-paste"), QIcon(QLatin1String(ICON_PASTE)));
    m_pasteAction = new QAction(icon, tr("&Paste"), this);
    cmd = gActionManager->registerAction(m_pasteAction, Constants::ID_PASTE, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::Paste);
    editMenu->addAction(cmd, Constants::G_EDIT_COPYPASTE);
    toolBar->addAction(cmd, Constants::GT_COPYPASTE);
    m_pasteAction->setEnabled(false);

    // Option Action
    icon = QIcon(QLatin1String(ICON_OPTION));
    tmpAction = new QAction(icon, tr("&Option..."), this);
    tmpAction->setMenuRole(QAction::PreferencesRole);
    cmd = gActionManager->registerAction(tmpAction, Constants::ID_OPTION, globalContext);
    toolsMenu->addAction(cmd, Constants::G_TOOLS_OPTION);

    // Help Action
    icon = QIcon(QLatin1String(ICON_HELPCONTENTS));
    tmpAction = new QAction(icon, tr("&Help Contents..."), this);
    cmd = gActionManager->registerAction(tmpAction, Constants::ID_HELPCONTENTS, globalContext);
    cmd->setDefaultKeySequence(QKeySequence::HelpContents);
    helpMenu->addAction(cmd, Constants::G_HELP_HELPCONTENTS);

    // About Action
    icon = QIcon(QLatin1String(ICON_ABOUT));
    tmpAction = new QAction(icon, tr("&About Orbits Writer..."), this);
    tmpAction->setMenuRole(QAction::AboutRole);
    cmd = gActionManager->registerAction(tmpAction, Constants::ID_ABOUT, globalContext);
    helpMenu->addAction(cmd, Constants::G_HELP_ABOUT);
    connect(tmpAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    // About Plugin Action
    tmpAction = new QAction(tr("About &Plugins..."), this);
    cmd = gActionManager->registerAction(tmpAction, Constants::ID_ABOUTPLUGINS, globalContext);
    helpMenu->addAction(cmd, Constants::G_HELP_ABOUT);
    connect(tmpAction, SIGNAL(triggered()), this, SLOT(showAboutPluginDialog()));
}

static const char settingsGroup[]     = "MainWindow";
static const char windowGeometryKey[] = "WindowGeometry";
static const char windowStateKey[]    = "WindowState";

void MainWindow::readSettings()
{
    QSettings *settings = gSettingsManager->settings();

    settings->beginGroup(QLatin1String(settingsGroup));
    if (!restoreGeometry(settings->value(QLatin1String(windowGeometryKey)).toByteArray())) {
        resize(1008, 700); // size without window decoration
    }
    restoreState(settings->value(QLatin1String(windowStateKey)).toByteArray());
    settings->endGroup();
}

void MainWindow::writeSettings()
{
    QSettings *settings = gSettingsManager->settings();

    settings->beginGroup(QLatin1String(settingsGroup));
    settings->setValue(QLatin1String(windowGeometryKey), saveGeometry());
    settings->setValue(QLatin1String(windowStateKey), saveState());
    settings->endGroup();
}
