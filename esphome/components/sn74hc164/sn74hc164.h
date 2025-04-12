#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

#include <vector>

namespace esphome {
namespace sn74hc164 {

class SN74HC164Component : public Component {
 public:
  SN74HC164Component() = default;

  void setup() override;
  float get_setup_priority() const override;
  void dump_config() override;

  void set_data_pin(GPIOPin *pin) { this->data_pin_ = pin; }
  void set_clock_pin(GPIOPin *pin) { this->clock_pin_ = pin; }

  void shift_out(uint8_t value);

 protected:
  GPIOPin *data_pin_;
  GPIOPin *clock_pin_;
};

}  // namespace sn74hc164
}  // namespace esphome