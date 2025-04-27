#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include <vector>

namespace esphome
{
  namespace sn74hc164
  {

    class SN74HC164Component : public Component
    {
    public:
      void set_clock_pin(GPIOPin *pin) { clock_pin_ = pin; }
      void set_a_pin(GPIOPin *pin) { a_pin_ = pin; }
      void set_b_pin(GPIOPin *pin) { b_pin_ = pin; }
      void set_reset_pin(GPIOPin *pin) { reset_pin_ = pin; }
      void set_sr_count(uint8_t count)
      {
        sr_count_ = count;
        output_bytes_.resize(count);
      }

      float get_setup_priority() const override { return setup_priority::IO; }
      void setup() override;
      void dump_config() override;

      void digital_write(uint16_t pin, bool value);
      void write_gpio();

    protected:
      GPIOPin *clock_pin_;
      GPIOPin *a_pin_;
      GPIOPin *b_pin_;
      GPIOPin *reset_pin_;
      uint8_t sr_count_;
      std::vector<uint8_t> output_bytes_;
    };

    class SN74HC164GPIOPin : public GPIOPin, public Parented<SN74HC164Component>
    {
    public:
      void setup() override {}
      void pin_mode(gpio::Flags flags) override {}
      bool digital_read() override { return false; }
      void digital_write(bool value) override;
      std::string dump_summary() const override;

      void set_pin(uint16_t pin) { pin_ = pin; }
      void set_inverted(bool inverted) { inverted_ = inverted; }

      gpio::Flags get_flags() const override { return gpio::Flags::FLAG_OUTPUT; }

    protected:
      uint16_t pin_;
      bool inverted_;
    };

  } // namespace sn74hc164
} // namespace esphome