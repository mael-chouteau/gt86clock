# Scion FRS / Subaru BRZ / Toyota GT86 Enhanced Clock

## About
GT86clock is a display, which replaces the stock clock of an Scion FRS / Subaru BRZ / Toyota GT86.

It shows the:
* time
* oil tempurature
* coolant tempurature
* oil pressure
* voltage
* Lambda/AFR
while looking completey stock at the first glance.

There is also a web server running, where you can get a view trending data of the last couple of hours the engine ran.

## Disclaimer & Background
I'm not a car mechanic nor a software developer. I bought my GT86 with a broken engine and replaced it with a new to me one. As many other Subaru engines the con rod bearings disintegrate from the lack of lubrication.
Oil temperature and oil pressure seems to be decent indications of the engine health, but unfortunately in stock form there is no way to keep an eye on those metrics. That is how this project started.

## Demo
![Demo](../assets/assets/gt86clock_demo.gif)

## Credits
Somebody else had the *ingenious* idea to reuse the clock location. Kudos to her/him!
There are few similar projects out there. I only looked at them very briefly, I enjoyed the challenge to build my own version.

## Picture
My test rig so I test the code outside of the car.
![testrig](../assets/assets/gt86clock_testrig.jpg)

Start up logo: It does not have any function. It just looks cool.
![startup](../assets/assets/gt86clock_startup.jpg)

Clock with small logo
![time](../assets/assets/gt86clock_time.jpg)

One of the data views:
![temp](../assets/assets/gt86clock_temp.jpg)

## How does it work
An ESP8266 is reading the following data:
- oil temp, water temp, volts and AFR via a CAN bus module connected to the OBD2 port
- oil pressure using a pressure sensor replacing the OEM oil pressure switch
- a real time clock module to keep track of time
- 3 micro switches (due to the lack of input pins on the ESP they are connected to only 2 input pins using two diodes)

What used to be AM/PM button is changing the views new. A long press is either changing the units or in case of the clock view it fetches the current time via NTP. The "H" and "M" buttons are remaining as before.

## Views
1. logo: useless start up screen
2. clock: time + small logo
3. oil temperature: single display, value read from CAN
4. water temperature: single display, value read from CAN
5. oil+water temperature: dual display, value read from CAN
6. oil pressure: single display, value read from pressure sensor connected to the analog input pin
7. AFR+voltage: dual display, value read from CAN (WiP)
8. settings 1: Wifi details: SSID + current IP
9. settings 2: date + time formar (12h/24h)
10. settings 3: temperature unit (°C/°F) and pressure unit (bar/psi)
11. settings 4: O2 unit (lambda/AFR)

## Hardware
* NodeMCU ESP8266 ESP-12E board
* SSD1306 OLED screen, 128x32 pixel, I²C interface
* RTC module, DS3231 chip, I²C interface
* CAN bus module, MCP2515 chip, SPI interface
* slim OBD2 female connector
* 1/8 NPT oil pressure sensor, 3 wire, 150psi
* 2x diodes
* optional: 3x micro switches or reuse the OEM ones

## Libraries
* ESP8266, version 2.5.0 tested
* U8G2, version 2.24.3 tested
* MCP_CAN, version 1.5 tested
* DS3231, version 1.0.2 tested
* RingBuf, version 1.0.3 tested
* NTPtimeESP, version 1.0 tested
* SPI
* EEPROM
* SPIFFS
* WiFi
* WebServer
* WiFiManager
* WebServer
* FS

## Pinout

| Function                | ESP8266 Pin | Module/Connection         |
|-------------------------|-------------|--------------------------|
| OLED SDA                | D2 (GPIO4)  | SSD1306 OLED (I2C)       |
| OLED SCL                | D1 (GPIO5)  | SSD1306 OLED (I2C)       |
| RTC SDA                 | D2 (GPIO4)  | DS3231 RTC (I2C shared)  |
| RTC SCL                 | D1 (GPIO5)  | DS3231 RTC (I2C shared)  |
| CAN CS                  | D8 (GPIO15) | MCP2515 CAN (SPI)        |
| CAN INT                 | D0 (GPIO16) | MCP2515 CAN (Interrupt)  |
| CAN MOSI                | D7 (GPIO13) | MCP2515 CAN (SPI)        |
| CAN MISO                | D6 (GPIO12) | MCP2515 CAN (SPI)        |
| CAN SCK                 | D5 (GPIO14) | MCP2515 CAN (SPI)        |
| Oil Pressure Sensor     | A0          | Analog Input             |
| Button 1 (MUX)          | D4 (GPIO2)  | Button/Diode Matrix      |
| Button 2 (MUX)          | D3 (GPIO0)  | Button/Diode Matrix      |
| 3.3V / GND              | 3.3V / GND  | All modules              |

## Button Wiring and Diode Multiplexing

The project uses 3 buttons (AM/PM, H, M) but only 2 GPIO pins on the ESP8266, thanks to a diode-multiplexing technique:

