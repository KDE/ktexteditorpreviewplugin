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

#include "previewwidget.h"

#include "ktexteditorpreviewplugin.h"

#include <documentpreviewwidget.h>
#include <documentpreviewplugin.h>

#include <KTextEditor/View>
#include <KTextEditor/Document>

#include <KLocalizedString>
#include <KToggleAction>
#include <KGuiItem>

#include <QLabel>
#include <QIcon>
#include <QAction>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDebug>

using namespace KTextEditorPreview;

PreviewWidget::PreviewWidget(KTextEditorPreviewPlugin* core, QWidget* parent)
    : QWidget(parent)
    , m_core(core)
{
    m_lockAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("object-unlocked")), i18n("Lock Current Document"), this);
    m_lockAction->setToolTip(i18n("Lock preview to current document"));
    m_lockAction->setCheckedState(KGuiItem(i18n("Unlock Current View"), QIcon::fromTheme(QStringLiteral("object-locked")), i18n("Unlock current view")));
    m_lockAction->setChecked(false);
//     connect(m_editRepoAction, &QAction::triggered, this, &SnippetView::slotEditRepo);
    addAction(m_lockAction);

    auto mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget);

    auto label = new QLabel(i18n("No preview available."), this);
    label->setAlignment(Qt::AlignHCenter);
    m_stackedWidget->addWidget(label);
}

PreviewWidget::~PreviewWidget()
{
    delete m_previewWidget;
}

void PreviewWidget::setTextEditorView(KTextEditor::View* view)
{
    if (m_lockAction->isChecked()) 
        return;

    DocumentPreviewPlugin* plugin = view ? m_core->pluginForMimeType(view->document()->mimeType()) : nullptr;

    // change of preview type?
    if (plugin != m_currentPlugin) {
        if (m_previewWidget) {
            m_stackedWidget->removeWidget(m_previewWidget->widget());
            delete m_previewWidget;
        }

        m_currentPlugin = plugin;

        if (m_currentPlugin) {
            m_previewWidget = m_currentPlugin->createWidget();
            int index = m_stackedWidget->addWidget(m_previewWidget->widget());
            m_stackedWidget->setCurrentIndex(index);
        } else {
            m_previewWidget = nullptr;
        }
    }

    if (m_previewWidget) {
        m_previewWidget->setDocument(view ? view->document() : nullptr);
    }
}
