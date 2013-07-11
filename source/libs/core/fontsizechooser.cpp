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

#include "fontsizechooser.h"

namespace GOW
{

class FontSizeChooser::Private : public QObject
{
    Q_OBJECT
public:
    Private(FontSizeChooser *q_ptr) : QObject(q_ptr), q(q_ptr) {}
public slots:
    void itemActivated(int index)
    {
        emit q->fontSizeActivated(q->itemData(index).toInt());
    }
private:
    Q_POINTER(FontSizeChooser)
}; // end of class GOW::FontSizeChooser::Private


class FontSizeChooserItemDelegate : public QStyledItemDelegate
{
public:
    FontSizeChooserItemDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &,
                   const QModelIndex &index) const;
}; // end of class GOW::FontSizeChooserItemDelegate

void FontSizeChooserItemDelegate::paint(QPainter *painter,
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
    int px = index.model()->data(index, Qt::UserRole).value<int>();
    QFont font;
    font.setPixelSize(px);
    painter->setFont(font);
    painter->drawText(option.rect.adjusted(3, 0, 0, 0), Qt::AlignVCenter, displayText);
    painter->restore();
}

QSize FontSizeChooserItemDelegate::sizeHint(const QStyleOptionViewItem &,
                                            const QModelIndex &index) const
{
    QSize size;
    int px = index.model()->data(index, Qt::UserRole).value<int>();
    QFont font;
    font.setPixelSize(px);
    QFontMetrics m(font);
    size.setWidth(m.width(index.model()->data(index).toString()));
    size.setHeight(m.height() + 10);
    return size;
}


/*!
  \class GOW::FontSizeChooser

  A font size selector according to HTML standard.

  There are 7 levels of font size with its size in pixel:
  - xx-small: 5px;
  - x-small: 7px;
  - small: 11px;
  - medium: 16px;
  - large: 24px;
  - x-large: 36px;
  - xx-large: 54px.
 */

FontSizeChooser::FontSizeChooser(QWidget *parent) :
    QComboBox(parent),
    d(this)
{
    addItem(tr("xx-small"), 5);
    addItem(tr("x-small"), 7);
    addItem(tr("small"), 11);
    addItem(tr("medium"), 16);
    addItem(tr("large"), 24);
    addItem(tr("x-large"), 36);
    addItem(tr("xx-large"), 54);
    setCurrentIndex(3);

    setItemDelegate(new FontSizeChooserItemDelegate(this));
    connect(this, SIGNAL(activated(int)),
            d.get(), SLOT(itemActivated(int)));
}

FontSizeChooser::~FontSizeChooser()
{
}

void FontSizeChooser::showPopup()
{
    view()->setFixedWidth(220);
    QComboBox::showPopup();
}

}

#include "fontsizechooser.moc"
