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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

// Qt headers
#include <QWidget>

class KTextEditorPreviewPlugin;

class DocumentPreviewPlugin;
class DocumentPreviewWidget;

namespace KTextEditor {
class View;
}
class KToggleAction;
class QStackedWidget;

class PreviewWidget: public QWidget
{
    Q_OBJECT

public:
    PreviewWidget(KTextEditorPreviewPlugin* core, QWidget* parent);
    ~PreviewWidget() override;

public:
    void setTextEditorView(KTextEditor::View* view);

private:
    QStackedWidget* m_stackedWidget;
    KToggleAction* m_lockAction;

    KTextEditorPreviewPlugin* const m_core;
    DocumentPreviewPlugin* m_currentPlugin = nullptr;
    DocumentPreviewWidget* m_previewWidget = nullptr;
};

#endif