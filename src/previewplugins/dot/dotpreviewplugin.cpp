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

#include "dotpreviewplugin.h"

#include "dotpreviewwidget.h"

// KF
#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(DotPreviewPluginFactory, "dotpreviewplugin.json", registerPlugin<DotPreviewPlugin>();)


DotPreviewPlugin::DotPreviewPlugin(QObject* parent, const QVariantList& args)
    : DocumentPreviewPlugin(parent)
{
    Q_UNUSED(args);
}

DotPreviewPlugin::~DotPreviewPlugin() = default;

DocumentPreviewWidget* DotPreviewPlugin::createWidget() const
{
    return new DotPreviewWidget;
}

#include "dotpreviewplugin.moc"
