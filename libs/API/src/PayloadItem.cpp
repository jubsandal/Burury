#include "API/PayloadItem.hpp"
#include <typeinfo>

namespace API::Payload {

template<class T>
bool cmpjvtype(QJsonValue::Type jvtype) {
        // cros-compiler capability
        const size_t int32_sign   = typeid(int).hash_code();
        const size_t double_sign   = typeid(double).hash_code();
        const size_t qint64_sign  = typeid(qint64).hash_code();
        const size_t bool_sign    = typeid(bool).hash_code();
        const size_t qstring_sign = typeid(QString).hash_code();

        const size_t type = typeid(T).hash_code();

        return (type == bool_sign && jvtype == QJsonValue::Bool) ||
                (type == qstring_sign && jvtype == QJsonValue::String) ||
                ((type == int32_sign || type == qint64_sign || type == double_sign) && jvtype == QJsonValue::Double);
}

Item<int>::Item(const QJsonValue& value, int defaultValue) :
        _value(value),
        _defaultValue(defaultValue),
        _check([](const QJsonValue& v){return true;})
{
}

Item<int>::Item(const QJsonValue& value, Item::Check_fn func, int defaultValue) :
        Item(value, defaultValue)
{
        _check = func;
}

Item<int>::Item(const QJsonValue& value, Payload::AutoCheck autoCheck, int defaultValue) :
        Item(value, defaultValue)
{
        // TODO create more intelegence solution with case of handlers
        switch (autoCheck) {
                case Payload::Empty:
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() == 0;
                                        }
                                        return true;
                                };
                                return v.isNull() && v.isUndefined() && checkString();
                        };
                        break;
                case Payload::NotEmpty:
                        // TODO call ^ this function
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() > 0;
                                        }
                                        return true;
                                };
                                return !v.isNull() && !v.isUndefined();
                        };
                        break;
                case Payload::Null:
                        _check = [](const QJsonValue& v) {
                                return v.isNull();
                        };
                        break;
                case Payload::NotNull:
                        _check = [](const QJsonValue& v) {
                                return !v.isNull();
                        };
                        break;
                default:
                        throw (QString("Item::") + QString(__func__) + QString(" Unknown AutoCheck value passed: ") + QString::number(autoCheck));
                        break;
        }
}

Item<int>::~Item()
{ }

Payload::ExtractionRes<int> Item<int>::extract() const {
        auto createErrorRes = [this](Payload::Errno e) {
                return Payload::ExtractionRes<int>{
                        .value = _defaultValue,
                        .error = e,
                };
        };

        if (!_check(_value)) {
                return createErrorRes(Payload::CheckFailed);
        }

        if (!_value.isNull() && !_value.isUndefined()) {
                if (!cmpjvtype<int>(_value.type())) {
                        return createErrorRes(Payload::TypesDontMatch);
                } else {
                        return Payload::ExtractionRes<int>{
                                .value = static_cast<int>(_value.toDouble()),
                                .error = Payload::Ok
                        };
                }
        }

        return createErrorRes(Payload::DefaultValueSetted);
}

// ------------------------------------------

Item<qint64>::Item(const QJsonValue& value, qint64 defaultValue) :
        _value(value),
        _defaultValue(defaultValue),
        _check([](const QJsonValue& v){return true;})
{ }

Item<qint64>::Item(const QJsonValue& value, Item::Check_fn func, qint64 defaultValue) :
        Item(value, defaultValue)
{
        _check = func;
}

Item<qint64>::Item(const QJsonValue& value, Payload::AutoCheck autoCheck, qint64 defaultValue) :
        Item(value, defaultValue)
{
        // TODO create more qint64elegence solution with case of handlers
        switch (autoCheck) {
                case Payload::Empty:
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() == 0;
                                        }
                                        return true;
                                };
                                return v.isNull() && v.isUndefined() && checkString();
                        };
                        break;
                case Payload::NotEmpty:
                        // TODO call ^ this function
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() > 0;
                                        }
                                        return true;
                                };
                                return !v.isNull() && !v.isUndefined();
                        };
                        break;
                case Payload::Null:
                        _check = [](const QJsonValue& v) {
                                return v.isNull();
                        };
                        break;
                case Payload::NotNull:
                        _check = [](const QJsonValue& v) {
                                return !v.isNull();
                        };
                        break;
                default:
                        throw (QString("Item::") + QString(__func__) + QString(" Unknown AutoCheck value passed: ") + QString::number(autoCheck));
                        break;
        }
}

Item<qint64>::~Item()
{ }

Payload::ExtractionRes<qint64> Item<qint64>::extract() const {
        auto createErrorRes = [this](Payload::Errno e) {
                return Payload::ExtractionRes<qint64>{
                        .value = _defaultValue,
                        .error = e,
                };
        };

        if (!_check(_value)) {
                return createErrorRes(Payload::CheckFailed);
        }

        if (!_value.isNull() && !_value.isUndefined()) {
                if (!cmpjvtype<qint64>(_value.type())) {
                        return createErrorRes(Payload::TypesDontMatch);
                } else {
                        return Payload::ExtractionRes<qint64>{
                                .value = static_cast<qint64>(_value.toDouble()),
                                .error = Payload::Ok
                        };
                }
        }

        return createErrorRes(Payload::DefaultValueSetted);
}

