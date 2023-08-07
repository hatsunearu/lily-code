# Lily STM32-Pi Serial API

Documentation on the STM-Pi serial communication

## Packet Definition

No payload packet
```
 0   1   2 
[S] [A] [C]
```
Payload packet
```
 0   1   2    3   ...  N
[S] [A] [P0] [P1] ... [C]
```

- `[S]` - Sync byte. Always `0x39`
- `[A]` - Address byte
- `[P]` - Payload byte (optional, variable length)
- `[C]` - Parity. All preceeding bytes XORed.

When sent a packet, the response will be another packet with the same address.

## STM Rx, Pi Tx Commands

### Read Voltage - `[0x01]`

#### Payload 

No payload

#### Return payload

Length: 4

- `P0` - Vsns Voltage (mV) (LSB)
- `P1` - Vsns Voltage (mV) (MSB)
- `P2` - Vdda Voltage (mV) (LSB)
- `P3` - Vdda Voltage (mV) (MSB)

### Read Channel State - `[0x2]`

#### Payload

No payload

#### Return payload

- `P0` - Channel 1 state
- `P1` - Channel 2 state
- `P2` - Channel 2 state
- `P3` - Channel 3 state

State is a bitmap with the following structure:

- `b7` (MSB): Reserved
- `b6` : Reserved
- `b5` : Reserved
- `b4` : Reserved
- `b3` : Reserved
- `b2` : Reserved
- `b1` : Overcurrent tripped
- `b0` : Channel on

### Read Current - `[0x03]`

#### Payload 

- `P0` - Channel number (0-3)

#### Return payload

- `P0` - Channel current (mA) (LSB)
- `P1` - Channel current (mA) (MSB)

### Write Channel State - `[0x4]`

#### Payload

- `P0` - Channel 1 state
- `P1` - Channel 2 state
- `P2` - Channel 2 state
- `P3` - Channel 3 state

State is a bitmap with the following structure:

- `b7` (MSB): Reserved
- `b6` : Reserved
- `b5` : Reserved
- `b4` : Reserved
- `b3` : Reserved
- `b2` : Reserved
- `b1` : Reserved
- `b0` : Channel on

#### Return payload

- `P0` - Channel 1 state
- `P1` - Channel 2 state
- `P2` - Channel 2 state
- `P3` - Channel 3 state

State is a bitmap with the following structure:

- `b7` (MSB): Reserved
- `b6` : Reserved
- `b5` : Reserved
- `b4` : Reserved
- `b3` : Reserved
- `b2` : Reserved
- `b1` : Overcurrent tripped
- `b0` : Channel on