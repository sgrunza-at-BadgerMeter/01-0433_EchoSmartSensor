# 01-0433_EchoSmartSensor

Firmware for EchoSmart / FilterSmart Sensor version 2 using an STM32H523 microcontroller

## ToDo Issues

1. Check Flash storage method for endurance issues.  Should probably implement storing multiple copies of the configuration
and only using the last valid configuration.  If a new configuration is needed, place it after the current unless there is
not enough room.  If there is not enough room, then erase the configuration sector.  This will help to wear-level the configuration
sector

2. Implement Receive Time Out in Modbus handling instead of using a hardware timer.