// ------------------------------------------

Item<double>::Item(const QJsonValue& value, double defaultValue) :
        _value(value),
        _defaultValue(defaultValue),
        _check([](const QJsonValue& v){return true;})
{
}

Item<double>::Item(const QJsonValue& value, Item::Check_fn func, double defaultValue) :
        Item(value, defaultValue)
{
        _check = func;
}

Item<double>::Item(const QJsonValue& value, Payload::AutoCheck autoCheck, double defaultValue) :
        Item(value, defaultValue)
{
        // TODO create more doubleelegence solution with case of handlers
        switch (autoCheck) {
                case Payload::Empty:
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() == 0;
                                        }
                                        return true;
                                };
                                return v.isNull() && v.isUndefined() && checkString();
                        };
                        break;
                case Payload::NotEmpty:
                        // TODO call ^ this function
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() > 0;
                                        }
                                        return true;
                                };
                                return !v.isNull() && !v.isUndefined();
                        };
                        break;
                case Payload::Null:
                        _check = [](const QJsonValue& v) {
                                return v.isNull();
                        };
                        break;
                case Payload::NotNull:
                        _check = [](const QJsonValue& v) {
                                return !v.isNull();
                        };
                        break;
                default:
                        throw (QString("Item::") + QString(__func__) + QString(" Unknown AutoCheck value passed: ") + QString::number(autoCheck));
                        break;
        }
}

Item<double>::~Item()
{ }

Payload::ExtractionRes<double> Item<double>::extract() const {
        auto createErrorRes = [this](Payload::Errno e) {
                return Payload::ExtractionRes<double>{
                        .value = _defaultValue,
                        .error = e,
                };
        };

        if (!_check(_value)) {
                return createErrorRes(Payload::CheckFailed);
        }

        if (!_value.isNull() && !_value.isUndefined()) {
                if (!cmpjvtype<double>(_value.type())) {
                        return createErrorRes(Payload::TypesDontMatch);
                } else {
                        return Payload::ExtractionRes<double>{
                                .value = _value.toDouble(),
                                .error = Payload::Ok
                        };
                }
        }

        return createErrorRes(Payload::DefaultValueSetted);
}

// ------------------------------------------

Item<bool>::Item(const QJsonValue& value, bool defaultValue) :
        _value(value),
        _defaultValue(defaultValue),
        _check([](const QJsonValue& v){return true;})
{
}

Item<bool>::Item(const QJsonValue& value, Item::Check_fn func, bool defaultValue) :
        Item(value, defaultValue)
{
        _check = func;
}

Item<bool>::Item(const QJsonValue& value, Payload::AutoCheck autoCheck, bool defaultValue) :
        Item(value, defaultValue)
{
        // TODO create more boolelegence solution with case of handlers
        switch (autoCheck) {
                case Payload::Empty:
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() == 0;
                                        }
                                        return true;
                                };
                                return v.isNull() && v.isUndefined() && checkString();
                        };
                        break;
                case Payload::NotEmpty:
                        // TODO call ^ this function
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() > 0;
                                        }
                                        return true;
                                };
                                return !v.isNull() && !v.isUndefined();
                        };
                        break;
                case Payload::Null:
                        _check = [](const QJsonValue& v) {
                                return v.isNull();
                        };
                        break;
                case Payload::NotNull:
                        _check = [](const QJsonValue& v) {
                                return !v.isNull();
                        };
                        break;
                default:
                        throw (QString("Item::") + QString(__func__) + QString(" Unknown AutoCheck value passed: ") + QString::number(autoCheck));
                        break;
        }
}

Item<bool>::~Item()
{ }

Payload::ExtractionRes<bool> Item<bool>::extract() const {
        auto createErrorRes = [this](Payload::Errno e) {
                return Payload::ExtractionRes<bool>{
                        .value = _defaultValue,
                        .error = e,
                };
        };

        if (!_check(_value)) {
                return createErrorRes(Payload::CheckFailed);
        }

        if (!_value.isNull() && !_value.isUndefined()) {
                if (!cmpjvtype<bool>(_value.type())) {
                        return createErrorRes(Payload::TypesDontMatch);
                } else {
                        return Payload::ExtractionRes<bool>{
                                .value = _value.toBool(),
                                .error = Payload::Ok
                        };
                }
        }

        return createErrorRes(Payload::DefaultValueSetted);
}

// ------------------------------------------

Item<QString>::Item(const QJsonValue& value, QString defaultValue) :
        _value(value),
        _defaultValue(defaultValue),
        _check([](const QJsonValue& v){return true;})
{
}

Item<QString>::Item(const QJsonValue& value, Item::Check_fn func, QString defaultValue) :
        Item(value, defaultValue)
{
        _check = func;
}

