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

#include "previewwidget.h"

#include "ktexteditorpreviewplugin.h"
#include "kpartview.h"
#include <ktexteditorpreview_debug.h>

// KF
#include <KTextEditor/View>
#include <KTextEditor/Document>
#include <KTextEditor/MainWindow>

#include <KService>
#include <KMimeTypeTrader>

#include <KLocalizedString>
#include <KToggleAction>
#include <KGuiItem>

// Qt
#include <QLabel>
#include <QIcon>
#include <QAction>


using namespace KTextEditorPreview;

PreviewWidget::PreviewWidget(KTextEditorPreviewPlugin* core, KTextEditor::MainWindow* mainWindow,
                             QWidget* parent)
    : QStackedWidget(parent)
    , m_core(core)
    , m_mainWindow(mainWindow)
{
    m_lockAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("object-unlocked")), i18n("Lock Current Document"), this);
    m_lockAction->setToolTip(i18n("Lock preview to current document"));
    m_lockAction->setCheckedState(KGuiItem(i18n("Unlock Current View"), QIcon::fromTheme(QStringLiteral("object-locked")), i18n("Unlock current view")));
    m_lockAction->setChecked(false);
    connect(m_lockAction, &QAction::triggered, this, &PreviewWidget::toggleDocumentLocking);
    addAction(m_lockAction);

    // TODO: better icon(s)
    const QIcon autoUpdateIcon = QIcon::fromTheme(QStringLiteral("media-playback-start"));
    m_autoUpdateAction = new KToggleAction(autoUpdateIcon, i18n("Automatically Update Preview"), this);
    m_autoUpdateAction->setToolTip(i18n("Enable automatic updates of the preview to the current document content"));
    m_autoUpdateAction->setCheckedState(KGuiItem(i18n("Manually Update Preview"), autoUpdateIcon, i18n("Disable automatic updates of the preview to the current document content")));
    m_autoUpdateAction->setChecked(true);
    connect(m_autoUpdateAction, &QAction::triggered, this, &PreviewWidget::toggleAutoUpdating);
    addAction(m_autoUpdateAction);

    m_updateAction = new QAction(QIcon::fromTheme(QStringLiteral("view-refresh")), i18n("Update Preview"), this);
    m_updateAction->setToolTip(i18n("Update the preview to the current document content"));
    connect(m_updateAction, &QAction::triggered, this, &PreviewWidget::updatePreview);
    m_updateAction->setEnabled(false);
    addAction(m_updateAction);

    auto label = new QLabel(i18n("No preview available."), this);
    label->setAlignment(Qt::AlignHCenter);
    addWidget(label);

    connect(m_mainWindow, SIGNAL(viewChanged(KTextEditor::View*)),
            this, SLOT(setTextEditorView(KTextEditor::View*)));

    setTextEditorView(m_mainWindow->activeView());
}

PreviewWidget::~PreviewWidget() = default;

void PreviewWidget::setTextEditorView(KTextEditor::View* view)
{
    if ((m_previewedTextEditorView == view) ||
        !isVisible() ||
        m_lockAction->isChecked()) {
        return;
    }

    m_previewedTextEditorView = view;

    KService::Ptr service;
    if (m_previewedTextEditorView) {
        // TODO: mimetype is not set for new document which have not been saved yet.
        // needs another way to get this info, or perhaps some proper fix in Kate/Kdevelop
        // to guess the mimetype based on current content, selected mode/highlighting etc.
        // which then also would needs a signal mimetypeChanged and handling here
        const auto mimeType = m_previewedTextEditorView->document()->mimeType();
        service = KMimeTypeTrader::self()->preferredService(mimeType, QStringLiteral("KParts/ReadOnlyPart"));
        if (service) {
            qCDebug(KTEPREVIEW) << "Found preferred kpart service named" << service->name()
                                << "with library" <<service->library()
                                << "for mimetype" << mimeType;

            if (service->library().isEmpty()) {
                qCWarning(KTEPREVIEW) << "Discarding preferred kpart service due to empty library name:" << service->name();
                service.reset();
            }

            // no interest in kparts which also just display the text (like katepart itself)
            // TODO: what about parts which also support importing plain text and turning into richer format
            // and thus have it in their mimetypes list?
            // could that perhaps be solved by introducing the concept of "native" and "imported" mimetypes?
            // or making a distinction between source editors/viewers and final editors/viewers?
            // latter would also help other source editors/viewers like a hexeditor, which "supports" any mimetype
            if (service && service->mimeTypes().contains(QStringLiteral("text/plain"))) {
                qCDebug(KTEPREVIEW) << "Blindly discarding preferred service as it also supports text/plain, to avoid useless plain/text preview.";
                service.reset();
            }
        } else {
            qCDebug(KTEPREVIEW) << "Found no preferred kpart service for mimetype" << mimeType;
        }
    }

    // change of preview type?
    // TODO: find a better id than library?
    const QString serviceId = service ? service->library() : QString();

    if (serviceId != m_currentServiceId) {
        if (m_partView) {
            removeWidget(m_partView->widget());
            delete m_partView;
        }

        m_currentServiceId = serviceId;

        if (service) {
            qCDebug(KTEPREVIEW) << "Creating new kpart service instance.";
            m_partView = new KPartView(service, this);
            m_partView->setAutoUpdating(m_autoUpdateAction->isChecked());
            int index = addWidget(m_partView->widget());
            setCurrentIndex(index);
        } else {
            m_partView = nullptr;
        }
    } else {
        if (m_partView) {
            qCDebug(KTEPREVIEW) << "Reusing active kpart service instance.";
        }
    }

    if (m_partView) {
        m_partView->setDocument(m_previewedTextEditorView->document());
    }

    m_updateAction->setEnabled(m_partView && !m_autoUpdateAction->isChecked());
}

void PreviewWidget::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    m_updateAction->setEnabled(m_partView && !m_autoUpdateAction->isChecked());

    setTextEditorView(m_mainWindow->activeView());
}

void PreviewWidget::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);

    // keep active part for reuse, but close preview document
    if (m_partView) {
        // TODO: we also get hide event in kdevelop when the view is changed,
        // need to find out how to filter this out or how to fix kdevelop
        // so currently keep the preview document
//         m_partView->setDocument(nullptr);
    }

    m_updateAction->setEnabled(false);
}

void PreviewWidget::toggleDocumentLocking(bool locked)
{
    if (locked) {
        if (!m_partView) {
            // nothing to do
            return;
        }
        auto document = m_partView->document();
        connect(document, &KTextEditor::Document::aboutToClose,
                this, &PreviewWidget::handleLockedDocumentClosing);
    } else {
        if (m_partView) {
            auto document = m_partView->document();
            disconnect(document, &KTextEditor::Document::aboutToClose,
                       this, &PreviewWidget::handleLockedDocumentClosing);
        }
        // jump tp current view
        setTextEditorView(m_mainWindow->activeView());
    }
}

void PreviewWidget::toggleAutoUpdating(bool autoRefreshing)
{
    if (!m_partView) {
        // nothing to do
        return;
    }

    m_updateAction->setEnabled(!autoRefreshing && isVisible());
    m_partView->setAutoUpdating(autoRefreshing);
}

void PreviewWidget::updatePreview()
{
    m_partView->updatePreview();
}

void PreviewWidget::handleLockedDocumentClosing()
{
    // remove any current partview
    if (m_partView) {
        removeWidget(m_partView->widget());
        delete m_partView;
        m_partView = nullptr;
    }

    m_currentServiceId.clear();
}
