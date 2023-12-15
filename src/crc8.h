/**
 * crc8.h
 *
 *  Created on: 12 dec 2023
 *      Author: Marek Gergel
 */

#ifndef CRC8_H_
#define CRC8_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Calculate CRC8 checksum
 * @param buf buffer
 * @param len buffer length
 * @return checksum
 */
unsigned char crc8(unsigned char *buf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif // CRC8_H_
