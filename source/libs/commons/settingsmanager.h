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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

#include "commons/commons_global.h"
#include "commons/singleton.h"

namespace Commons
{

namespace Internal
{
class SettingsManagerPrivate;
} // end of namespace Commons::Internal

class COMMONS_EXPORT SettingsManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(SettingsManager)
public:
    void setSettings(QSettings *settings);
    void setGlobalSettings(QSettings *settings);

    QSettings * settings() const;
    QSettings * globalSettings() const;

private:
    SettingsManager();
    ~SettingsManager();

    Internal::SettingsManagerPrivate *d;
    friend class Internal::SettingsManagerPrivate;
}; // end of class Commons::SettingsManager

} // end of namespace Commons

#endif // SETTINGSMANAGER_H
