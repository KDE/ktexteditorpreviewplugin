# KTextEditor Document Preview Plugin

## Introduction

This repository once contained the KTextEditor Document Preview Plugin, a plugin for the editor [Kate](https://www.kate-editor.org/), the IDE [KDevelop](https://www.kdevelop.org/), or other software using the [KTextEditor](https://api.kde.org/frameworks/ktexteditor/html/index.html) framework.

The plugin enables a live preview of the currently edited text document in the final format, in the sidebar (Kate) or as tool view (KDevelop). So when editing e.g. a Markdown text or an SVG image, the result is instantly visible next to the source text. For the display the plugin uses that [KParts](https://api.kde.org/frameworks/kparts/html/index.html) plugin which is currently selected as the preferred one for the MIME type of the document. If there is no KParts plugin for that type, no preview is possible.

## New Location of Code

After the 0.2.1 release the code of the plugin has been merged into the repository of Kate, into the subdir [addons/preview](https://commits.kde.org/kate?path=addons/preview).

Starting with KDE Applications 17.12, the KTextEditor Document Preview Plugin is thus released as part of Kate.

## Issues

Please report bugs and feature requests as before in the [KDE issue tracker](https://bugs.kde.org/enter_bug.cgi?format=guided&product=kate&component=plugin-preview&version=Git).


## Contact

Feedback welcome, email kossebau AT kde org or catch me on freenode irc in #kdevelop.

(*Note*: This file was happily edited with the support of this plugin.)
