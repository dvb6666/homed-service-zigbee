#include <math.h>
#include <QtEndian>
#include "device.h"
#include "property.h"

void PropertyObject::registerMetaTypes(void)
{
    qRegisterMetaType <Properties::BatteryVoltage>              ("batteryVoltageProperty");
    qRegisterMetaType <Properties::BatteryPercentage>           ("batteryPercentageProperty");
    qRegisterMetaType <Properties::Status>                      ("statusProperty");
    qRegisterMetaType <Properties::Contact>                     ("contactProperty");
    qRegisterMetaType <Properties::PowerOnStatus>               ("powerOnStatusProperty");
    qRegisterMetaType <Properties::Level>                       ("levelProperty");
    qRegisterMetaType <Properties::ColorHS>                     ("colorHSProperty");
    qRegisterMetaType <Properties::ColorXY>                     ("colorXYProperty");
    qRegisterMetaType <Properties::ColorTemperature>            ("colorTemperatureProperty");
    qRegisterMetaType <Properties::Illuminance>                 ("illuminanceProperty");
    qRegisterMetaType <Properties::Temperature>                 ("temperatureProperty");
    qRegisterMetaType <Properties::Humidity>                    ("humidityProperty");
    qRegisterMetaType <Properties::Occupancy>                   ("occupancyProperty");
    qRegisterMetaType <Properties::Energy>                      ("energyProperty");
    qRegisterMetaType <Properties::Voltage>                     ("voltageProperty");
    qRegisterMetaType <Properties::Current>                     ("currentProperty");
    qRegisterMetaType <Properties::Power>                       ("powerProperty");
    qRegisterMetaType <Properties::Scene>                       ("sceneProperty");
    qRegisterMetaType <Properties::IdentifyAction>              ("identifyActionProperty");
    qRegisterMetaType <Properties::SwitchAction>                ("switchActionProperty");
    qRegisterMetaType <Properties::LevelAction>                 ("levelActionProperty");
    qRegisterMetaType <Properties::ColorAction>                 ("colorActionProperty");

    qRegisterMetaType <PropertiesIAS::Contact>                  ("iasContactProperty");
    qRegisterMetaType <PropertiesIAS::Contact>                  ("iasGasProperty");
    qRegisterMetaType <PropertiesIAS::Occupancy>                ("iasOccupancyProperty");
    qRegisterMetaType <PropertiesIAS::Smoke>                    ("iasSmokeProperty");
    qRegisterMetaType <PropertiesIAS::WaterLeak>                ("iasWaterLeakProperty");

    qRegisterMetaType <PropertiesPTVO::CO2>                     ("ptvoCO2Property");
    qRegisterMetaType <PropertiesPTVO::Temperature>             ("ptvoTemperatureProperty");
    qRegisterMetaType <PropertiesPTVO::ChangePattern>           ("ptvoChangePatternProperty");
    qRegisterMetaType <PropertiesPTVO::Pattern>                 ("ptvoPatternProperty");
    qRegisterMetaType <PropertiesPTVO::SwitchAction>            ("ptvoSwitchActionProperty");

    qRegisterMetaType <PropertiesLUMI::Data>                    ("lumiDataProperty");
    qRegisterMetaType <PropertiesLUMI::BatteryVoltage>          ("lumiBatteryVoltageProperty");
    qRegisterMetaType <PropertiesLUMI::Power>                   ("lumiPowerProperty");
    qRegisterMetaType <PropertiesLUMI::ButtonAction>            ("lumiButtonActionProperty");
    qRegisterMetaType <PropertiesLUMI::SwitchAction>            ("lumiSwitchActionProperty");
    qRegisterMetaType <PropertiesLUMI::CubeRotation>            ("lumiCubeRotationProperty");
    qRegisterMetaType <PropertiesLUMI::CubeMovement>            ("lumiCubeMovementProperty");
    qRegisterMetaType <PropertiesLUMI::Unknown>                 ("lumiUnknownProperty");

    qRegisterMetaType <PropertiesTUYA::NeoSiren>                ("tuyaNeoSirenProperty");
    qRegisterMetaType <PropertiesTUYA::PresenceSensor>          ("tuyaPresenceSensorProperty");
    qRegisterMetaType <PropertiesTUYA::PowerOnStatus>           ("tuyaPowerOnStatusProperty");
    qRegisterMetaType <PropertiesTUYA::SwitchMode>              ("tuyaSwitchModeProperty");
    qRegisterMetaType <PropertiesTUYA::Unknown>                 ("tuyaUnknownProperty");

    qRegisterMetaType <PropertiesOther::KonkeButtonAction>      ("konkeButtonActionProperty");
    qRegisterMetaType <PropertiesOther::LifeControlAirQuality>  ("lifeControlAirQualityProperty");
    qRegisterMetaType <PropertiesOther::PerenioSmartPlug>       ("perenioSmartPlugProperty");
}

