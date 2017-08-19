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

#ifndef MARKDOWNPART_H
#define MARKDOWNPART_H

// KF
#include <KParts/ReadOnlyPart>

// Qt
#include <QByteArray>

class MarkdownBrowserExtension;
class MarkdownSourceDocument;
class KMarkdownView;

class MarkdownPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

public:
    /**
     * Default constructor, with arguments as expected by KPluginFactory
     */
    MarkdownPart(QWidget* parentWidget, QObject* parent, const QVariantList& arg);

    /**
     * Destructor
     */
    ~MarkdownPart() override;

    KMarkdownView* view() const;

protected: // KParts::ReadOnlyPart API
    bool openFile() override;

    bool doOpenStream(const QString& mimeType) override;
    bool doWriteStream(const QByteArray& data) override;
    bool doCloseStream() override;

private:
    MarkdownSourceDocument* m_sourceDocument;
    KMarkdownView* m_widget;

    MarkdownBrowserExtension* m_browserExtension;

    QByteArray m_streamedData;
};

inline KMarkdownView* MarkdownPart::view() const { return m_widget; }

#endif
