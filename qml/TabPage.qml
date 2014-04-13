/*
  Copyright (C) 2014 Michal Kosciesza <michal@mkiol.net>

  This file is part of Kaktus.

  Kaktus is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kaktus is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kaktus.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: root

    SilicaListView {
        id: listView
        model: tabModel

        anchors { top: parent.top; left: parent.left; right: parent.right }
        height: app.height - (dm.busy||fetcher.busy ? Theme.itemSizeMedium : 0);
        clip:true

        MainMenu{}

        header: PageHeader {
            title: qsTr("Tabs")
        }

        delegate: ListItem {
            id: listItem
            contentHeight: {
                if (visible) {
                    if (model.uid==="readlater")
                        return itemReadlater.height + 3 * Theme.paddingMedium;
                    else
                        return item.height + 2 * Theme.paddingMedium;
                } else {
                    return 0;
                }
            }

            visible: {
                if (model.uid==="readlater") {
                    if (listView.count==1)
                        return false;
                    if (settings.showStarredTab)
                        return true
                    return false;
                }
                return true;
            }

            /*Rectangle {
                id: background
                anchors.fill: parent
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                visible: model.uid==="readlater"
            }

            OpacityRampEffect {
                id: effect
                slope: 1
                offset: 0.1
                direction: OpacityRamp.BottomToTop
                sourceItem: background
                enabled: background.visible
            }*/

            Column {
                id: item

                visible: model.uid!=="readlater"
                spacing: 0.5*Theme.paddingSmall
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: image.visible ? image.right : parent.left
                anchors.right: unreadbox.visible ? unreadbox.left : parent.right

                Label {
                    wrapMode: Text.AlignLeft
                    anchors.left: parent.left; anchors.right: parent.right;
                    anchors.leftMargin: Theme.paddingLarge; anchors.rightMargin: Theme.paddingLarge
                    font.pixelSize: Theme.fontSizeMedium
                    color: listItem.down ? Theme.highlightColor : Theme.primaryColor
                    text: title
                }
            }

            Rectangle {
                id: unreadbox
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                width: unreadlabel.width + 3 * Theme.paddingSmall
                height: unreadlabel.height + 2 * Theme.paddingSmall
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                radius: 5
                visible: model.unread!=0

                Label {
                    id: unreadlabel
                    anchors.centerIn: parent
                    text: model.unread
                    //color: listItem.down ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    color: Theme.highlightColor
                }

            }

            Image {
                id: image
                width: Theme.iconSizeSmall
                height: Theme.iconSizeSmall
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                visible: settings.showTabIcons && model.uid!=="readlater"
            }

            // readlater

            Column {
                id: itemReadlater
                spacing: 1.0 * Theme.paddingSmall
                anchors.left: star.right; anchors.right: parent.right
                anchors.top: parent.top
                visible: model.uid==="readlater"

                Label {
                    wrapMode: Text.AlignLeft
                    anchors.left: parent.left; anchors.right: parent.right;
                    anchors.leftMargin: Theme.paddingLarge; anchors.rightMargin: Theme.paddingLarge
                    font.pixelSize: Theme.fontSizeMedium
                    color: listItem.down ? Theme.highlightColor : Theme.primaryColor
                    text: title
                }
            }

            Image {
                id: star
                width: Theme.iconSizeSmall
                height: Theme.iconSizeSmall
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingLarge
                anchors.verticalCenter: itemReadlater.verticalCenter
                //anchors.verticalCenter: item.verticalCenter
                visible: model.uid==="readlater"
                source: listItem.down ? "image://theme/icon-m-favorite-selected?"+Theme.highlightColor :
                                        "image://theme/icon-m-favorite-selected"
            }

            // --

            Connections {
                target: settings
                onShowTabIconsChanged: {
                    if (settings.showTabIcons && model.uid!=="readlater")
                        image.source = cache.getUrlbyUrl(iconUrl);
                    else
                        image.source = "";
                }
            }

            Component.onCompleted: {
                if (settings.showTabIcons && model.uid!=="readlater") {
                    image.source = cache.getUrlbyUrl(iconUrl);
                } else {
                    image.source = "";
                }
            }

            onClicked: {
                if (model.uid==="readlater") {
                    utils.setEntryModel(uid);
                    pageStack.push(Qt.resolvedUrl("EntryPage.qml"),{"title": title, "index": model.index});
                } else {
                    utils.setFeedModel(uid);
                    pageStack.push(Qt.resolvedUrl("FeedPage.qml"),{"title": title});
                }
            }
        }


        ViewPlaceholder {
            enabled: listView.count == 1
            text: qsTr("No tabs")

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryHighlightColor
                text: fetcher.busy ? qsTr("Wait until Sync finish") : qsTr("Pull down to do first Sync")
            }
        }

        VerticalScrollDecorator {
            flickable: listView
        }

    }
}