quint8 PropertyObject::deviceVersion(void)
{
    EndpointObject *endpoint = reinterpret_cast <EndpointObject*> (m_parent);
    return (endpoint && !endpoint->device().isNull()) ? endpoint->device()->version() : 0;
}

QString PropertyObject::deviceModelName(void)
{
    EndpointObject *endpoint = reinterpret_cast <EndpointObject*> (m_parent);
    return (endpoint && !endpoint->device().isNull()) ? endpoint->device()->modelName() : QString();
}

QVariant PropertyObject::deviceOption(const QString &key)
{
    EndpointObject *endpoint = reinterpret_cast <EndpointObject*> (m_parent);
    return (endpoint && !endpoint->device().isNull()) ? endpoint->device()->options().value(key) : QVariant();
}

quint8 PropertyObject::percentage(double min, double max, double value)
{
    if (value < min)
        value = min;

    if (value > max)
        value = max;

    return static_cast <quint8> ((value - min) / (max - min) * 100);
}

void Properties::BatteryVoltage::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0020 || dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
        return;

    m_value = percentage(2850, 3200, static_cast <quint8> (data.at(0)) * 100);
}

void Properties::BatteryPercentage::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0021 || dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
        return;

    m_value = static_cast <quint8> (data.at(0)) / (deviceOption("batteryUndivided").toBool() ? 1.0 : 2.0);
}

void Properties::Status::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0000 || (dataType != DATA_TYPE_BOOLEAN && dataType != DATA_TYPE_8BIT_UNSIGNED) || data.length() != 1)
        return;

    m_value = data.at(0) ? "on" : "off";
}

void Properties::Contact::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0000 || dataType != DATA_TYPE_BOOLEAN || data.length() != 1)
        return;

    m_value = data.at(0) ? true : false;
}

void Properties::PowerOnStatus::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x4003 || dataType != DATA_TYPE_8BIT_ENUM || data.length() != 1)
        return;

    switch (static_cast <quint8> (data.at(0)))
    {
        case 0x00: m_value = "off"; break;
        case 0x01: m_value = "on"; break;
        case 0x02: m_value = "toggle"; break;
        case 0xFF: m_value = "previous"; break;
    }
}

void Properties::Level::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0000 || dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
        return;

    m_value = static_cast <quint8> (data.at(0));
}

void Properties::ColorHS::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    switch (attributeId)
    {
        case 0x0000:

            if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                return;

            m_colorH = static_cast <quint8> (data.at(0));
            break;

        case 0x0001:

            if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                return;

            m_colorS = static_cast <quint8> (data.at(0));
            break;
    }

    if (!m_colorH.isValid() || !m_colorS.isValid())
        return;

    m_value = QList <QVariant> {m_colorH, m_colorS};
}

void Properties::ColorXY::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    switch (attributeId)
    {
        case 0x0003:
        {
            quint16 value = 0;

            if (dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
                return;

            memcpy(&value, data.constData(), data.length());
            m_colorX = static_cast <double> (qFromLittleEndian(value)) / 0xFFFF;
            break;
        }

        case 0x0004:
        {
            quint16 value = 0;

            if (dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
                return;

            memcpy(&value, data.constData(), data.length());
            m_colorY = static_cast <double> (qFromLittleEndian(value)) / 0xFFFF;
            break;
        }
    }

    if (!m_colorX.isValid() || !m_colorY.isValid())
        return;

    m_value = QList <QVariant> {m_colorX, m_colorY};
}

void Properties::ColorTemperature::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    qint16 value = 0;

    if (attributeId != 0x0007 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value);
}

