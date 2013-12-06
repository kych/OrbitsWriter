/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

#include "commons/commons_global.h"
#include "commons/singleton.h"

#define gSettingsManager (Commons::SettingsManager::instance())

namespace Commons
{

namespace Internal
{
class SettingsManagerPrivate;
} // end of namespace Commons::Internal

/*!
  The SettingsManager class is used for save/restore application global settings.

  This is a singleton class. If you need to access its instance, use SettingsManager::instance()
  method.
 */
class COMMONS_EXPORT SettingsManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(SettingsManager)
public:

    /*!
      Define the user specific settings to use for information about enabled/disabled plugins.

      Needs to be set before the plugin search path is set with PluginManager::setPluginPaths().
     */
    void setSettings(QSettings *settings);

    /*!
      Define the global (user-independent) settings to use for information about default disabled plugins.
      Needs to be set before the plugin search path is set with PluginManager::setPluginPaths().
     */
    void setGlobalSettings(QSettings *settings);

    /*!
      Gets the user specific settings used for information about enabled/disabled plugins.
     */
    QSettings * settings() const;

    /*!
      Gets the global (user-independent) settings used for information about default disabled plugins.
     */
    QSettings * globalSettings() const;

private:
    //! Constructs an instance of SettingsManager.
    SettingsManager();
    //! Destroys the instance of SettingsManager.
    ~SettingsManager();

    Internal::SettingsManagerPrivate *d;
    friend class Internal::SettingsManagerPrivate;
}; // end of class Commons::SettingsManager

} // end of namespace Commons

#endif // SETTINGSMANAGER_H
