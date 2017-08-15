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

#include "markdownpreviewpage.h"
#include "documentproxy.h"

#include <KTextEditor/Document>

#include <QWebEngineView>
#include <QWebChannel>


MarkdownPreviewWidget::MarkdownPreviewWidget(QObject* parent)
    : DocumentPreviewWidget(parent)
    , m_widget(new QWebEngineView)
    , m_previewPage(new MarkdownPreviewPage(this))
    , m_documentProxy(new DocumentProxy(this))
{
    m_widget->setPage(m_previewPage);
    m_widget->setContextMenuPolicy(Qt::NoContextMenu);

    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), m_documentProxy);
    m_previewPage->setWebChannel(channel);

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
