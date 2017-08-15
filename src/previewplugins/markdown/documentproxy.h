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

#ifndef DOCUMENTPROXY_H
#define DOCUMENTPROXY_H

#include <QObject>

namespace KTextEditor {
class Document;
}

class DocumentProxy : public QObject
{
    Q_OBJECT

    // TODO: see if this could be done without a member which holds a copy
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged FINAL)

public:
    explicit DocumentProxy(QObject *parent) : QObject(parent) {}

    void setDocument(const KTextEditor::Document* document);

    void handleTextChanged();

Q_SIGNALS:
    void textChanged(const QString& text);

private:
    const KTextEditor::Document* m_document = nullptr;
    QString m_text;
};

#endif
