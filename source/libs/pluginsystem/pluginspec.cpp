/*-------------------------------------------------
 *
  OrbitsWriter - an Offline Blog Writer
 *
  Copyright (C) 2013 devbean@galaxyworld.org
 *
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 *
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 *
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *-------------------------------------------------*/

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QRegExp>
#include <QCoreApplication>
#include <QDebug>
#include <QPluginLoader>

#include "plugin.h"
#include "plugin_p.h"
#include "pluginspec.h"
#include "pluginspec_p.h"

using namespace PluginSystem;
using namespace PluginSystem::Internal;

uint PluginSystem::qHash(const PluginDependency &value)
{
    return qHash(value.name);
}

// ========== PluginSystem::PluginDependency ========== //

bool PluginDependency::operator ==(const PluginDependency &other) const
{
    return name == other.name
            && version == other.version
            && type == other.type;
}

// ========== PluginSystem::PluginSpec ========== //

PluginSpec::PluginSpec() :
    d(new PluginSpecPrivate(this))
{
}

PluginSpec::~PluginSpec()
{
    delete d;
    d = 0;
}

QString PluginSpec::name() const
{
    return d->name;
}

QString PluginSpec::version() const
{
    return d->version;
}

QString PluginSpec::compatVersion() const
{
    return d->compatVersion;
}

QString PluginSpec::vendor() const
{
    return d->vendor;
}

QString PluginSpec::copyright() const
{
    return d->copyright;
}

QString PluginSpec::license() const
{
    return d->license;
}

QString PluginSpec::category() const
{
    return d->category;
}

QString PluginSpec::description() const
{
    return d->description;
}

QString PluginSpec::url() const
{
    return d->url;
}

QList<PluginDependency> PluginSpec::dependencyList() const
{
    return d->dependencyList;
}

bool PluginSpec::hasError() const
{
    return d->hasError;
}

QString PluginSpec::errorString() const
{
    return d->errorString();
}

PluginSpec::State PluginSpec::state() const
{
    return d->state;
}

QString PluginSpec::location() const
{
    return d->location;
}

QString PluginSpec::filePath() const
{
    return d->filePath;
}

bool PluginSpec::provides(const QString &pluginName, const QString &version) const
{
    return d->provides(pluginName, version);
}

QHash<PluginDependency, PluginSpec *> PluginSpec::dependencySpecs() const
{
    return d->dependencySpecs;
}

Plugin *PluginSpec::plugin() const
{
    return d->plugin;
}

bool PluginSpec::isDisabledByDefault() const
{
    return d->disabledByDefault;
}

bool PluginSpec::isEnabled() const
{
    return d->enabled;
}

bool PluginSpec::isDisabledIndirectly() const
{
    return d->disabledIndirectly;
}

void PluginSpec::setEnabled(bool value)
{
    d->enabled = value;
}

void PluginSpec::setDisabledByDefault(bool value)
{
    d->disabledByDefault = value;
}

void PluginSpec::setDisabledIndirectly(bool value)
{
    d->disabledIndirectly = value;
}

// ========== PluginSystem::Internal::PluginSpecPrivate ========== //

namespace {
    const char PLUGIN[]               = "plugin";
    const char PLUGIN_NAME[]          = "name";
    const char PLUGIN_VERSION[]       = "version";
    const char PLUGIN_COMPATVERSION[] = "compatVersion";
    const char VENDOR[]               = "vendor";
    const char COPYRIGHT[]            = "copyright";
    const char LICENSE[]              = "license";
    const char DESCRIPTION[]          = "description";
    const char URL[]                  = "url";
    const char CATEGORY[]             = "category";
    const char DEPENDENCYLIST[]       = "dependencyList";
    const char DEPENDENCY[]           = "dependency";
    const char DEPENDENCY_NAME[]      = "name";
    const char DEPENDENCY_VERSION[]   = "version";
    const char DEPENDENCY_TYPE[]      = "type";
    const char DEPENDENCY_TYPE_SOFT[] = "optional";
    const char DEPENDENCY_TYPE_HARD[] = "required";
}

