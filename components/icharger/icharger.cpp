#include "icharger.h"
#include "esphome/core/log.h"
#include <algorithm>  // std::min

namespace esphome {
namespace icharger {

static const char *const TAG = "icharger";

void IChargerComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "ICharger:");

  LOG_SENSOR("  ", "Channel", channel_sensor_);
  LOG_SENSOR("  ", "Charging Mode ID", charging_mode_id_sensor_);
  LOG_TEXT_SENSOR("  ", "Charging Mode", charging_mode_text_sensor_);
  LOG_SENSOR("  ", "Timestamp", timestamp_sensor_);
  LOG_SENSOR("  ", "Cycle Count", cycle_count_sensor_);
  LOG_SENSOR("  ", "Step ID", step_id_sensor_);
  LOG_SENSOR("  ", "Current", current_sensor_);
  LOG_SENSOR("  ", "Input Voltage", input_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR("  ", "Capacity mAh", capacity_mah_sensor_);
  LOG_SENSOR("  ", "Internal Temperature", int_temp_sensor_);
  LOG_SENSOR("  ", "External Temprature", ext_temp_sensor_);
  LOG_SENSOR("  ", "C01", c01_sensor_);
  LOG_SENSOR("  ", "C02", c02_sensor_);
  LOG_SENSOR("  ", "C03", c03_sensor_);
  LOG_SENSOR("  ", "C04", c04_sensor_);
  LOG_SENSOR("  ", "C05", c05_sensor_);
  LOG_SENSOR("  ", "C06", c06_sensor_);
  LOG_SENSOR("  ", "C07", c07_sensor_);
  LOG_SENSOR("  ", "C08", c08_sensor_);
  LOG_SENSOR("  ", "C09", c09_sensor_);
  LOG_SENSOR("  ", "C10", c10_sensor_);

  check_uart_settings(115200);
}

// example input line:
// $2;1;251036000;3;3;316;12039;3255;13784;401;259;0;0;0;0;0;0;0;0;0;0;17


void IChargerComponent::loop() {
  const uint32_t now = millis();
  if ((state_ > 0) && (now - last_transmission_ >= 200)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago.");
    state_ = 0;
  }

  if (!available())
    return;

  last_transmission_ = now;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    if (c == '$') continue;
    if ((c == '\r') || (c == '\n')) {
      handle_value_();
      value_.clear();
      state_ = 0;
    } else if (c == ';') {
      handle_value_();
      value_.clear();
      state_ += 1;
    } else {
      value_.push_back(c);
    }
  }
}

static const __FlashStringHelper *charging_mode_text(int value) {
  switch (value) {
    case 0:
      return F("Off");
    case 2:
      return F("Balance");
    case 3:
      return F("Charging");
    case 4:
      return F("Cycling");
    case 5:
      return F("Discharging");
    case 7:
      return F("Failing");
    case 9:
      return F("Explosion imminent");
    case 245:
      return F("Initializing");
    default:
      return F("Unknown");
  }
}

static const __FlashStringHelper *error_code_text(int value) {
  switch (value) {
    case 0:
      return F("No error");
    case 2:
      return F("Battery voltage too high");
    case 17:
      return F("Charger temperature too high");
    case 18:
      return F("Charger over current");
    case 19:
      return F("Charger current reversed");
    case 20:
      return F("Bulk time limit exceeded");
    case 21:
      return F("Current sensor issue");
    case 26:
      return F("Terminals overheated");
    case 28:
      return F("Converter issue");
    case 33:
      return F("Input voltage too high (solar panel)");
    case 34:
      return F("Input current too high (solar panel)");
    case 38:
      return F("Input shutdown (excessive battery voltage)");
    case 39:
      return F("Input shutdown (due to current flow during off mode)");
    case 65:
      return F("Lost communication with one of devices");
    case 66:
      return F("Synchronised charging device configuration issue");
    case 67:
      return F("BMS connection lost");
    case 68:
      return F("Network misconfigured");
    case 116:
      return F("Factory calibration data lost");
    case 117:
      return F("Invalid/incompatible firmware");
    case 119:
      return F("User settings invalid");
    default:
      return F("Unknown");
  }
}
static const __FlashStringHelper *warning_code_text(int value) {
  switch (value) {
    case 0:
      return F("No warning");
    case 1:
      return F("Low Voltage");
    case 2:
      return F("High Voltage");
    case 4:
      return F("Low SOC");
    case 8:
      return F("Low Starter Voltage");
    case 16:
      return F("High Starter Voltage");
    case 32:
      return F("Low Temperature");
    case 64:
      return F("High Temperature");
    case 128:
      return F("Mid Voltage");
    case 256:
      return F("Overload");
    case 512:
      return F("DC-ripple");
    case 1024:
      return F("Low V AC out");
    case 2048:
      return F("High V AC out");
    default:
      return F("Multiple warnings");
  }
}

