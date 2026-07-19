/*
 *  Copyright (C) 2020 KeePassXC Team <team@keepassxc.org>
 *  Copyright (C) 2026 KeePassXC-OLED fork contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DarkStyle.h"

#include <QDialog>
#include <QFile>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

#ifdef Q_OS_MACOS
#include "gui/osutils/OSUtils.h"
#endif

/*
 * OLED pure-black dark theme.
 *
 * True #000000 for primary chrome so OLED pixels can turn off.
 * Slightly elevated near-black surfaces (#0A0A0A / #121212) keep
 * hierarchy and contrast without washing out to stock gray dark.
 */
DarkStyle::DarkStyle()
    : BaseStyle()
{
#ifdef Q_OS_MACOS
    m_drawNativeMacOsToolBar = osUtils->isDarkMode();
#endif
}

QPalette DarkStyle::standardPalette() const
{
    auto palette = BaseStyle::standardPalette();

    // Primary window chrome — pure black for OLED
    palette.setColor(QPalette::Active, QPalette::Window, QRgb(0x000000));
    palette.setColor(QPalette::Inactive, QPalette::Window, QRgb(0x000000));
    palette.setColor(QPalette::Disabled, QPalette::Window, QRgb(0x0A0A0A));

    // Text on pure black
    palette.setColor(QPalette::Active, QPalette::WindowText, QRgb(0xE8E8EA));
    palette.setColor(QPalette::Inactive, QPalette::WindowText, QRgb(0xC8C8CA));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QRgb(0x666666));

    palette.setColor(QPalette::Active, QPalette::Text, QRgb(0xE8E8EA));
    palette.setColor(QPalette::Inactive, QPalette::Text, QRgb(0xC8C8CA));
    palette.setColor(QPalette::Disabled, QPalette::Text, QRgb(0x666666));

    palette.setColor(QPalette::Active, QPalette::PlaceholderText, QRgb(0x6B6B70));
    palette.setColor(QPalette::Inactive, QPalette::PlaceholderText, QRgb(0x6B6B70));
    palette.setColor(QPalette::Disabled, QPalette::PlaceholderText, QRgb(0x555555));

    palette.setColor(QPalette::Active, QPalette::BrightText, QRgb(0xFFFFFF));
    palette.setColor(QPalette::Inactive, QPalette::BrightText, QRgb(0xF0F0F0));
    palette.setColor(QPalette::Disabled, QPalette::BrightText, QRgb(0x888888));

    // Content areas — near-black elevation (still OLED-friendly)
    palette.setColor(QPalette::Active, QPalette::Base, QRgb(0x0A0A0A));
    palette.setColor(QPalette::Inactive, QPalette::Base, QRgb(0x0A0A0A));
    palette.setColor(QPalette::Disabled, QPalette::Base, QRgb(0x121212));

    palette.setColor(QPalette::Active, QPalette::AlternateBase, QRgb(0x121212));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QRgb(0x121212));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QRgb(0x1A1A1A));

    // Tooltips use a deep green accent on near-black
    palette.setColor(QPalette::All, QPalette::ToolTipBase, QRgb(0x0D1F0D));
    palette.setColor(QPalette::All, QPalette::ToolTipText, QRgb(0xD0D0D0));

    // Buttons / chrome
    palette.setColor(QPalette::Active, QPalette::Button, QRgb(0x141414));
    palette.setColor(QPalette::Inactive, QPalette::Button, QRgb(0x141414));
    palette.setColor(QPalette::Disabled, QPalette::Button, QRgb(0x101010));

    palette.setColor(QPalette::Active, QPalette::ButtonText, QRgb(0xE0E0E4));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, QRgb(0xA0A0A4));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QRgb(0x55555A));

    // Selection — keep KeePassXC green family, tuned for OLED
    palette.setColor(QPalette::Active, QPalette::Highlight, QRgb(0x1B5E20));
    palette.setColor(QPalette::Inactive, QPalette::Highlight, QRgb(0x143D16));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QRgb(0x0F2A10));

    palette.setColor(QPalette::Active, QPalette::HighlightedText, QRgb(0xF5F5F5));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QRgb(0xE0E0E0));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QRgb(0x666666));

    // Surface ladder
    palette.setColor(QPalette::All, QPalette::Light, QRgb(0x1A1A1A));
    palette.setColor(QPalette::All, QPalette::Midlight, QRgb(0x141414));
    palette.setColor(QPalette::All, QPalette::Mid, QRgb(0x0F0F0F));
    palette.setColor(QPalette::All, QPalette::Dark, QRgb(0x000000));
    palette.setColor(QPalette::All, QPalette::Shadow, QRgb(0x000000));

    palette.setColor(QPalette::All, QPalette::Link, QRgb(0x6BCF6B));
    palette.setColor(QPalette::Disabled, QPalette::Link, QRgb(0x4A8A4A));
    palette.setColor(QPalette::All, QPalette::LinkVisited, QRgb(0x7AD47A));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited, QRgb(0x4A8A4A));

    return palette;
}

QString DarkStyle::getAppStyleSheet() const
{
    QFile extStylesheetFile(QStringLiteral(":/styles/dark/darkstyle.qss"));
    if (extStylesheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return extStylesheetFile.readAll();
    }
    qWarning("Failed to load dark theme stylesheet.");
    return {};
}

void DarkStyle::polish(QWidget* widget)
{
    if (qobject_cast<QMainWindow*>(widget) || qobject_cast<QDialog*>(widget) || qobject_cast<QMenuBar*>(widget)
        || qobject_cast<QToolBar*>(widget)) {
        auto palette = widget->palette();
        // Force pure black window chrome regardless of system light/dark
        palette.setColor(QPalette::Active, QPalette::Window, QRgb(0x000000));
        palette.setColor(QPalette::Inactive, QPalette::Window, QRgb(0x000000));
        palette.setColor(QPalette::Disabled, QPalette::Window, QRgb(0x0A0A0A));
        widget->setPalette(palette);
    }
}
