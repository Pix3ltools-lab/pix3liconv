#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QBuffer>
#include <QFile>
#include <QStandardPaths>
#include <QScrollArea>
#include <QResizeEvent>

#include <QSvgRenderer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PNG/JPEG/SVG to ICO Converter");
    setWindowIcon(QIcon(":/icons/resources/icons/app_icon.ico"));
    resize(700, 550);  // Dimensione iniziale più ampia per l'anteprima
    setAcceptDrops(true);

    setupMenuBar();

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // === Selezione file ===
    inputLabel = new QLabel("Input File (PNG, JPEG or SVG):");
    mainLayout->addWidget(inputLabel);

    QHBoxLayout *fileLayout = new QHBoxLayout();
    inputPathEdit = new QLineEdit();
    inputPathEdit->setReadOnly(true);
    browseButton = new QPushButton("Browse...");
    browseButton->setFixedWidth(100);
    fileLayout->addWidget(inputPathEdit);
    fileLayout->addWidget(browseButton);
    mainLayout->addLayout(fileLayout);

    // === Titolo anteprima ===
    QLabel *previewTitle = new QLabel("Preview:");
    previewTitle->setStyleSheet("font-weight: bold; margin-top: 10px;");
    mainLayout->addWidget(previewTitle);

    // === Area anteprima ===
    // Create a container widget for preview
    QWidget *previewContainer = new QWidget();
    previewContainer->setStyleSheet("background-color: #3a3a3a;");
    previewContainer->setMinimumSize(400, 300);

    QVBoxLayout *previewLayout = new QVBoxLayout(previewContainer);
    previewLayout->setContentsMargins(0, 0, 0, 0);

    // Create placeholder widget (shown when no image is loaded)
    placeholderWidget = new QWidget();
    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholderWidget);
    placeholderLayout->setAlignment(Qt::AlignCenter);
    placeholderLayout->setSpacing(20);

    placeholderIconLabel = new QLabel();
    placeholderIconLabel->setAlignment(Qt::AlignCenter);
    QPixmap placeholderIcon = createPlaceholderIcon(120);
    placeholderIconLabel->setPixmap(placeholderIcon);

    placeholderTextLabel = new QLabel("Drag & Drop Image Here\n\nor use File → Open Image (Ctrl+O)");
    placeholderTextLabel->setAlignment(Qt::AlignCenter);
    placeholderTextLabel->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: bold;");

    placeholderLayout->addWidget(placeholderIconLabel);
    placeholderLayout->addWidget(placeholderTextLabel);

    // Create actual preview label
    previewLabel = new QLabel();
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("background-color: transparent;");
    previewLabel->setScaledContents(false);
    previewLabel->hide();

    previewLayout->addWidget(placeholderWidget);
    previewLayout->addWidget(previewLabel);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #3a3a3a; }");
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(previewContainer);

    mainLayout->addWidget(scrollArea, 1);  // Espande per occupare spazio disponibile

    // === Pulsante conversione ===
    convertButton = new QPushButton("Convert to .ico");
    convertButton->setEnabled(false);
    convertButton->setFixedHeight(45);
    convertButton->setStyleSheet("QPushButton { font-size: 16px; font-weight: bold; }");
    mainLayout->addWidget(convertButton);

    // Connessioni segnali
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::selectInputFile);
    connect(convertButton, &QPushButton::clicked, this, &MainWindow::convertToIco);
}

