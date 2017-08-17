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

#ifndef QTUIPREVIEWWIDGET_H
#define QTUIPREVIEWWIDGET_H

#include <documentpreviewwidget.h>

#include <QTemporaryFile>

namespace KParts {
class ReadOnlyPart;
}
class QLabel;

class QtUiPreviewWidget : public KTextEditorPreview::DocumentPreviewWidget
{
    Q_OBJECT

public:
    explicit QtUiPreviewWidget(QObject* parent = nullptr);
    ~QtUiPreviewWidget() override;

    QWidget* widget() const override;
    void setDocument(const KTextEditor::Document* document) override;

private:
    void updatePreview();

private:
    QLabel* m_errorLabel = nullptr;
    KParts::ReadOnlyPart* m_uiViewerPart = nullptr;
    const KTextEditor::Document* m_document = nullptr;

    QTemporaryFile m_helperFile;
};

#endif