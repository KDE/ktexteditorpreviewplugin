/*
 *   Copyright (C) 2017 by Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .
 */

#include "markdownbrowserextension.h"

#include "markdownpart.h"
#include <kmarkdownview.h>

MarkdownBrowserExtension::MarkdownBrowserExtension(MarkdownPart* part)
    : KParts::BrowserExtension(part)
    , m_part(part)
{
     emit enableAction("copy", false);
}

void MarkdownBrowserExtension::copy()
{
    m_part->view()->copyText();
}

void MarkdownBrowserExtension::updateEditActions()
{
    emit enableAction("copy", m_part->view()->canCopyText());
}

void MarkdownBrowserExtension::requestOpenUrl(const QUrl& url)
{
    emit openUrlRequest(url);
}

void MarkdownBrowserExtension::requestOpenUrlNewWindow(const QUrl& url)
{
    emit createNewWindow(url);
}
