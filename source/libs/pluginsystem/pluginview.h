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

#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include <QWidget>

#include "pluginsystem_export.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QTreeWidget;
QT_END_NAMESPACE

namespace PluginSystem
{

class PLUGINSYSTEM_EXPORT PluginView : public QWidget
{
    Q_OBJECT
public:
    explicit PluginView(QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    QPushButton *detailButton;
    QPushButton *errButton;
    QTreeWidget *pluginTreeWidget;
}; // end of class PluginSystem::PluginView

} // end of namespace PluginSystem

#endif // PLUGINVIEW_H
