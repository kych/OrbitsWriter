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

#include <QMutex>

#include "settingsmanager.h"

/*!
  \class Commons::SettingsManager
  The SettingsManager class is used for save/restore application global settings.

  This is a singleton class. If you need to access its instance, use SettingsManager::instance()
  method.
 */

namespace Commons
{

namespace Internal
{
class SettingsManagerPrivate
{
public:
    SettingsManagerPrivate(SettingsManager *s) :
        settings(0),
        globalSettings(0),
        q(s)
    {}

    void setSettings(QSettings * s);
    void setGlobalSettings(QSettings * s);
    QSettings *settings;
    QSettings *globalSettings;

    SettingsManager *q;
}; // end of class Commons::Internal::SettingsManagerPrivate

void SettingsManagerPrivate::setSettings(QSettings *s)
{
    if (settings) {
        delete settings;
    }
    settings = s;
    if (settings) {
        settings->setParent(q);
    }
}

void SettingsManagerPrivate::setGlobalSettings(QSettings *s)
{
    if (globalSettings) {
        delete globalSettings;
    }
    globalSettings = s;
    if (globalSettings) {
        globalSettings->setParent(q);
    }
}

} // end of namespace Commons::Internal

// ========== Commons::SettingsManager ========== //

GET_INSTANCE(SettingsManager)

/*!
  Constructs an instance of SettingsManager.
 */
SettingsManager::SettingsManager() :
    d(new Internal::SettingsManagerPrivate(this))
{
}

/*!
  Destroys the instance of SettingsManager.
 */
SettingsManager::~SettingsManager()
{
    delete d;
}

/*!
  Define the user specific settings to use for information about
  enabled/disabled plugins.

  Needs to be set before the plugin search path is set with
  PluginManager::setPluginPaths().
 */
void SettingsManager::setSettings(QSettings *settings)
{
    d->setSettings(settings);
}

/*!
  Define the global (user-independent) settings to use for information
  about default disabled plugins.

  Needs to be set before the plugin search path is set with
  PluginManager::setPluginPaths().
 */
void SettingsManager::setGlobalSettings(QSettings *settings)
{
    d->setGlobalSettings(settings);
}

/*!
  Gets the user specific settings used for information
  about enabled/disabled plugins.
 */
QSettings *SettingsManager::settings() const
{
    return d->settings;
}

/*!
  Gets the global (user-independent) settings used
  for information about default disabled plugins.
 */
QSettings *SettingsManager::globalSettings() const
{
    return d->globalSettings;
}

} // end of namespace Commons
