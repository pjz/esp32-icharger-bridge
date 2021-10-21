import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID

from . import COMPONENT_NAME, CONF_COMPONENT_ID, IChargerComponent, CODEOWNERS

CODEOWNERS  # hack to make pylint shut up

DEPENDENCIES = [COMPONENT_NAME]


CONF_CHARGING_MODE = "charging_mode"

TEXT_SENSORS = [
    CONF_CHARGING_MODE,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMPONENT_ID): cv.use_id(IChargerComponent),

        cv.Optional(CONF_CHARGING_MODE): text_sensor.TEXT_SENSOR_SCHEMA.extend(
            {cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}
        ),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_COMPONENT_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            yield text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
