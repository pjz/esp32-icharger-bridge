#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace icharger {

class IChargerComponent : public uart::UARTDevice, public Component {
 public:

  void set_channel_sensor(sensor::Sensor *s) {
    channel_sensor_ = s;
  }

  // TODO: add set_<sensorname>_sensor for:
  // # 'charging_mode', 'timestamp', 'cycle_count', 'step_id', 'current', 'input_voltage', 'battery_voltage', 'capacity_mah', 'int_temp', 'ext_temp', 'c01', 'c02', 'c03', 'c04', 'c05', 'c06', 'c07', 'c08', 'c09', 'c10', 'checksum'


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
  // TODO: add set_<sensorname>_sensor for:
  // # 'charging_mode', 'timestamp', 'cycle_count', 'step_id', 'current', 'input_voltage', 'battery_voltage', 'capacity_mah', 'int_temp', 'ext_temp', 'c01', 'c02', 'c03', 'c04', 'c05', 'c06', 'c07', 'c08', 'c09', 'c10', 'checksum'

  text_sensor::TextSensor *charging_mode_text_sensor_{nullptr};

  int state_{0};
  std::string value_;
  uint32_t last_transmission_{0};
};

}  // namespace icharger
}  // namespace esphome
