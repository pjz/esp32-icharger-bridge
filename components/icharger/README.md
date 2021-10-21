# iCharger battery charger

A configured uart component is required.

Example:
```yaml
icharger:
  uart_id: the_uart

sensor:
  - platform: icharger
    panel_voltage:
      id: pv
    battery_voltage:
      id: bv
    battery_current:
      id: bc
```

The `uart_id` is optional.

All sensors are optional.

The available numeric sensors are:
- 'channel'
- 'charging_mode_id'
- 'timestamp'
- 'cycle_count'
- 'step_id'
- 'current'
- 'input_voltage'
- 'battery_voltage'
- 'capacity_mah'
- 'int_temp'
- 'ext_temp'
- 'c01'
- 'c02'
- 'c03'
- 'c04'
- 'c05'
- 'c06'
- 'c07'
- 'c08'
- 'c09'
- 'c10'

The available numeric sensors are:
- 'charging_mode_text'

Example for multiple devices:
```yaml
icharger:
  - id: icharger_charger1
    uart_id: the_uart_of_charger1
  - id: icharger_charger2
    uart_id: the_uart_of_charger2

sensor:
  - platform: icharger
    icharger_id: icharger_charger1
    device_type:
      name: "charger on the left"
  - platform: icharger
    icharger_id: icharger_charger2
    device_type:
      name: "charger on the right"
```