void Properties::Illuminance::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    quint16 value = 0;

    if (attributeId != 0x0000 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = static_cast <quint32> (value ? pow(10, (qFromLittleEndian(value) - 1) / 10000.0) : 0) + deviceOption("illuminanceOffset").toInt();
}

void Properties::Temperature::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    qint16 value = 0;

    if (attributeId != 0x0000 || dataType != DATA_TYPE_16BIT_SIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value) / 100.0 + deviceOption("temperatureOffset").toDouble();
}

void Properties::Humidity::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    qint16 value = 0;

    if (attributeId != 0x0000 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value) / 100.0 + deviceOption("humidityOffset").toDouble();
}

void Properties::Occupancy::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0000 || dataType != DATA_TYPE_8BIT_BITMAP || data.length() != 1)
        return;

    m_value = data.at(0) ? true : false;
}

void Properties::Occupancy::resetValue(void)
{
    m_value = false;
}

void Properties::Energy::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    double multiplier = deviceOption("energyMutiplier").toDouble();
    qint64 value = 0;

    if (attributeId != 0x0000 || dataType != DATA_TYPE_48BIT_UNSIGNED || data.length() != 6)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value) * (multiplier ? multiplier : 1);
}

void Properties::Voltage::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    double multiplier = deviceOption("voltageMutiplier").toDouble();
    qint16 value = 0;

    if (attributeId != 0x0505 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value) * (multiplier ? multiplier : 1) + deviceOption("voltageOffset").toDouble();
}

void Properties::Current::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    double multiplier = deviceOption("currentMutiplier").toDouble();
    qint16 value = 0;

    if (attributeId != 0x0508 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value) * (multiplier ? multiplier : 1) + deviceOption("currentMutiplier").toDouble();
}

void Properties::Power::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    double multiplier = deviceOption("powerMutiplier").toDouble();
    qint16 value = 0;

    if (attributeId != 0x050B || dataType != DATA_TYPE_16BIT_SIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = qFromLittleEndian(value) * (multiplier ? multiplier : 1) + deviceOption("powerOffset").toDouble();
}

void Properties::Scene::parseCommand(quint8 commandId, const QByteArray &payload)
{
    const recallSceneStruct *command = reinterpret_cast <const recallSceneStruct*> (payload.constData());
    QVariant scene = deviceOption("scenes").toMap().value(QString::number(command->sceneId));

    if (commandId != 0x05)
        return;

    m_value = scene.isValid() ? scene : command->sceneId;
}

void Properties::IdentifyAction::parseCommand(quint8 commandId, const QByteArray &payload)
{
    Q_UNUSED(payload)

    if (commandId != 0x01)
        return;

    m_value = "identify";
}

void Properties::SwitchAction::parseCommand(quint8 commandId, const QByteArray &payload)
{
    Q_UNUSED(payload)

    switch (commandId)
    {
        case 0x00: m_value = "off"; break;
        case 0x01: m_value = "on"; break;
        case 0x02: m_value = "toggle"; break;
    }
}

void Properties::LevelAction::parseCommand(quint8 commandId, const QByteArray &payload)
{
    switch (commandId)
    {
        case 0x01:
        case 0x05:
            m_value = payload.at(0) ? "moveLevelDown" : "moveLevelUp";
            break;

        case 0x02:
        case 0x06:
            m_value = payload.at(0) ? "stepLevelDown" : "stepLevelUp";
            break;

        case 0x03:
        case 0x07:
            m_value = "stopLevel";
            break;
    }
}

void Properties::ColorAction::parseCommand(quint8 commandId, const QByteArray &payload)
{
    switch (commandId)
    {
        case 0x4B:

            switch (payload.at(0))
            {
                case 0x00: m_value = "stopColorTemperature"; break;
                case 0x01: m_value = "moveColorTemperatureUp"; break;
                case 0x03: m_value = "moveColorTemperatureDown"; break;
            }

            break;


        case 0x4C:

            switch (payload.at(0))
            {
                case 0x01: m_value = "stepColorTemperatureUp"; break;
                case 0x03: m_value = "stepColorTemperatureDown"; break;
            }

            break;
    }
}

