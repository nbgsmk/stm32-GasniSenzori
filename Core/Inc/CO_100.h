/*
 * CO.h
 *
 *  Created on: May 27, 2024
 *      Author: peca
 */

#ifndef CO_100_H_
#define CO_100_H_

	////////////////////////////////
	// Transmission mode switching
	////////////////////////////////

	//	COMMAND 1
	//	Switches to active upload
	const uint8_t to_active_mode[] = {
			// Start bit | Retain | Switch command | Active upload | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |       2        |       3       |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,       0x78,            0x40,       0x00,    0x00,     0x00,    0x00,    0x47
	};


	//	COMMAND 1
	//	Switches to passive upload
	const uint8_t to_passive_mode[] = {
			// Start bit | Retain | Switch command | Answer | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |       2        |    3   |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,       0x78,         0x41,   0x00,    0x00,     0x00,    0x00,    0x46
	};



	////////////////////////////////
	// Commands in query mode
	////////////////////////////////

	//	COMMAND 3
	//	Get the sensor type, maximum range, unit, and decimal places: 0xD1
	const uint8_t get_type_range_unit_decimals_0xD1[] = {
			0xD1
	};
	// Return value:	(valjda su to vredosti za primer - prim.prev)
	//	0x19	0	Sensor type
	//	0x00	1	Maximum range high
	//	0xC8	2	Maximum range low
	//	0x02	3	Unit
	//	0x00	4	Retain
	//	0x00	5	Retain
	//	0x00	6	Retain
	//	0x01	7	Number of decimal places(bit[4]~bit[7] | Data sign (bit[0]~bit[3])
	//	0x35	8	Parity bit
	//
	//	NOTE:
	//	Max range = (Max range high << 8) | Max range low
	//	Unit: 0x02 -> (ppm and mg/m3) or 0x04 (ppb and ug/m3)
	//	Signs: 0 (positive) or 1 (negative)
	//	Decimal places: the maximum number of decimal places is 3 (three prim. prev.)


	//	COMMAND 4
	//	Get the sensor type, maximum range, unit, and decimal places: 0xD7
	const uint8_t get_type_range_unit_decimals_0xD7[] = {
			0xD7
	};
	// Return value (ne pise ali valjda je to return value i valjda su ovo vrednosti kao za primer - prim. prev.)
	//	0xFF	0	Command header 1
	//	0xD7	1	Command header 2
	//	0x19	2	Sensor type
	//	0x00	3	Maximum range high
	//	0xC8	4	Maximum range low
	//	0x02	5	Unit
	//	0x01	6	Number of decimal places(bit[4]~bit[7] | Data sign (bit[0]~bit[3]
	//	0x00	7	Retain
	//	0x45	8	Parity bit
	//
	//	NOTE:
	//	Checksum: Add 1 ~ 7 to generate an 8-bit data, invert each bit, add 1 at the end
	//
	//	Decimal places bit[4] ~ bit[7]:
	//		(bit[7] << 3) | (bit[6] << 2) | (bit[5] << 1) | bit[4] = number of decimal places
	//
	//	Data sign (bit[0]~bit[3]):
	//		(bit[3]<<3) | (bit[2]<<2) | (bit[1]<<1) | bit[0] = 0 Negative inhibition
	//		(bit[3]<<3) | (bit[2]<<2) | (bit[1]<<1) | bit[0] = 1 Positive inhibition
	//
	//	Unit :
	//		0x02: unit is mg/m3 and ppm
	//		0x04: unit is um/m3 and ppb
	//		0x08: unit is 10g/m3 and %


	//	COMMAND 5
	//	Actively reading the gas concentration
	const uint8_t read_gas_concentration[] = {
			// Start bit | Retain | Command | Retain | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |    2    |    3   |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,    0x86,     0x00,   0x00,    0x00,     0x00,    0x00,    0x79
	};
	// RETURN VALUE:
	//
	//	0xFF	0	Start bit
	//	0x86	1	Command
	//	0x00	2	High gas concentration (ug/m3)
	//	0x2A	3	Low gas concentration (ug/m3)
	//	0x00	4	Full range high
	//	0x00	5	Full range low
	//	0x00	6	High gas concentration (ppb)
	//	0x20	7	Low gas concentration (ppb)
	//	0x30	8	Checksum
	// NOTE:
	// Checksum: Add 1 ~ 7 digits of data to generate an 8-bit data, invert each bit, add 1 at the end
	// Gas concentration = gas concentration high bit * 256 + gas concentration bit (valjda misli low bit - prim.prev.)
	// (The high and low concentrations need to be converted from hexadecimal to decimal and then brought into this formula to calculate)



	//	COMMAND 6
	//	Combined reading command of gas concentration, temperature, humidity
	const uint8_t read_gas_concentration_temp_and_hmidity[] = {
			// Start bit | Retain | Command | Retain | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |    2    |    3   |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,    0x87,     0x00,   0x00,    0x00,     0x00,    0x00,    0x78
	};
	// RETURN VALUE:
	//
	//	0xFF	0	Start bit
	//	0x87	1	Command
	//	0x00	2	High gas concentration (ug/m3)
	//	0x2A	3	Low gas concentration (ug/m3)
	//	0x03	4	Full range high
	//	0xE8	5	Full range low
	//	0x00	6	High gas concentration (ppb)
	//	0x20	7	Low gas concentration (ppb)
	//	0x09	8	Temperature high
	//	0xC4	9	Temperature low
	//	0x13	10	Humidity high
	//	0x88	11	Humidity low
	//	0xDC	12	Parity bit (valjda bajt tupsoni! - prim. prev)
	//
	// NOTE:
	// Checksum: 1 ~ 11 bits of data are added to generate an 8-bit data, each bit is inverted, and 1 is added at the end
	//
	// Gas concentration = gas concentration high bit * 256 + gas concentration (valjda -low- prim. prev.)
	//
	// Temperature is signed data with two decimal places, the unit is (℃ -Celsius). Pseudo code calculation formula:
	// T = (float) ((int)((0x0A<<8) | 0x09)) / 100
	//
	// Humidity is data without sign and two decimal places, the unit is (rh%). Pseudo code calculation formula:
	// Rh = (float) ( (uint)((0x0A<<8) | 0x09)) / 100


	//	COMMAND 7
	//	Get the current temperature and humidity
