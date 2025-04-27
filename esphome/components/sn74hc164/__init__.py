from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_CLOCK_PIN,
    CONF_ID,
    CONF_INVERTED,
    CONF_NUMBER,
    CONF_OUTPUT,
    CONF_RESET_PIN,
    CONF_TYPE,
)

MULTI_CONF = True

sn74hc164_ns = cg.esphome_ns.namespace("sn74hc164")

SN74HC164Component = sn74hc164_ns.class_("SN74HC164Component", cg.Component)
SN74HC164GPIOPin = sn74hc164_ns.class_(
    "SN74HC164GPIOPin", cg.GPIOPin, cg.Parented.template(SN74HC164Component)
)

CONF_SN74HC164 = "sn74hc164"
CONF_A_PIN = "a_pin"
CONF_B_PIN = "b_pin"
CONF_RESET_PIN = "reset_pin"
CONF_SR_COUNT = "sr_count"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SN74HC164Component),
        cv.Required(CONF_CLOCK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_A_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_B_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_RESET_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_SR_COUNT, default=1): cv.int_range(min=1, max=256),
    }
).extend(cv.COMPONENT_SCHEMA)

def _validate_output_mode(value):
    if value.get(CONF_OUTPUT) is not True:
        raise cv.Invalid("Only output mode is supported")
    return value

SN74HC164_PIN_SCHEMA = pins.gpio_base_schema(
    SN74HC164GPIOPin,
    cv.int_range(min=0, max=2047),
    modes=[CONF_OUTPUT],
    mode_validator=_validate_output_mode,
    invertable=True,
).extend(
    {
        cv.Required(CONF_SN74HC164): cv.use_id(SN74HC164Component),
    }
)

def sn74hc164_pin_final_validate(pin_config, parent_config):
    max_pins = parent_config[CONF_SR_COUNT] * 8
    if pin_config[CONF_NUMBER] >= max_pins:
        raise cv.Invalid(f"Pin number must be less than {max_pins}")

@pins.PIN_SCHEMA_REGISTRY.register(
    CONF_SN74HC164, SN74HC164_PIN_SCHEMA, sn74hc164_pin_final_validate
)
async def sn74hc164_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_SN74HC164])
    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    clock_pin = await cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(clock_pin))
    
    a_pin = await cg.gpio_pin_expression(config[CONF_A_PIN])
    cg.add(var.set_a_pin(a_pin))
    
    b_pin = await cg.gpio_pin_expression(config[CONF_B_PIN])
    cg.add(var.set_b_pin(b_pin))
    
    reset_pin = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
    cg.add(var.set_reset_pin(reset_pin))
    
    cg.add(var.set_sr_count(config[CONF_SR_COUNT]))