void PropertiesIAS::ZoneStatus::parseCommand(quint8 commandId, const QByteArray &payload)
{
    QMap <QString, QVariant> map = m_value.toMap();
    quint16 value;

    if (commandId != 0x00)
        return;

    memcpy(&value, payload.constData(), sizeof(value));
    value = qFromLittleEndian(value);
    map.insert(m_name, (value & 0x0001) ? true : false);

    if (value & 0x0004)
        map.insert("tamper", true);

    if (value & 0x0008)
        map.insert("batteryLow", true);

    m_value = map;
}

void PropertiesIAS::ZoneStatus::clearValue(void)
{
    QMap <QString, QVariant> map = m_value.toMap();
    map.remove(m_name);
    m_value = map;
}

void PropertiesIAS::ZoneStatus::resetValue(void)
{
    QMap <QString, QVariant> map = m_value.toMap();
    map.insert(m_name, false);
    m_value = map;
}

void PropertiesPTVO::CO2::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    switch (attributeId)
    {
        case 0x0055:
        {
            float value = 0;

            if (dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
                return;

            memcpy(&value, data.constData(), data.length());
            m_buffer = value;
            break;
        }

        case 0x001C:
        {
            if (dataType != DATA_TYPE_CHARACTER_STRING || QString(data) != "ppm")
                return;

            m_value = m_buffer;
            break;
        }
    }
}

void PropertiesPTVO::Temperature::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    switch (attributeId)
    {
        case 0x0055:
        {
            float value = 0;

            if (dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
                return;

            memcpy(&value, data.constData(), data.length());
            m_buffer = value;
            break;
        }

        case 0x001C:
        {
            if (dataType != DATA_TYPE_CHARACTER_STRING || QString(data) != "C")
                return;

            m_value = m_buffer;
            break;
        }
    }
}

void PropertiesPTVO::ChangePattern::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0000 || dataType != DATA_TYPE_BOOLEAN || data.length() != 1)
        return;

    m_value = data.at(0) ? "on" : "off";
}

void PropertiesPTVO::Pattern::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    float value = 0;

    if (attributeId != 0x0055 || dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = static_cast <quint8> (value);
}

void PropertiesPTVO::SwitchAction::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0055 || dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
        return;

    m_value = data.at(0) ? "on" : "off";
}

void PropertiesLUMI::Data::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    QMap <QString, QVariant> map = m_value.toMap();

    if (attributeId == 0x00F7)
    {
        if (dataType != DATA_TYPE_OCTET_STRING)
            return;

        for (quint8 i = 0; i < static_cast <quint8> (data.length()); i++)
        {
            quint8 itemType = static_cast <quint8> (data.at(i + 1)), offset = i + 2, size = zclDataSize(itemType, data, &offset);

            if (!size)
                break;

            parseData(data.at(i), itemType, data.mid(offset, size), map);
            i += size + 1;
        }
    }
    else
        parseData(attributeId, dataType, data, map);

    if (map.isEmpty())
        return;

    m_value = map;
}

