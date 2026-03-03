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


  // [PLACE FUNCTIONS HERE]


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
        
        // NEW: App Icon
        Text {
          text: "📦"
          font.pixelSize: 20
          Layout.rightMargin: 4
        }

        Row {
          // Navigate Back Button
          spacing: 4
          Button {
            text: "←";
            flat: true;
            implicitWidth: 32
          }
          // Navigate Forward Button
          Button {
            text: "→";
            flat: true;
            implicitWidth: 32
          }
          // Go to Parent Directory Button
          Button {
            text: "↑";
            flat: true;
            implicitWidth: 32
            onClicked: AppBackend.goToParentDirectory()
          }
          // Refresh Button
          Button { 
            text: "↻"
            onClicked: AppBackend.refreshFileList()
          }
        }

        TextField {
          Layout.fillWidth: true
          placeholderText: "Search"
          color: "white"
          background: Rectangle {
            radius: 4
            color: "#2a2a2a"
          }
        }

        Row {
          spacing: 0 // Tighter spacing for window controls
          Button { 
            text: "—"; flat: true
            onClicked: window.showMinimized() 
          }
          // NEW: Maximize/Restore Button
          Button {
            text: window.visibility === Window.Maximized ? "❐" : "⬜"
            flat: true
            onClicked: {
              if (window.visibility === Window.Maximized) window.showNormal()
              else window.showMaximized()
            }
          }
          Button { 
            text: "✕"; flat: true
            onClicked: Qt.quit() 
          }
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

      // LEFT SIDEBAR
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
      }

      // [MAIN CONTENT]
      ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 0

        GridView {
          id: fileGrid
          Layout.fillWidth: true
          Layout.fillHeight: true
          cellWidth: 150
          cellHeight: 150
          clip: true
          model: AppBackend ? AppBackend.filesystem_model : null

          // [SCROLLBAR]
          ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true 
            contentItem: Rectangle {
              implicitWidth: 6
              radius: 3
              color: "#444"
            }
          }

          // [INTERACTIVE FILE BUTTONS HERE]
          Flow {
            anchors.fill: parent
            spacing: 10

            Repeater {
              model: AppBackend.filesystem_model // Matches your Q_PROPERTY name

              Button {
                width: 100
                height: 100
                
                // Customizing the button to show your icon and filename
                contentItem: Column {
                  spacing: 5
                  Text { 
                    text: model.icon // "📁" or "📄" from C++
                    font.pixelSize: 32
                    anchors.horizontalCenter: parent.horizontalCenter 
                  }
                  Text { 
                    text: model.filename 
                    color: "white"
                    elide: Text.ElideRight
                    width: 90
                    horizontalAlignment: Text.AlignHCenter
                  }
                }

                background: Rectangle {
                  color: parent.down ? "#222" : (parent.hovered ? "#333" : "#1a1a1a")
                  border.color: "#444"
                  radius: 4
                }

                onClicked: {
                  if (model.is_dir) {
                    AppBackend.enterDirectory(model.filename)
                  } else {
                    AppBackend.addFileToMerge(model.filename)
                  }
                }
              }
            }
          }
        }


        // [PATH BAR]
        Rectangle {
          Layout.fillWidth: true
          Layout.preferredHeight: 36
          color: "#181818"
          Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 12
            text: AppBackend ? AppBackend.current_filesystem_path : ""
            color: "#777"
            font.family: "Monospace"
          }
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