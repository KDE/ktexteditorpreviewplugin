/*
 *  Copyright (C) 2017 by Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "kpartview.h"

#include <ktepreview_debug.h>

// KF
#include <KTextEditor/Document>

#include <KLocalizedString>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KParts/ReadOnlyPart>
#include <KParts/BrowserExtension>

// Qt
#include <QDesktopServices>
#include <QTemporaryFile>
#include <QLabel>


static const int updateDelay = 300; // ms

KPartView::KPartView(const KService::Ptr& service, QObject* parent)
    : QObject(parent)
{
    QString errorString;
    m_part = service->createInstance<KParts::ReadOnlyPart>(nullptr, this, QVariantList(), &errorString);

    if (!m_part) {
        m_errorLabel = new QLabel(errorString);
    } else {
        m_updateSquashingTimer.setSingleShot(true);
        m_updateSquashingTimer.setInterval(updateDelay);
        connect(&m_updateSquashingTimer, &QTimer::timeout, this, &KPartView::updatePreview);

        auto browserExtension = m_part->browserExtension();
        if (browserExtension) {
            connect(browserExtension, &KParts::BrowserExtension::openUrlRequestDelayed,
                    this, &KPartView::handleOpenUrlRequest);
        }
    }
}

KPartView::~KPartView()
{
    delete m_errorLabel;
}

QWidget* KPartView::widget() const
{
    return m_part ? m_part->widget() : m_errorLabel;
}

KTextEditor::Document* KPartView::document() const
{
    return m_document;
}

void KPartView::setDocument(KTextEditor::Document* document)
{
    if (m_document == document) {
        return;
    }
    if (!m_part) {
        return;
    }

    if (m_document) {
        disconnect(m_document, &KTextEditor::Document::textChanged, this, &KPartView::triggerUpdatePreview);
        m_updateSquashingTimer.stop();
    }

    m_document = document;

    if (m_document) {
        updatePreview();
        connect(m_document, &KTextEditor::Document::textChanged, this, &KPartView::triggerUpdatePreview);
    } else {
        m_part->closeUrl();
    }
}

void KPartView::triggerUpdatePreview()
{
    if (!m_updateSquashingTimer.isActive()) {
        m_updateSquashingTimer.start();
    }
}

void KPartView::updatePreview()
{
    const auto mimeType = m_document->mimeType();
    KParts::OpenUrlArguments arguments;
    arguments.setMimeType(mimeType);
    m_part->setArguments(arguments);

    // try to stream the data to avoid filesystem I/O
    // create url unique for this document
    // TODO: encode existing url instead, and for yet-to-be-stored docs some other unique id
    const QUrl streamUrl(QStringLiteral("ktexteditorpreview:/object/%1")
                         .arg(reinterpret_cast<quintptr>(m_document), 0, 16));
    if (m_part->openStream(mimeType, streamUrl)) {
        qCDebug(KTEPREVIEW) << "Pushing data via streaming API";
        m_part->writeStream(m_document->text().toUtf8());
        m_part->closeStream();
        return;
    }

    // have to go via filesystem for now, not nice
    // TODO: use a new temporary file for each document, to have some unique url
    qCDebug(KTEPREVIEW) << "Pushing data via temporary file";
    if (!m_bufferFile) {
        m_bufferFile = new QTemporaryFile(this);
    } else {
        // drop any old data
        m_bufferFile->close();
    }

    // write current data
    m_bufferFile->open();
    m_bufferFile->write(m_document->text().toUtf8());
    // truncate at end of new content
    m_bufferFile->resize(m_bufferFile->pos());
    m_bufferFile->flush();

    // TODO: find out why we need to send this queued
    QMetaObject::invokeMethod(m_part, "openUrl", Qt::QueuedConnection, Q_ARG(QUrl, QUrl::fromLocalFile(m_bufferFile->fileName())));
}

void KPartView::handleOpenUrlRequest(const QUrl& url)
{
    QDesktopServices::openUrl(url);
}
