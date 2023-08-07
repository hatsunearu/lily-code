# Lily STM32-Pi Serial API

Documentation on the STM-Pi serial communication

## Packet Definition

```
 0   1   2 
[S] [A] [C]
```

```
 0   1   2   3  ...  N
[S] [A] [P] [P] ... [C]
```

- `[S]` - Sync byte. Always `0x39`
- `[A]` - Address byte
- `[P]` - Payload byte (optional, variable length)
- `[C]` - Parity. All preceeding bytes XORed.

## STM Commands

### Read Voltage `[0x01]`

