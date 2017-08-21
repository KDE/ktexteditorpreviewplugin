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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

// Qt
#include <QStackedWidget>

class KTextEditorPreviewPlugin;
class KPartView;

namespace KTextEditor {
class View;
}
class KToggleAction;

class PreviewWidget: public QStackedWidget
{
    Q_OBJECT

public:
    PreviewWidget(KTextEditorPreviewPlugin* core, QWidget* parent);
    ~PreviewWidget() override;

public:
    void setTextEditorView(KTextEditor::View* view);

private:
    void toggleDocumentLocking(bool locked);
    void handleLockedDocumentClosing();

private:
    KToggleAction* m_lockAction;

    KTextEditorPreviewPlugin* const m_core;

    KTextEditor::View* m_currentTextEditorView = nullptr;
    QString m_currentServiceId;
    KPartView* m_partView = nullptr;
};

#endif
