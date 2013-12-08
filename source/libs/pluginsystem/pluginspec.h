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

#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include <QString>

#include "pluginsystem/pluginsystem_global.h"

QT_BEGIN_NAMESPACE
template<typename K, typename V> class QHash;
QT_END_NAMESPACE

namespace PluginSystem
{

class Plugin;

namespace Internal
{
class PluginSpecPrivate;
class PluginManagerPrivate;
} // end of namespace PluginSystem::Internal

/*!
  Struct that contains the name and required compatible version number of a plugin's dependency.

  Each dependency has a name and a minimum version.
  If the dependency does not match, this plugin should not be loaded.
 */
struct PluginDependency
{
    /*!
      Dependency type.

      Defines whether the dependency is required or optional.
     */
    enum Type
    {
        Required, //!< Dependency needs to be there.
        Optional  //!< Dependency is not necessarily needed.
                  //!< You need to make sure that the plugin is able to load without
                  //!< this dependency installed, so for example you may not link to
                  //!< the dependency's library.
    };

    /*!
      Constructs an instance of PluginDependency.
     */
    PluginDependency() : type(Required) {}

    QString name;    //!< Dependency name.
    QString version; //!< Dependency version.
    Type type;       //!< Dependency type.

    /*!
      Returns true if two PluginDependency instances are equal.
     */
    bool operator==(const PluginDependency &other) const;
}; // end of class PluginSystem::PluginDependency

//! Hash functions for PluginDependency.
uint qHash(const PluginDependency &value);

/*!
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
class PLUGINSYSTEM_EXPORT PluginSpec
{
public:
    /*!
      The state gives a hint on what went wrong in case of an error.
     */
    enum State
    {
        Invalid,     //!< Starting point: Even the xml description file was not read.
        Read,        //!< The xml description file has been successfully read,
                     //!< and its information is available via the PluginSpec.
        Resolved,    //!< The dependencies given in the description file have been
                     //!< successfully found, and are available via the dependencySpecs() method.
        Loaded,      //!< The plugin's library is loaded and the plugin instance created
                     //!< (available through plugin()).
        Initialized, //!< The plugin instance's initialize() method has been
                     //!< called and returned a success value.
        Running,     //!< The plugin's dependencies are successfully initialized
                     //!< and extensionsInitialized has been called. The loading process is complete.
        Stopped,     //!< The plugin has been shut down, i.e. the plugin's
                     //!< aboutToShutdown() method has been called.
        Deleted      //!< The plugin instance has been deleted.
    };

    /*!
      Destroys the instance of PluginSpec.
     */
    ~PluginSpec();

    // ---------- plugin properties ---------- //
    /*!
      The plugin name.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString name() const;

    /*!
      The plugin version.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString version() const;

    /*!
      The plugin compatibility version.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString compatVersion() const;

    /*!
      The plugin vendor.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString vendor() const;

    /*!
      The plugin copyright.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString copyright() const;

    /*!
      The plugin license.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString license() const;

    /*!
      The category that the plugin belongs to.

      Categories are groups of plugins which allow for keeping them together in the UI.
      Returns an empty string if the plugin does not belong to a category.
     */
    QString category() const;

    /*!
      The plugin description.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString description() const;

    /*!
      The plugin url where you can find more information about the plugin.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString url() const;

    /*!
      The plugin dependencies.

      This is valid after the PluginSpec::Read state is reached.
     */
    QList<PluginDependency> dependencyList() const;

    // ---------- error ---------- //
    /*!
      Return true if there is some error while loading this spec.
     */
    bool hasError() const;

    /*!
      Detailed, possibly multi-line, error description in case of an error.
     */
    QString errorString() const;

    // ---------- extend information ---------- //
    /*!
      Plugin loads state.
     */
    State state() const;


    /*!
      The absolute path to the directory containing the plugin xml description file
      this PluginSpec corresponds to.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString location() const;

    /*!
      The absolute path to the plugin xml description file (including the file name)
      this PluginSpec corresponds to.

      This is valid after the PluginSpec::Read state is reached.
     */
    QString filePath() const;

    /*!
      Returns if this plugin can be used to fill in a dependency of the given
      \a pluginName and \a version.

      \sa PluginSpec::dependencyList()
     */
    bool provides(const QString &pluginName, const QString &version) const;

    /*!
      The plugin dependency specs.

      This is valid after the PluginSpec::Resolved state is reached.
     */
    QHash<PluginDependency, PluginSpec *> dependencySpecs() const;

    /*!
      The plugin of this spec.
     */
    Plugin *plugin() const;

    /*!
      Returns if the plugin is disabled by default.
     */
    bool isDisabledByDefault() const;

    /*!
      Returns if the plugin is loaded at startup.

      True by default - the user can change it from the Plugin settings.
     */
    bool isEnabled() const;

    /*!
      Returns true if loading was not done due to user unselecting this plugin or its dependencies,
      or if command-line parameter -noload was used.
     */
    bool isDisabledIndirectly() const;

    /*!
      Sets this plugin enabled or not.
     */
    void setEnabled(bool value);

    /*!
      Sets this plugin disabled by default or not.
     */
    void setDisabledByDefault(bool value);

    /*!
      Sets this plugin disabled indirectly or not.
     */
    void setDisabledIndirectly(bool value);

private:
    //! Constructs an instance of PluginSpec.
    PluginSpec();

    Internal::PluginSpecPrivate *d;
    friend class Internal::PluginSpecPrivate;
    friend class Internal::PluginManagerPrivate;
}; // end of class PluginSystem::PluginSpec

} // end of namespace PluginSystem

#endif // PLUGINSPEC_H
