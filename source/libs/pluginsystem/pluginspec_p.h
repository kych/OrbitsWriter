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

#ifndef PLUGINSPEC_P_H
#define PLUGINSPEC_P_H

#include <QObject>
#include <QHash>
#include <QStringList>

#include "pluginsystem/plugin.h"
#include "pluginsystem/pluginspec.h"

QT_BEGIN_NAMESPACE
class QXmlStreamReader;
QT_END_NAMESPACE

namespace PluginSystem
{

namespace Internal
{

/*!
  \brief The PluginSpecPrivate class is the private implementation of PluginSpec.
 */
class PluginSpecPrivate : public QObject
{
    Q_OBJECT
public:
    PluginSpecPrivate(PluginSpec *spec);

    // plugin information
    QString name;
    QString version;
    QString compatVersion;
    QString vendor;
    QString copyright;
    QString license;
    QString category;
    QString description;
    QString url;
    QList<PluginDependency> dependencyList;
    QString location;
    QString filePath;
    QStringList arguments;

    bool disabledByDefault;
    bool enabled;
    bool disabledIndirectly;

    PluginSpec::State state;
    QHash<PluginDependency, PluginSpec *> dependencySpecs;

    Plugin *plugin;

    bool resolveDependencies(const QList<PluginSpec *> &specs);

    // error
    bool hasError;
    bool reportError(const QString &err);
    QString errorString() const;
    void setErrorString(const QString &err);

    bool initializePlugins();
    bool initializePlugin();

    bool read(const QString &fileName);
    bool provides(const QString &pluginName, const QString &pluginVersion) const;

    bool load();
    void kill();
    Plugin::ShutdownFlag stop();

    void disableIndirectlyIfDependencyDisabled();

    static bool isValidVersion(const QString &version);
    static int compareVersion(const QString &version1, const QString &version2);
    static QRegExp & versionRegExp();

private:
    QString m_errorString;
    PluginSpec *q;

    void readPluginSpec(QXmlStreamReader &reader);
    void readDependencies(QXmlStreamReader &reader);
    void readDependencyEntry(QXmlStreamReader &reader);
}; // end of class PluginSystem::Internal::PluginSpecPrivate

} // end of namespace PluginSystem::Internal

} // end of namespace PluginSystem

#endif // PLUGINSPEC_P_H
