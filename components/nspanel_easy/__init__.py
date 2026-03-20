# __init__.py

from esphome import automation
from esphome import pins
from esphome.components.esp32 import add_idf_sdkconfig_option
from esphome.const import (CONF_ID, CONF_TRIGGER_ID)
from esphome.core import CORE, coroutine_with_priority
import esphome.codegen as cg
import esphome.config_validation as cv
import logging


CODEOWNERS = ["@edwardtfn"]

_LOGGER = logging.getLogger(__name__)

nspanel_easy_ns = cg.esphome_ns.namespace('nspanel_easy')

CONF_ON_DUMP_CONFIG = "on_dump_config"
CONF_ON_SETUP = "on_setup"
DISABLE_BOOTLOADER_LOGS = "disable_bootloader_logs"
LWIP_TCP_MSS = "lwip_tcp_mss"
MAIN_TASK_STACK_SIZE = "main_task_stack_size"
PSRAM_CLK_PIN = "psram_clk_pin"
PSRAM_CS_PIN = "psram_cs_pin"
REQUIRE_DISARM_BEFORE_REARM = "require_disarm_before_rearm"
TASK_WDT_TIMEOUT_S = "task_wdt_timeout_s"

NSPanelEasyComponent = nspanel_easy_ns.class_("NSPanelEasyComponent", cg.Component)
SetupTrigger = nspanel_easy_ns.class_("SetupTrigger", automation.Trigger.template())
DumpConfigTrigger = nspanel_easy_ns.class_("DumpConfigTrigger", automation.Trigger.template())

CONFIG_SCHEMA = cv.Schema({
    cv.Optional(CONF_ID, default="nspanel_easy_component"): cv.declare_id(NSPanelEasyComponent),
    cv.Optional(CONF_ON_SETUP): automation.validate_automation(
        {
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(SetupTrigger),
        },
    ),
    cv.Optional(CONF_ON_DUMP_CONFIG): automation.validate_automation(
        {
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(DumpConfigTrigger),
        },
    ),
    cv.Optional(DISABLE_BOOTLOADER_LOGS): cv.boolean,
    cv.Optional(LWIP_TCP_MSS): cv.int_range(min=536, max=1460),
    cv.Optional(MAIN_TASK_STACK_SIZE): cv.int_range(8192, 32768),
    cv.Optional(PSRAM_CLK_PIN): pins.internal_gpio_output_pin_number,
    cv.Optional(PSRAM_CS_PIN): pins.internal_gpio_output_pin_number,
    cv.Optional(REQUIRE_DISARM_BEFORE_REARM): cv.boolean,
    cv.Optional(TASK_WDT_TIMEOUT_S): cv.int_range(min=5, max=300),
})


@coroutine_with_priority(1.0)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for conf in config.get(CONF_ON_SETUP, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_DUMP_CONFIG, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

    # Arduino framework deprecation warning
    if CORE.using_arduino:
        _LOGGER.warning("Arduino framework deprecated. Migrate to ESP-IDF.")

    if PSRAM_CLK_PIN in config:
        clk_pin = config[PSRAM_CLK_PIN]
        add_idf_sdkconfig_option("CONFIG_D0WD_PSRAM_CLK_IO", clk_pin)

    if PSRAM_CS_PIN in config:
        cs_pin = config[PSRAM_CS_PIN]
        add_idf_sdkconfig_option("CONFIG_D0WD_PSRAM_CS_IO", cs_pin)

    # Handle bootloader logs configuration - Only when explicitly disabled
    if DISABLE_BOOTLOADER_LOGS in config and config[DISABLE_BOOTLOADER_LOGS]:
        add_idf_sdkconfig_option("CONFIG_BOOTLOADER_LOG_LEVEL_NONE", True)
        add_idf_sdkconfig_option("CONFIG_BOOTLOADER_LOG_LEVEL", 0)

    if MAIN_TASK_STACK_SIZE in config:
        add_idf_sdkconfig_option("CONFIG_ESP_MAIN_TASK_STACK_SIZE", config[MAIN_TASK_STACK_SIZE])
    if TASK_WDT_TIMEOUT_S in config:
        add_idf_sdkconfig_option("CONFIG_ESP_TASK_WDT_TIMEOUT_S", config[TASK_WDT_TIMEOUT_S])
    if LWIP_TCP_MSS in config:
        add_idf_sdkconfig_option("CONFIG_LWIP_TCP_MSS", config[LWIP_TCP_MSS])

    if REQUIRE_DISARM_BEFORE_REARM in config and config[REQUIRE_DISARM_BEFORE_REARM]:
        cg.add_define("USE_REQUIRE_DISARM_BEFORE_REARM")

    cg.add_define("USE_NSPANEL_EASY")
    cg.add_global(cg.RawExpression("using namespace esphome::nspanel_easy"))
