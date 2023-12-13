# GlBoard

Electric longboard program for ESP32 that exchange communication data between VESC and remote Bluetooth device

Link for ESP32 library: https://github.com/espressif/arduino-esp32

# Communication protocol

| CRC    | Flags  | Payload Length | Payload |
|--------|--------|----------------|---------|
| 1 byte | 1 byte | 1 byte         | n bytes |

## CRC

CRC byte of message is CRC8 checksum for data transfer error detection.

## Flags

0-1 bit - Message type  
2-3 bit - Source node  
4-6 bit - Command  
7 bit - Reserved  

### Message type

| Value | Type     |
|-------|----------|
| 00    | Request  |
| 01    | Response |

### Source nodes

| Value | Type   | Description |
|-------|--------|-------------|
| 00    | Master | Board       |
| 01    | Slave  | Remote      |

### Commands

| Value | Command   | Description     |
|-------|-----------|-----------------|
| 000   | Test      | Connection test |
| 001   | Metrics   | Metrics data    |
| 010   | Control   | Control data    |
| 011   | Settings  | Settings data   |