bool PluginSpecPrivate::read(const QString &fileName)
{
    name
            = version
            = compatVersion
            = vendor
            = copyright
            = license
            = description
            = url
            = "";
    state = PluginSpec::Invalid;
    setErrorString("");
    dependencyList.clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return reportError(tr("Cannot open spec file %1 for reading: %2")
                           .arg(QDir::toNativeSeparators(file.fileName()), file.errorString()));
    }
    QFileInfo fileInfo(file);
    location = fileInfo.absolutePath();
    filePath = fileInfo.absoluteFilePath();

    QXmlStreamReader reader(&file);
    while (!reader.atEnd()) {
        reader.readNext();
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:
            readPluginSpec(reader);
            break;
        default:
            break;
        }
    }
    if (reader.hasError()) {
        return reportError(tr("Error parsing file %1: %2, at line %3, column %4")
                           .arg(QDir::toNativeSeparators(file.fileName()))
                           .arg(reader.errorString())
                           .arg(reader.lineNumber())
                           .arg(reader.columnNumber()));
    }
    state = PluginSpec::Read;
    return true;
}

bool PluginSpecPrivate::provides(const QString &pluginName, const QString &pluginVersion) const
{
    if (QString::compare(pluginName, name, Qt::CaseInsensitive) != 0) {
        return false;
    }
    return (compareVersion(version, pluginVersion) >= 0) && (compareVersion(compatVersion, pluginVersion) <= 0);
}

bool PluginSpecPrivate::load()
{
    if (hasError) {
        return false;
    }
    if (state != PluginSpec::Resolved) {
        if (state == PluginSpec::Loaded) {
            return true;
        }
        setErrorString(QCoreApplication::translate("PluginSpec", "Loading the library failed because state != Resolved"));
        return false;
    }
#ifdef QT_NO_DEBUG

#ifdef Q_OS_WIN
    QString libName = QString("%1/%2.dll").arg(location).arg(name);
#elif defined(Q_OS_MAC)
    QString libName = QString("%1/lib%2.dylib").arg(location).arg(name);
#else
    QString libName = QString("%1/lib%2.so").arg(location).arg(name);
#endif

#else // Q_NO_DEBUG

#ifdef Q_OS_WIN
    QString libName = QString("%1/%2d.dll").arg(location).arg(name);
#elif defined(Q_OS_MAC)
    QString libName = QString("%1/lib%2_debug.dylib").arg(location).arg(name);
#else
    QString libName = QString("%1/lib%2.so").arg(location).arg(name);
#endif

#endif

    QPluginLoader loader(libName);
    if (!loader.load()) {
        setErrorString(QDir::toNativeSeparators(libName) + QString::fromLatin1(": ") + loader.errorString());
        return false;
    }
    Plugin *pluginObject = qobject_cast<Plugin*>(loader.instance());
    if (!pluginObject) {
        setErrorString(QCoreApplication::translate("PluginSpec", "Plugin is not valid (does not derive from IPlugin)"));
        loader.unload();
        return false;
    }
    state = PluginSpec::Loaded;
    plugin = pluginObject;
    plugin->d->pluginSpec = q;
    return true;
}

void PluginSpecPrivate::kill()
{
    if (!plugin) {
        return;
    }
    delete plugin;
    plugin = 0;
    state = PluginSpec::Deleted;
}

Plugin::ShutdownFlag PluginSpecPrivate::stop()
{
    if (!plugin) {
        return Plugin::SynchronousShutdown;
    }
    state = PluginSpec::Stopped;
    return plugin->aboutToShutdown();
}

bool PluginSpecPrivate::isValidVersion(const QString &version)
{
    return versionRegExp().exactMatch(version);
}

int PluginSpecPrivate::compareVersion(const QString &version1, const QString &version2)
{
    QRegExp reg1 = versionRegExp();
    QRegExp reg2 = versionRegExp();
    if (!reg1.exactMatch(version1)) {
        return 0;
    }
    if (!reg2.exactMatch(version2)) {
        return 0;
    }
    int number1;
    int number2;
    for (int i = 0; i < 4; ++i) {
        number1 = reg1.cap(i+1).toInt();
        number2 = reg2.cap(i+1).toInt();
        if (number1 < number2) {
            return -1;
        }
        if (number1 > number2) {
            return 1;
        }
    }
    return 0;
}

QRegExp & PluginSpecPrivate::versionRegExp()
{
    static QRegExp reg("([0-9]+)(?:[.]([0-9]+))?(?:[.]([0-9]+))?(?:_([0-9]+))?");
    return reg;
}

static inline QString errAttributeMissing(const char *elt, const char *attribute)
{
    return QCoreApplication::translate("PluginSpec", "'%1' misses attribute '%2'").arg(QLatin1String(elt), QLatin1String(attribute));
}

