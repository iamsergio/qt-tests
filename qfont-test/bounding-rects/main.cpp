#include <QtWidgets>
#include <QImage>

static QString s_text = "0123456789";
static QString s_fontName = "Arial";

static int s_initialPoints = 11;

inline int logicalDpi()
{
    QScreen *screen = qApp->screens().constFirst();
    return screen->logicalDotsPerInch();
}

inline int devicePixelRatio()
{
    QScreen *screen = qApp->screens().constFirst();
    return screen->devicePixelRatio();
}

qreal pointsToPixels(qreal points, int dpi, bool round = true)
{
    const qreal pixels = std::floor(((points * dpi) / 72) * 100 + 0.5) / 100;
    return round ? qRound(pixels) : pixels;
}


void setPixelSize(QFont &f, qreal size)
{
    f.setPixelSize(size);
}

void setPointsSize(QFont &f, qreal size)
{
    f.setPointSizeF(size);
}


QSizeF boundingSize(qreal size, bool points)
{
    QFont font(s_fontName);
    auto setSize = points ? setPointsSize
                          : setPixelSize;
    font.setHintingPreference(QFont::PreferFullHinting);

    setSize(font, size);

    QFontMetricsF fm(font);
    return fm.boundingRect(s_text).size();
}

qreal round2Places(qreal v)
{
    return std::round(v * 100) / 100;
}

void dump_boundingRects(int initialSize, bool points)
{
    for (int size = initialSize; size <= initialSize + 30; ++size) {
        const QVector<qreal> scales = { 1.0, 1.25, 1.50, 1.75, 2.00 };

        // Get the bounding the scaled bounding sizes
        QVector<QSizeF> boundingSizes;
        for (qreal scale : scales) {
            boundingSizes << boundingSize(scale * size, points);
        }

        // Calculate how much we're off
        QVector<qreal> widthPercentages;
        QVector<qreal> heightPercentages;
        for (int i = 0; i < scales.size(); ++i) {
            widthPercentages << round2Places((boundingSizes[i].width()* 1.0 / boundingSizes[0].width()));
            heightPercentages << round2Places((boundingSizes[i].height()* 1.0 / boundingSizes[0].height()));
        }

        if (points)
            qDebug() << "\npointSize:" << size;
        else
            qDebug() << "\npixelSize:" << size;

        const bool is100 = logicalDpi() == 96 && devicePixelRatio() == 1;

        for (int i = 0; i < scales.size(); ++i) {

            if (!is100 && i > 0)
                continue;

            const qreal scale = scales[i];
            const QString percentagesStr = QString("w=%1%; h=%2%").arg(widthPercentages[i]).arg(heightPercentages[i]);
            qDebug() << "scale=" << scale << "; sz=" << boundingSizes[i] << (i == 0 ? QString() : percentagesStr);
            if (points) {
                qreal pixels = pointsToPixels(scale * size, logicalDpi(), false);
                qreal pixelsRounded = pointsToPixels(scale * size, logicalDpi(), true);
                qDebug() << "In pixels=" << pixels << "; rounded=" << pixelsRounded << "\n";
            }
        }
        qDebug() << "\n";
    }
}


void dump_decimalPoints()
{
    // Tests floating point QFont::setPointSizeF();
    // It has no influence whatsoever
    qDebug() << "\n";
    qDebug() << "Dumping decimal points:";
    for (qreal pointsSize = 14; pointsSize < 19; pointsSize += 0.10) {
        qDebug() << pointsSize << boundingSize(pointsSize, true);
    }
}

void test_qimage()
{
#ifdef Q_OS_WIN
    const bool is100 = logicalDpi() == 96 && devicePixelRatio() == 1;
#else
    const bool is100 = devicePixelRatio() == 1;
#endif
    auto scales = is100 ? QVector<qreal>{ 1.0, 1.25, 1.50, 1.75, 2.00 }
                        : QVector<qreal>{ 1.0 };


    QPixmap pix(500, 500);
    QPaintDevice *pd = &pix;
    QFont font(QFont("Arial"), pd);
    const int initialPixelSize = 20;

    // Dumps creates an image on disk, and prints the bounding-rect.
    // Re-run with 100% and with 150% etc, results should be the same
    for (qreal scale : scales ) {
        const QString filename = QString("%1%2.png").arg(int(scale*100)).arg(is100 ? QString()
                                                                                   : QString("_os_scaled"));

        font.setPixelSize(initialPixelSize * scale);
        font.setHintingPreference(QFont::PreferFullHinting);

        QPainter painter(&pix);
        painter.fillRect(pix.rect(), Qt::white);
        painter.setFont(font);
        painter.drawText(0, 50, s_text);
        for (int x = 0; x < pix.width(); x += 50)
            painter.drawLine(x, 0, x, pix.height());


        QFontMetricsF fm(font);
        qDebug() << "bounding size for scale" << scale << fm.boundingRect(s_text).size()
                 << "; pix.dpr=" << pix.devicePixelRatioF();
        pix.save(filename);
    }

    font.setPixelSize(initialPixelSize);

    QRawFont raw = QRawFont::fromFont(font);
    qDebug() << "Used font hinting=" << raw.hintingPreference()
             << "; pixelSize=" << raw.pixelSize()
             << "; ascent=" << raw.ascent()
             << "; descent=" <<raw.descent() << raw.leading()
             << raw.maxCharWidth() << raw.averageCharWidth() << raw.lineThickness()
             << raw.weight() << raw.xHeight() << raw.unitsPerEm();

}


int main(int a, char **b)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(a, b);

    QScreen *screen = app.screens().constFirst();
    qDebug() << "Device Pixel Ratio:" << screen->devicePixelRatio();
    qDebug() << "Logical DPI:" << screen->logicalDotsPerInch();
    qDebug() << "Phys DPI:" << screen->physicalDotsPerInch();
    qDebug();


    dump_boundingRects(s_initialPoints, true);

    qDebug() << "=========================================================================================";

    dump_boundingRects(pointsToPixels(s_initialPoints, screen->logicalDotsPerInch()), false);

    //qDebug() << "=========================================================================================";
    //dump_decimalPoints();

    test_qimage();

    return 0;
}
