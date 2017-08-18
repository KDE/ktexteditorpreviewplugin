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

#include "markdownsourcedocument.h"

#include <kmarkdownview.h>

#include <KTextEditor/Document>

#include <QDesktopServices>

MarkdownPreviewWidget::MarkdownPreviewWidget(QObject* parent)
    : KTextEditorPreview::DocumentPreviewWidget(parent)
    , m_markdownSourceDocument(new MarkdownSourceDocument(this))
    , m_widget(new KMarkdownView(m_markdownSourceDocument))
{
    connect(m_widget, &KMarkdownView::openUrlRequested, this, &MarkdownPreviewWidget::handleOpenUrlRequested);
}

MarkdownPreviewWidget::~MarkdownPreviewWidget()
{
    delete m_widget;
}

QWidget* MarkdownPreviewWidget::widget() const
{
    return m_widget;
}

void MarkdownPreviewWidget::handleOpenUrlRequested(const QUrl& url) const
{
    QDesktopServices::openUrl(url);
}

void MarkdownPreviewWidget::setDocument(const KTextEditor::Document* document)
{
    m_markdownSourceDocument->setDocument(document);
}
