# walker

Walker is a simple and fast tool to search for memory structures in binary files such as memory dumps.

## How to use

Walker is a command line tool. It takes a binary file and a structure definition file as input and outputs the found offsets to a file, defaulting at `output.txt`, in the current working directory.

### Structure definition file

The structure definition file is a simple JSON file that defines the structure to search for. It consists of a list of fields, each of which has a type (primitive) and one or more criteria. The criteria are used to filter the results allowing it to find the right structure in a large binary file.

The following types are supported:
- `uint8`, `uint16`, `uint32`, `uint64`: Unsigned integers of the given size
- `int8`, `int16`, `int32`, `int64`: Signed integers of the given size
- `float` and `double`: Floating point numbers
- `pointer`: A pointer to another structure. The pointer size is determined by the architecture of the current machine (maybe this should be configurable in the future).
- *More types will be added in the future...*

The following criteria are supported:
- `eq`: The field must be equal to the given value
- `neq`: The field must not be equal to the given value
- `gt`: The field must be greater than the given value
- `gte`: The field must be greater than or equal to the given value
- `lt`: The field must be less than the given value
- `lte`: The field must be less than or equal to the given value
- `nullptr`: The field must be a null pointer
- `notnullptr`: The field must not be a null pointer
- `any`: The field can be any value