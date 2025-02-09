---
layout: docs
title:  "Encodings: Integer"
---

`BOUNDED_MULTIPLE_8BITS_ENUM_FIXED`
-----------------------------------

The encoding consists of the integer value divided by the `multiplier`, minus
the ceil of `minimum` divided by the `multiplier`, encoded as an 8-bit
fixed-length unsigned integer.

### Options

| Option       | Type   | Description                 |
|--------------|--------|-----------------------------|
| `minimum`    | `int`  | The inclusive minimum value |
| `maximum`    | `int`  | The inclusive maximum value |
| `multiplier` | `uint` | The multiplier value        |

### Conditions

| Condition                    | Description                                                         |
|------------------------------|---------------------------------------------------------------------|
| `value >= minimum`           | The input value must be greater than or equal to the minimum        |
| `value <= maximum`           | The input value must be less than or equal to the maximum           |
| `value % multiplier == 0`    | The input value must be divisible by the multiplier                 |
| `floor(maximum / multiplier) - ceil(minimum / multiplier) < 2 ** 8` | The divided range must be representable in 8 bits |

### Examples

Given the input value 15, where the minimum is 1, the maximum is 19, and the
multiplier is 5, the encoding results in the 8-bit unsigned integer 2:

```
+------+
| 0x02 |
+------+
```

`FLOOR_MULTIPLE_ENUM_VARINT`
----------------------------

The encoding consists of the integer value divided by the `multiplier`, minus
the ceil of `minimum` divided by the `multiplier`, encoded as a Base-128 64-bit
Little Endian variable-length unsigned integer.

### Options

| Option       | Type   | Description                 |
|--------------|--------|-----------------------------|
| `minimum`    | `int`  | The inclusive minimum value |
| `multiplier` | `uint` | The multiplier value        |

### Conditions

| Condition                 | Description                                                  |
|---------------------------|--------------------------------------------------------------|
| `value >= minimum`        | The input value must be greater than or equal to the minimum |
| `value % multiplier == 0` | The input value must be divisible by the multiplier          |

### Examples

Given the input value 1000, where the minimum is -2 and the multiplier is 4,
the encoding results in the Base-128 64-bit Little Endian variable-length
unsigned integer 250:

```
+------+------+
| 0xfa | 0x01 |
+------+------+
```

`ROOF_MULTIPLE_MIRROR_ENUM_VARINT`
----------------------------------

The encoding consists of the floor of `maximum` divided by the `multiplier`,
minus the integer value divided by the `multiplier`, encoded as a Base-128
64-bit Little Endian variable-length unsigned integer.

### Options

| Option       | Type   | Description                 |
|--------------|--------|-----------------------------|
| `maximum`    | `int`  | The inclusive maximum value |
| `multiplier` | `uint` | The multiplier value        |

### Conditions

| Condition                 | Description                                               |
|---------------------------|-----------------------------------------------------------|
| `value <= maximum`        | The input value must be less than or equal to the maximum |
| `value % multiplier == 0` | The input value must be divisible by the multiplier       |

### Examples

Given the input value 5, where the maximum is 16 and the multiplier is 5, the
encoding results in the Base-128 64-bit Little Endian variable-length unsigned
integer 2:

```
+------+
| 0x02 |
+------+
```

`ARBITRARY_MULTIPLE_ZIGZAG_VARINT`
----------------------------------

The encoding consists of the the integer value divided by the `multiplier`
encoded as a ZigZag-encoded Base-128 64-bit Little Endian variable-length
unsigned integer.


### Options

| Option       | Type   | Description          |
|--------------|--------|----------------------|
| `multiplier` | `uint` | The multiplier value |

### Conditions

| Condition                 | Description                                         |
|---------------------------|-----------------------------------------------------|
| `value % multiplier == 0` | The input value must be divisible by the multiplier |

### Examples

Given the input value 10, where the multiplier is 5, the encoding results in
the Base-128 64-bit Little Endian variable-length unsigned integer 4:

```
+------+
| 0x04 |
+------+
```