static const __FlashStringHelper *device_type_text(int value) {
  switch (value) {
    case 0x203:
      return F("BMV-700");
    case 0x204:
      return F("BMV-702");
    case 0x205:
      return F("BMV-700H");
    case 0xA389:
      return F("SmartShunt");
    case 0xA381:
      return F("BMV-712 Smart");
    case 0xA04C:
      return F("BlueSolar MPPT 75/10");
    case 0x300:
      return F("BlueSolar MPPT 70/15");
    case 0xA042:
      return F("BlueSolar MPPT 75/15");
    case 0xA043:
      return F("BlueSolar MPPT 100/15");
    case 0xA044:
      return F("BlueSolar MPPT 100/30 rev1");
    case 0xA04A:
      return F("BlueSolar MPPT 100/30 rev2");
    case 0xA041:
      return F("BlueSolar MPPT 150/35 rev1");
    case 0xA04B:
      return F("BlueSolar MPPT 150/35 rev2");
    case 0xA04D:
      return F("BlueSolar MPPT 150/45");
    case 0xA040:
      return F("BlueSolar MPPT 75/50");
    case 0xA045:
      return F("BlueSolar MPPT 100/50 rev1");
    case 0xA049:
      return F("BlueSolar MPPT 100/50 rev2");
    case 0xA04E:
      return F("BlueSolar MPPT 150/60");
    case 0xA046:
      return F("BlueSolar MPPT 150/70");
    case 0xA04F:
      return F("BlueSolar MPPT 150/85");
    case 0xA047:
      return F("BlueSolar MPPT 150/100");
    case 0xA050:
      return F("SmartSolar MPPT 250/100");
    case 0xA051:
      return F("SmartSolar MPPT 150/100");
    case 0xA052:
      return F("SmartSolar MPPT 150/85");
    case 0xA053:
      return F("SmartSolar MPPT 75/15");
    case 0xA054:
      return F("SmartSolar MPPT 75/10");
    case 0xA055:
      return F("SmartSolar MPPT 100/15");
    case 0xA056:
      return F("SmartSolar MPPT 100/30");
    case 0xA057:
      return F("SmartSolar MPPT 100/50");
    case 0xA058:
      return F("SmartSolar MPPT 150/35");
    case 0xA059:
      return F("SmartSolar MPPT 150/100 rev2");
    case 0xA05A:
      return F("SmartSolar MPPT 150/85 rev2");
    case 0xA05B:
      return F("SmartSolar MPPT 250/70");
    case 0xA05C:
      return F("SmartSolar MPPT 250/85");
    case 0xA05D:
      return F("SmartSolar MPPT 250/60");
    case 0xA05E:
      return F("SmartSolar MPPT 250/45");
    case 0xA05F:
      return F("SmartSolar MPPT 100/20");
    case 0xA060:
      return F("SmartSolar MPPT 100/20 48V");
    case 0xA061:
      return F("SmartSolar MPPT 150/45");
    case 0xA062:
      return F("SmartSolar MPPT 150/60");
    case 0xA063:
      return F("SmartSolar MPPT 150/70");
    case 0xA064:
      return F("SmartSolar MPPT 250/85 rev2");
    case 0xA065:
      return F("SmartSolar MPPT 250/100 rev2");
    case 0xA201:
      return F("Phoenix Inverter 12V 250VA 230V");
    case 0xA202:
      return F("Phoenix Inverter 24V 250VA 230V");
    case 0xA204:
      return F("Phoenix Inverter 48V 250VA 230V");
    case 0xA211:
      return F("Phoenix Inverter 12V 375VA 230V");
    case 0xA212:
      return F("Phoenix Inverter 24V 375VA 230V");
    case 0xA214:
      return F("Phoenix Inverter 48V 375VA 230V");
    case 0xA221:
      return F("Phoenix Inverter 12V 500VA 230V");
    case 0xA222:
      return F("Phoenix Inverter 24V 500VA 230V");
    case 0xA224:
      return F("Phoenix Inverter 48V 500VA 230V");
    case 0xA231:
      return F("Phoenix Inverter 12V 250VA 230V");
    case 0xA232:
      return F("Phoenix Inverter 24V 250VA 230V");
    case 0xA234:
      return F("Phoenix Inverter 48V 250VA 230V");
    case 0xA239:
      return F("Phoenix Inverter 12V 250VA 120V");
    case 0xA23A:
      return F("Phoenix Inverter 24V 250VA 120V");
    case 0xA23C:
      return F("Phoenix Inverter 48V 250VA 120V");
    case 0xA241:
      return F("Phoenix Inverter 12V 375VA 230V");
    case 0xA242:
      return F("Phoenix Inverter 24V 375VA 230V");
    case 0xA244:
      return F("Phoenix Inverter 48V 375VA 230V");
    case 0xA249:
      return F("Phoenix Inverter 12V 375VA 120V");
    case 0xA24A:
      return F("Phoenix Inverter 24V 375VA 120V");
    case 0xA24C:
      return F("Phoenix Inverter 48V 375VA 120V");
    case 0xA251:
      return F("Phoenix Inverter 12V 500VA 230V");
    case 0xA252:
      return F("Phoenix Inverter 24V 500VA 230V");
    case 0xA254:
      return F("Phoenix Inverter 48V 500VA 230V");
    case 0xA259:
      return F("Phoenix Inverter 12V 500VA 120V");
    case 0xA25A:
      return F("Phoenix Inverter 24V 500VA 120V");
    case 0xA25C:
      return F("Phoenix Inverter 48V 500VA 120V");
    case 0xA261:
      return F("Phoenix Inverter 12V 800VA 230V");
    case 0xA262:
      return F("Phoenix Inverter 24V 800VA 230V");
    case 0xA264:
      return F("Phoenix Inverter 48V 800VA 230V");
    case 0xA269:
      return F("Phoenix Inverter 12V 800VA 120V");
    case 0xA26A:
      return F("Phoenix Inverter 24V 800VA 120V");
    case 0xA26C:
      return F("Phoenix Inverter 48V 800VA 120V");
    case 0xA271:
      return F("Phoenix Inverter 12V 1200VA 230V");
    case 0xA272:
      return F("Phoenix Inverter 24V 1200VA 230V");
    case 0xA274:
      return F("Phoenix Inverter 48V 1200VA 230V");
    case 0xA279:
      return F("Phoenix Inverter 12V 1200VA 120V");
    case 0xA27A:
      return F("Phoenix Inverter 24V 1200VA 120V");
    case 0xA27C:
      return F("Phoenix Inverter 48V 1200VA 120V");
    default:
      return nullptr;
  }
}

static std::string flash_to_string(const __FlashStringHelper *flash) {
  std::string result;
  const char *fptr = (PGM_P) flash;
  result.reserve(strlen_P(fptr));
  char c;
  while ((c = pgm_read_byte(fptr++)) != 0)
    result.push_back(c);
  return result;
}

void IChargerComponent::handle_value_() {
  int value;

  switch (state) {
    case 0:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 1:
      value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
      if (charging_mode_id_sensor_ != nullptr)
        charging_mode_id_sensor_->publish_state(value);
      if (charging_mode_text_sensor_ != nullptr)
        charging_mode_text_sensor_->publish_state(flash_to_string(charging_mode_text(value)));
      break;
    case 2:
      if (timestamp_sensor_ != nullptr) timestamp_sensor_->publish_state(atol(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 3:
      if (cycle_count_sensor_ != nullptr) cycle_count_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 4:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 5:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 6:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 7:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 8:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 9:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 10:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 11:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 12:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 13:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 14:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 15:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 16:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 17:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 18:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 19:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 20:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 21:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    default:
      break;
  }

}

}  // namespace icharger
}  // namespace esphome
