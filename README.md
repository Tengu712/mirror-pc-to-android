# Mirror PC to Android

## What is this?

An app that mirrors the client area of Windows windows to an Android device.
It utilizes USB for communication and establishes TCP connections through localhost:63200.

## Usage

1. Download "pc.exe" to your PC.
2. Install "Mirror PC to Android" on your Android device.
3. Connect your Android device to the PC via USB.
4. Run `adb forward tcp:63200 tcp:63200`.
5. Launch "Mirror PC to Android" on your Android device.
6. Run "pc.exe" on your PC.
7. Input the process ID and select the window.
8. Enter the screen refresh interval.

Please note the following:

- Ensure that adb is available for use with the Android device.
- Make sure there are no overlapping windows on the window to be mirrored.
- Consider USB communication speed and Android CPU processing speed when selecting window size and refresh interval.
