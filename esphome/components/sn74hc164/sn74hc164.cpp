#include "sn74hc164.h"
#include "esphome/core/log.h"
#include <bitset>

namespace esphome {
namespace sn74hc164 {

static const char *const TAG = "sn74hc164";

void SN74HC164Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SN74HC164...");
  this->data_pin_->setup();
  this->clock_pin_->setup();
  this->data_pin_->digital_write(false);
  this->clock_pin_->digital_write(false);
}

float SN74HC164Component::get_setup_priority() const {
  return setup_priority::IO;
}

void SN74HC164Component::dump_config() {
  ESP_LOGCONFIG(TAG, "SN74HC164:");
}

void SN74HC164Component::shift_out(uint8_t value) {
  for (int i = 7; i >= 0; i--) {
    bool bit = (value >> i) & 1;
    this->data_pin_->digital_write(bit);
    this->clock_pin_->digital_write(true);
    this->clock_pin_->digital_write(false);
  }
}

void SN74HC164Component::set_output_state(uint8_t pin, bool state) {
  if (pin >= 8) {
    ESP_LOGE(TAG, "Invalid pin: %d", pin);
    return;
  }
  this->current_state_[pin] = state;
  this->shift_out(static_cast<uint8_t>(this->current_state_.to_ulong()));
}

void SN74HC164Component::loop() {
  // Continuously write the current state to maintain output stability
  this->shift_out(static_cast<uint8_t>(this->current_state_.to_ulong()));
}

}  // namespace sn74hc164
}  // namespace esphome