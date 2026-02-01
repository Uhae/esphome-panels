#pragma once
#define LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  static constexpr int I2C_PORT_NUM = I2C_NUM_0;
  static constexpr int I2C_PIN_SDA  = 38;
  static constexpr int I2C_PIN_SCL  = 39;
  static constexpr int I2C_PIN_INT  = 40;

  lgfx::Bus_Parallel16 _bus;
  lgfx::Panel_ILI9488  _panel;
  lgfx::Light_PWM      _light;
  lgfx::ITouch*        _touch = nullptr;

  bool init_impl(bool use_reset, bool use_clear) override {
    if (_touch == nullptr) {
      lgfx::i2c::init(I2C_PORT_NUM, I2C_PIN_SDA, I2C_PIN_SCL);

      // FT5x06/FT6236 @ 0x38 (bei dir im I2C-Scan bestätigt)
      if (lgfx::i2c::beginTransaction(I2C_PORT_NUM, 0x38, 400000, false).has_value()
       && lgfx::i2c::endTransaction(I2C_PORT_NUM).has_value()) {
        _touch = new lgfx::Touch_FT5x06();
        auto cfg = _touch->config();
        cfg.i2c_port = I2C_PORT_NUM;
        cfg.i2c_addr = 0x38;
        cfg.pin_sda  = I2C_PIN_SDA;
        cfg.pin_scl  = I2C_PIN_SCL;
        cfg.pin_int  = I2C_PIN_INT;
        cfg.freq     = 400000;
        cfg.x_max    = 320;
        cfg.y_max    = 480;
        cfg.bus_shared = false;
        _touch->config(cfg);
        _panel.touch(_touch);
      }
    }
    return lgfx::LGFX_Device::init_impl(use_reset, use_clear);
  }

public:
  LGFX() {
    // 16-bit i8080 parallel bus
    {
      auto cfg = _bus.config();

      // Makerfabs nutzt 40MHz; falls instabil später 20MHz/10MHz testen
      cfg.freq_write = 40000000;

      cfg.pin_wr = 35;
      cfg.pin_rd = 48;
      cfg.pin_rs = 36;

      cfg.pin_d0  = 47;
      cfg.pin_d1  = 21;
      cfg.pin_d2  = 14;
      cfg.pin_d3  = 13;
      cfg.pin_d4  = 12;
      cfg.pin_d5  = 11;
      cfg.pin_d6  = 10;
      cfg.pin_d7  = 9;
      cfg.pin_d8  = 3;
      cfg.pin_d9  = 8;
      cfg.pin_d10 = 16;
      cfg.pin_d11 = 15;
      cfg.pin_d12 = 7;
      cfg.pin_d13 = 6;
      cfg.pin_d14 = 5;
      cfg.pin_d15 = 4;

      _bus.config(cfg);
      _panel.bus(&_bus);
    }

    // Panel config
    {
      auto cfg = _panel.config();
      cfg.pin_cs          = 37;
      cfg.pin_rst         = -1;
      cfg.pin_busy        = -1;
      cfg.offset_rotation = 0;
      cfg.readable        = true;
      cfg.invert          = false;
      cfg.rgb_order       = false;
      cfg.dlen_16bit      = true;
      cfg.bus_shared      = false;
      _panel.config(cfg);
    }

    // Backlight (bei dir verifiziert: GPIO45)
    {
      auto cfg = _light.config();
      cfg.pin_bl = 45;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;
      _light.config(cfg);
      _panel.light(&_light);
    }

    setPanel(&_panel);
  }
};