static inline QString errInvalidFormat(const char *content)
{
    return QCoreApplication::translate("PluginSpec", "'%1' has invalid format").arg(content);
}

static inline QString errInvalidElement(const QString &name)
{
    return QCoreApplication::translate("PluginSpec", "Invalid element '%1'").arg(name);
}

static inline QString errUnexpectedToken()
{
    return QCoreApplication::translate("PluginSpec", "Unexpected token");
}

static inline QString errUnexpectedClosing(const QString &name)
{
    return QCoreApplication::translate("PluginSpec", "Unexpected closing element '%1'").arg(name);
}

void PluginSpecPrivate::readPluginSpec(QXmlStreamReader &reader)
{
    QString element = reader.name().toString();
    if (element != QString(PLUGIN)) {
        reader.raiseError(QCoreApplication::translate("PluginSpec", "Expected element '%1' as top level element").arg(PLUGIN));
        return;
    }
    name = reader.attributes().value(PLUGIN_NAME).toString();
    if (name.isEmpty()) {
        reader.raiseError(errAttributeMissing(PLUGIN, PLUGIN_NAME));
        return;
    }
    version = reader.attributes().value(PLUGIN_VERSION).toString();
    if (version.isEmpty()) {
        reader.raiseError(errAttributeMissing(PLUGIN, PLUGIN_VERSION));
        return;
    }
    if (!isValidVersion(version)) {
        reader.raiseError(errInvalidFormat(PLUGIN_VERSION));
        return;
    }
    compatVersion = reader.attributes().value(PLUGIN_COMPATVERSION).toString();
    if (!compatVersion.isEmpty() && !isValidVersion(compatVersion)) {
        reader.raiseError(errInvalidFormat(PLUGIN_COMPATVERSION));
        return;
    } else if (compatVersion.isEmpty()) {
        compatVersion = version;
    }
    while (!reader.atEnd()) {
        reader.readNext();
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:
            element = reader.name().toString();
            if (element == VENDOR) {
                vendor = reader.readElementText().trimmed();
            } else if (element == COPYRIGHT) {
                copyright = reader.readElementText().trimmed();
            } else if (element == LICENSE) {
                license = reader.readElementText().trimmed();
            } else if (element == DESCRIPTION) {
                description = reader.readElementText().trimmed();
            } else if (element == URL) {
                url = reader.readElementText().trimmed();
            } else if (element == CATEGORY) {
                category = reader.readElementText().trimmed();
            } else if (element == DEPENDENCYLIST) {
                readDependencies(reader);
            } else {
                reader.raiseError(errInvalidElement(name));
            }
            break;
        case QXmlStreamReader::EndDocument:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::Characters:
            break;
        default:
            reader.raiseError(errUnexpectedToken());
            break;
        }
    }
}

void PluginSpecPrivate::readDependencies(QXmlStreamReader &reader)
{
    QString element;
    while (!reader.atEnd()) {
        reader.readNext();
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:
            element = reader.name().toString();
            if (element == DEPENDENCY) {
                readDependencyEntry(reader);
            } else {
                reader.raiseError(errInvalidElement(name));
            }
            break;
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::Characters:
            break;
        case QXmlStreamReader::EndElement:
            element = reader.name().toString();
            if (element == DEPENDENCYLIST)
                return;
            reader.raiseError(errUnexpectedClosing(element));
            break;
        default:
            reader.raiseError(errUnexpectedToken());
            break;
        }
    }
}

void PluginSpecPrivate::readDependencyEntry(QXmlStreamReader &reader)
{
    PluginDependency dep;
    dep.name = reader.attributes().value(DEPENDENCY_NAME).toString();
    if (dep.name.isEmpty()) {
        reader.raiseError(errAttributeMissing(DEPENDENCY, DEPENDENCY_NAME));
        return;
    }
    dep.version = reader.attributes().value(DEPENDENCY_VERSION).toString();
    if (!dep.version.isEmpty() && !isValidVersion(dep.version)) {
        reader.raiseError(errInvalidFormat(DEPENDENCY_VERSION));
        return;
    }
    dep.type = PluginDependency::Required;
    if (reader.attributes().hasAttribute(DEPENDENCY_TYPE)) {
        QString typeValue = reader.attributes().value(DEPENDENCY_TYPE).toString();
        if (typeValue == QLatin1String(DEPENDENCY_TYPE_HARD)) {
            dep.type = PluginDependency::Required;
        } else if (typeValue == QLatin1String(DEPENDENCY_TYPE_SOFT)) {
            dep.type = PluginDependency::Optional;
        } else {
            reader.raiseError(errInvalidFormat(DEPENDENCY_TYPE));
            return;
        }
    }
    dependencyList.append(dep);
    reader.readNext();
    if (reader.tokenType() != QXmlStreamReader::EndElement) {
        reader.raiseError(errUnexpectedToken());
    }
}

