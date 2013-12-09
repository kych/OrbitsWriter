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

/*!
  Hash functions for PluginDependency.
 */
uint PluginSystem::qHash(const PluginDependency &value)
{
    return qHash(value.name);
}

// ========== PluginSystem::PluginDependency ========== //

/*!
  \struct PluginSystem::PluginDependency
  Struct that contains the name and required compatible version number of a plugin's dependency.

  Each dependency has a name and a minimum version.
  If the dependency does not match, this plugin should not be loaded.
 */

/*!
  \enum PluginDependency::Type
  Dependency type.

  Defines whether the dependency is required or optional.
 */

/*!
  \var PluginDependency::Type PluginDependency::Required
  Dependency needs to be there.
 */

/*!
  \var PluginDependency::Type PluginDependency::Optional
  Dependency is not necessarily needed.

  You need to make sure that the plugin is able to load without
  this dependency installed, so for example you may not link to
  the dependency's library.
 */

/*!
  \fn PluginDependency::PluginDependency()
  Constructs an instance of PluginDependency.
 */

/*!
  \property PluginDependency::name
  Dependency name.
 */

/*!
  \property PluginDependency::version
  Dependency version.
 */

/*!
  \property PluginDependency::type
  Dependency type.
 */

/*!
  Returns true if two PluginDependency instances are equal.
 */
bool PluginDependency::operator ==(const PluginDependency &other) const
{
    return name == other.name
            && version == other.version
            && type == other.type;
}

// ========== PluginSystem::PluginSpec ========== //

/*!
  \class PluginSystem::PluginSpec
  Contains the information of the plugins xml description file and
  information about the plugin's current state.

  The plugin spec is also filled with more information as the plugin
  goes through its loading process (see PluginSpec::State).
  If an error occurs, the plugin spec is the place to look for the error details.

  Spec information is the meta-information about a plugin. Meta-information means
  the name, version number, dependencies and so on. Plugins must give a spec file
  in order to load into application. The spec file should be described in XML format.
  The XML Schema is as following:
  \code
  <?xml version="1.0"?>
  <xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema">
      <xsd:element name="plugin">
          <xsd:complexType>
              <xsd:all>
                  <xsd:element name="vendor" type="xsd:string"/>
                  <xsd:element name="copyright" type="xsd:string"/>
                  <xsd:element name="license" type="xsd:string"/>
                  <xsd:element name="category">
                      <xsd:simpleType>
                          <xsd:restriction base="xsd:string">
                              <xsd:pattern value="BASE|HTML|API"/>
                          </xsd:restriction>
                      </xsd:simpleType>
                  </xsd:element>
                  <xsd:element name="description" type="xsd:string"/>
                  <xsd:element name="url" type="xsd:string"/>
                  <xsd:element name="dependencyList">
                      <xsd:complexType>
                          <xsd:sequence>
                              <xsd:element name="dependency" maxOccurs="unbounded">
                                  <xsd:complexType>
                                      <xsd:attribute name="name" type="xsd:string" use="required"/>
                                      <xsd:attribute name="version" type="xsd:string" use="required"/>
                                  </xsd:complexType>
                              </xsd:element>
                          </xsd:sequence>
                      </xsd:complexType>
                  </xsd:element>
              </xsd:all>
              <xsd:attribute name="name" type="xsd:string" use="required"/>
              <xsd:attribute name="version" type="xsd:string" use="required"/>
              <xsd:attribute name="compatVersion" type="xsd:string" use="required"/>
          </xsd:complexType>
      </xsd:element>
  </xsd:schema>
  \endcode

  As the scheme shows, spec files should have a root node named plugin. The root must have 3 attributes:
  - name: plugin's name;
  - version: plugin's version;
  - compatVersion: plugin's required application's version.

  There are 7 elements and they could in any order:
  - vendor: the plugin's vendor;
  - copyright: the plugin's copyright;
  - license: the plugin's license, must wrap with CDATA. You could put any license contents here;
  - category: the plugin's category, OrbitsWriter classifies this plugin by its category.
  The category should be the same as the interfaces which this plugin implements.
  The valid categories are:
       - BASE: Base plugins.
       - API: API plugins.
       - HTML: HTML plugins.
  - description: the description for this plugin, usually a single sentence to explain what the plugin
  is used for;
  - url: The plugin's URL. Maybe vendor's website or some others;
  - dependencyList: The dependencies for this plugin. It could contain any number of dependency node
  which must have 2 attributes:
       - name: dependency library name;
       - version: dependency library minimum version.

  Usually you need not create the instance of this class. OrbitsWriter will create an instance
  when the plugin loaded successfully and set it to this plugin.

  \note You cannot access any field of spec in constructors of plugins for the spec information
  will be set after getting the instance of plugins.
 */

/*!
  \enum PluginSpec::State
  The state gives a hint on what went wrong in case of an error.
 */

