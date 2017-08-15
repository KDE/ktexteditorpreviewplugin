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

#include "svgpreviewwidget.h"

#include <KTextEditor/Document>

#include <QSvgWidget>

SvgPreviewWidget::SvgPreviewWidget(QObject* parent)
    : DocumentPreviewWidget(parent)
    , m_widget(new QSvgWidget)
{
}

SvgPreviewWidget::~SvgPreviewWidget()
{
    delete m_widget;
}

QWidget* SvgPreviewWidget::widget() const
{
    return m_widget;
}

void SvgPreviewWidget::setDocument(const KTextEditor::Document* document)
{
    if (m_document == document) {
        return;
    }

    if (m_document) {
        disconnect(m_document, &KTextEditor::Document::textChanged, this, &SvgPreviewWidget::updatePreview);
    }

    m_document = document;

    if (m_document) {
        updatePreview();
        connect(m_document, &KTextEditor::Document::textChanged, this, &SvgPreviewWidget::updatePreview);
    } else {
        m_widget->load(QByteArray());
    }
}

void SvgPreviewWidget::updatePreview()
{
    m_widget->load(m_document->text().toUtf8());
}
