# Pix3lIconv - Implementation TODO List

## Overview
This document outlines the implementation steps to enhance Pix3lIconv with English translation, application icon, Help menu, File menu, and drag & drop functionality.

---

## 1. Translate GUI to English

### Files to modify:
- `mainwindow.cpp` (lines 25, 36, 49-52, 70-73, etc.)
- `main.cpp` (line 7)

### Implementation steps:
1. **Window title**
   - Change `setWindowTitle("Convertitore PNG/JPEG/SVG → ICO")` to `setWindowTitle("PNG/JPEG/SVG to ICO Converter")`
   - Update `main.cpp` line 7: `a.setApplicationName("PNG/JPEG/SVG to ICO Converter")`

2. **UI labels and buttons**
   - Line 36: Change `"File di input (PNG, JPEG o SVG):"` to `"Input File (PNG, JPEG or SVG):"`
   - Line 42: Change `"Sfoglia..."` to `"Browse..."`
   - Line 49: Change `"Anteprima:"` to `"Preview:"`
   - Line 58: Change `"Nessuna immagine caricata"` to `"No image loaded"`
   - Line 70: Change `"Converti in .ico"` to `"Convert to .ico"`

3. **File dialog and messages**
   - Line 85: Change filter to `"Images (*.png *.jpeg *.jpg *.svg *.svgz);;All files (*.*)"`
   - Line 87: Change title to `"Select Image"`
   - Line 105: Change text to `"No image loaded"`
   - Line 117: Change text to `"Unable to render SVG for preview"`
   - Line 142: Change text to `"Unable to load image"`
   - Line 249: Change error to `"Unable to load SVG file."`
   - Line 268: Change error to `"Unable to load image."`
   - Line 291: Change title to `"Save as ICO"`
   - Line 292: Change default name and filter to `"icon.ico"` and `"Windows Icons (*.ico)"`
   - Line 301-302: Change success message to `"ICO file created successfully!\n\n" + savePath`
   - Line 304-305: Change error to `"Unable to save ICO file.\nCheck write permissions."`

---

## 2. Create Application Icon

### Files to create/modify:
- Create `resources/icons/app_icon.ico` (Windows icon file)
- Create `resources/icons/app_icon.svg` or `.png` (source image)
- Create/modify `resources.qrc` (Qt resource file)
- Modify `pixeliconv.pro` to include resources

### Implementation steps:
1. **Design icon**
   - Create a simple icon representing image-to-icon conversion
   - Suggested design: An image/photo icon with an arrow pointing to a Windows icon
   - Create multi-resolution ICO file (16x16, 32x32, 48x48, 256x256)

2. **Add icon to resources**
   - Create `resources.qrc` file:
   ```xml
   <RCC>
       <qresource prefix="/icons">
           <file>resources/icons/app_icon.ico</file>
       </qresource>
   </RCC>
   ```

3. **Update project file**
   - Add to `pixeliconv.pro`:
   ```qmake
   RESOURCES += resources.qrc
   RC_ICONS = resources/icons/app_icon.ico  # For Windows executable
   ```

4. **Set window icon in code**
   - Add to `MainWindow` constructor (after `setupUI()`):
   ```cpp
   setWindowIcon(QIcon(":/icons/resources/icons/app_icon.ico"));
   ```
   - Modify `main.cpp` (after line 11):
   ```cpp
   a.setWindowIcon(QIcon(":/icons/resources/icons/app_icon.ico"));
   ```

---

## 3. Add Help Menu

### Reference:
- See `../FrameExtractor/mainwindow.cpp` lines 759-774

### Files to modify:
- `mainwindow.h` - Add method declarations
- `mainwindow.cpp` - Add Help menu implementation

### Implementation steps:

1. **Update mainwindow.h**
   - Add includes:
   ```cpp
   #include <QMenuBar>
   #include <QMenu>
   #include <QAction>
   ```

   - Add private methods in class declaration:
   ```cpp
   private slots:
       void showAboutDialog();
       void showAboutQt();

   private:
       void setupMenuBar();
   ```

2. **Create setupMenuBar() method in mainwindow.cpp**
   - Reference FrameExtractor lines 650-777
   - Add Help menu creation:
   ```cpp
   void MainWindow::setupMenuBar()
   {
       QMenuBar *menuBar = new QMenuBar(this);
       setMenuBar(menuBar);

       // Help Menu
       QMenu *helpMenu = menuBar->addMenu(tr("&Help"));

       QAction *aboutAction = new QAction(tr("&About Pix3lIconv"), this);
       connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
       helpMenu->addAction(aboutAction);

       QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
       connect(aboutQtAction, &QAction::triggered, this, &MainWindow::showAboutQt);
       helpMenu->addAction(aboutQtAction);
   }
   ```

3. **Implement slot methods**
   - Reference FrameExtractor lines 936-945
   ```cpp
   void MainWindow::showAboutDialog()
   {
       QMessageBox::about(this, tr("About Pix3lIconv"),
           tr("<h2>Pix3lIconv</h2>"
              "<p>Version 1.0</p>"
              "<p>A simple converter for PNG, JPEG, and SVG images to ICO format.</p>"
              "<p>Supports multi-resolution ICO files (16x16, 32x32, 48x48, 256x256).</p>"
              "<p>Copyright © 2024</p>"));
   }

   void MainWindow::showAboutQt()
   {
       QMessageBox::aboutQt(this);
   }
   ```

4. **Call setupMenuBar() in constructor**
   - Add after `setupStatusBar()` or similar init code:
   ```cpp
   setupMenuBar();
   ```

---

## 4. Add File Menu

### Reference:
- See `../FrameExtractor/mainwindow.cpp` lines 656-705

