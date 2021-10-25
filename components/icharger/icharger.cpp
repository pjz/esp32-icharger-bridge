
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
  LOG_TEXT_SENSOR("  ", "Charging Mode", charging_mode_text_sensor_);

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

#ifdef USE_STORE_LOG_STR_IN_FLASH
static std::string flash_to_string(const __FlashStringHelper *flash) {
  std::string result;
  const char *fptr = (PGM_P) flash;
  result.reserve(strlen_P(fptr));
  char c;
  while ((c = pgm_read_byte(fptr++)) != 0)
    result.push_back(c);
  return result;
}

static const __FlashStringHelper *charging_mode_text(int value) {
#else

#define F(s) s
#define flash_to_string(s) s

char* charging_mode_text(int value) {
#endif
// TODO: Make these strings accurate based on the icharger protocol
  switch (value) {
    case 1:
      return F("Charging");
    case 2:
      return F("Discharging");
    case 4:
      return F("Waiting");
    default:
      return F("Unknown Charging Mode");
  }
}

void IChargerComponent::handle_value_() {
  int value;

  // 'channel', 'charging_mode_id', 'timestamp', 'cycle_count', 'step_id', 'current', 'input_voltage', 'battery_voltage', 'capacity_mah', 'int_temp', 'ext_temp', 'c01', 'c02', 'c03', 'c04', 'c05', 'c06', 'c07', 'c08', 'c09', 'c10'

  switch ( state_ ) {
    case 0:
      if (channel_sensor_ != nullptr) channel_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 1:
      value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
      if (charging_mode_id_sensor_ != nullptr)
        charging_mode_id_sensor_->publish_state(value);
      if (charging_mode_text_sensor_ != nullptr) {
        charging_mode_text_sensor_->publish_state(flash_to_string(charging_mode_text(value)));
      }
      break;
    case 2:
      if (timestamp_sensor_ != nullptr) timestamp_sensor_->publish_state(atol(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 3:
      if (cycle_count_sensor_ != nullptr) cycle_count_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 4:
      if (step_id_sensor_ != nullptr) step_id_sensor_->publish_state(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
      break;
    case 5:
      if (current_sensor_ != nullptr) current_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 6:
      if (input_voltage_sensor_ != nullptr) input_voltage_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 7:
      if (battery_voltage_sensor_ != nullptr) battery_voltage_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 8:
      if (capacity_mah_sensor_ != nullptr) capacity_mah_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 9:
      if (int_temp_sensor_ != nullptr) int_temp_sensor_->publish_state(atoi(value_.c_str()) / 10 );  // NOLINT(cert-err34-c)
      break;
    case 10:
      if (ext_temp_sensor_ != nullptr) ext_temp_sensor_->publish_state(atoi(value_.c_str()) / 10 );  // NOLINT(cert-err34-c)
      break;
    case 11:
      if (c01_sensor_ != nullptr) c01_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 12:
      if (c02_sensor_ != nullptr) c02_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 13:
      if (c03_sensor_ != nullptr) c03_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 14:
      if (c04_sensor_ != nullptr) c04_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 15:
      if (c05_sensor_ != nullptr) c05_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 16:
      if (c06_sensor_ != nullptr) c06_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 17:
      if (c07_sensor_ != nullptr) c07_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 18:
      if (c08_sensor_ != nullptr) c08_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 19:
      if (c09_sensor_ != nullptr) c09_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    case 20:
      if (c10_sensor_ != nullptr) c10_sensor_->publish_state(atoi(value_.c_str()) / 1000 );  // NOLINT(cert-err34-c)
      break;
    default:
      break;
  }

}

}  // namespace icharger
}  // namespace esphome
