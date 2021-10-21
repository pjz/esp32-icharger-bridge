#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace icharger {

class IChargerComponent : public uart::UARTDevice, public Component {
 public:

  // 'channel', 'charging_mode_id', 'timestamp', 'cycle_count', 'step_id', 'current', 'input_voltage', 'battery_voltage', 'capacity_mah', 'int_temp', 'ext_temp', 'c01', 'c02', 'c03', 'c04', 'c05', 'c06', 'c07', 'c08', 'c09', 'c10'
  void set_channel_sensor(sensor::Sensor *s) {
    channel_sensor_ = s;
  }
  void set_charging_mode_id_sensor(sensor::Sensor *s) {
    charging_mode_id_sensor_ = s;
  }
  void set_timestamp_sensor(sensor::Sensor *s) {
    timestamp_sensor_ = s;
  }
  void set_cycle_count_sensor(sensor::Sensor *s) {
    cycle_count_sensor_ = s;
  }
  void set_step_id_sensor(sensor::Sensor *s) {
    step_id_sensor_ = s;
  }
  void set_current_sensor(sensor::Sensor *s) {
    current_sensor_ = s;
  }
  void set_input_voltage_sensor(sensor::Sensor *s) {
    input_voltage_sensor_ = s;
  }
  void set_battery_voltage_sensor(sensor::Sensor *s) {
    battery_voltage_sensor_ = s;
  }
  void set_capacity_mah_sensor(sensor::Sensor *s) {
    capacity_mah_sensor_ = s;
  }
  void set_int_temp_sensor(sensor::Sensor *s) {
    int_temp_sensor_ = s;
  }
  void set_ext_temp_sensor(sensor::Sensor *s) {
    ext_temp_sensor_ = s;
  }
  void set_c01_sensor(sensor::Sensor *s) {
    c01_sensor_ = s;
  }
  void set_c02_sensor(sensor::Sensor *s) {
    c02_sensor_ = s;
  }
  void set_c03_sensor(sensor::Sensor *s) {
    c03_sensor_ = s;
  }
  void set_c04_sensor(sensor::Sensor *s) {
    c04_sensor_ = s;
  }
  void set_c05_sensor(sensor::Sensor *s) {
    c05_sensor_ = s;
  }
  void set_c06_sensor(sensor::Sensor *s) {
    c06_sensor_ = s;
  }
  void set_c07_sensor(sensor::Sensor *s) {
    c07_sensor_ = s;
  }
  void set_c08_sensor(sensor::Sensor *s) {
    c08_sensor_ = s;
  }
  void set_c09_sensor(sensor::Sensor *s) {
    c09_sensor_ = s;
  }
  void set_c10_sensor(sensor::Sensor *s) {
    c10_sensor_ = s;
  }
  void set_charging_mode_text_sensor(text_sensor::TextSensor *s) {
    charging_mode_text_sensor_ = s;
  }

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const { return setup_priority::DATA; }

 protected:
  void handle_value_();

  sensor::Sensor *channel_sensor_{nullptr};
  sensor::Sensor *charging_mode_id_sensor_{nullptr};
  sensor::Sensor *timestamp_sensor_{nullptr};
  sensor::Sensor *cycle_count_sensor_{nullptr};
  sensor::Sensor *step_id_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *input_voltage_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *capacity_mah_sensor_{nullptr};
  sensor::Sensor *int_temp_sensor_{nullptr};
  sensor::Sensor *ext_temp_sensor_{nullptr};
  sensor::Sensor *c01_sensor_{nullptr};
  sensor::Sensor *c02_sensor_{nullptr};
  sensor::Sensor *c03_sensor_{nullptr};
  sensor::Sensor *c04_sensor_{nullptr};
  sensor::Sensor *c05_sensor_{nullptr};
  sensor::Sensor *c06_sensor_{nullptr};
  sensor::Sensor *c07_sensor_{nullptr};
  sensor::Sensor *c08_sensor_{nullptr};
  sensor::Sensor *c09_sensor_{nullptr};
  sensor::Sensor *c10_sensor_{nullptr};

  text_sensor::TextSensor *charging_mode_text_sensor_{nullptr};

  int state_{0};
  std::string value_;
  uint32_t last_transmission_{0};
};

}  // namespace icharger
}  // namespace esphome
