# eProsima microCDR

<a href="http://www.eprosima.com"><img src="https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcSd0PDlVz1U_7MgdTe0FRIWD0Jc9_YH-gGi0ZpLkr-qgCI6ZEoJZ5GBqQ" align="left" hspace="8" vspace="2" width="100" height="100" ></a>

*eProsima MicroCDR* is a *C* library implementing the *CDR* standard serialization methods.
This library is focused on embedded and resource-limited systems.

*MicroCDR* uses a static buffer, and allow to serialize and deserialize in both, big endianness and little endianness.

## Usage examples
This is a code example showing the serialization and deserialization of a string.
As *MicroCDR* uses a static buffer, that means the user has to provide a defined buffer and its size during the *ucdrBuffer* creation.

```c
    #include <ucdr/microcdr.h>
    #include <stdio.h>

    #define BUFFER_LENGTH 256

    int main(int argc, char** args)
    {
        // Data buffer
        uint8_t buffer[BUFFER_LENGTH];

        // Structs for handle the buffer.
        ucdrBuffer writer;
        ucdrBuffer reader;

        // Initialize the MicroBuffers for working with an user-managed buffer.
        ucdr_init_buffer(&writer, buffer, BUFFER_LENGTH);
        ucdr_init_buffer(&reader, buffer, BUFFER_LENGTH);

        // Serialize data
        char input[16] = "Hello MicroCDR!"; //16 characters
        ucdr_serialize_array_char(&writer, input, 16);

        // Deserialize data
        char output[16];
        ucdr_deserialize_array_char(&reader, output, 16);

        printf("Input: %s\n", input);
        printf("Output: %s\n", output);

        return 0;
    }
```

## API functions

```c
void ucdr_init_buffer        (ucdrBuffer* mb, uint8_t* data, const uint32_t size);
void ucdr_init_buffer_offset (ucdrBuffer* mb, uint8_t* data, const uint32_t size, uint32_t offset);
```
Initialize a `ucdrBuffer` structure, the main struct of *MicroCDR*.
- `mb`: the `ucdrBuffer` struct
- `data`: the buffer that the `ucdrBuffer` will use.
- `size`: the size of the buffer that the `ucdrBuffer` will use.
- `offset`: where the serialization/deserialization will start.
Initially, the serialization/deserialization starts at the beginning of the buffer.

---

```c
void ucdr_copy_buffer (ucdrBuffer* mb_dest, const ucdrBuffer* mb_source);
```
Copy a `ucdrBuffer` structure data to another `ucdrBuffer` structure.
- `mb_dest`: the destination `ucdrBuffer` struct.
- `mb_source`: the origin initialized `ucdrBuffer` struct.

---

```c
void ucdr_reset_buffer       (ucdrBuffer* mb);
void ucdr_reset_buffer_offset(ucdrBuffer* mb, const uint32_t offset);
```
Reset the `ucdrBuffer` as the same state that it was created.
- `mb`: the `ucdrBuffer` struct.
- `offset`: where the serialization/deserialization will start.
Initially, the serialization/deserialization starts at the beginning of the buffer.

---

```c
void ucdr_align_to (ucdrBuffer* mb, const uint32_t alignment);
```
Align the ucdrBuffer to an `alignment` position.
After call this function, the serialization pointer will be moved only if the current `ucdrBuffer` was not aligment to the passed value.

- `mb`: the `ucdrBuffer` struct
- `alignment`: the alignment value used.

---

```c
uint32_t ucdr_aligment(uint32_t buffer_position, const uint32_t data_size);
```
Returns the aligment necessary to serialize/deserialize a type with `data_size` size.

- `buffer_position`: the current serialization/deserialization position of the `ucdrBuffer`. (Typically  `mb->iterator - mb->init`).
- `data_size`: the bytes of the data that you are asking for.

---

```c
uint32_t ucdr_buffer_alignment(const ucdrBuffer* mb, const uint32_t data_size);
```
Returns the aligment necessary to serialize/deserialize a type with `data_size` size into the `ucdrBuffer` given.

- `mb`: the `ucdrBuffer` struct to ask the alignment.
- `data_size`: the bytes of the data that you are asking for.
---

```c
size_t ucdr_buffer_size(const ucdrBuffer* mb);
```
Returns the memory size of the buffer.
- `mb`: the `ucdrBuffer` struct

---