bool PluginSpecPrivate::reportError(const QString &err)
{
    setErrorString(err);
    return false;
}

QString PluginSpecPrivate::errorString() const
{
    return m_errorString;
}

void PluginSpecPrivate::setErrorString(const QString &err)
{
    m_errorString = err;
    if (err.isNull() || err.isEmpty()) {
        hasError = false;
    } else {
        qCritical() << m_errorString;
        hasError = true;
    }
}

bool PluginSpecPrivate::initializePlugins()
{
    if (hasError) {
        return false;
    }
    if (state != PluginSpec::Initialized) {
        if (state == PluginSpec::Running) {
            return true;
        }
        setErrorString(QCoreApplication::translate("PluginSpec", "Cannot perform extensionsInitialized because state != Initialized"));
        return false;
    }
    if (!plugin) {
        setErrorString(QCoreApplication::translate("PluginSpec", "Internal error: have no plugin instance to perform extensionsInitialized"));
        return false;
    }
    plugin->dependenciesInitialized();
    state = PluginSpec::Running;
    return true;
}

bool PluginSpecPrivate::initializePlugin()
{
    if (hasError) {
        return false;
    }
    if (state != PluginSpec::Loaded) {
        if (state == PluginSpec::Initialized) {
            return true;
        }
        setErrorString(QCoreApplication::translate("PluginSpec", "Initializing the plugin failed because state != Loaded"));
        return false;
    }
    if (!plugin) {
        setErrorString(QCoreApplication::translate("PluginSpec", "Internal error: have no plugin instance to initialize"));
        return false;
    }
    QString err;
    if (!plugin->initialize(arguments, &err)) {
        setErrorString(QCoreApplication::translate("PluginSpec", "Plugin initialization failed: %1").arg(err));
        return false;
    }
    state = PluginSpec::Initialized;
    return true;
}

PluginSpecPrivate::PluginSpecPrivate(PluginSpec *spec) :
    disabledByDefault(false),
    enabled(true),
    disabledIndirectly(false),
    state(PluginSpec::Invalid),
    hasError(false),
    m_errorString(""),
    q(spec)
{
}

bool PluginSpecPrivate::resolveDependencies(const QList<PluginSpec *> &specs)
{
    if (hasError) {
        return false;
    }
    if (state == PluginSpec::Resolved) {
        state = PluginSpec::Read; // Go back, so we just re-resolve the dependencies.
    }
    if (state != PluginSpec::Read) {
        setErrorString(QCoreApplication::translate("PluginSpec", "Resolving dependencies failed because state != Read"));
        return false;
    }
    QHash<PluginDependency, PluginSpec *> resolvedDependencies;
    foreach (const PluginDependency &dependency, dependencyList) {
        PluginSpec *found = 0;

        foreach (PluginSpec *spec, specs) {
            if (spec->provides(dependency.name, dependency.version)) {
                found = spec;
                break;
            }
        }
        if (!found) {
            if (dependency.type == PluginDependency::Required) {
                QString errStr = errorString();
                if (!errStr.isEmpty()) {
                    errStr.append(QLatin1Char('\n'));
                }
                errStr.append(QCoreApplication::translate("PluginSpec", "Could not resolve dependency '%1(%2)'")
                    .arg(dependency.name).arg(dependency.version));
                setErrorString(errStr);
            }
            continue;
        }
        resolvedDependencies.insert(dependency, found);
    }
    if (hasError) {
        return false;
    }

    dependencySpecs = resolvedDependencies;

    state = PluginSpec::Resolved;

    return true;
}

void PluginSpecPrivate::disableIndirectlyIfDependencyDisabled()
{
    if (!enabled) {
        return;
    }

    if (disabledIndirectly) {
        return;
    }

    QHashIterator<PluginDependency, PluginSpec *> it(dependencySpecs);
    while (it.hasNext()) {
        it.next();
        if (it.key().type == PluginDependency::Optional) {
            continue;
        }
        PluginSpec *dependencySpec = it.value();
        if (dependencySpec->isDisabledIndirectly() || !dependencySpec->isEnabled()) {
            disabledIndirectly = true;
            break;
        }
    }
}
