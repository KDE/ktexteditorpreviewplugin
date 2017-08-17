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

#include "qtuipreviewwidget.h"

#include <KTextEditor/Document>

#include <KLocalizedString>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KParts/ReadOnlyPart>

#include <QLabel>
#include <QDebug>

QtUiPreviewWidget::QtUiPreviewWidget(QObject* parent)
    : KTextEditorPreview::DocumentPreviewWidget(parent)
{
    static KPluginFactory* factory = KPluginLoader(QStringLiteral("kuiviewerpart")).factory();
    if (factory) {
        m_uiViewerPart = factory->create<KParts::ReadOnlyPart>(QString(), this);
        if (m_uiViewerPart) {
        }
    }

    if (!m_uiViewerPart) {
        m_errorLabel = new QLabel(i18n("Missing kuiviewerpart."));
    }
}

QtUiPreviewWidget::~QtUiPreviewWidget()
{
    delete m_uiViewerPart;
    delete m_errorLabel;
}

QWidget* QtUiPreviewWidget::widget() const
{
    return m_uiViewerPart ? m_uiViewerPart->widget() : m_errorLabel;
}

void QtUiPreviewWidget::setDocument(const KTextEditor::Document* document)
{
    if (m_document == document) {
        return;
    }
    if (!m_uiViewerPart) {
        return;
    }

    if (m_document) {
        disconnect(m_document, &KTextEditor::Document::textChanged, this, &QtUiPreviewWidget::updatePreview);
    }

    m_document = document;

    if (m_document) {
        updatePreview();
        connect(m_document, &KTextEditor::Document::textChanged, this, &QtUiPreviewWidget::updatePreview);
    } else {
        m_uiViewerPart->closeUrl();
    }
}

void QtUiPreviewWidget::updatePreview()
{
    // TODO: try to add support for streaming
    // have to go via filesystem for now, not nice
    // TODO: add delay for squashing updates

    // drop any old data
    m_helperFile.close();
    // write current data
    m_helperFile.open();
    m_helperFile.write(m_document->text().toUtf8());
    m_helperFile.flush();

    // TODO: find out why we need to send this queued
    QMetaObject::invokeMethod(m_uiViewerPart, "openUrl", Qt::QueuedConnection, Q_ARG(QUrl, QUrl::fromLocalFile(m_helperFile.fileName())));
}
