#ifndef HASH_HPP_5XQNUTL6
#define HASH_HPP_5XQNUTL6

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

enum charWeakness : quint8 {
    Alpha = 0x1,
    Digit = 0x2,
    Special = 0x4,
};

enum class PasswordQuality {
    STRONG,
    GOOD,
    WEAK,
    UNAVALIBLE,
};

const quint32 salt_length(32);
    // mean VERY BIG system max unique password
    // if /dev/urandom will not fail :)
    //
    // so big value choosed for handle auto gen salt entropy maximize

QByteArray saltGen(quint8 weakness_f);
quint8 passWeaknesses(const QByteArray& password);
PasswordQuality passwordQuality(const QByteArray& password);
QByteArray encryptPassword(const QByteArray& password, const QByteArray& salt,
    QCryptographicHash::Algorithm = QCryptographicHash::Algorithm::Sha512);

#endif /* end of include guard: HASH_HPP_5XQNUTL6 */