void PropertiesLUMI::Data::parseData(quint16 dataPoint, quint8 dataType, const QByteArray &data, QMap <QString, QVariant> &map)
{
    QString modelName = deviceModelName();

    switch (dataPoint)
    {
        case 0x0003:
        {
            if (modelName != "lumi.remote.b686opcn01" && modelName != "lumi.sen_ill.mgl01")
            {
                if (dataType != DATA_TYPE_8BIT_SIGNED || data.length() != 1)
                    break;

                map.insert("temperature", static_cast <qint8> (data.at(0)) + deviceOption("temperatureOffset").toDouble());
            }

            break;
        }

        case 0x0005:
        {
            quint16 value;

            if (dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
                break;

            memcpy(&value, data.constData(), data.length());
            map.insert("outageCount", qFromLittleEndian(value) - 1);
            break;
        }

        case 0x0009:
        {
            if (modelName == "lumi.remote.b686opcn01")
            {
                QList <QString> list = {"command", "event"};

                if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                    break;

                map.insert("mode", list.value(data.at(0), "unknown"));
            }

            break;
        }

        case 0x0064:
        {
            if (modelName == "lumi.sen_ill.mgl01")
            {
                quint32 value;

                if (dataType != DATA_TYPE_32BIT_UNSIGNED || data.length() != 4)
                    break;

                memcpy(&value, data.constData(), data.length());
                map.insert("illuminance", qFromLittleEndian(value) + deviceOption("illuminanceOffset").toInt());
            }

            break;
        }

        case 0x0065:
        case 0x0142:
        {
            if (modelName == "lumi.motion.ac01")
            {
                if (dataType != DATA_TYPE_8BIT_SIGNED || data.length() != 1)
                    break;

                map.insert("occupancy", data.at(0) ? true : false);
            }

            break;
        }

        case 0x0066:
        case 0x010C:
        case 0x0143:
        {
            if (modelName == "lumi.motion.ac01")
            {
                if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                    break;

                if (dataPoint != 0x0066 ? dataPoint == 0x010C : deviceVersion() >= 50)
                {
                    QList <QString> list = {"low", "medium", "high"};
                    map.insert("sensitivity", list.value(data.at(0) - 1, "unknown"));
                }
                else
                {
                    QList <QString> list = {"enter", "leave", "enterLeft", "leaveRight", "enterRight", "leaveLeft", "approach", "absent"};
                    map.insert("event", list.value(data.at(0), "unknown"));
                    map.insert("occupancy", data.at(0) != 0x01 ? true : false);
                }
            }

            break;
        }

        case 0x0067:
        case 0x0144:
        {
            if (modelName == "lumi.motion.ac01")
            {
                QList <QString> list = {"undirected", "directed"};

                if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                    break;

                map.insert("mode", list.value(data.at(0), "unknown"));
            }

            break;
        }

        case 0x0069:
        case 0x0146:
        {
            if (modelName == "lumi.motion.ac01")
            {
                QList <QString> list = {"far", "middle", "near"};

                if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                    break;

                map.insert("distance", list.value(data.at(0), "unknown"));
            }

            break;
        }

        case 0x0095:
        {
            float value;

            if (dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
                break;

            memcpy(&value, data.constData(), data.length());
            map.insert("energy", static_cast <double> (round(value * 100)) / 100);
            break;
        }

        case 0x0096:
        {
            float value;

            if (dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
                break;

            memcpy(&value, data.constData(),  data.length());
            map.insert("voltage", static_cast <double> (round(value)) / 10) + deviceOption("voltageOffset").toDouble();
            break;
        }

        case 0x0097:
        {
            float value;

            if (dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
                break;

            memcpy(&value, data.constData(),  data.length());
            map.insert("current", static_cast <double> (round(value)) / 1000) + deviceOption("currentOffset").toDouble();
            break;
        }

        case 0x0098:
        {
            float value;

            if (dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
                break;

            memcpy(&value, data.constData(), data.length());
            map.insert("power", static_cast <double> (round(value * 100) + deviceOption("powerOffset").toDouble()) / 100);
            break;
        }
    }
}

void PropertiesLUMI::BatteryVoltage::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    switch (attributeId)
    {
        case 0xFF01:
        {
            quint16 value = 0;

            if (dataType != DATA_TYPE_CHARACTER_STRING || data.length() < 4)
                break;

            memcpy(&value, data.constData() + 2, sizeof(value));
            m_value = percentage(2850, 3200, qFromLittleEndian(value));
            break;
        }

        case 0xFF02:
        {
            quint16 value = 0;

            if (dataType != DATA_TYPE_STRUCTURE || data.length() < 7)
                break;

            memcpy(&value, data.constData() + 5, sizeof(value));
            m_value = percentage(2850, 3200, qFromLittleEndian(value));
            break;
        }
    }
}

void PropertiesLUMI::Power::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    float value = 0;

    if (attributeId != 0x0055 || dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = static_cast <double> (round(value * 100)) / 100;
}

void PropertiesLUMI::ButtonAction::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (((attributeId != 0x0000 || dataType != DATA_TYPE_BOOLEAN) && (attributeId != 0x8000 || dataType != DATA_TYPE_8BIT_UNSIGNED)) || data.length() != 1)
        return;

    switch (static_cast <quint8> (data.at(0)))
    {
        case 0x00: m_value = "on"; break;               // TODO: singleClick
        case 0x01: m_value = "off"; break;              // TODO: release
        case 0x02: m_value = "doubleClick"; break;
        case 0x03: m_value = "tripleClick"; break;
        case 0x04: m_value = "quadrupleClick"; break;
        case 0x80: m_value = "multipleClick"; break;
    }
}

void PropertiesLUMI::SwitchAction::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    qint16 value = 0;

    if (attributeId != 0x0055 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());

    switch (qFromLittleEndian(value))
    {
        case 0x0000: m_value = "hold"; break;
        case 0x0001: m_value = "singleClick"; break;
        case 0x0002: m_value = "doubleClick"; break;
        case 0x0003: m_value = "tripleClick"; break;
        case 0x00FF: m_value = "release"; break;
    }
}

void PropertiesLUMI::CubeRotation::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    float value = 0;

    if (attributeId != 0x0055 || dataType != DATA_TYPE_SINGLE_PRECISION || data.length() != 4)
        return;

    memcpy(&value, data.constData(), data.length());
    m_value = value < 0 ? "rotateLeft" : "rotateRight";
}

void PropertiesLUMI::CubeMovement::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    qint16 value = 0;

    if (attributeId != 0x0055 || dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());
    value = qFromLittleEndian(value);

    if (!value)
        m_value = "shake";
    else if (value == 2)
        m_value = "wake";
    else if (value == 3)
        m_value = "fall";
    else if (value >= 512)
        m_value = "tap";
    else if (value >= 256)
        m_value = "slide";
    else if (value >= 128)
        m_value = "flip";
    else if (value >= 64)
        m_value = "drop";
}

