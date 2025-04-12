#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/components/output/binary_output.h"

#include <vector>
#include <bitset>

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
  void set_output_state(uint8_t pin, bool state);

 private:
  GPIOPin *data_pin_;
  GPIOPin *clock_pin_;
  std::bitset<8> current_state_;
};

class SN74HC164BinaryOutput : public esphome::output::BinaryOutput {
 public:
  SN74HC164BinaryOutput(SN74HC164Component *parent, uint8_t pin) : parent_(parent), pin_(pin) {}

  void write_state(bool state) override {
    this->parent_->set_output_state(this->pin_, state);
  }

 private:
  SN74HC164Component *parent_;
  uint8_t pin_;
};

}  // namespace sn74hc164
}  // namespace esphome