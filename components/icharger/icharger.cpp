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

  // TODO: add blocks for case 4.. for:
  // # 'step_id', 'current', 'input_voltage', 'battery_voltage', 'capacity_mah', 'int_temp', 'ext_temp', 'c01', 'c02', 'c03', 'c04', 'c05', 'c06', 'c07', 'c08', 'c09', 'c10', 'checksum'

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