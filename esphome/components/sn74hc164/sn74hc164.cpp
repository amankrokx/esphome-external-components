#include "sn74hc164.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace sn74hc164
  {

    static const char *const TAG = "sn74hc164";

    void SN74HC164Component::setup()
    {
      ESP_LOGCONFIG(TAG, "Setting up SN74HC164...");

      this->clock_pin_->setup();
      this->a_pin_->setup();
      this->b_pin_->setup();
      this->reset_pin_->setup();

      // Initialize pins to known state
      this->clock_pin_->digital_write(false);
      this->a_pin_->digital_write(false);
      this->b_pin_->digital_write(false);

      // Reset the shift register
      this->reset_pin_->digital_write(false);
      delayMicroseconds(1);
      this->reset_pin_->digital_write(true);
    }

    void SN74HC164Component::dump_config()
    {
      ESP_LOGCONFIG(TAG, "SN74HC164:");
      ESP_LOGCONFIG(TAG, "  Shift registers: %d", this->sr_count_);
    }

    void SN74HC164Component::digital_write(uint16_t pin, bool value)
    {
      if (pin >= this->sr_count_ * 8)
      {
        ESP_LOGE(TAG, "Pin %u is out of range! Maximum pin: %u", pin, (this->sr_count_ * 8) - 1);
        return;
      }

      const uint8_t reg = pin / 8;
      const uint8_t bit = pin % 8;
      if (value)
      {
        this->output_bytes_[reg] |= (1 << bit);
      }
      else
      {
        this->output_bytes_[reg] &= ~(1 << bit);
      }
    }

    void SN74HC164Component::write_gpio()
    {
      // Shift out data in reverse order for chaining (last chip first)
      for (int reg = this->sr_count_ - 1; reg >= 0; reg--)
      {
        uint8_t data = this->output_bytes_[reg];

        for (int8_t bit = 7; bit >= 0; bit--)
        {
          bool state = (data >> bit) & 0x01;

          // Set ANDed inputs (both must be high for 1)
          this->a_pin_->digital_write(state);
          this->b_pin_->digital_write(state);

          // Clock pulse
          this->clock_pin_->digital_write(true);
          delayMicroseconds(1);
          this->clock_pin_->digital_write(false);
        }
      }
    }

    void SN74HC164GPIOPin::digital_write(bool value)
    {
      this->parent_->digital_write(this->pin_, value != this->inverted_);
      this->parent_->write_gpio();
    }

    std::string SN74HC164GPIOPin::dump_summary() const
    {
      return str_snprintf("%u via SN74HC164", 18, pin_);
    }

  } // namespace sn74hc164
} // namespace esphome