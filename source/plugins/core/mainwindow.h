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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "core/core_global.h"

namespace Core {

class CORE_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    bool init(QString *errorMessage);
    void prepareToShow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newDocument();
    void showAboutDialog();
    void showAboutPluginDialog();

private:
    void registerDefaultContainers();
    void registerDefaultActions();

    void readSettings();
    void writeSettings();

    // Actions
    QAction *m_newDocAction;
    QAction *m_openDocAction;
    QAction *m_exitAction;
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
}; // end of class MainWindow

} // end of namespace Core

#endif // MAINWINDOW_H
