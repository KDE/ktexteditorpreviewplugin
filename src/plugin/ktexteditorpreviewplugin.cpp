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

#include "ktexteditorpreviewplugin.h"

#include "ktexteditorpreviewview.h"

#include <documentpreviewplugin.h>

// KF
#include <KTextEditor/MainWindow>

#include <KPluginLoader>
#include <KPluginMetaData>
#include <KPluginFactory>
#include <KLocalizedString>

// Qt
#include <QMimeDatabase>
#include <QDebug>


K_PLUGIN_FACTORY_WITH_JSON(KTextEditorPreviewPluginFactory, "ktexteditorpreview.json", registerPlugin<KTextEditorPreviewPlugin>();)


KTextEditorPreviewPlugin::KTextEditorPreviewPlugin(QObject* parent, const QVariantList& /*args*/)
    : KTextEditor::Plugin(parent)
{
}

KTextEditorPreviewPlugin::~KTextEditorPreviewPlugin()
{
}

DocumentPreviewPlugin* KTextEditorPreviewPlugin::pluginForMimeType(const QString& mimeType)
{
    if (m_pluginsByMimeType.isEmpty()) {
        const auto pluginsMetadata = KPluginLoader::findPlugins(QStringLiteral("ktexteditorpreview"));
        for (const auto& metadata : pluginsMetadata) {
            KPluginLoader loader(metadata.fileName());
            auto factory = loader.factory();
            if (!factory) {
                qWarning() << "Can't load plugin" << metadata.pluginId()
                        << "because a factory to load the plugin could not be obtained:" << loader.errorString();
                return nullptr;
            }

            // now create it
            auto previewPlugin = factory->create<DocumentPreviewPlugin>(this);
            if (!previewPlugin) {
                qWarning() << "Could not instantiate plugin" << metadata.fileName() << "\": not a DocumentPreviewPlugin class.";
                continue;
            }

            for (auto mimetype : metadata.mimeTypes()) {
                m_pluginsByMimeType.insert(mimetype, previewPlugin);
            }
        }
    }

    DocumentPreviewPlugin* plugin = nullptr;

    const auto documentMimetype = QMimeDatabase().mimeTypeForName(mimeType);

    // TODO: needs check if there is a more specialized preview plugin (e.g. should svg > xml)
    if (documentMimetype.isValid()) {
        auto it = m_pluginsByMimeType.constBegin();
        const auto end = m_pluginsByMimeType.constEnd();
        for (; it != end; ++it) {
            if (documentMimetype.inherits(it.key())) {
                plugin = it.value();
                break;
            }
        }
    }

    return plugin;
}

QObject* KTextEditorPreviewPlugin::createView(KTextEditor::MainWindow* mainwindow)
{
    return new KTextEditorPreviewView(this, mainwindow);
}


// needed for K_PLUGIN_FACTORY_WITH_JSON
#include <ktexteditorpreviewplugin.moc>
