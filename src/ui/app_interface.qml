import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  id: window
  width: 900
  height: 700
  visible: true
  color: "#121212" // Deep dark background

  // Root Layout: Vertical stack
  ColumnLayout {
    anchors.fill: parent
    spacing: 0

    // --- SECTION 1: TOP BAR (Navigation & Search) ---
    Rectangle {
      Layout.fillWidth: true
      Layout.preferredHeight: 60
      color: "#1e1e1e"

      RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 15; anchors.rightMargin: 15
        spacing: 15

        // Nav Buttons
        Row {
          spacing: 10
          Button { text: "←"; flat: true; palette.buttonText: "white" }
          Button { text: "↑"; flat: true; palette.buttonText: "white" }
        }

        // Search Bar
        TextField {
          placeholderText: "Search Files..."
          Layout.fillWidth: true
          color: "white"
          background: Rectangle {
            color: "#2a2a2a"
            radius: 5
            border.color: parent.activeFocus ? "#3daee9" : "transparent"
          }
        }
      }
    }

    // --- SECTION 2: FILE GRID (The custom "Explorer" replacement) ---
    // This is where your dynamically added "Dir" and "File" squares live
    GridView {
      id: fileGrid
      Layout.fillWidth: true
      Layout.fillHeight: true
      cellWidth: 140
      cellHeight: 140
      clip: true
      
      // The Data (This can be modified dynamically)
      model: ListModel { id: fileModel }

      // The Visual "Look" of each item
      delegate: Item {
        width: 140; height: 140
        
        Rectangle {
          anchors.centerIn: parent
          width: 110; height: 110
          color: "#252525"
          border.color: "#333"
          radius: 4

          Column {
            anchors.centerIn: parent
            spacing: 8
            // Icon Placeholder
            Rectangle { 
              width: 40; height: 40; color: type === "dir" ? "#f39c12" : "#3498db"
              anchors.horizontalCenter: parent.horizontalCenter
            }
            Text { text: name; color: "white"; font.pixelSize: 12 }
          }

          // Index badge (top right as in your sketch)
          Rectangle {
            width: 25; height: 25; color: "#333"
            anchors.right: parent.right; anchors.top: parent.top
            Text { text: index_num; color: "white"; anchors.centerIn: parent }
          }

          MouseArea {
            anchors.fill: parent
            onClicked: console.log("Selected: " + name)
          }
        }
      }
    }

    // --- SECTION 3: CONTROL PANEL (The Bottom Area) ---
    Rectangle {
      Layout.fillWidth: true
      Layout.preferredHeight: 180
      color: "#181818"
      border.color: "#222"

      ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        // Path Display
        Text {
          text: "C:/Users/Documents/Target_Folder"
          color: "#666"
          font.family: "Monospace"
        }

        // Naming Scheme Dropdown
        ComboBox {
          Layout.fillWidth: true
          model: ["Sequential (01, 02...)", "Date-Based", "Original_Suffix"]
        }

        // Final Actions
        RowLayout {
            CheckBox {
              id: deleteCheck
              text: "Delete Original Files"
              contentItem: Text { text: parent.text; color: "white"; leftPadding: 30 }
            }
            Item { Layout.fillWidth: true } // Spacer
            Button {
              text: "MERGE"
              palette.button: "#27ae60"
              onClicked: {
                // Link to your C++ logic here
              }
          }
        }
      }
    }
  }

  // Component.onCompleted is a great place to test dynamic adding
  Component.onCompleted: {
    fileModel.append({"name": "Source_A", "type": "dir", "index_num": "1"})
    fileModel.append({"name": "Project_v1", "type": "file", "index_num": "3"})
    fileModel.append({"name": "Assets", "type": "dir", "index_num": "4"})
  }
}