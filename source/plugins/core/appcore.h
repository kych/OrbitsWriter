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

#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include <commons/singleton.h>

#include "core_global.h"

#define gCore (Core::AppCore::instance())

namespace Commons
{
class ObjectPool;
class SettingsManager;
}

namespace Core
{

class ActionManager;
class Context;
class DocumentManager;
class EditorManager;
class MainWindow;
class WidgetContext;

namespace Internal
{
class AppCorePrivate;
}

class CORE_EXPORT AppCore : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(AppCore)
public:
    QWidget * mainWindow() const;


    // ---------- contexts ---------- //
    WidgetContext * currentWidgetContext() const;
    void updateAdditionalContexts(const Context &remove, const Context &add);
    bool hasContext(int context) const;
    void addWidgetContext(WidgetContext *context);
    void removeWidgetContext(WidgetContext *context);

signals:
    void coreAboutToOpen();
    void coreAboutToClose();
    void coreOpened();
    void contextAboutToChange(Core::WidgetContext *context);
    void contextChanged(Core::WidgetContext *context, const Core::Context &additionalContexts);

private:
    explicit AppCore(QObject *parent = 0);
    ~AppCore();

    Internal::AppCorePrivate *d;
    friend class Internal::AppCorePrivate;
    friend class MainWindow;
}; // end of class Core::AppCore

} // end of namespace Core

#endif // APPCORE_H