- **Button 1 (AM/PM):** Connects between D4 (GPIO2) and GND, with a diode in series (anode to D4, cathode to button, then to GND).
- **Button 2 (H):** Connects between D3 (GPIO0) and GND, with a diode in series (anode to D3, cathode to button, then to GND).
- **Button 3 (M):** Connects between both D3 and D4 (GPIO0 and GPIO2) through two diodes (anode to each GPIO, cathodes joined together, then to button, then to GND).

**Why use diodes?**
The diodes prevent current from flowing back into the other GPIO pin, allowing the ESP8266 to detect which button is pressed even though two buttons share the same pins.

**Wiring summary:**
- Use 1N4148 or similar small-signal diodes.
- Each button gets its own diode(s) as described above.
- Connect the other side of each button to GND.

## Oil Pressure Sensor Connection

The oil pressure sensor outputs an analog voltage (0.5-4.5V) proportional to pressure, and is powered by 5-16V DC. Specs:
- Output: 0.5-4.5V
- Supply: 5-16V DC
- Max current: 3mA
- Output is ratiometric (analog)

**Can you connect it directly to the ESP8266?**
- The ESP8266 analog input (A0) accepts 0-1V by default. If you connect the sensor directly, you risk damaging the ESP8266!
- **You must use a voltage divider** to scale the 0.5-4.5V output down to 0-1V for the ESP8266 A0 pin.
- Example: Use a resistor divider (e.g., 180kΩ from sensor output to A0, 47kΩ from A0 to GND) to scale the voltage safely.
- Alternatively, use an external ADC or an op-amp buffer if you want more accuracy.

**Wiring summary:**
- Sensor power: Connect sensor V+ to 5V, GND to GND.
- Sensor output: Connect to a voltage divider, then to A0.
- Do not connect the sensor output directly to A0 without scaling!

### Voltage Divider for Oil Pressure Sensor

To safely connect the sensor's 0.5–4.5V output to the ESP8266's 0–1V analog input, use a voltage divider:

```
Sensor Output (0.5–4.5V)
   |
   R1 (e.g., 180kΩ)
   |
   +----> To ESP8266 A0 (reads 0–1V)
   |
   R2 (e.g., 47kΩ)
   |
  GND
```

**How to calculate resistor values:**
- The voltage at A0 is:  
  `V_A0 = V_sensor * (R2 / (R1 + R2))`
- For 0.5–4.5V in, you want 0–1V out:
  - Example: R1 = 180kΩ, R2 = 47kΩ
  - At 4.5V: 4.5V × (47k / (180k + 47k)) ≈ 0.96V
  - At 0.5V: 0.5V × (47k / (180k + 47k)) ≈ 0.11V
- This keeps the input safely within the ESP8266's range.
- You can adjust R1 and R2 to fine-tune the range if needed.

**Tip:** Use 1% tolerance resistors for best accuracy.

## Calibrating and Reading the Oil Pressure Sensor

### Reading the Sensor Value
- The sensor output (after the voltage divider) connects to the ESP8266 A0 pin.
- In code, use `analogRead(A0)` to get a value from 0 to 1023 (corresponding to 0–1V at A0).

**Example:**
```cpp
int raw = analogRead(A0); // 0–1023
float vA0 = raw * (1.0 / 1023.0); // 0–1V at A0
```

### Calculating the Actual Pressure
1. **Reverse the voltage divider:**
   - If you used R1 = 180kΩ and R2 = 47kΩ:
   ```cpp
   float vSensor = vA0 * ((180000.0 + 47000.0) / 47000.0); // ≈ vA0 * 4.83
   ```
2. **Convert voltage to pressure:**
   - For a sensor with 0.5V = 0 PSI, 4.5V = 150 PSI:
   ```cpp
   float pressurePSI = (vSensor - 0.5) * (150.0 / 4.0); // 0.5V offset, 4V span
   if (pressurePSI < 0) pressurePSI = 0; // Clamp to zero if negative
   ```
   - For bar: `float pressureBar = (vSensor - 0.5) * (10.0 / 4.0);`

### Calibration Steps
- **Zero calibration:** With the engine off (0 pressure), note the raw value. If it's not exactly 0.5V (after reversing the divider), adjust your code to subtract the actual zero voltage.
- **Span calibration:** If you have a known pressure (e.g., using a test gauge), compare the calculated value to the real value and adjust the scaling factor if needed.
- **Averaging:** For more stable readings, average several samples.

### Summary Table
| Step                | Formula/Action                                                                 |
|---------------------|-------------------------------------------------------------------------------|
| Read ADC            | `raw = analogRead(A0);`                                                       |
| A0 Voltage          | `vA0 = raw * (1.0 / 1023.0);`                                                 |
| Sensor Voltage      | `vSensor = vA0 * ((R1 + R2) / R2);`                                           |
| Pressure (PSI)      | `pressure = (vSensor - 0.5) * (150.0 / 4.0);`                                 |
| Pressure (bar)      | `pressure = (vSensor - 0.5) * (10.0 / 4.0);`                                  |

**Tip:** Use a multimeter to measure the actual voltages for best calibration. Adjust the code if you notice a consistent offset or scaling error.

For a more detailed schematic, see the code comments and hardware datasheets.
