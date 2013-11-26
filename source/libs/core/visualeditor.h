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

#ifndef VISUALEDITOR_H
#define VISUALEDITOR_H

#include <QTextEdit>

#include "DPointer"
#include "Editor"

namespace GOW
{

class VisualEditor : public QTextEdit, public Editor
{
    Q_OBJECT
public:
    explicit VisualEditor(QWidget *parent = 0);
    ~VisualEditor();
    
signals:
    
public slots:
    void setTextBold(bool bold);
    void setTextItalic(bool italic);
    void setTextUnderline(bool underline);
    void setTextStrikeOut(bool strike);

    void setTextAlign(TextAlignment alignment);
    void setTextFontFamily(const QString &family);
    void setTextFontSize(int size);

    void setTextColor(const QColor &color);
    void setTextBackgroundColor(const QColor &color);

private:
    D_POINTER
}; // end of class GOW::VisualEditor

} // end of namespace GOW

#endif // VISUALEDITOR_H
