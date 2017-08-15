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

#ifndef KTEXTEDITORPREVIEWPLUGIN_H
#define KTEXTEDITORPREVIEWPLUGIN_H

// KF
#include <KTextEditor/Plugin>

// Qt
#include <QHash>

class DocumentPreviewPlugin;

class KTextEditorPreviewPlugin : public KTextEditor::Plugin
{
    Q_OBJECT

public:
    /**
     * Default constructor, with arguments as expected by KPluginFactory
     */
    KTextEditorPreviewPlugin(QObject* parent, const QVariantList& args);

    ~KTextEditorPreviewPlugin() override;

    DocumentPreviewPlugin* pluginForMimeType(const QString& mimeType);

public: // KTextEditor::Plugin API
    QObject* createView(KTextEditor::MainWindow* mainWindow) override;

private:
    QHash<QString,DocumentPreviewPlugin*> m_pluginsByMimeType;
};

#endif
