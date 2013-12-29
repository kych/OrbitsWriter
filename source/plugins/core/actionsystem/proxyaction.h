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

#ifndef PROXYACTION_H
#define PROXYACTION_H

#include <QAction>
#include <QPointer>

namespace Core
{

class ProxyAction : public QAction
{
    Q_OBJECT
public:
    enum Attribute {
        Hide = 0x01,
        UpdateText = 0x02,
        UpdateIcon = 0x04
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    explicit ProxyAction(QObject *parent = 0);

    void initialize(QAction *action);

    void setAction(QAction *action);
    QAction *action() const;

    void setAttribute(Attribute attr);
    void removeAttribute(Attribute attr);
    bool hasAttribute(Attribute attr);

    bool shortcutVisibleInToolTip() const;
    void setShortcutVisibleInToolTip(bool visible);

    static QString stringWithAppendedShortcut(const QString &str, const QKeySequence &shortcut);

private slots:
    void actionChanged();
    void updateState();
    void updateToolTipWithKeySequence();

private:
    void disconnectAction();
    void connectAction();
    void update(QAction *action, bool initialize);

    QPointer<QAction> m_action;
    Attributes m_attributes;
    bool m_showShortcut;
    QString m_toolTip;
    bool m_block;
}; // end of class Core::ProxyAction

} // end of namespace Core

#endif // PROXYACTION_H
