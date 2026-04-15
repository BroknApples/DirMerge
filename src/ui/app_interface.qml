import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ApplicationWindow {
  id: window
  width: 1000
  height: 700
  visible: true
  color: "#121212"
  flags: Qt.FramelessWindowHint | Qt.Window

  // --- State & Data ---
  property string viewMode: "grid"
  property int iconSize: 64


  // --- Components ---

  // [GRID VIEW COMPONENT]
  Component {
    id: gridViewComponent
    GridView {
      id: fileGrid
      anchors.fill: parent
      model: AppBackend ? AppBackend.filesystem_model : null
      cellWidth: iconSize + 40
      cellHeight: iconSize + 60
      clip: true
      delegate: fileDelegateGrid
    }
  } // [END GRID VIEW COMPONENT]

  // [LIST VIEW COMPONENT]
  Component {
    id: listViewComponent
    ListView {
      id: fileList
      anchors.fill: parent
      model: AppBackend ? AppBackend.filesystem_model : null
      clip: true
      delegate: fileDelegateList
    }
  } // [END LIST VIEW COMPONENT]

  // [GRID DELEGATE COMPONENT]
  Component {
    id: fileDelegateGrid
    Item {
      width: GridView.view.cellWidth
      height: GridView.view.cellHeight

      Button {
        anchors.centerIn: parent
        width: parent.width - 10
        height: parent.height - 10
        flat: true

        contentItem: ColumnLayout {
          spacing: 5
          Text {
            text: icon
            font.pixelSize: iconSize * 0.8
            Layout.alignment: Qt.AlignHCenter
          }
          Text {
            text: filename
            color: "white"
            elide: Text.ElideRight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
          }
        }

        background: Rectangle {
          color: parent.hovered ? "#333" : "transparent"
          radius: 6
        }

        onClicked: {
          if (is_dir) AppBackend.enterSubdirectory(full_path)
        }
      }
    }
  } // [END GRID DELEGATE COMPONENT]

  // [LIST DELEGATE COMPONENT]
  Component {
    id: fileDelegateList
    Rectangle {
      width: ListView.view.width
      height: 40
      color: mouseAreaList.containsMouse ? "#333" : "transparent"

      RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        spacing: 15

        Text { text: icon; font.pixelSize: 20 }
        Text {
          text: filename
          color: "white"
          Layout.preferredWidth: 200
          elide: Text.ElideRight
        }
        Text {
          text: full_path
          color: "#777"
          Layout.fillWidth: true
          elide: Text.ElideLeft
        }
      }

      MouseArea {
        id: mouseAreaList
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
          if (is_dir) AppBackend.enterSubdirectory(full_path)
        }
      }
    }
  } // [END LIST DELEGATE COMPONENT]


  // --- Actual UI Content ---
  ColumnLayout {
    anchors.fill: parent
    spacing: 0

    // ======================
    // TOP BAR
    // ======================
    Rectangle {
      id: delegateRoot
      Layout.fillWidth: true
      Layout.preferredHeight: 56
      color: "#1e1e1e"

      MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: window.startSystemMove()
      }

      RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 10
        
        // [App Icon]
        Text {
          text: "📦"
          font.pixelSize: 20
          Layout.rightMargin: 4
        } // [END App Icon]

        Row {
          // [Navigate Back Button]
          spacing: 4
          Button {
            text: "←";
            flat: true;
            implicitWidth: 32
            onClicked: AppBackend.goBack()
          } // [END Navigate Back Button]

          // Navigate Forward Button
          Button {
            text: "→";
            flat: true;
            implicitWidth: 32
            onClicked: AppBackend.goForward()
          } // [END Navigate Forward Button]

          // [Navigate Up Button]
          Button {
            text: "↑";
            flat: true;
            implicitWidth: 32
            onClicked: AppBackend.goToParentDirectory()
          } // [END Navigate Up Button]

          // [Refresh Button]
          Button { 
            text: "↻"
            onClicked: AppBackend.refreshFileList()
          } // [END Refresh Button]
        }

        // [Search Bar]
        TextField { 
          Layout.fillWidth: true
          placeholderText: "Search"
          color: "white"
          background: Rectangle {
            radius: 4
            color: "#2a2a2a"
          }
        } // [END Search Bar]

        Row {
          spacing: 0 // Tighter spacing for window controls

          // [Minimize Button]
          Button { 
            text: "—"; flat: true
            onClicked: window.showMinimized() 
          } // [END Minimize Button]

          // [Maximize/Restore Button]
          Button {
            text: window.visibility === Window.Maximized ? "❐" : "⬜"
            flat: true
            onClicked: {
              if (window.visibility === Window.Maximized) window.showNormal()
              else window.showMaximized()
            }
          } // [END Maximize/Restore Button]

          // [Close Button]
          Button { 
            text: "✕"; flat: true
            onClicked: Qt.quit() 
          } // [END Close Button]
        }
      }
    }

    // ======================
    // MIDDLE AREA
    // ======================
    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true
      spacing: 0

      // [LEFT SIDEBAR]
      Rectangle {
        Layout.preferredWidth: 56
        Layout.fillHeight: true
        color: "#171717"
        border.color: "#222"

        ColumnLayout {
          anchors.fill: parent
          anchors.topMargin: 12
          spacing: 12

          ToolButton {
            Layout.alignment: Qt.AlignHCenter
            text: viewMode === "list" ? "≡" : "▦"
            onClicked: viewMode = (viewMode === "grid" ? "list" : "grid")
          }

          ToolButton { Layout.alignment: Qt.AlignHCenter; text: "A↕" }
          
          Item { Layout.fillHeight: true }

          ToolButton { Layout.alignment: Qt.AlignHCenter; text: "⚙" }
        }
      } // [END LEFT SIDEBAR]


      // [MAIN CONTENT]
      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Loader {
          anchors.fill: parent
          sourceComponent: viewMode === "grid" ? gridViewComponent : listViewComponent
        }
      }
    }

    // ======================
    // BOTTOM CONTROLS & FOOTER
    // ======================
    ColumnLayout {
      Layout.fillWidth: true
      spacing: 0

      // The Merge Action Bar
      Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 120
        color: "#151515"
        border.color: "#222"

        RowLayout {
          anchors.fill: parent
          anchors.margins: 24
          spacing: 40

          ColumnLayout {
            ComboBox {
              Layout.preferredWidth: 250
              model: ["Sequential (01, 02, 03)", "Date Based", "Original Suffix"]
            }
          }

          CheckBox {
            id: deleteOriginals
            text: "Delete Original Files"
            palette.windowText: "#999"
          }

          Item { Layout.fillWidth: true }

          Button {
            text: "MERGE FILES"
            font.bold: true
            
            onClicked: {
              // currentPath is defined at line 16
              AppBackend.merge()
            }
            
            background: Rectangle {
              color: parent.down ? "#1e8449" : "#27ae60"
              radius: 6
            }
          }
        }
      }

      // THE SYSTEM FOOTER BAR
      Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 28
        color: "#0f0f0f"

        RowLayout {
          anchors.fill: parent
          anchors.leftMargin: 8
          anchors.rightMargin: 12
          spacing: 12

          Row {
            spacing: 4
            ToolButton { 
              text: "⚙"; font.pixelSize: 12; implicitHeight: 24
              ToolTip.visible: hovered; ToolTip.text: "Settings"
            }
            ToolButton { 
              text: "ⓘ"; font.pixelSize: 12; implicitHeight: 24
              ToolTip.visible: hovered; ToolTip.text: "Documentation"
            }
            ToolButton { 
              text: "📋"; font.pixelSize: 12; implicitHeight: 24
              ToolTip.visible: hovered; ToolTip.text: "Open Logs"
            }
          }

          Item { Layout.fillWidth: true }

          Text {
            id: clockDisplay
            color: "#555"
            font.pixelSize: 11
            font.family: "Monospace"
            
            Timer {
              interval: 1000; running: true; repeat: true
              onTriggered: clockDisplay.text = Qt.formatTime(new Date(), "hh:mm:ss")
            }
            Component.onCompleted: text = Qt.formatTime(new Date(), "hh:mm:ss")
          }
        }
      }
    }
  }
}