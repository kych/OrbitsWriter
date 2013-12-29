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

#include <QDebug>

//#include "core/appcore.h"
#include "proxyaction.h"

using namespace Core;

ProxyAction::ProxyAction(QObject *parent) :
    QAction(parent),
    m_action(0),
    m_attributes(0),
    m_showShortcut(false),
    m_block(false)
{
    connect(this, SIGNAL(changed()), this, SLOT(updateToolTipWithKeySequence()));
    updateState();
}

void ProxyAction::initialize(QAction *action)
{
    update(action, true);
}

void ProxyAction::setAction(QAction *action)
{
    if (m_action == action) {
        return;
    }
    disconnectAction();
    m_action = action;
    connectAction();
    updateState();
}

QAction *ProxyAction::action() const
{
    return m_action;
}

void ProxyAction::setAttribute(ProxyAction::Attribute attr)
{
    m_attributes |= attr;
    updateState();
}

void ProxyAction::removeAttribute(ProxyAction::Attribute attr)
{
    m_attributes &= ~attr;
    updateState();
}

bool ProxyAction::hasAttribute(ProxyAction::Attribute attr)
{
    return m_attributes & attr;
}

bool ProxyAction::shortcutVisibleInToolTip() const
{
    return m_showShortcut;
}

void ProxyAction::setShortcutVisibleInToolTip(bool visible)
{
    m_showShortcut = visible;
    updateToolTipWithKeySequence();
}

QString ProxyAction::stringWithAppendedShortcut(const QString &str, const QKeySequence &shortcut)
{
    return QString::fromLatin1("%1 <span style=\"color: gray; font-size: small\">%2</span>").
            arg(str, shortcut.toString(QKeySequence::NativeText));
}

void ProxyAction::actionChanged()
{
    update(m_action, false);
}

void ProxyAction::updateState()
{
    if (m_action) {
        update(m_action, false);
    } else {
        // no active/delegate action, "visible" action is not enabled/visible
        if (hasAttribute(Hide)) {
            setVisible(false);
        }
        setEnabled(false);
    }
}

void ProxyAction::updateToolTipWithKeySequence()
{
    if (m_block) {
        return;
    }
    m_block = true;
    if (!m_showShortcut || shortcut().isEmpty()) {
        setToolTip(m_toolTip);
    } else {
        setToolTip(stringWithAppendedShortcut(m_toolTip, shortcut()));
    }
    m_block = false;
}

void ProxyAction::disconnectAction()
{
    if (m_action) {
        disconnect(m_action, SIGNAL(changed()), this, SLOT(actionChanged()));
        disconnect(this, SIGNAL(triggered(bool)), m_action, SIGNAL(triggered(bool)));
        disconnect(this, SIGNAL(toggled(bool)), m_action, SLOT(setChecked(bool)));
    }
}

void ProxyAction::connectAction()
{
    if (m_action) {
        connect(m_action, SIGNAL(changed()), this, SLOT(actionChanged()));
        connect(this, SIGNAL(triggered(bool)), m_action, SIGNAL(triggered(bool)));
        connect(this, SIGNAL(toggled(bool)), m_action, SLOT(setChecked(bool)));
    }
}

void ProxyAction::update(QAction *action, bool initialize)
{
    if (!action) {
        return;
    }
    disconnectAction();
    disconnect(this, SIGNAL(changed()), this, SLOT(updateToolTipWithKeySequence()));
    if (initialize) {
        setSeparator(action->isSeparator());
    }
    if (hasAttribute(UpdateIcon) || initialize) {
        setIcon(action->icon());
        setIconText(action->iconText());
        setIconVisibleInMenu(action->isIconVisibleInMenu());
    }
    if (hasAttribute(UpdateText) || initialize) {
        setText(action->text());
        m_toolTip = action->toolTip();
        updateToolTipWithKeySequence();
        setStatusTip(action->statusTip());
        setWhatsThis(action->whatsThis());
    }

    setCheckable(action->isCheckable());

    if (!initialize) {
        setChecked(action->isChecked());
        setEnabled(action->isEnabled());
        setVisible(action->isVisible());
    }
    connectAction();
    connect(this, SIGNAL(changed()), this, SLOT(updateToolTipWithKeySequence()));
}
