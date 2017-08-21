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

#include "ktexteditorpreviewview.h"

#include "ktexteditorpreviewplugin.h"
#include "previewwidget.h"

// KF
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/MainWindow>

#include <KLocalizedString>

// Qt
#include <QIcon>
#include <QLayout>

KTextEditorPreviewView::KTextEditorPreviewView(KTextEditorPreviewPlugin* plugin, KTextEditor::MainWindow* mainWindow)
    : QObject(mainWindow)
{
    Q_UNUSED(plugin);

    // Toolview for snippets
    m_toolView = mainWindow->createToolView(plugin, QStringLiteral("ktexteditorpreviewplugin"),
                     KTextEditor::MainWindow::Right,
                     QIcon::fromTheme(QStringLiteral("document-preview")),
                     i18n("Preview"));

    // add preview widget
    m_previewView = new PreviewWidget(plugin, m_toolView.data());
    m_toolView->layout()->setMargin(0);
    m_toolView->layout()->addWidget(m_previewView);
    m_toolView->addActions(m_previewView->actions());

    connect(mainWindow, SIGNAL(viewChanged(KTextEditor::View*)),
            this, SLOT(handleViewChanged(KTextEditor::View*)));

    m_previewView->setTextEditorView(mainWindow->activeView());
}

KTextEditorPreviewView::~KTextEditorPreviewView()
{
}

void KTextEditorPreviewView::handleViewChanged(KTextEditor::View* view)
{
    m_previewView->setTextEditorView(view);
}
