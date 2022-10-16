#ifndef UTILITY_HPP_H9ZYO2LK
#define UTILITY_HPP_H9ZYO2LK

#include <array>
#include <QVector>
#include <QImage>
#include <QBuffer>
#include <QPixmap>
#include <QIcon>

#define Q(x) #x
#define QUOTE(x) Q(x)

QPixmap QPixmapFromQString(const QString& str);
QIcon QIconFromQString(const QString& str);
QImage QImageFromQString(const QString& str);
QString QStringFromQImage(const QImage& img);

namespace API {

        using money_t = float;

        const auto TABLE_ARRAY_SEPARATOR = R"(:)";

        bool conntainJoinRequest(const QString& whereReq);

        QString extractImg(QVariant const& v);

} /* API  */ 

#endif /* end of include guard: UTILITY_HPP_H9ZYO2LK */
