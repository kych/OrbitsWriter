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
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "plugincollection.h"
#include "pluginmanager.h"
#include "pluginspec.h"
#include "pluginview.h"

using namespace PluginSystem;

PluginView::PluginView(QWidget *parent) :
    QWidget(parent)
{
    detailButton = new QPushButton(tr("Details"), this);

    errButton = new QPushButton(tr("Errors"), this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(QDialogButtonBox::Close);
    buttonBox->addButton(detailButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(errButton, QDialogButtonBox::ActionRole);

    pluginTreeWidget = new QTreeWidget(this);
    QStringList headers;
    headers << tr("Name") << tr("Loaded") << tr("Version") << tr("Vendor");
    pluginTreeWidget->setHeaderLabels(headers);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(pluginTreeWidget);
    layout->addWidget(buttonBox);

    setLayout(layout);

    foreach (PluginCollection *collection, PluginManager::instance()->pluginCollections()) {
        if (collection->plugins().isEmpty()) {
            continue;
        }
        QTreeWidgetItem *root = new QTreeWidgetItem(pluginTreeWidget, QStringList(collection->name()));
        root->setExpanded(true);
        foreach (PluginSpec *spec, collection->plugins()) {
            QStringList data;
            data << spec->name() << "" << spec->version() << spec->vendor();
            QTreeWidgetItem *node = new QTreeWidgetItem(root, data);
            node->setFlags(Qt::ItemIsEnabled);
            if (spec->hasError()) {
                node->setCheckState(1, Qt::Checked);
            } else {
                node->setCheckState(1, Qt::Unchecked);
            }
            root->addChild(node);
        }
    }
}
