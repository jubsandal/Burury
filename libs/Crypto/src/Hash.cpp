#include "Crypto/Hash.hpp"

#include <functional>

#include <QMap>
#include <QLatin1String>
#include <QCryptographicHash>
#include <QString>
#include <QVector>
#include <QByteArray>
#include <QRandomGenerator>

static void setBit(unsigned long& num, unsigned long bit) { num |= (1 << bit); }
static int getBit(unsigned long num, unsigned long bit) { return (num & ( 1 << bit  )) >> bit; }

static const QLatin1String specials(R"(!"”#$%&'()*+,-./:;?@[\]^_{| }~`)");
static const QLatin1String alphabet(R"(abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ)");
static const QLatin1String digits(R"(0123456789)");
    // for salt gen

static quint32 strongRand(quint32 min,
                          quint32 max = std::numeric_limits<quint32>::max())
{
    return QRandomGenerator::securelySeeded().generate() % (max+1 - min) + min;
}

static char randCharFrom(const QLatin1String& d, quint32 rand32) { return d[rand32 % d.size()].toLatin1(); }

static QMap<quint8, std::function<char(quint32)>> saltCharGen({
            { Alpha,           [](quint32 rand32) { return randCharFrom(alphabet, rand32);                                                      }},
            { Digit,           [](quint32 rand32) { return randCharFrom(digits, rand32);                                                        }},
            { Special,         [](quint32 rand32) { return randCharFrom(specials, rand32);                                                      }},
            { Alpha | Digit,   [](quint32 rand32) { return (strongRand(0, 1) ? randCharFrom(alphabet, rand32) : randCharFrom(digits, rand32));  }},
            { Alpha | Special, [](quint32 rand32) { return (strongRand(0, 1) ? randCharFrom(alphabet, rand32) : randCharFrom(specials, rand32));}},
            { Special | Digit, [](quint32 rand32) { return (strongRand(0, 1) ? randCharFrom(specials, rand32) : randCharFrom(digits, rand32));  }},
        });

QByteArray saltGen(quint8 w)
{
    QByteArray salt(salt_length, '\0');
    QVector<quint32> rand;
    rand.resize(salt_length);
    QRandomGenerator::securelySeeded().fillRange(rand.data(), rand.size());
    auto genf = saltCharGen[w];

    for (int i = 0; i < salt_length; i++) {
        salt[i] = genf(rand[i]);
    }

    return salt;
}

static void countChars(int& spec, int& dig, int& alph, const char * str) {
    spec = dig = alph = 0;
    for (int i = 0; i < strlen(str); i++) {
        char ch = str[i];
        if (isalpha(ch)) { alph++;
        } else if (isdigit(ch)) { dig++;
        } else if (isgraph(ch) || isspace(ch)) { spec++;
        }
    }
}

quint8 passWeaknesses(const QByteArray& data)
{
    int len = data.length();
    int spec, nums, alph;
    countChars(spec, nums, alph, data.data());
    double spec_c = static_cast<double>(spec)/len,
           nums_c = static_cast<double>(nums)/len,
           alph_c = static_cast<double>(alph)/len;

    // weakness determinission algo:
    //  1. find max coef char type - mostch
    //  2. if mostch count grater then 50% of password mean that weak in both other char types;
    //  3. if diff of other char types grater then 10% mean that weak only in one min char type;
    //  4. if diff less or eq to 10% - weak in both;
    auto determWeakness = [](double f, double s, double t, quint8 wf, quint8 ws, quint8 wt) -> quint8 {
        if (f > s && f > t) {
            if (f > 0.5) {
                return ws | wt;
            } else {
                if (std::abs(s - t) > 0.1) {
                    return (s > t ? wt : ws);
                } else {
                    return ws | wt;
                }
            }
        }

        return 0;
    };

    quint8 w1 = determWeakness(spec_c, nums_c, alph_c, Special, Digit, Alpha);
    quint8 w2 = determWeakness(nums_c, spec_c, alph_c, Digit, Special, Alpha);
    quint8 w3 = determWeakness(alph_c, spec_c, nums_c, Alpha, Special, Digit);

    return std::max(w1, std::max(w2, w3)); // only one gr then 0
}

PasswordQuality
passwordQuality(const QByteArray& password)
{
    PasswordQuality quality = PasswordQuality::WEAK;
    int spec, dig, alph;
    countChars(spec, dig, alph, password.data());

    // strong password standart:
    // return length > 7
    // specials > 2
    // letters > 0
    // digits > 0

    return quality;
}

// There is no check bit or little endiang, may currupt on db export to another machine :) im not care :)
// Implmented by Algorithm struct published in "Proposed Algorithm from IAENG International Journal of Computer Science, 43:1, IJCS_43_1_04"
QByteArray encryptPassword(const QByteArray& pass, const QByteArray& salt, QCryptographicHash::Algorithm hashAlgo)
{
    QByteArray toCrypt;
    QByteArray hash = QCryptographicHash::hash(pass, hashAlgo);

    int prev = -1;
    int cur;
    int inserted = 0;
    for (int i = 0; i < pass.length(); i++) {
        toCrypt.push_back(pass[i]);
            // push real password char

        // used Rule №2
        /* if (inserted < salt_length) { */
        if (inserted < salt.length()) {
            cur = getBit(pass[i], sizeof(pass[i])) ^
                  getBit(hash[i], sizeof(hash[i]));

            if (cur == 1) {
                toCrypt.push_back(salt[inserted++]);
            } else if (prev == 0) { // two consecutive zeros
                toCrypt.push_back(salt[inserted++]);
                toCrypt.push_back(salt[inserted++]);
                i++; //skip 2
            }
            prev = cur;
        }
    }

    // append remaining salt
    if (inserted < salt_length) {
        for (int i = inserted; i < salt_length; i++) {
            toCrypt.push_back(salt[i]);
        }
    }

    return QCryptographicHash::hash(toCrypt, hashAlgo);
}

