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
#include <QDebug>


PreviewWidget::PreviewWidget(KTextEditorPreviewPlugin* core, QWidget* parent)
    : QStackedWidget(parent)
    , m_core(core)
{
    m_lockAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("object-unlocked")), i18n("Lock Current Document"), this);
    m_lockAction->setToolTip(i18n("Lock preview to current document"));
    m_lockAction->setCheckedState(KGuiItem(i18n("Unlock Current View"), QIcon::fromTheme(QStringLiteral("object-locked")), i18n("Unlock current view")));
    m_lockAction->setChecked(false);
//     connect(m_editRepoAction, &QAction::triggered, this, &SnippetView::slotEditRepo);
    addAction(m_lockAction);

    auto label = new QLabel(i18n("No preview available."), this);
    label->setAlignment(Qt::AlignHCenter);
    addWidget(label);
}

PreviewWidget::~PreviewWidget() = default;

void PreviewWidget::setTextEditorView(KTextEditor::View* view)
{
    if (m_lockAction->isChecked()) 
        return;

    KService::Ptr service;
    if (view) {
        service = KMimeTypeTrader::self()->preferredService(view->document()->mimeType(),
                                                            QStringLiteral("KParts/ReadOnlyPart"));
        // no interest in kparts which also just display the text (like katepart itself)
        // TODO: what about parts which also support importing plain text and turning into richer format
        // and thus have it in their mimetypes list?
        // could that perhaps be solved by introducing the concept of "native" and "imported" mimetypes?
        if (service && service->mimeTypes().contains(QStringLiteral("text/plain"))) {
            service.reset();
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
            m_partView = new KPartView(service, this);
            int index = addWidget(m_partView->widget());
            setCurrentIndex(index);
        } else {
            m_partView = nullptr;
        }
    }

    if (m_partView) {
        m_partView->setDocument(view ? view->document() : nullptr);
    }
}