MainWindow::~MainWindow() = default;

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

    // Help Menu
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));

    QAction *aboutAction = new QAction(tr("&About Pix3lIconv"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, &QAction::triggered, this, &MainWindow::showAboutQt);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::selectInputFile()
{
    QString filter = "Images (*.png *.jpeg *.jpg *.svg *.svgz);;All files (*.*)";
    inputFilePath = QFileDialog::getOpenFileName(this,
                                                 "Select Image",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                                                 filter);

    if (!inputFilePath.isEmpty()) {
        inputPathEdit->setText(inputFilePath);
        convertButton->setEnabled(true);
        updatePreview();
    } else {
        inputPathEdit->clear();
        convertButton->setEnabled(false);
        updatePreview();
    }
}

void MainWindow::updatePreview()
{
    if (inputFilePath.isEmpty()) {
        placeholderWidget->show();
        previewLabel->hide();
        previewLabel->setPixmap(QPixmap());
        currentPixmap = QPixmap();
        return;
    }

    placeholderWidget->hide();
    previewLabel->show();

    QString lowerPath = inputFilePath.toLower();
    bool isSvg = lowerPath.endsWith(".svg") || lowerPath.endsWith(".svgz");

    if (isSvg) {
        QSvgRenderer renderer(inputFilePath);
        if (!renderer.isValid()) {
            previewLabel->setText("Unable to render SVG for preview");
            previewLabel->setPixmap(QPixmap());
            currentPixmap = QPixmap();
            return;
        }

        // Usa la dimensione attuale del label (o un fallback)
        QSize targetSize = previewLabel->size();
        if (targetSize.width() < 100 || targetSize.height() < 100) {
            targetSize = QSize(400, 400);
        }

        QImage image(targetSize, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        renderer.render(&painter);
        painter.end();

        currentPixmap = QPixmap::fromImage(image);
    } else {
        currentPixmap = QPixmap(inputFilePath);
        if (currentPixmap.isNull()) {
            previewLabel->setText("Unable to load image");
            currentPixmap = QPixmap();
            return;
        }
    }

    // Ridimensiona mantenendo aspect ratio
    QPixmap scaledPixmap = currentPixmap.scaled(previewLabel->size(),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation);
    previewLabel->setPixmap(scaledPixmap);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (!currentPixmap.isNull()) {
        QPixmap scaledPixmap = currentPixmap.scaled(previewLabel->size(),
                                                    Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation);
        previewLabel->setPixmap(scaledPixmap);
    }
}

bool MainWindow::saveMultiSizeIco(const QList<QImage> &images, const QString &icoPath)
{
    QFile icoFile(icoPath);
    if (!icoFile.open(QIODevice::WriteOnly))
        return false;

    int count = images.size();
    if (count == 0) return false;

    // Header ICO (6 byte)
    icoFile.write("\0\0", 2);                 // Reserved
    icoFile.write("\1\0", 2);                 // Type 1 = ICO
    quint16 count16 = static_cast<quint16>(count);
    icoFile.write(reinterpret_cast<const char*>(&count16), 2);

    struct IcoDirEntry {
        quint8  width;          // 0 = 256
        quint8  height;         // 0 = 256
        quint8  colorCount;     // 0
        quint8  reserved;
        quint16 colorPlanes;    // 1
        quint16 bitsPerPixel;   // 32
        quint32 dataSize;
        quint32 dataOffset;
    };

    QVector<IcoDirEntry> entries(count);
    QVector<QByteArray> pngData(count);

    quint32 offset = 6 + count * 16;  // header + directory

    for (int i = 0; i < count; ++i) {
        const QImage &img = images[i];

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");
        pngData[i] = ba;

        IcoDirEntry &e = entries[i];
        int w = img.width();
        int h = img.height();
        e.width       = (w == 256 ? 0 : static_cast<quint8>(w));
        e.height      = (h == 256 ? 0 : static_cast<quint8>(h));
        e.colorCount  = 0;
        e.reserved    = 0;
        e.colorPlanes = 1;
        e.bitsPerPixel = 32;
        e.dataSize    = pngData[i].size();
        e.dataOffset  = offset;

        offset += e.dataSize;
    }

    // Scrivi directory
    for (const auto &e : entries) {
        icoFile.write(reinterpret_cast<const char*>(&e), 16);
    }

    // Scrivi dati PNG
    for (const auto &data : pngData) {
        icoFile.write(data);
    }

    icoFile.close();
    return true;
}

void MainWindow::convertToIco()
{
    if (inputFilePath.isEmpty()) return;

    QString lowerPath = inputFilePath.toLower();
    bool isSvg = lowerPath.endsWith(".svg") || lowerPath.endsWith(".svgz");

    const QList<int> sizes = {16, 32, 48, 256};
    QList<QImage> scaledImages;

    if (isSvg) {
        QSvgRenderer renderer(inputFilePath);
        if (!renderer.isValid()) {
            QMessageBox::critical(this, "Error", "Unable to load SVG file.");
            return;
        }

        for (int size : sizes) {
            QImage image(size, size, QImage::Format_ARGB32_Premultiplied);
            image.fill(Qt::transparent);

            QPainter painter(&image);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            renderer.render(&painter);
            painter.end();

            scaledImages.append(image);
        }
    } else {
        QImage original;
        if (!original.load(inputFilePath)) {
            QMessageBox::critical(this, "Error", "Unable to load image.");
            return;
        }

        if (original.hasAlphaChannel()) {
            original = original.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        }

        for (int size : sizes) {
            QImage scaled = original.scaled(size, size,
                                            Qt::KeepAspectRatioByExpanding,
                                            Qt::SmoothTransformation);

            int side = qMin(scaled.width(), scaled.height());
            scaled = scaled.copy((scaled.width() - side)/2,
                                 (scaled.height() - side)/2,
                                 side, side);

            scaledImages.append(scaled);
        }
    }

    QString savePath = QFileDialog::getSaveFileName(this,
                                                    "Save as ICO",
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/icon.ico",
                                                    "Windows Icons (*.ico)");

    if (savePath.isEmpty()) return;

    if (!savePath.endsWith(".ico", Qt::CaseInsensitive))
        savePath += ".ico";

    if (saveMultiSizeIco(scaledImages, savePath)) {
        QMessageBox::information(this, "Success",
                                 "ICO file created successfully!\n\n" + savePath);
    } else {
        QMessageBox::critical(this, "Error",
                              "Unable to save ICO file.\nCheck write permissions.");
    }
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, tr("About Pix3lIconv"),
        tr("<h2>Pix3lIconv</h2>"
           "<p>Version 1.0</p>"
           "<p>A simple converter for PNG, JPEG, and SVG images to ICO format.</p>"
           "<p>Supports multi-resolution ICO files (16x16, 32x32, 48x48, 256x256).</p>"
           "<p>Copyright © 2025</p>"));
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::exitApplication()
{
    close();
}

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

QPixmap MainWindow::createPlaceholderIcon(int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw rounded rectangle (border)
    QPen borderPen(QColor(150, 150, 150), 3);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    QRectF borderRect(5, 5, size - 10, size - 10);
    painter.drawRoundedRect(borderRect, 8, 8);

    // Draw mountain shape (simplified landscape icon)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(120, 120, 120));

    QPolygonF mountain1;
    mountain1 << QPointF(size * 0.25, size * 0.7)
              << QPointF(size * 0.45, size * 0.4)
              << QPointF(size * 0.65, size * 0.7);
    painter.drawPolygon(mountain1);

    QPolygonF mountain2;
    mountain2 << QPointF(size * 0.5, size * 0.7)
              << QPointF(size * 0.7, size * 0.5)
              << QPointF(size * 0.9, size * 0.7);
    painter.drawPolygon(mountain2);

    // Draw sun/moon circle
    painter.setBrush(QColor(140, 140, 140));
    painter.drawEllipse(QPointF(size * 0.75, size * 0.3), size * 0.08, size * 0.08);

    painter.end();
    return pixmap;
}