void PropertiesLUMI::Unknown::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    Q_UNUSED(attributeId)
    Q_UNUSED(dataType)
    Q_UNUSED(data)
}

void PropertiesTUYA::Data::parseCommand(quint8 commandId, const QByteArray &payload)
{
    const tuyaHeaderStruct *header = reinterpret_cast <const tuyaHeaderStruct*> (payload.constData());
    QVariant data;

    if (commandId != 0x01 && commandId != 0x02)
        return;

    data = parseData(header, payload.mid(sizeof(tuyaHeaderStruct)));

    if (!data.isValid())
        return;

    update(header->dataPoint, data);
}

QVariant PropertiesTUYA::Data::parseData(const tuyaHeaderStruct *header, const QByteArray &data)
{
    switch (header->dataType)
    {
        case 0x01:

            if (header->length == 1)
                return data.at(0) ? true : false;

            break;

        case 0x02:

            if (header->length == 4)
            {
                quint32 value;
                memcpy(&value, data.constData(), header->length);
                return qFromBigEndian(value);
            }

            break;

        case 0x04:

            if (header->length == 1)
                return static_cast <quint8> (data.at(0));

            break;
    }

    return QVariant();
}

void PropertiesTUYA::NeoSiren::update(quint8 dataPoint, const QVariant &data)
{
    QMap <QString, QVariant> map = m_value.toMap();

    switch (dataPoint)
    {
        case 0x05:
        {
            QList <QString> list = {"low", "medium", "high"};
            map.insert("volume", list.at(data.toInt()));
            break;
        }

        case 0x07: map.insert("duration", data.toInt()); break;
        case 0x0D: map.insert("alarm", data.toBool()); break;
        case 0x0F: map.insert("battery", data.toInt()); break;
        case 0x15: map.insert("melody", data.toInt()); break;
    }

    if (map.isEmpty())
        return;

    m_value = map;
}

void PropertiesTUYA::PresenceSensor::update(quint8 dataPoint, const QVariant &data)
{
    QMap <QString, QVariant> map = m_value.toMap();

    switch (dataPoint)
    {
        case 0x01: map.insert("occupancy", data.toBool()); break;
        case 0x02: map.insert("sensitivity", data.toInt()); break;
        case 0x03: map.insert("distanceMin", data.toDouble() / 100); break;
        case 0x04: map.insert("distanceMax", data.toDouble() / 100); break;
        case 0x65: map.insert("detectionDelay", data.toInt()); break;
        case 0x68: map.insert("illuminance", data.toInt() + deviceOption("illuminanceOffset").toInt()); break;
    }

    if (map.isEmpty())
        return;

    m_value = map;
}

