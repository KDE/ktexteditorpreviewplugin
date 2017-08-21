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

#ifndef KPARTVIEW_H
#define KPARTVIEW_H

#include <KService>

#include <QObject>
#include <QTimer>

namespace KTextEditor {
class Document;
}
namespace KParts {
class ReadOnlyPart;
}
class QLabel;
class QTemporaryFile;

class KPartView : public QObject
{
    Q_OBJECT

public:
    KPartView(const KService::Ptr& service, QObject* parent);
    ~KPartView() override;

    /**
     * Returns the widget object, Ownership is not transferred.
     */
    QWidget* widget() const;

    void setDocument(KTextEditor::Document* document);

    KTextEditor::Document* document() const;

private:
    void triggerUpdatePreview();
    void updatePreview();
    void handleOpenUrlRequest(const QUrl& url);

private:
    QLabel* m_errorLabel = nullptr;
    KParts::ReadOnlyPart* m_part = nullptr;
    KTextEditor::Document* m_document = nullptr;

    QTimer m_updateTimer;
    QTemporaryFile* m_helperFile = nullptr;
};

#endif
