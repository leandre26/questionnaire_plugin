# tui plugin for MADS

This is a Source plugin for [MADS](https://github.com/MADS-NET/MADS). 

It provides an interactive Teminal User Interface to send commands and markers to the MADS network. It is an easier to compile replacement for the QT6 MADSMetadata app.

*Required MADS version: 1.0.3.*


## Supported platforms

Currently, the supported platforms are:

* **Linux** 
* **MacOS**
* **Windows** (NOT TESTED!)


## Installation

Linux and MacOS:

```bash
cmake -Bbuild -DCMAKE_INSTALL_PREFIX="$(mads -p)"
cmake --build build -j4
sudo cmake --install build
```

Windows:

```powershell
cmake -Bbuild -DCMAKE_INSTALL_PREFIX="$(mads -p)"
cmake --build build --config Release
cmake --install build --config Release
```

## A note on the agent name/PUB topic

The plugin uses the agent name as the PUB topic. This means that the default PUB topic is going to be `tui`. Since the logger agent is expecting to receive commands on the `metadata` topic, you need to change name of the tui plugin agent to `metadata` either in the INI file (bu setting `pub_topic = "metadata"`, see the next section) or on the command line (by using the `-n metadata` option).

The first is the recommended way, as it allows to keep the agent name in the INI file and avoid having to remember to pass the `-n` option every time you run the plugin.


## INI settings

The plugin supports the following settings in the INI file:

```ini
[tui]
pub_topic = "metadata" # NOTHE THIS!!!
silent = false # Avoids polluting the console with debug messages
```

All settings are optional; if omitted, the default values are used.


## Executable demo

It allows to test TUI layout and functionality.

---