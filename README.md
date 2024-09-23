# AirQuality-Model1

## Required BME680 library for Arduino IDE:
https://github.com/Seeed-Studio/Seeed_Arduino_BME68x

In Arduino IDE, please use "Add .ZIP Library" in "Include Library" of "Sketch" to add BME680 .ZIP library.

Also, the "Bridge" library is required which can be installed from "Manage Libraries..." (Library Manager).

## Information of AirQuality-Model1 in IoTtalk is

```
dm_name = 'Model1'

idf_list = [
    ('AtPressure', float),
    ('CO2', float),
    ('Gas', float),
    ('Humidity', float),
    ('Temperature', float),
    ]
```

If you want to change IDF name, please remember to modify the Bridge Variables in Aduino code as well.
