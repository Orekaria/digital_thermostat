/// <summary>Activate debug mode if set to true. Debug mode activates and shows log</summary>
#define DEBUG false
#define SERIAL_SPEED (uint32_t) 9600
#define SPEED_FULL 0X02 // full processor speed (0x00) = 16Mhz. Formula: time * pow(2, (uint8_t)SPEED_)
#define SPEED_REDUCED 0X03 // 0x01 = 16/2 = 8Mhz, 0x02 = 16/4 = 4 Mhz, 0x03 = 16/8 = 2 Mhz
#define MIN_TEMPERATURE (uint16_t) 8 // degress celsius
#define MAX_TEMPERATURE (uint16_t) 28

/* AREF value rather than blindly entering the theoretical 5.0V:
*  Connect the Arduino to the definitive power supply
*  With a voltimeter, measure the voltage between the REF pin and GND
*/
#define AREF (float) 4.975

#define CARRIAGE_RETURN (String) "\n\r"