#include "API/Utility.hpp"
#include <QVariant>

QPixmap QPixmapFromQString(const QString& str)
{
        return QPixmap::fromImage(QImageFromQString(str));
}

QIcon QIconFromQString(const QString& str)
{
        return QIcon(QPixmapFromQString(str));
}

QImage QImageFromQString(const QString& str)
{
        QByteArray by = QByteArray::fromBase64(str.toLatin1());
        QImage img = QImage::fromData(by, "JPEG");
        return img;
}

QString QStringFromQImage(const QImage& img)
{
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        img.save(&buffer, "JPEG");
        QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
        return iconBase64;
}

namespace API {

// move to another pack
QString extractImg(QVariant const& v) {
        return QString::fromLatin1(v.toByteArray().toBase64());
}

bool conntainJoinRequest(const QString& whereReq) {
        if (whereReq.contains(" JOIN ", Qt::CaseSensitivity::CaseInsensitive)) {
                return true;
        }
        return false;
}

} /* API */
