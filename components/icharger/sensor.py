import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    CONF_CURRENT,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_TEMPERATURE,
    ICON_CURRENT_AC,
    ICON_EMPTY,
    ICON_FLASH,
    ICON_POWER,
    UNIT_CELSIUS,
    UNIT_AMPERE,
    UNIT_EMPTY,
    UNIT_VOLT,
)

from . import COMPONENT_NAME, CONF_COMPONENT_ID, IChargerComponent, CODEOWNERS

CODEOWNERS  # hack to make pylint shut up

DEPENDENCIES = [COMPONENT_NAME]

# int    'channel'
# int    'charging_mode_id'
# long   'timestamp'
# int    'cycle_count'
# int    'step_id'
# int    'current'
# int    'input_voltage'
# int    'battery_voltage'
# int    'capacity_mah'
# int    'int_temp'
# int    'ext_temp'
# int    'c01'
# int    'c02'
# int    'c03'
# int    'c04'
# int    'c05'
# int    'c06'
# int    'c07'
# int    'c08'
# int    'c09'
# int    'c10'
# ignore 'checksum'


SENSORS = [
    CONF_CHANNEL          := 'channel',
    CONF_CHARGING_MODE_ID := 'charging_mode_id',
    CONF_TIMESTAMP        := 'timestamp',
    CONF_CYCLE_COUNT      := 'cycle_count',
    CONF_STEP_ID          := 'step_id',
    CONF_CURRENT,          # standard, from esphome.const
    CONF_INPUT_VOLTAGE    := 'input_voltage',
    CONF_BATTERY_VOLTAGE,  # standard, from esphome.const
    CONF_CAPACITY_MAH     := 'capacity_mah',
    CONF_INT_TEMP         := 'int_temp',
    CONF_EXT_TEMP         := 'ext_temp',
    CONF_C01              := 'c01',
    CONF_C02              := 'c02',
    CONF_C03              := 'c03',
    CONF_C04              := 'c04',
    CONF_C05              := 'c05',
    CONF_C06              := 'c06',
    CONF_C07              := 'c07',
    CONF_C08              := 'c08',
    CONF_C09              := 'c09',
    CONF_C10              := 'c10',
    # CONF_CHECKSUM         = 'checksum',
]

s_s = sensor.sensor_schema


def empty_schema(icon=ICON_EMPTY):
    return s_s(UNIT_EMPTY, icon, 0, DEVICE_CLASS_EMPTY)


def volt_schema():
    return s_s(UNIT_VOLT, ICON_FLASH, 0, DEVICE_CLASS_VOLTAGE)


def celsius_schema():
    return s_s(UNIT_CELSIUS, ICON_FLASH, 0, DEVICE_CLASS_TEMPERATURE)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMPONENT_ID): cv.use_id(IChargerComponent),

        cv.Optional(CONF_CHANNEL):          empty_schema(),
        cv.Optional(CONF_CHARGING_MODE_ID): empty_schema(),
        cv.Optional(CONF_TIMESTAMP):        empty_schema(),
        cv.Optional(CONF_CYCLE_COUNT):      empty_schema(),
        cv.Optional(CONF_STEP_ID):          empty_schema(),
        cv.Optional(CONF_CURRENT): sensor.sensor_schema(
            UNIT_AMPERE, ICON_CURRENT_AC, 0, DEVICE_CLASS_CURRENT
        ),
        cv.Optional(CONF_INPUT_VOLTAGE):    volt_schema(),
        cv.Optional(CONF_BATTERY_VOLTAGE):  volt_schema(),
        cv.Optional(CONF_CAPACITY_MAH):     empty_schema(icon=ICON_POWER),
        cv.Optional(CONF_INT_TEMP): celsius_schema(),
        cv.Optional(CONF_EXT_TEMP): celsius_schema(),
        cv.Optional(CONF_C01): volt_schema(),
        cv.Optional(CONF_C02): volt_schema(),
        cv.Optional(CONF_C03): volt_schema(),
        cv.Optional(CONF_C04): volt_schema(),
        cv.Optional(CONF_C05): volt_schema(),
        cv.Optional(CONF_C06): volt_schema(),
        cv.Optional(CONF_C07): volt_schema(),
        cv.Optional(CONF_C08): volt_schema(),
        cv.Optional(CONF_C09): volt_schema(),
        cv.Optional(CONF_C10): volt_schema(),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_COMPONENT_ID])
    for key in SENSORS:
        if key in config:
            conf = config[key]
            sens = yield sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
