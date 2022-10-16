#pragma once

#include <QVector>
#include <QJsonObject>
#include <functional>

namespace API::Payload {

enum AutoCheck {
        Empty = 0,
        NotEmpty,
        Null,
        NotNull,
};

enum Errno {
        Ok = 0,
                // No error
        DefaultValueSetted,
        CheckFailed,
        TypesDontMatch
                // Type passed to template and read QJsonValue type not matched
};

template<class T>
struct ExtractionRes {
        T value;
        Errno error;
};

template<typename>
struct DefaultValue;

template<>
struct DefaultValue<int> {
        static constexpr int value{0};
};

template<>
struct DefaultValue<double> {
        static constexpr double value{0.0};
};

template<>
struct DefaultValue<bool> {
        static constexpr bool value{false};
};

template<>
struct DefaultValue<qint64> {
        static constexpr qint64 value{0};
};

template<>
struct DefaultValue<QString> {
        static constexpr char * value{nullptr};
};

template<typename>
class Item;

template<class T>
bool cmpjvtype(QJsonValue::Type);

template<>
class Item<int> {
public:
        using Check_fn = std::function<bool(const QJsonValue&)>;

        Item(const QJsonValue&, int defaultValue = DefaultValue<int>::value);
        Item(const QJsonValue&, Check_fn, int defaultValue = DefaultValue<int>::value);
        Item(const QJsonValue&, Payload::AutoCheck, int defaultValue = DefaultValue<int>::value);
        ~Item();

        Payload::ExtractionRes<int> extract() const;

private:
        Check_fn _check;
        QJsonValue _value;
        int _defaultValue;
};

template<>
class Item<qint64> {
public:
        using Check_fn = std::function<bool(const QJsonValue&)>;

        Item(const QJsonValue&, qint64 defaultValue = DefaultValue<qint64>::value);
        Item(const QJsonValue&, Check_fn, qint64 defaultValue = DefaultValue<qint64>::value);
        Item(const QJsonValue&, Payload::AutoCheck, qint64 defaultValue = DefaultValue<qint64>::value);
        ~Item();

        Payload::ExtractionRes<qint64> extract() const;

private:
        Check_fn _check;
        QJsonValue _value;
        qint64 _defaultValue;
};

template<>
class Item<bool> {
public:
        using Check_fn = std::function<bool(const QJsonValue&)>;

        Item(const QJsonValue&, bool defaultValue = DefaultValue<bool>::value);
        Item(const QJsonValue&, Check_fn, bool defaultValue = DefaultValue<bool>::value);
        Item(const QJsonValue&, Payload::AutoCheck, bool defaultValue = DefaultValue<bool>::value);
        ~Item();

        Payload::ExtractionRes<bool> extract() const;

private:
        Check_fn _check;
        QJsonValue _value;
        bool _defaultValue;
};

template<>
class Item<double> {
public:
        using Check_fn = std::function<bool(const QJsonValue&)>;

        Item(const QJsonValue&, double defaultValue = DefaultValue<double>::value);
        Item(const QJsonValue&, Check_fn, double defaultValue = DefaultValue<double>::value);
        Item(const QJsonValue&, Payload::AutoCheck, double defaultValue = DefaultValue<double>::value);
        ~Item();

        Payload::ExtractionRes<double> extract() const;

private:
        Check_fn _check;
        QJsonValue _value;
        double _defaultValue;
};

template<>
class Item<QString> {
public:
        using Check_fn = std::function<bool(const QJsonValue&)>;

        Item(const QJsonValue&, QString defaultValue = DefaultValue<QString>::value);
        Item(const QJsonValue&, Check_fn, QString defaultValue = DefaultValue<QString>::value);
        Item(const QJsonValue&, Payload::AutoCheck, QString defaultValue = DefaultValue<QString>::value);
        ~Item();

        Payload::ExtractionRes<QString> extract() const;

private:
        Check_fn _check;
        QJsonValue _value;
        QString _defaultValue;
};

// template<class ArrayType>
// class Item<QVector<ArrayType>> {
// public:
//         using Check_fn = std::function<ArrayType(const QJsonValue&)>;
//
//         Item(const QJsonValue&, ArrayType defaultValue = DefaultValue<ArrayType>::value);
//         Item(const QJsonValue&, Check_fn, ArrayType defaultValue = DefaultValue<ArrayType>::value);
//         Item(const QJsonValue&, Payload::AutoCheck, ArrayType defaultValue = DefaultValue<ArrayType>::value);
//         ~Item();
//
//         Payload::ExtractionRes<ArrayType> extract() const;
//
// private:
//         Check_fn _check;
//         QJsonValue _value;
//         ArrayType _defaultValue;
// };

}
