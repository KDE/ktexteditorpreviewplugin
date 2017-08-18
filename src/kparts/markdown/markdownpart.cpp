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

#include "markdownpart.h"

#include "markdownsourcedocument.h"
#include "markdownbrowserextension.h"

#include <kmarkdownview.h>

// KF
#include <KPluginFactory>
#include <KAboutData>
#include <KLocalizedString>

// Qt
#include <QFile>
#include <QTextStream>

K_PLUGIN_FACTORY(MarkdownPartFactory, registerPlugin<MarkdownPart>();)


MarkdownPart::MarkdownPart(QWidget* parentWidget, QObject* parent, const QVariantList& /*args*/)
    : KParts::ReadOnlyPart(parent)
    , m_sourceDocument(new MarkdownSourceDocument(this))
    , m_widget(new KMarkdownView(m_sourceDocument, parentWidget))
    , m_browserExtension(new MarkdownBrowserExtension(this))
{
    // set component data
    // the first arg must be the same as the subdirectory into which the part's rc file is installed
    KAboutData aboutData("markdownpart", i18n("Markdown"), QStringLiteral("0.1"));
    aboutData.addAuthor(i18n("Friedrich W. H. Kossebau"), i18n("Author"), QStringLiteral("kossebau@kde.org"));
    setComponentData(aboutData);

    // set internal UI
    setWidget(m_widget);

    // set KXMLUI resource file
    setXMLFile(QStringLiteral("markdownpartui.rc"));

    connect(m_widget, &KMarkdownView::openUrlRequested, m_browserExtension, &MarkdownBrowserExtension::requestOpenUrl);
}

MarkdownPart::~MarkdownPart() = default;

bool MarkdownPart::openFile()
{
    QFile file(localFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    QString text = stream.readAll();

    file.close();

    m_sourceDocument->setText(text);

    return true;
}

// needed for K_PLUGIN_FACTORY
#include <markdownpart.moc>
