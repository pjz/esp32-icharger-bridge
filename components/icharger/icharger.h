#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace icharger {

class IChargerComponent : public uart::UARTDevice, public Component {
 public:

  // DONE TODO: add set_<sensorname>_sensor for:
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
  void set_c1_sensor(sensor::Sensor *s) {
    c1_sensor_ = s;
  }
  void set_c2_sensor(sensor::Sensor *s) {
    c2_sensor_ = s;
  }
  void set_c3_sensor(sensor::Sensor *s) {
    c3_sensor_ = s;
  }
  void set_c4_sensor(sensor::Sensor *s) {
    c4_sensor_ = s;
  }
  void set_c5_sensor(sensor::Sensor *s) {
    c5_sensor_ = s;
  }
  void set_c6_sensor(sensor::Sensor *s) {
    c6_sensor_ = s;
  }
  void set_c7_sensor(sensor::Sensor *s) {
    c7_sensor_ = s;
  }
  void set_c8_sensor(sensor::Sensor *s) {
    c8_sensor_ = s;
  }
  void set_c9_sensor(sensor::Sensor *s) {
    c9_sensor_ = s;
  }
}
  void set_charging_mode_text_sensor(text_sensor::TextSensor *s) {
    charging_mode_text_sensor_ = s;
  }

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const { return setup_priority::DATA; }

 protected:
  void handle_value_();

  // TODO: add set_<sensorname>_sensor for:
  // 'channel', 'charging_mode_id', 'timestamp', 'cycle_count', 'step_id', 'current', 'input_voltage', 'battery_voltage', 'capacity_mah', 'int_temp', 'ext_temp', 'c01', 'c02', 'c03', 'c04', 'c05', 'c06', 'c07', 'c08', 'c09', 'c10'
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
