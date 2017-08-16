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

#include "markdownpreviewwidget.h"

#ifndef USE_QTWEBKIT
#include "markdownpreviewpage.h"
#endif
#include "documentproxy.h"

#include <KTextEditor/Document>

#ifdef USE_QTWEBKIT
#include <QDesktopServices>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#else
#include <QWebEngineView>
#include <QWebChannel>
#endif

MarkdownPreviewWidget::MarkdownPreviewWidget(QObject* parent)
    : DocumentPreviewWidget(parent)
#ifdef USE_QTWEBKIT
    , m_widget(new QWebView)
#else
    , m_widget(new QWebEngineView)
    , m_previewPage(new MarkdownPreviewPage(this))
#endif
    , m_documentProxy(new DocumentProxy(this))
{
#ifdef USE_QTWEBKIT
    auto page = m_widget->page();
    page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(m_widget, &QWebView::linkClicked, this, &MarkdownPreviewWidget::openUrlExternally);
#else
    m_widget->setPage(m_previewPage);
#endif
    m_widget->setContextMenuPolicy(Qt::NoContextMenu);

#ifdef USE_QTWEBKIT
    auto frame = page->mainFrame();
    frame->addToJavaScriptWindowObject("sourceTextObject", m_documentProxy);
#else
    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), m_documentProxy);
    m_previewPage->setWebChannel(channel);
#endif

    m_widget->setUrl(QUrl("qrc:/ktexteditorpreview/markdown/index.html"));
}

MarkdownPreviewWidget::~MarkdownPreviewWidget()
{
    delete m_widget;
}

QWidget* MarkdownPreviewWidget::widget() const
{
    return m_widget;
}

void MarkdownPreviewWidget::setDocument(const KTextEditor::Document* document)
{
    m_documentProxy->setDocument(document);
}

#ifdef USE_QTWEBKIT
void MarkdownPreviewWidget::openUrlExternally(const QUrl& url)
{
    QDesktopServices::openUrl(url);
}
#endif
