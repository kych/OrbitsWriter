/*-------------------------------------------------
 *
 * OrbitsWriter - An Offline Blog Writer
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

#include <QAbstractItemView>
#include <QPainter>

#include "fontchooser.h"

namespace GOW
{

class FontChooser::Private : public QObject
{
    Q_OBJECT
};

FontChooser::FontChooser(QWidget *parent) :
    QComboBox(parent)
{
    addItem(tr("Song"), QFont(QLatin1String("SimSun")));
    addItem(tr("New Song"), QFont(QLatin1String("NSimSun")));
    addItem(tr("FangSong_GB2312"), QFont(QLatin1String("FangSong_GB2312")));
    addItem(tr("KaiTi_GB2312"), QFont(QLatin1String("KaiTi_GB2312")));
    addItem(tr("Hei"), QFont(QLatin1String("SimHei")));
    addItem(tr("Microsoft YaHei"), QFont(QLatin1String("Microsoft YaHei")));
    addItem(tr("Arial"), QFont(QLatin1String("Arial")));
    addItem(tr("Arial Black"), QFont(QLatin1String("Arial Black")));
    addItem(tr("Times New Roman"), QFont(QLatin1String("Times New Roman")));
    addItem(tr("Courier New"), QFont(QLatin1String("Courier New")));
    addItem(tr("Tahoma"), QFont(QLatin1String("Tahoma")));
    addItem(tr("Verdana"), QFont(QLatin1String("Verdana")));

    setItemDelegate(new FontChooserItemDelegate(this));

    connect(this, SIGNAL(activated(QString)),
            this, SIGNAL(fontFamilyChanged(QString)));
}

void FontChooser::showPopup()
{
    view()->setFixedWidth(200);
    QComboBox::showPopup();
}

FontChooserItemDelegate::FontChooserItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void FontChooserItemDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    QPalette palette = qobject_cast<QComboBox *>(this->parent())->palette();
    const QAbstractItemModel *model = index.model();
    QString displayText = model->data(index, Qt::DisplayRole).toString();
    painter->save();
    if (option.state.testFlag(QStyle::State_MouseOver)) {
        painter->fillRect(option.rect, palette.highlight());
        painter->setPen(palette.highlightedText().color());
    }
    QFont font = index.model()->data(index, Qt::UserRole).value<QFont>();
    font.setPointSize(16);
    painter->setFont(font);
    painter->drawText(option.rect.adjusted(3, 0, 0, 0), Qt::AlignVCenter, displayText);
    painter->restore();
}

QSize FontChooserItemDelegate::sizeHint(const QStyleOptionViewItem &,
                                        const QModelIndex &index) const
{
    QSize size;
    QFont font = index.model()->data(index, Qt::UserRole).value<QFont>();
    QFontMetrics m(font);
    size.setWidth(m.width(index.model()->data(index).toString()));
    size.setHeight(m.height() + 6);
    return size;
}

}

#include "fontchooser.moc"