/*!
  \var PluginSpec::State PluginSpec::Invalid
  Starting point: Even the xml description file was not read.
 */

/*!
  \var PluginSpec::State PluginSpec::Read
  The xml description file has been successfully read,
  and its information is available via the PluginSpec.
 */

/*!
  \var PluginSpec::State PluginSpec::Resolved
  The dependencies given in the description file have been
  successfully found, and are available via the dependencySpecs() method.
 */

/*!
  \var PluginSpec::State PluginSpec::Loaded
  The plugin's library is loaded and the plugin instance created
  (available through plugin()).
 */

/*!
  \var PluginSpec::State PluginSpec::Initialized
  The plugin instance's initialize() method has been
  called and returned a success value.
 */

/*!
  \var PluginSpec::State PluginSpec::Running
  The plugin's dependencies are successfully initialized
  and extensionsInitialized has been called. The loading process is complete.
 */

/*!
  \var PluginSpec::State PluginSpec::Stopped
  The plugin has been shut down, i.e. the plugin's
  aboutToShutdown() method has been called.
 */

/*!
  \var PluginSpec::State PluginSpec::Deleted
  The plugin instance has been deleted.
 */

/*!
  Constructs an instance of PluginSpec.
 */
PluginSpec::PluginSpec() :
    d(new PluginSpecPrivate(this))
{
}

/*!
  Destroys the instance of PluginSpec.
 */
PluginSpec::~PluginSpec()
{
    delete d;
    d = 0;
}

/*!
  The plugin name.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::name() const
{
    return d->name;
}

/*!
  The plugin version.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::version() const
{
    return d->version;
}

/*!
  The plugin compatibility version.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::compatVersion() const
{
    return d->compatVersion;
}

/*!
  The plugin vendor.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::vendor() const
{
    return d->vendor;
}

/*!
  The plugin copyright.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::copyright() const
{
    return d->copyright;
}

/*!
  The plugin license.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::license() const
{
    return d->license;
}

/*!
  The category that the plugin belongs to.

  Categories are groups of plugins which allow for keeping them together in the UI.
  Returns an empty string if the plugin does not belong to a category.
 */
QString PluginSpec::category() const
{
    return d->category;
}

/*!
  The plugin description.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::description() const
{
    return d->description;
}

/*!
  The plugin url where you can find more information about the plugin.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::url() const
{
    return d->url;
}

/*!
  The plugin dependencies.

  This is valid after the PluginSpec::Read state is reached.
 */
QList<PluginDependency> PluginSpec::dependencyList() const
{
    return d->dependencyList;
}

/*!
  Return true if there is some error while loading this spec.
 */
bool PluginSpec::hasError() const
{
    return d->hasError;
}

/*!
  Detailed, possibly multi-line, error description in case of an error.
 */
QString PluginSpec::errorString() const
{
    return d->errorString();
}

/*!
  Plugin loads state.
 */
PluginSpec::State PluginSpec::state() const
{
    return d->state;
}

/*!
  The absolute path to the directory containing the plugin xml description file
  this PluginSpec corresponds to.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::location() const
{
    return d->location;
}

/*!
  The absolute path to the plugin xml description file (including the file name)
  this PluginSpec corresponds to.

  This is valid after the PluginSpec::Read state is reached.
 */
QString PluginSpec::filePath() const
{
    return d->filePath;
}

/*!
  Returns if this plugin can be used to fill in a dependency of the given
  \a pluginName and \a version.

  \sa PluginSpec::dependencyList()
 */
bool PluginSpec::provides(const QString &pluginName, const QString &version) const
{
    return d->provides(pluginName, version);
}

/*!
  The plugin dependency specs.

  This is valid after the PluginSpec::Resolved state is reached.
 */
QHash<PluginDependency, PluginSpec *> PluginSpec::dependencySpecs() const
{
    return d->dependencySpecs;
}

/*!
  The plugin of this spec.
 */
Plugin *PluginSpec::plugin() const
{
    return d->plugin;
}

/*!
  Returns if the plugin is disabled by default.
 */
bool PluginSpec::isDisabledByDefault() const
{
    return d->disabledByDefault;
}

/*!
  Returns if the plugin is loaded at startup.

  True by default - the user can change it from the Plugin settings.
 */
bool PluginSpec::isEnabled() const
{
    return d->enabled;
}

/*!
  Returns true if loading was not done due to user unselecting this plugin
  or its dependencies, or if command-line parameter -noload was used.
 */
bool PluginSpec::isDisabledIndirectly() const
{
    return d->disabledIndirectly;
}

/*!
  Sets this plugin enabled or not.
 */
void PluginSpec::setEnabled(bool value)
{
    d->enabled = value;
}

/*!
  Sets this plugin disabled by default or not.
 */
void PluginSpec::setDisabledByDefault(bool value)
{
    d->disabledByDefault = value;
}

/*!
  Sets this plugin disabled indirectly or not.
 */
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