### Files to modify:
- `mainwindow.h` - Add exit slot
- `mainwindow.cpp` - Extend setupMenuBar()

### Implementation steps:

1. **Update mainwindow.h**
   - Add to private slots:
   ```cpp
   void openInputFile();  // Rename from selectInputFile to be consistent
   void exitApplication();
   ```

2. **Extend setupMenuBar() method**
   - Add File menu BEFORE Help menu:
   ```cpp
   void MainWindow::setupMenuBar()
   {
       QMenuBar *menuBar = new QMenuBar(this);
       setMenuBar(menuBar);

       // File Menu
       QMenu *fileMenu = menuBar->addMenu(tr("&File"));

       QAction *openAction = new QAction(tr("&Open Image..."), this);
       openAction->setShortcut(QKeySequence::Open);  // Ctrl+O
       connect(openAction, &QAction::triggered, this, &MainWindow::selectInputFile);
       fileMenu->addAction(openAction);

       fileMenu->addSeparator();

       QAction *exitAction = new QAction(tr("E&xit"), this);
       exitAction->setShortcut(QKeySequence::Quit);  // Ctrl+Q
       connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
       fileMenu->addAction(exitAction);

       // Help Menu (as shown in step 3)
       // ...
   }
   ```

3. **Implement exitApplication() slot**
   ```cpp
   void MainWindow::exitApplication()
   {
       close();
   }
   ```

---

## 5. Add Drag & Drop Support

### Reference:
- See `../FrameExtractor/mainwindow.h` lines 14-18, 95-97
- See `../FrameExtractor/mainwindow.cpp` lines 447-501

### Files to modify:
- `mainwindow.h` - Add event handlers and includes
- `mainwindow.cpp` - Implement drag & drop logic

### Implementation steps:

1. **Update mainwindow.h**
   - Add includes at top:
   ```cpp
   #include <QDragEnterEvent>
   #include <QDragMoveEvent>
   #include <QDropEvent>
   #include <QMimeData>
   ```

   - Add protected methods:
   ```cpp
   protected:
       void dragEnterEvent(QDragEnterEvent *event) override;
       void dragMoveEvent(QDragMoveEvent *event) override;
       void dropEvent(QDropEvent *event) override;
   ```

2. **Enable drag & drop in constructor**
   - Add in MainWindow constructor:
   ```cpp
   setAcceptDrops(true);
   ```

3. **Implement dragEnterEvent()**
   - Reference FrameExtractor lines 447-467
   ```cpp
   void MainWindow::dragEnterEvent(QDragEnterEvent *event)
   {
       if (event->mimeData()->hasUrls()) {
           QList<QUrl> urls = event->mimeData()->urls();
           if (!urls.isEmpty()) {
               QString filePath = urls.first().toLocalFile();
               // Accept only image files
               if (filePath.endsWith(".png", Qt::CaseInsensitive) ||
                   filePath.endsWith(".jpg", Qt::CaseInsensitive) ||
                   filePath.endsWith(".jpeg", Qt::CaseInsensitive) ||
                   filePath.endsWith(".svg", Qt::CaseInsensitive) ||
                   filePath.endsWith(".svgz", Qt::CaseInsensitive)) {
                   event->acceptProposedAction();
               }
           }
       }
   }
   ```

4. **Implement dragMoveEvent()**
   - Reference FrameExtractor lines 469-487
   ```cpp
   void MainWindow::dragMoveEvent(QDragMoveEvent *event)
   {
       if (event->mimeData()->hasUrls()) {
           QList<QUrl> urls = event->mimeData()->urls();
           if (!urls.isEmpty()) {
               QString filePath = urls.first().toLocalFile();
               if (filePath.endsWith(".png", Qt::CaseInsensitive) ||
                   filePath.endsWith(".jpg", Qt::CaseInsensitive) ||
                   filePath.endsWith(".jpeg", Qt::CaseInsensitive) ||
                   filePath.endsWith(".svg", Qt::CaseInsensitive) ||
                   filePath.endsWith(".svgz", Qt::CaseInsensitive)) {
                   event->acceptProposedAction();
               }
           }
       }
   }
   ```

5. **Implement dropEvent()**
   - Reference FrameExtractor lines 489-501
   ```cpp
   void MainWindow::dropEvent(QDropEvent *event)
   {
       const QMimeData *mimeData = event->mimeData();

       if (mimeData->hasUrls()) {
           QList<QUrl> urls = mimeData->urls();
           if (!urls.isEmpty()) {
               QString filePath = urls.first().toLocalFile();

               // Set the file path and update UI
               inputFilePath = filePath;
               inputPathEdit->setText(filePath);
               convertButton->setEnabled(true);
               updatePreview();

               event->acceptProposedAction();
           }
       }
   }
   ```

---

## Implementation Order Recommendation

1. **Start with translations** (simplest, no architectural changes)
2. **Add File and Help menus** (establishes menu bar structure)
3. **Create application icon** (requires resource file creation)
4. **Add drag & drop** (final polish feature)

---

## Testing Checklist

- [ ] All UI text is in English
- [ ] Application icon appears in window title bar
- [ ] Application icon appears in Windows taskbar
- [ ] File → Open Image works and shows file dialog
- [ ] File → Exit closes the application
- [ ] Ctrl+O shortcut opens file dialog
- [ ] Ctrl+Q shortcut exits application
- [ ] Help → About Pix3lIconv shows about dialog
- [ ] Help → About Qt shows Qt about dialog
- [ ] Drag & drop PNG file loads preview
- [ ] Drag & drop JPEG file loads preview
- [ ] Drag & drop SVG file loads preview
- [ ] Drag & drop non-image file is rejected
- [ ] Convert button enables after file is loaded (via browse or drag & drop)
- [ ] Preview updates correctly for all file types
