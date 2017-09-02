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
#include <ktepreview_debug.h>

// KF
#include <KTextEditor/View>
#include <KTextEditor/Document>

#include <KService>
#include <KMimeTypeTrader>

#include <KLocalizedString>
#include <KToggleAction>
#include <KGuiItem>

// Qt
#include <QLabel>
#include <QIcon>
#include <QAction>


PreviewWidget::PreviewWidget(KTextEditorPreviewPlugin* core, QWidget* parent)
    : QStackedWidget(parent)
    , m_core(core)
{
    m_lockAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("object-unlocked")), i18n("Lock Current Document"), this);
    m_lockAction->setToolTip(i18n("Lock preview to current document"));
    m_lockAction->setCheckedState(KGuiItem(i18n("Unlock Current View"), QIcon::fromTheme(QStringLiteral("object-locked")), i18n("Unlock current view")));
    m_lockAction->setChecked(false);
    connect(m_lockAction, &QAction::triggered, this, &PreviewWidget::toggleDocumentLocking);
    addAction(m_lockAction);

    auto label = new QLabel(i18n("No preview available."), this);
    label->setAlignment(Qt::AlignHCenter);
    addWidget(label);
}

PreviewWidget::~PreviewWidget() = default;

void PreviewWidget::setTextEditorView(KTextEditor::View* view)
{
    // cache active view
    m_currentTextEditorView = view;

    if (!isVisible()) {
        return;
    }

    if (m_lockAction->isChecked()) 
        return;

    KService::Ptr service;
    if (view) {
        service = KMimeTypeTrader::self()->preferredService(view->document()->mimeType(),
                                                            QStringLiteral("KParts/ReadOnlyPart"));
        if (service) {
            qCDebug(KTEPREVIEW) << "Found preferred kpart service named" << service->name()
                                << "with library" <<service->library()
                                << "for mimetype" << view->document()->mimeType();

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
            qCDebug(KTEPREVIEW) << "Found no preferred kpart service for mimetype" << view->document()->mimeType();
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
        m_partView->setDocument(view->document());
    }
}

void PreviewWidget::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    setTextEditorView(m_currentTextEditorView);
}

void PreviewWidget::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    // keep active part for reuse, but close preview document
    if (m_partView) {
        m_partView->setDocument(nullptr);
    }
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
        setTextEditorView(m_currentTextEditorView);
    }
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
