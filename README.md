# KTextEditor Document Preview Plugin

## Introduction

This repository contains the KTextEditor Document Preview Plugin, a plugin for the editor [Kate](https://www.kate-editor.org/), the IDE [KDevelop](https://www.kdevelop.org/) or other software using the [KTextEditor](https://api.kde.org/frameworks/ktexteditor/html/index.html) framework.

The plugin enables a live preview of the currently edited text document in the final format. For the display it uses the KParts plugin which is currently selected as the preferred one for the MIME type of the document. If there is no matching KParts, no preview is possible.

### State: Early working prototype

This plugin is still work in progress and serving as a prototype. It has not yet seen a first official release (besides being accessable from public repos).

While it can be used already and should stay usable all the time, there is no support or guarantee on anything yet.

The features and UI are still up for discussion. What would *you* like to see?


## Installation instructions

Define some testing install dir:

    export MYKATEPLUGINPATH=$HOME/mykateplugins

Build and install:

    mkdir build
    cd build
    cmake .. -DKDE_INSTALL_PLUGINDIR=$MYKATEPLUGINPATH
    make
    make install

## Use the plugin

Start Kate or KDevelop on the commandline, with adapted QT_PLUGIN_PATH (make sure to not use a session open in another running app instance):

    export QT_PLUGIN_PATH=$MYKATEPLUGINPATH:$QT_PLUGIN_PATH
    kate --startanon
    # or
    kdevelop --ps

In Kate, go to `Settings / Configure Kate... / Application / Plugins` and enable the "Document Preview" plugin. A "Preview" tool view should appear on the right sidebar.

In KDevelop, go to `Settings / Configure KDevelop... / Plugins` and make sure the "Document Preview" plugin is enabled. Then go to `Window / Add Tool View...` and select the "Preview" tool view. It should appear on the right sidebar.


## Contact

Feedback welcome, email kossebau AT kde org or catch me on freenode irc in #kdevelop.

(Note: This file was happily edited with the support of this plugin. )
