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

#include "kpartview.h"

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
#include <QDebug>


static const int updateDelay = 300; // ms

KPartView::KPartView(const KService::Ptr& service, QObject* parent)
    : QObject(parent)
{
    QString errorString;
    m_part = service->createInstance<KParts::ReadOnlyPart>(nullptr, this, QVariantList(), &errorString);

    if (!m_part) {
        m_errorLabel = new QLabel(errorString);
    } else {
        m_updateTimer.setSingleShot(true);
        m_updateTimer.setInterval(updateDelay);
        connect(&m_updateTimer, &QTimer::timeout, this, &KPartView::updatePreview);

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
        m_updateTimer.stop();
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
    if (!m_updateTimer.isActive()) {
        m_updateTimer.start();
    }
}

void KPartView::updatePreview()
{
    // try to stream the data to avoid filesystem I/O
    if (m_part->openStream(m_document->mimeType(), QUrl(QStringLiteral("ktexteditorpreview:/data")))) {
        m_part->writeStream(m_document->text().toUtf8());
        m_part->closeStream();
        return;
    }

    // have to go via filesystem for now, not nice
    if (!m_helperFile) {
        m_helperFile = new QTemporaryFile(this);
    } else {
        // drop any old data
        m_helperFile->close();
    }

    // write current data
    m_helperFile->open();
    m_helperFile->write(m_document->text().toUtf8());
    m_helperFile->flush();

    // TODO: find out why we need to send this queued
    QMetaObject::invokeMethod(m_part, "openUrl", Qt::QueuedConnection, Q_ARG(QUrl, QUrl::fromLocalFile(m_helperFile->fileName())));
}

void KPartView::handleOpenUrlRequest(const QUrl& url)
{
    QDesktopServices::openUrl(url);
}
