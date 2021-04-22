/*
  Streaming data from Bluetooth to internal DAC of ESP32
  
  Copyright (C) 2020 Phil Schatzmann
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "BluetoothA2DPSink.h"
#include <Sparkfun_DRV2605L.h>
#include <Wire.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//#include <Adafruit_DRV2605.h>
//
//#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library

#define I2C_SDA 18
#define I2C_SCL 19


BluetoothA2DPSink a2dp_sink;
SFE_HMD_DRV2605L HMD;
TwoWire I2DRV2605 = TwoWire(0);

void setup() {
//  Wire.begin(18,19);
I2DRV2605.begin(I2C_SDA, I2C_SCL, 400000);
  
  HMD.begin(&I2DRV2605);
  Serial.begin(9600);
  Serial.print("Audio to Vibe");
  HMD.Mode(0x04); //Audio INPUT 
  HMD.cntrl1(0xB3); // Set the AC Couple bit in Control 1
  HMD.cntrl3(0x62); // Set the Analog bit in Control 3
  HMD.MotorSelect(0x36);
  HMD.Library(2); //change to 6 for LRA motors 
//  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  static const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
      .sample_rate = 44100, // corrected by info from bluetooth
      .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false
      
  };

  a2dp_sink.set_i2s_config(i2s_config);  
  a2dp_sink.start("MyMusic");  

  

}


void loop() {
 
}
