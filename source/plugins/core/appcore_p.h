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

#ifndef APPCORE_P_H
#define APPCORE_P_H

#include <QObject>
#include <QMap>

#include "context.h"

namespace Core
{

class AppCore;

namespace Internal
{

class AppCorePrivate : public QObject
{
    Q_OBJECT
public:
    AppCorePrivate(AppCore *ac);

    // global managers
    QWidget * mainWindow;

    // Context
    WidgetContext *widgetContext(QWidget *widget) const;
    void addWidgetContext(WidgetContext *context);
    void removeWidgetContext(WidgetContext *context);
    void updateWidgetContext(WidgetContext *context);

    void resetContext();
    void updateAdditionalContexts(const Context &remove, const Context &add);
    void updateContext();

    Context additionalContexts;
    WidgetContext *currentWidgetContext;
    QMap<QWidget *, WidgetContext *> widgetContexts;

private:
    AppCore *q;
}; // end of class Core::Internal::AppCorePrivate

} // end of namespace Core::Internal
} // end of namespace Core

#endif // APPCORE_P_H
