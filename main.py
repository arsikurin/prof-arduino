import sys

import serial

PORT = "/dev/cu.usbserial-210"

try:
    arduino = serial.Serial(PORT, 9600)
except serial.serialutil.SerialException as err:
    print(err)
    sys.exit(1)

if sys.platform == "darwin":
    import subprocess


    def set_brightness(target: int):
        subprocess.run(
            args=["./brightness", f"{target / 100}"],
            capture_output=True
        )
elif sys.platform in {"linux", "win32"}:
    import screen_brightness_control as sbc


    def set_brightness(target: int):
        sbc.set_brightness(target)
else:
    raise NotImplemented("Current platform is not supported")

while True:
    raw = arduino.readline(256).decode()
    try:
        set_brightness(int(raw))
        print(int(raw), "set")

    except ValueError:
        print(raw, "err")
