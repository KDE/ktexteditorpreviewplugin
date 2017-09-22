# KTextEditor Document Preview Plugin

## Introduction

This repository contains the KTextEditor Document Preview Plugin, a plugin for the editor [Kate](https://www.kate-editor.org/), the IDE [KDevelop](https://www.kdevelop.org/), or other software using the [KTextEditor](https://api.kde.org/frameworks/ktexteditor/html/index.html) framework.

The plugin enables a live preview of the currently edited text document in the final format. For the display it uses the [KParts](https://api.kde.org/frameworks/kparts/html/index.html) plugin which is currently selected as the preferred one for the MIME type of the document. If there is no matching KParts plugin, no preview is possible.


## Build and install the plugin

Define some testing install dir (example: `$HOME/mykteplugins`):

    export MYKTEPLUGINSPATH=$HOME/mykteplugins

Build and install:

    # assuming current dir being the top-level source folder
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=$MYKTEPLUGINSPATH
    make
    make install

## Use the plugin

Start Kate or KDevelop on the commandline, with adapted QT_PLUGIN_PATH (make sure to not use a session open in another running app instance):

    # check the used plugin installation path as it depends on the system
    # and replace "_REPLACE_LIB_" with e.g. "lib", "lib64" or "lib/x86_64-linux-gnu"
    export QT_PLUGIN_PATH=$MYKTEPLUGINSPATH/_REPLACE_LIB_/plugins:$QT_PLUGIN_PATH
    kate --startanon
    # or
    kdevelop --ps

In Kate, go to `Settings / Configure Kate... / Application / Plugins` and enable the "Document Preview" plugin. A "Preview" tool view should appear on the right sidebar.

In KDevelop, go to `Settings / Configure KDevelop... / Plugins` and make sure the "Document Preview" plugin is enabled. Then go to `Window / Add Tool View...` and select the "Preview" tool view. It should appear on the right sidebar.


## Issues

Please report bugs and feature requests in the [KDE issue tracker](https://bugs.kde.org/enter_bug.cgi?format=guided&amp;product=kate&amp;component=plugin-preview).


## Contact

Feedback welcome, email kossebau AT kde org or catch me on freenode irc in #kdevelop.

(*Note*: This file was happily edited with the support of this plugin.)
