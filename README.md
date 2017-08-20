# KTextEditor Document Preview Plugin

## Introduction

This repository contains the KTextEditor Document Preview Plugin, a plugin
for [Kate](https://www.kate-editor.org/) & [KDevelop](https://www.kdevelop.org/) or other 
KTextEditor using programs.

The plugin enables a live preview of the currently edited text document in the
final format. For the display it uses the KParts plugin for the given mimetype
which is currently selected as the preferred one. If there is no matching KParts,
no preview is possible.

### State: Early working prototype

This plugin is still work in progress and serving as a prototype. It has not yet seen a first official
release (besides being accessable from public repos).

While it can be used already and should stay usable all the time,
there is no support or guarantee on anything yet.

The features and UI are still up for discussion.
What would *you* like to see?


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

Start Kate on the commandline, with adapted QT_PLUGIN_PATH:

    export QT_PLUGIN_PATH=$MYKATEPLUGINPATH:$QT_PLUGIN_PATH
    kate --startanon

Go to `Settings / Configurate Kate / Application / Plugins` and enable
the "Document Preview" plugin. A "Preview" toolview should appear on the right sidebar.

## Contact

Feedback welcome, email kossebau AT kde org or catch me on freenode irc in #kdevelop.

(Note: This file was happily edited with the support of this plugin. )
