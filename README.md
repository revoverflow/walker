# 🚶 walker

[![Build Status](https://app.travis-ci.com/revoverflow/walker.svg?branch=main)](https://app.travis-ci.com/revoverflow/walker)

Walker is a simple and fast tool to search for memory structures in binary files such as memory dumps. It is written in C++ and is made to be as fast as possible while still being easy to use, portable and extensible.

## How to use

Walker is a command line tool. It takes a binary file and a structure definition file as input and outputs the found offsets to a file, defaulting at `output.txt`, in the current working directory.

### Structure definition file

The structure definition file is a simple JSON file that defines the structure to search for. It consists of a list of fields, each of which has a type (primitive) and one or more criterias. The criterias are used to filter the results allowing it to find the right structure in a large binary file.

The following types are supported:
- `uint8`, `uint16`, `uint32`, `uint64`: Unsigned integers of the given size
- `int8`, `int16`, `int32`, `int64`: Signed integers of the given size
- `float` and `double`: Floating point numbers
- `bytes` : A sequence of bytes of the given size
- `pointer`: A pointer to another structure. The pointer size is determined by the architecture of the current machine (maybe this should be configurable in the future).
- *More types will be added in the future...*

The following criterias are supported:
- **Numeric fields**
  - `eq`: The field must be equal to the given value
  - `neq`: The field must not be equal to the given value
  - `gt`: The field must be greater than the given value
  - `gte`: The field must be greater than or equal to the given value
  - `lt`: The field must be less than the given value
  - `lte`: The field must be less than or equal to the given value
- **Pointer fields**
  - `notnullptr`: The field is not a null pointer
  - `nullptr`: The field is a null pointer
- **Bytes fields**
  - `match`: The field must match the given pattern (IDA style)
  - `not_match`: The field must not match the given pattern (IDA style)
- `any`: The field can be any value

### Example

Here is an example structure definition file that searches for a structure that contains a `uint32` field that is equal to `1337`, a pointer that is not null and a `uint8` field that can be any value.

```json
[
    {
        "type": "uint32",
        "criterias": [
            {
                "type": "eq",
                "value": 1337
            }
        ]
    },
    {
        "type": "pointer",
        "criterias": [
            {
                "type": "notnullptr"
            }
        ]
    },
    {
        "type": "uint8",
        "criterias": [
            {
                "type": "any"
            }
        ]
    }
]
```

To run this example, you can use the following command:

```bash
walker -f example.bin -s example.json -o example_output.txt
```

## Releases

Releases are available on the [releases page](https://github.com/revoverflow/walker/releases) and are automatically built for Linux using Travis CI. If you want to build it yourself, just clone the repository and run a cmake build.

## Is this just pattern scanning?

Pattern scanning is a common technique used in reverse engineering to find memory structures, it works by searching for a sequence of bytes. This is a very simple and fast technique, but it has a major drawback: it is very unreliable. It is very easy to get false positives when using pattern scanning, especially when the pattern is short.

walker works the same, but adding criterias to the fields allows it to filter out false positives. This makes it much more reliable than pattern scanning and makes the offset finding process much faster when searching for complex structures.