//	const uint8_t read_temp_and_hmidity[] = {
//			// STOPSHIP	- PA GDE JE KOMANDA???
//	};
	// RETURN VALUE:
	//
	//	0x0A	0	Temerature high 8 bit
	//	0x09	1	Temerature low 8 bit
	//	0x11	2	Humidity high 8 bit
	//	0xF4	3	Humidity low 8 bit
	//
	// NOTE:
	// Temperature is signed data with two decimal places, the unit is (℃ -Celsius). Pseudo code calculation formula:
	// T = (float) ( (int)((0x0A<<8) | 0x09)) / 100
	//
	// Humidity is data without sign and two decimal places, the unit is (rh%). Pseudo code calculation formula:
	// Rh = (float) ( (uint)((0x0A<<8) | 0x09)) / 100


	//	COMMAND 8
	//	Get the current temperature and humidity with calibration
//	const uint8_t read_temp_and_hmidity[] = {
//			 STOPSHIP	- PA GDE JE KOMANDA???
//	};
	// RETURN VALUE:
	//
	//	0x0A	0	Temerature high 8 bit
	//	0x09	1	Temerature low 8 bit
	//	0x11	2	Humidity high 8 bit
	//	0xF4	3	Humidity low 8 bit
	//	0xEB	3	Checksum
	//
	// NOTE:
	// Checksum: 0 ~ 3 bits of data are added to generate an 8-bit data. Each bit is inverted, plus 1 at the end
	// Temperature is signed data with two decimal places, the unit is (℃ -Celsius). Pseudo code calculation formula:
	// T = (float) ( (int)((0x0A<<8) | 0x09)) / 100
	//
	// Humidity is data without sign and two decimal places, the unit is (rh%). Pseudo code calculation formula:
	// Rh = (float) ( (uint)((0x0A<<8) | 0x09)) / 100


	//	COMMAND 9
	//	Get the current version number
	//	const uint8_t read_temp_and_hmidity[] = {
	//			 STOPSHIP	- PA GDE JE KOMANDA???
	//	};
	// RETURN VALUE:
	//
	//	0x19	0
	//	0x05	1
	//	0x27	2
	//	0x00	3
	//	0x10	4
	//	0x01	5
	// (Ne pise sta su, pretpostavljam da ih treba concatenate kao stringove i to je verzija - prim. prev.)




	// DATA ACTIVE UPLOAD MODE FORMAT (zasto se ovo ponavlja kad gore vec pise?? - prim. prev.)
	//
	//	0xFF	0	Start bit
	//	0x86	1	Command
	//	0x00	2	High gas concentration (ug/m3)
	//	0x2A	3	Low gas concentration (ug/m3)
	//	0x00	4	Full range high
	//	0x00	5	Full range low
	//	0x00	6	High gas concentration (ppb)
	//	0x20	7	Low gas concentration (ppb)
	//	0x30	8	Checksum
	//
	// NOTE:
	// Checksum: 1 ~ 11 bits of data are added to generate an 8-bit data, each bit is inverted, and 1 is added at the end.
	// Gas concentration = gas concentration high bit * 256 + gas concentration bit	(valjda misli na low bit - prim. prev.)
	// (The high and low concentrations need to be converted from hexadecimal to decimal and then brought into this formula to calculate)



	// LOW POWER SWITCHING
	// FIXME	- treba da implementiram celu ovu sekciju
	//
	// Enter sleep mode
	//
	// Exit sleep mode
	// NOTE
	// After exiting sleep mode, it takes 5 seconds to recover, no data within 5 sec.



	// RUNNING LIGHTS ON SENSOR MODULE  (led koji trepce dok senzor radi - prim. prev.)

	// LED OFF
	// Turn off the running light
	const uint8_t running_light_off[] = {
			// Start bit | Retain | Command | Retain | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |    2    |    3   |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,    0x88,     0x00,   0x00,    0x00,     0x00,    0x00,    0x77
	};
	// RETURN VALUE:
	//
	//	0x4F	0
	//	0x4B	1


	// LED ON
	// Turn on the running light (led koji trepce dok senzor radi - prim. prev.)
	const uint8_t running_light_on[] = {
			// Start bit | Retain | Command | Retain | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |    2    |    3   |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,    0x89,     0x00,   0x00,    0x00,     0x00,    0x00,    0x76
	};
	// RETURN VALUE:
	//
	//	0x4F	0
	//	0x4B	1


	// LED STATUS
	// Query the running light status
	const uint8_t running_light_get_status[] = {
			// Start bit | Retain | Command | Retain | Retain | Retain | Retain | Retain | Checksum
			//    0      |    1   |    2    |    3   |   4    |   5    |    6   |    7   |    8
			     0xFF,       0x01,    0x8A,     0x00,   0x00,    0x00,     0x00,    0x00,    0x75
	};
	// RETURN VALUE:
	//
	//	0xFF	0	Start bit
	//	0x8A	1	Command
	//	0x01	2	State value
	//	0x00	3	Retain
	//	0x00	4	Retain
	//	0x00	5	Retain
	//	0x00	6	Retain
	//	0x00	7	Retain
	//	0x75	8	Checksum
	//
	// NOTE
	// Status value 1 (light on), 0 (light off


#endif /* CO_100_H_ */
