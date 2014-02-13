/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2014 devbean@galaxyworld.org
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

#ifndef FORMATTINGCONSTANTS_H
#define FORMATTINGCONSTANTS_H

namespace Formatting {
namespace Constants {

// Context
//! Global context.
const char CONTEXT_FORMATTING[] = "Formatting Context";

// IDs
const char ID_TEXTBOLD[]                   = "OrbitsWriter.Formatting.TextBold";
const char ID_TEXTITALIC[]                 = "OrbitsWriter.Formatting.TextItalic";
const char ID_TEXTSTRIKE[]                 = "OrbitsWriter.Formatting.TextStrike";
const char ID_TEXTUNDERLINE[]              = "OrbitsWriter.Formatting.TextUnderline";
const char ID_TEXTFONT[]                   = "OrbitsWriter.TextFormat.TextFont";
const char ID_TOOLBARTEXTFONT[]            = "OrbitsWriter.TextFormat.ToolBarTextFont";
const char ID_TOOLBARTEXTSIZE[]            = "OrbitsWriter.TextFormat.ToolBarTextSize";
const char ID_TEXTCOLOR[]                  = "OrbitsWriter.TextFormat.TextColor";
const char ID_TOOLBARTEXTCOLOR[]           = "OrbitsWriter.TextFormat.ToolBarTextColor";
const char ID_TEXTBACKGROUNDCOLOR[]        = "OrbitsWriter.TextFormat.TextBackgroundColor";
const char ID_TOOLBARTEXTBACKGROUNDCOLOR[] = "OrbitsWriter.TextFormat.ToolBarTextBackgroundColor";
const char ID_ALIGNCENTER[]                = "OrbitsWriter.TextFormat.AlignCenter";
const char ID_ALIGNLEFT[]                  = "OrbitsWriter.TextFormat.AlignLeft";
const char ID_ALIGNRIGHT[]                 = "OrbitsWriter.TextFormat.AlignRight";
const char ID_ALIGNFILL[]                  = "OrbitsWriter.TextFormat.AlignFill";

} // end of namespace Formatting::Constants
} // end of namespace Formatting

#endif // FORMATTINGCONSTANTS_H
