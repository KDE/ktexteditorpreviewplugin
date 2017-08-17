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

#ifndef SVGPREVIEWWIDGET_H
#define SVGPREVIEWWIDGET_H

#include <documentpreviewwidget.h>

class QSvgWidget;

class SvgPreviewWidget : public KTextEditorPreview::DocumentPreviewWidget
{
    Q_OBJECT

public:
    explicit SvgPreviewWidget(QObject* parent = nullptr);
    ~SvgPreviewWidget() override;

    QWidget* widget() const override;
    void setDocument(const KTextEditor::Document* document) override;

private:
    void updatePreview();

private:
    QSvgWidget* m_widget = nullptr;
    const KTextEditor::Document* m_document = nullptr;
};

#endif
