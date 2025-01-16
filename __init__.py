import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID

ht1621_ns = cg.esphome_ns.namespace("ht1621")
HT1621Display = ht1621_ns.class_("HT1621Display", cg.Component, display.DisplayBuffer)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HT1621Display),
    }
).extend(display.DISPLAY_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await display.register_display(var, config)