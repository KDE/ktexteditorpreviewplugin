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

#include "kmarkdownview.h"

#ifndef USE_QTWEBKIT
#include "kmarkdownviewpage.h"
#endif

#include "kabstractmarkdownsourcedocument.h"

#ifdef USE_QTWEBKIT
#include <QWebPage>
#include <QWebFrame>
#else
#include <QWebChannel>
#endif

KMarkdownView::KMarkdownView(KAbstractMarkdownSourceDocument* sourceDocument, QWidget* parent)
#ifdef USE_QTWEBKIT
    : QWebView(parent)
#else
    : QWebEngineView(parent)
    , m_viewPage(new KMarkdownViewPage(this))
#endif
    , m_sourceDocument(sourceDocument)
{
#ifdef USE_QTWEBKIT
    auto page = this->page();
    page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this, &QWebView::linkClicked, this, &KMarkdownView::openUrlRequested);
#else
    setPage(m_viewPage);
    connect(m_viewPage, &KMarkdownViewPage::openUrlRequested, this, &KMarkdownView::openUrlRequested);
#endif
    setContextMenuPolicy(Qt::NoContextMenu);

#ifdef USE_QTWEBKIT
    auto frame = page->mainFrame();
    frame->addToJavaScriptWindowObject("sourceTextObject", m_sourceDocument);
#else
    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), m_sourceDocument);
    m_viewPage->setWebChannel(channel);
#endif

    setUrl(QUrl("qrc:/kmarkdownview/index.html"));
}

KMarkdownView::~KMarkdownView() = default;

KMarkdownView::ScrollPosition KMarkdownView::scrollPosition() const
{
#ifdef USE_QTWEBKIT
    return page()->mainFrame()->scrollPosition();
#else
    return page()->scrollPosition();
#endif
}
