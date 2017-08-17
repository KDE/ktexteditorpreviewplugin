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

#include "dotpreviewwidget.h"

#include <kgraphviewer_interface.h>

#include <KTextEditor/Document>

#include <KPluginLoader>
#include <KPluginFactory>
#include <KParts/ReadOnlyPart>

#include <QDebug>

DotPreviewWidget::DotPreviewWidget(QObject* parent)
    : KTextEditorPreview::DocumentPreviewWidget(parent)
{
    static KPluginFactory* factory = KPluginLoader(QStringLiteral("kgraphviewerpart")).factory();
    if (factory) {
        m_graphViewerPart = factory->create<KParts::ReadOnlyPart>(QString(), this);
        if (m_graphViewerPart) {
//             m_graphViewer = qobject_cast<KGraphViewer::KGraphViewerInterface*>(m_graphViewerPart);
        }
    }
}

DotPreviewWidget::~DotPreviewWidget()
{
    delete m_graphViewerPart;
}

QWidget* DotPreviewWidget::widget() const
{
    return m_graphViewerPart->widget();
}

void DotPreviewWidget::setDocument(const KTextEditor::Document* document)
{
    if (m_document == document) {
        return;
    }

    if (m_document) {
        disconnect(m_document, &KTextEditor::Document::textChanged, this, &DotPreviewWidget::updatePreview);
    }

    m_document = document;

    if (m_document) {
        updatePreview();
        connect(m_document, &KTextEditor::Document::textChanged, this, &DotPreviewWidget::updatePreview);
    } else {
        m_graphViewerPart->closeUrl();
    }
}

void DotPreviewWidget::updatePreview()
{
// TODO: try to add support for streaming
//     m_graphViewerPart->openStream(mimeType, QUrl("foo:/bar"));
//     m_graphViewerPart->writeStream(m_document->text().toUtf8());
//     m_graphViewerPart->closeStream();
//     m_graphViewerPart->openUrl(m_document->url());

    // have to go via filesystem for now, not nice
    // TODO: add delay for squashing updates
    // drop any old data
    m_helperFile.close();
    // write current data
    m_helperFile.open();
    m_helperFile.write(m_document->text().toUtf8());
    m_helperFile.flush();

    // TODO: find out why we need to send this queued
    QMetaObject::invokeMethod(m_graphViewerPart, "openUrl", Qt::QueuedConnection, Q_ARG(QUrl, QUrl::fromLocalFile(m_helperFile.fileName())));
}
