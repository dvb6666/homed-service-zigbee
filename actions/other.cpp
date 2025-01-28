#include <QtEndian>
#include "other.h"
#include "zcl.h"

QVariant ActionsYandex::Settings::request(const QString &name, const QVariant &data)
{
    int index = m_actions.indexOf(name);

    switch (index)
    {
        case 0: // switchMode
        case 1: // switchType
        case 2: // powerMode
        {
            qint8 value = static_cast <qint8> (enumIndex(name, data));
            m_attributes = {static_cast <quint16> (index + 1)};
            return value < 0 ? QByteArray() : zclHeader(FC_CLUSTER_SPECIFIC, m_transactionId++, static_cast <quint8> (m_attributes.at(0)), m_manufacturerCode).append(value);
        }

        case 3: // indicator
        case 4: // interlock
        {
            quint8 value = data.toBool() ? 0x00 : 0x01;
            m_attributes = {static_cast <quint16> (index == 3 ? 0x0005 : 00007)};
            return writeAttribute(DATA_TYPE_BOOLEAN, &value, sizeof(value));
        }
    }

    return QByteArray();
}

QVariant ActionsCustom::Attribute::request(const QString &, const QVariant &data)
{
    QList <QString> types = {"bool", "value", "enum"}; // TODO: refactor this
    QVariant value;

    switch (types.indexOf(m_type))
    {
        case 0: value = data.toBool() ? 0x01 : 0x00; break; // bool
        case 1: value = data.toDouble() * m_divider; break; // value

        case 2: // enum
        {
            int index = enumIndex(m_name, data);

            if (index < 0)
                return QByteArray();

            value = index;
            break;
        }
    }

    switch (m_dataType)
    {
        case DATA_TYPE_SINGLE_PRECISION:
        {
            float number = qToLittleEndian(value.toFloat());
            return writeAttribute(m_dataType, &number, sizeof(number));
        }

        case DATA_TYPE_DOUBLE_PRECISION:
        {
            double number = qToLittleEndian(value.toDouble());
            return writeAttribute(m_dataType, &number, sizeof(number));
        }

        default:
        {
            qint64 number = qToLittleEndian <qint64> (value.toDouble());
            return writeAttribute(m_dataType, &number, zclDataSize(m_dataType));
        }
    }
}
