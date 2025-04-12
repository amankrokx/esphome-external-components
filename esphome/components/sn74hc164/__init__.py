import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_CLOCK_PIN, CONF_DATA_PIN, CONF_ID

sn74hc164_ns = cg.esphome_ns.namespace("sn74hc164")
SN74HC164Component = sn74hc164_ns.class_("SN74HC164Component", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.declare_id(SN74HC164Component),
    cv.Required(CONF_DATA_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_CLOCK_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    data_pin = await cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data_pin))
    clock_pin = await cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(clock_pin))