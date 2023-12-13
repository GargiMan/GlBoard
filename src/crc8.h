/**
 * crc8.h
 *
 *  Created on: 12 dec 2023
 *      Author: Marek Gergel (xgerge01)
 */

#ifndef CRC8_H_
#define CRC8_H_

/**
 * Calculate CRC8 checksum
 * @param buf buffer
 * @param len buffer length
 * @return checksum
 */
unsigned char crc8(unsigned char *buf, unsigned int len);

#endif // CRC8_H_
