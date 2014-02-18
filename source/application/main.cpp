/*-------------------------------------------------
 *
 * OrbitsWriter - An Offline Blog Writer
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

#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QSettings>

#include "../libs/extern/QtSingleApplication/QtSingleApplication"
#include <commons/settingsmanager.h>
#include <pluginsystem/pluginmanager.h>
#include <pluginsystem/pluginspec.h>
#include <version.h>

#ifdef Q_OS_MAC
#  define SHARE_PATH "/../Resources"
#else
#  define SHARE_PATH "/../share/orbitswriter"
#endif

static const char CorePluginName[]     = "Core";
static const char ApplicationLogFile[] = "OrbitsWriter.log";

// Gets all plugin paths
static inline QStringList getPluginPaths()
{
    QStringList rc;
    // Figure out root:  Up one from 'bin'
    QDir rootDir = QCoreApplication::applicationDirPath();
#if defined(Q_OS_MAC)
    rootDir.cdUp();
#endif
    const QString rootDirPath = rootDir.canonicalPath();
#if !defined(Q_OS_MAC)
    // 1) "plugins" (Win/Linux)
    QString pluginPath = rootDirPath;
    pluginPath += QLatin1String("/plugins/");
    pluginPath += QLatin1String(OrbitsWriter::ORGANIZATION);
    rc.push_back(pluginPath);
#else
    // 2) "PlugIns" (OS X)
    QString pluginPath = rootDirPath;
    pluginPath += QLatin1String("/PlugIns");
    rc.push_back(pluginPath);
#endif
    // 3) <localappdata>/plugins/<ideversion>
    //    where <localappdata> is e.g.
    //    <drive>:\Users\<username>\AppData\Local\GalaxyWorld\orbitswriter on Windows Vista and later
    //    $XDG_DATA_HOME or ~/.local/share/data/GalaxyWorld/orbitswriter on Linux
    //    ~/Library/Application Support/GalaxyWorld/Orbits Writer on Mac
#if (QT_VERSION >= 0x050000)
    pluginPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    pluginPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
//#if !defined(Q_OS_MAC)
//    pluginPath += QLatin1String("orbitswriter");
//#else
//    pluginPath += QLatin1String("Orbits Writer");
//#endif
    pluginPath += QLatin1String("/plugins/");
    pluginPath += QLatin1String(OrbitsWriter::VERSION_LONG);
    rc.push_back(pluginPath);
    return rc;
}

void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile logFile(ApplicationLogFile);
    if (!logFile.open(QFile::WriteOnly | QFile::Text | QFile::Append)) {
        return;
    }
    QTextStream out(&logFile);
    out << QDateTime::currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss.zzz")) << " [";
    switch (type) {
#ifdef QT_DEBUG
    case QtDebugMsg:
        out << "Debug] [" << context.file << ":" << context.line << ", " << context.function << "]: ";
        break;
#endif
    case QtWarningMsg:
        out << "Warning] ";
        break;
    case QtCriticalMsg:
        out << "Critical] ";
        break;
    case QtFatalMsg:
        out << "Fatal] ";
    }
    out << msg << endl;
    logFile.close();
}

int main(int argc, char** argv)
{
    using namespace Commons;
    using namespace PluginSystem;

#ifdef QT_NO_DEBUG
    qInstallMessageHandler(logMessageOutput);
#endif
    ExternLib::QtSingleApplication app((QLatin1String(OrbitsWriter::APPLICATION_NAME)), argc, argv);
    app.setOrganizationName(QLatin1String(OrbitsWriter::ORGANIZATION));
    app.setApplicationName(QLatin1String(OrbitsWriter::APPLICATION_NAME));
    app.setApplicationVersion(QLatin1String(OrbitsWriter::VERSION_LONG));

    // <<<<<<<<<< settings
    // Must be done before any QSettings class is created
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope,
                       QCoreApplication::applicationDirPath() + QLatin1String(SHARE_PATH));
    QSettings::setDefaultFormat(QSettings::IniFormat);
    // plugin manager takes control of this settings object
    QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                        QLatin1String(OrbitsWriter::SETTINGSVARIANT_STR),
                                        QLatin1String(OrbitsWriter::APPLICATION_NAME));
    QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
                                              QLatin1String(OrbitsWriter::SETTINGSVARIANT_STR),
                                              QLatin1String(OrbitsWriter::APPLICATION_NAME));
    SettingsManager *settingsManager = SettingsManager::instance();
    settingsManager->setSettings(settings);
    settingsManager->setGlobalSettings(globalSettings);

    // <<<<<<<<<< load plugins
    const QStringList pluginPaths = getPluginPaths();
    PluginManager *pluginManager = PluginManager::instance();
    pluginManager->setPluginPaths(pluginPaths);

    const QList<PluginSpec *> plugins = pluginManager->plugins();
    // ensure core plugin spec loads successfully
    PluginSpec *corePlugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(CorePluginName)) {
            corePlugin = spec;
            break;
        }
    }
    if (!corePlugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1String(", ")));
        const QString reason = QCoreApplication::translate("Application", "Could not find \"Core.spec\" in %1").arg(nativePaths);
        // displayError(msgCoreLoadFailure(reason));
        qFatal(qPrintable(reason));
        return 1;
    }
    if (corePlugin->hasError()) {
        // displayError(msgCoreLoadFailure(corePlugin->errorString()));
        qFatal(qPrintable(corePlugin->errorString()));
        return 1;
    }

    pluginManager->loadPlugins();
    if (corePlugin->hasError()) {
        // displayError(msgCoreLoadFailure(coreplugin->errorString()));
        qFatal(qPrintable(corePlugin->errorString()));
        return 1;
    }
    if (pluginManager->hasError()) {
        // PluginErrorOverview errorOverview;
        // errorOverview.exec();
    }

    // shutdown plugin manager on the exit
    QObject::connect(&app, SIGNAL(aboutToQuit()), pluginManager, SLOT(unloadPlugins()));

    return app.exec();
}
