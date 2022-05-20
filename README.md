# world-clock

**Simple 2D 24-hour clock with a hand for each time-zone**

<!--insert video link here-->

## Usage

1. Build and run
1. [optional] Customize `world_clock.json` in the working directory
1. Press A/D/Left/Right to rotate hands
1. Press Space to reset rotation
1. Press Escape to quit

### `world_clock.json` format

The document root must be a single object containing a list of objects (key-value pairs). Currently supported variables are listed below:

```json
{
 "some_unique_id": {
    "offset": -3.25,
    "colour": "#ff8800ff"
  },
  ...
}
```

## Building

### Requirements

1. CMake 3.19+
1. C++20
1. [optional] [vulkify](https://github.com/vulkify/vulkify)

### Configuration

1. If `vulkify` is installed, pass its path as `CMAKE_PREFIX_PATH` when configuring:

```
cmake -S . -B out -DCMAKE_PREFIX_PATH=path/to/vulkify
```

1. Otherwise `world-clock` will download it via CMake `FetchContent`
