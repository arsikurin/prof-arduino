import platform
import time

# import serial

# arduino = serial.Serial("/dev/cu.usbserial-10", 9600)

with open("lib.scpt", mode="r") as f:
    cmd = f.read()

if platform.system() == "Darwin":
    import subprocess


    def increase(repeat: int = 1):
        subprocess.run(
            args=['osascript', '-e', cmd, "1", str(repeat)], capture_output=True
        )


    def decrease(repeat: int = 1):
        subprocess.run(
            args=['osascript', '-e', cmd, str(repeat)], capture_output=True
        )
    # decrease = ft.partial(subprocess.run, args=['osascript', '-e', cmd], capture_output=True)

elif platform.system() in {"Linux", "Windows"}:
    import screen_brightness_control as sbc


    def increase(repeat: int = 1):
        sbc.fade_brightness(
            finish=sbc.get_brightness()[0] + 10 * repeat, start=sbc.get_brightness()[0], blocking=False, increment=10
        )


    def decrease(repeat: int = 1):
        sbc.fade_brightness(
            finish=sbc.get_brightness()[0] - 10 * repeat, start=sbc.get_brightness()[0], blocking=False, increment=10
        )
    # decrease = sbc.set_brightness(sbc.get_brightness()[0] - 10)
else:
    raise NotImplemented("Current platform is not supported")

while True:
    time.sleep(1)
    decrease(5)
    # print(arduino.read(256).decode())

# arduino.write(bytes(f"${345};"))