```c
size_t ucdr_buffer_length(const ucdrBuffer* mb);
```
Returns the size of the serialized/deserialized data.
- `mb`: the `ucdrBuffer` struct

---

```c
size_t ucdr_buffer_remaining(const ucdrBuffer* mb);
```
Returns the remaining size for the serializing/deserializing.
- `mb`: the `ucdrBuffer` struct

---

```c
ucdrEndianness ucdr_buffer_endianness(const ucdrBuffer* mb);
```
Returns the serialization/deserialization endianness.
- `mb`: the `ucdrBuffer` struct

---

```c
bool ucdr_buffer_error(const ucdrBuffer* mb);
```
Returns the status error of the `ucdrBuffer`.
- `mb`: the `ucdrBuffer` struct


### Serialization/deserialization functions
Adding to this, there is a big set of functions for deserialize and deserialize different kind of types:
- Basics: `bool`, `char`, `int8_t`, `uint8_t`,`int16_t`, `uint16_t`,`int32_t`, `uint32_t`,`int64_t`, `uint64_t`,`float`, `double`.
- Arrays: Any fixed size of basics types.
- Sequence: Similar to arrays, but the information about the size is serialized along with the data.

### Endianness
*MicroCDR* supports little and big endianness.
The configuration can be done by cmake with the cmake `__BIG_ENDIAN__` variable.
A `0` value implies that the serialization will performed into a little endian machine, and `1` into a big endian machine.

The default endianness serialization can be choosen by setting the `endianness` parameter of a `ucdrBuffer`  to `UCDR_BIG_ENDIANNESS` or `UCDR_LITTLE_ENDIANNESS`.
Also, there are a functions that allow to force an endianness in their serialization/deserialization.
These functions contains the name `endiannness` in their signature.

### Error
All serialization/deserialization functions return a boolean indicating the result of their operations.
When a serialization/deserialization could not be possible (the type can not be serialized, or the capacity of the destination buffer is not enough),
an status error is setted into the `ucdrBuffer`.
If a `ucdrBuffer` has an error state, the next serialization/deserialization operations will not works and will return `false` in their execution.
A buffer marked with an error can be used, but any serialization/deserialization operation over it will not produce any effect.

If is kwown that an operation can fails over a `ucdrBuffer`, and its necessary to continue with the serialization/deserialization if it happens,
the `ucdrBuffer` state can be saved using the `ucdr_copy_buffer` function.
After the application of the wrong serialization/deserialization, only the `ucdrBuffer` that performed the operation will have a dirty state.

## Serialization/deserialization list
The available modes of serialization/deserializations in *MicroCDR* are shown in the following table.


| Type                 | Endianness |
| -------------------- | ---------- |
| bool                 |            |
| char                 |            |
| int8                 |            |
| uint8                |            |
| int16                |            |
| int16                | endianness |
| uint16               |            |
| uint16               | endianness |
| int32                |            |
| int32                | endianness |
| uint32               |            |
| uint32               | endianness |
| int64                |            |
| int64                | endianness |
| uint64               |            |
| uint64               | endianness |
| float                |            |
| float                | endianness |
| double               |            |
| double               | endianness |
| string               |            |
| string               | endianness |
| bool array           |            |
| char array           |            |
| int8 array           |            |
| uint8 array          |            |
| int16 array          |            |
| int16 array          | endianness |
| uint16 array         |            |
| uint16 array         | endianness |
| int32 array          |            |
| int32 array          | endianness |
| uint32 array         |            |
| uint32 array         | endianness |
| int64 array          |            |
| int64 array          | endianness |
| uint64 array         |            |
| uint64 array         | endianness |
| float array          |            |
| float array          | endianness |
| double array         |            |
| double array         | endianness |
| bool sequence        |            |
| bool sequence        | endianness |
| char sequence        |            |
| char sequence        | endianness |
| int8 sequence        |            |
| int8 sequence        | endianness |
| uint8 sequence       |            |
| uint8 sequence       | endianness |
| int16 sequence       |            |
| int16 sequence       | endianness |
| uint16 sequence      |            |
| uint16 sequence      | endianness |
| int32 sequence       |            |
| int32 sequence       | endianness |
| uint32 sequence      |            |
| uint32 sequence      | endianness |
| int64 sequence       |            |
| int64 sequence       | endianness |
| uint64 sequence      |            |
| uint64 sequence      | endianness |
| float sequence       |            |
| float sequence       | endianness |
| double sequence      |            |
| double sequence      | endianness |

