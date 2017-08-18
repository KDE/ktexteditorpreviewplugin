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

#ifndef KMARKDOWNVIEW_H
#define KMARKDOWNVIEW_H

#include <kmarkdownview_export.h>
#include <kmarkdownview-config.h>

// Qt headers
#ifdef USE_QTWEBKIT
#include <QWebView>
#else
#include <QWebEngineView>
#endif

class KMarkdownViewPage;
class KAbstractMarkdownSourceDocument;
class QUrl;

class KMARKDOWNVIEW_EXPORT KMarkdownView : public
#ifdef USE_QTWEBKIT
    QWebView
#else
    QWebEngineView
#endif
{
    Q_OBJECT

public:
    KMarkdownView(KAbstractMarkdownSourceDocument* sourceDocument, QWidget* parent = nullptr);
    ~KMarkdownView() override;

Q_SIGNALS:
    void openUrlRequested(const QUrl& url);

private:
#ifdef USE_QTWEBKIT
    void openUrlExternally(const QUrl& url);
#endif

private:
#ifndef USE_QTWEBKIT
    KMarkdownViewPage* m_viewPage = nullptr;
#endif

    KAbstractMarkdownSourceDocument* const m_sourceDocument;
};

#endif