void PropertiesTUYA::PowerOnStatus::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x8002 || dataType != DATA_TYPE_8BIT_ENUM || data.length() != 1)
        return;

    switch (static_cast <quint8> (data.at(0)))
    {
        case 0x00: m_value = "off"; break;
        case 0x01: m_value = "on"; break;
        case 0x02: m_value = "previous"; break;
    }
}

void PropertiesTUYA::SwitchMode::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0xD030 || dataType != DATA_TYPE_8BIT_ENUM || data.length() != 1)
        return;

    switch (static_cast <quint8> (data.at(0)))
    {
        case 0x00: m_value = "toggle"; break;
        case 0x01: m_value = "state"; break;
        case 0x02: m_value = "momentary"; break;
    }
}

void PropertiesTUYA::Unknown::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    Q_UNUSED(attributeId)
    Q_UNUSED(dataType)
    Q_UNUSED(data)
}

void PropertiesOther::KonkeButtonAction::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    if (attributeId != 0x0000 && dataType != DATA_TYPE_BOOLEAN)
        return;

    switch (static_cast <quint8> (data.at(0)))
    {
        case 0x80: m_value = "singleClick"; break;
        case 0x81: m_value = "doubleClick"; break;
        case 0x82: m_value = "hold"; break;
    }
}

void PropertiesOther::LifeControlAirQuality::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    QMap <QString, QVariant> map = m_value.toMap();
    qint16 value;

    if ((dataType != DATA_TYPE_16BIT_UNSIGNED && dataType != DATA_TYPE_16BIT_SIGNED) || data.length() != 2)
        return;

    memcpy(&value, data.constData(), data.length());

    switch (attributeId)
    {
        case 0x0000: map.insert("tempertature", qFromLittleEndian(value) / 100.0); break;
        case 0x0001: map.insert("humidity", qFromLittleEndian(value) / 100.0); break;
        case 0x0002: map.insert("eco2", qFromLittleEndian(value)); break;
        case 0x0003: map.insert("voc", qFromLittleEndian(value)); break;
    }

    if (map.isEmpty())
        return;

    m_value = map;
}

void PropertiesOther::PerenioSmartPlug::parseAttribte(quint16 attributeId, quint8 dataType, const QByteArray &data)
{
    QMap <QString, QVariant> map = m_value.toMap();

    switch (attributeId)
    {
        case 0x0000:
        {
            if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                break;

            switch (data.at(0))
            {
                case 0x00: map.insert("powerOnStatus", "off"); break;
                case 0x01: map.insert("powerOnStatus", "on"); break;
                case 0x02: map.insert("powerOnStatus", "prevoious"); break;
            }

            break;
        }

        case 0x0001:
        {
            if (dataType != DATA_TYPE_8BIT_UNSIGNED || data.length() != 1)
                break;

            map.insert("alarmVoltateMin",  data.at(0) & 0x01 ? true : false);
            map.insert("alarmVoltateMax",  data.at(0) & 0x02 ? true : false);
            map.insert("alarmPowerMax",    data.at(0) & 0x04 ? true : false);
            map.insert("alarmEnergyLimit", data.at(0) & 0x08 ? true : false);

            break;
        }

        case 0x000E:
        {
            quint32 value;

            if (dataType != DATA_TYPE_32BIT_UNSIGNED || data.length() != 4)
                break;

            memcpy(&value, data.constData(), data.length());
            map.insert("energy", static_cast <double> (qFromLittleEndian(value)) / 1000);
            break;
        }

        default:
        {
            quint16 value;

            if (dataType != DATA_TYPE_16BIT_UNSIGNED || data.length() != 2)
                break;

            memcpy(&value, data.constData(), data.length());


            switch (attributeId)
            {
                case 0x0003: map.insert("voltage", qFromLittleEndian(value)); break;
                case 0x0004: map.insert("voltageMin", qFromLittleEndian(value)); break;
                case 0x0005: map.insert("voltageMax", qFromLittleEndian(value)); break;
                case 0x000A: map.insert("power", qFromLittleEndian(value)); break;
                case 0x000B: map.insert("powerMax", qFromLittleEndian(value)); break;
                case 0x000F: map.insert("energyLimit", qFromLittleEndian(value)); break;
            }

            break;
        }
    }

    if (map.isEmpty())
        return;

    m_value = map;
}
