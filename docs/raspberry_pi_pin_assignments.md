This is mainly a placeholder pin assignments document, until I have time to make a schematic

Physical Pin  | BCM Pin | Usage
===================================
      4       |    -    |   5V power input
      6       |    -    |   GND
      11      |   17    |   ce1 (chip select 1 for aux SPI) -> Mag sensor CS
      12      |   18    |   PWM0  -> main rotor PWM
      13      |   27    |   GPIO INPUT -> Mag sensor DATA RDY pin
      16      |   23    |   GPIO INPUT -> IMU INT1 pin
      17      |   -     |   3V3 OUTPUT (Drive VDD on INS system)
      19      |   10    |   SPI0 MOSI -> IMU SDI
      21      |   9     |   SPI0 MISO -> IMU SDO
      23      |   11    |   SPI0 SCLK -> IMU SCLK
      24      |   8     |   SPI0 CE0  -> IMU CS
      26      |   -     |   GND
      33      |   13    |   PWM1 -> main rotor 2 PWM
      36      |   16    |   GPIO Output -> soft PWM for tail rotor
      37      |   26    |   GPIO Output -> tail rotor Direction

The chips:
==========
- Adafruit Powerboost 1000C
- N-channel power MOSFET x2
- Lithium Ion Polymer Battery - 3.7v 2500mAh
- DRV8835 Dual Motor Driver Carrier
- LSM6DS33 3D Accelerometer and Gyro Carrier with Voltage Regulator
- LIS3MDL 3-Axis Magnetometer Carrier with Voltage Regulator
