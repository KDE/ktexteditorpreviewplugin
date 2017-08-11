/*
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>
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

// KF headers
#include <KTextEditor/MainWindow>

#include <KPluginFactory>
#include <KLocalizedString>

K_PLUGIN_FACTORY_WITH_JSON(ktexteditorpreviewPluginFactory, "ktexteditorpreview.json", registerPlugin<ktexteditorpreviewPlugin>();)


ktexteditorpreviewPlugin::ktexteditorpreviewPlugin(QObject* parent, const QVariantList& /*args*/)
    : KTextEditor::Plugin(parent)
{
}

ktexteditorpreviewPlugin::~ktexteditorpreviewPlugin()
{
}

QObject* ktexteditorpreviewPlugin::createView(KTextEditor::MainWindow* mainwindow)
{
    return new ktexteditorpreviewView(this, mainwindow);
}


// needed for K_PLUGIN_FACTORY_WITH_JSON
#include <ktexteditorpreviewplugin.moc>
