# JSON Editor
A desktop application built using Qt that allows users to open, edit, and save JSON files in a convenient tree view format—similar to a property editor. This project was developed to explore and understand Qt's Abstract Item Model and the Model-View-Delegate design pattern.

## ✨ Features
- 📂 Open JSON files from disk

- 🛠️ Edit JSON values in a structured tree view

- 💾 Save / Save As functionality for editing and exporting files

- 🧠 Built using Qt's Model-View-Delegate pattern for flexible UI logic

## 🔧 Dependencies
- Qt `v6.8.x`
- jsoncons `v0.174.0`
- CMake
- Make
- GCC or mingw64

## 🛠️ Build Instructions

```bash
# Clone the repo
git clone https://github.com/anirudhasj441/qt_tree_model_practice

cd qt_tree_model_practice

# Create a build directory
mkdir build && cd build

# Configure the project with CMake
cmake ../ -DCMAKE_BUILD_TYPE=Releas

# Or for windows with msys2
cmake ../ -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Releas 
```

## 🚀 Run

After building the project, run the executable inside the `run/bin` folder:
```bash
# Navigate to run/bin folder.
cd run/bin/

# Run the executable file.
./app
```

## 📁 File Structure

```bash
qt_tree_model_practice/
├── src
│   ├── gui         # Source code for gui classes 
│   ├── models      # Source code for models
│   └── res         # Ui assets files (Icons, images, etc)     
└── temp.json       # Sample Json files to test
```

## 📚 Learning Goals
- This project was created primarily to:

- Practice working with Qt’s Abstract Item Model

- Understand and implement the Model-View-Delegate architecture

- Build a practical tool for editing structured data

## 📜 License
This project is open-source and available under the [MIT License.](https://opensource.org/license/mit)