Item<QString>::Item(const QJsonValue& value, Payload::AutoCheck autoCheck, QString defaultValue) :
        Item(value, defaultValue)
{
        // TODO create more QStringelegence solution with case of handlers
        switch (autoCheck) {
                case Payload::Empty:
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() == 0;
                                        }
                                        return true;
                                };
                                return v.isNull() && v.isUndefined() && checkString();
                        };
                        break;
                case Payload::NotEmpty:
                        // TODO call ^ this function
                        _check = [](const QJsonValue& v) {
                                auto checkString = [v]() {
                                        if (v.type() == QJsonValue::String) {
                                                return v.toString().length() > 0;
                                        }
                                        return true;
                                };
                                return !v.isNull() && !v.isUndefined();
                        };
                        break;
                case Payload::Null:
                        _check = [](const QJsonValue& v) {
                                return v.isNull();
                        };
                        break;
                case Payload::NotNull:
                        _check = [](const QJsonValue& v) {
                                return !v.isNull();
                        };
                        break;
                default:
                        throw (QString("Item::") + QString(__func__) + QString(" Unknown AutoCheck value passed: ") + QString::number(autoCheck));
                        break;
        }
}

Item<QString>::~Item()
{ }

Payload::ExtractionRes<QString> Item<QString>::extract() const {
        auto createErrorRes = [this](Payload::Errno e) {
                return Payload::ExtractionRes<QString>{
                        .value = _defaultValue,
                        .error = e,
                };
        };

        if (!_check(_value)) {
                return createErrorRes(Payload::CheckFailed);
        }

        if (!_value.isNull() && !_value.isUndefined()) {
                if (!cmpjvtype<QString>(_value.type())) {
                        return createErrorRes(Payload::TypesDontMatch);
                } else {
                        return Payload::ExtractionRes<QString>{
                                .value = _value.toString(),
                                .error = Payload::Ok
                        };
                }
        }

        return createErrorRes(Payload::DefaultValueSetted);
}

// ------------------------------------------

// template<class ArrayType>
// Item<QVector<ArrayType>>::Item(const QJsonValue& value, ArrayType defaultValue) :
//         _value(value),
//         _defaultValue(defaultValue),
//         _check([](const QJsonValue& v){return true;})
// {
// }
//
// template<class ArrayType>
// Item<QVector<ArrayType>>::Item(const QJsonValue& value, Item::Check_fn func, ArrayType defaultValue) :
//         Item(value, defaultValue)
// {
//         _check = func;
// }
//
// template<class ArrayType>
// Item<QVector<ArrayType>>::Item(const QJsonValue& value, Payload::AutoCheck autoCheck, ArrayType defaultValue) :
//         Item(value, defaultValue)
// {
//         // TODO create more QStringelegence solution with case of handlers
//         switch (autoCheck) {
//                 case Payload::Empty:
//                         _check = [](const QJsonValue& v) {
//                                 auto checkString = [v]() {
//                                         if (v.type() == QJsonValue::String) {
//                                                 return v.toString().length() == 0;
//                                         }
//                                         return true;
//                                 };
//                                 return v.isNull() && v.isUndefined() && checkString();
//                         };
//                         break;
//                 case Payload::NotEmpty:
//                         // TODO call ^ this function
//                         _check = [](const QJsonValue& v) {
//                                 auto checkString = [v]() {
//                                         if (v.type() == QJsonValue::String) {
//                                                 return v.toString().length() > 0;
//                                         }
//                                         return true;
//                                 };
//                                 return !v.isNull() && !v.isUndefined();
//                         };
//                         break;
//                 case Payload::Null:
//                         _check = [](const QJsonValue& v) {
//                                 return v.isNull();
//                         };
//                         break;
//                 case Payload::NotNull:
//                         _check = [](const QJsonValue& v) {
//                                 return !v.isNull();
//                         };
//                         break;
//                 default:
//                         throw (QString("Item::") + QString(__func__) + QString(" Unknown AutoCheck value passed: ") + QString::number(autoCheck));
//                         break;
//         }
// }
//
// template<class ArrayType>
// Item<QVector<ArrayType>>::~Item()
// { }
//
// template<class ArrayType>
// Payload::ExtractionRes<QVector<ArrayType>> Item<QVector<ArrayType>>::extract() const {
//         auto createErrorRes = [this](Payload::Errno e) {
//                 return Payload::ExtractionRes<QVector<ArrayType>>{
//                         .value = _defaultValue,
//                         .error = e,
//                 };
//         };
//
//         if (!_check(_value)) {
//                 return createErrorRes(Payload::CheckFailed);
//         }
//
//         if (!_value.isNull() && !_value.isUndefined()) {
//                 if (!cmpjvtype(_value.type())) {
//                         return createErrorRes(Payload::TypesDontMatch);
//                 } else {
//                         auto arr = _value.toArray();
//                         Payload
//                         return Payload::ExtractionRes<QString>{
//                                 .value = _value.toArray(),
//                                 .error = Payload::Ok
//                         };
//                 }
//         }
//
//         return createErrorRes(Payload::DefaultValueSetted);
// }

} /* API::Payload */
