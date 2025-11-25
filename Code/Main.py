import board
import digitalio
import time
import rotaryio

from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode
from adafruit_hid.consumer_control import ConsumerControl
from adafruit_hid.consumer_control_code import ConsumerControlCode
import usb_hid

# ---- USB Devices ----
keyboard = Keyboard(usb_hid.devices)
consumer = ConsumerControl(usb_hid.devices)

# ---- Buttons ----
key1 = digitalio.DigitalInOut(board.D1)   # Firefox
key1.direction = digitalio.Direction.INPUT
key1.pull = digitalio.Pull.UP

key2 = digitalio.DigitalInOut(board.D2)   # File Explorer
key2.direction = digitalio.Direction.INPUT
key2.pull = digitalio.Pull.UP

key3 = digitalio.DigitalInOut(board.D3)   # Task Manager
key3.direction = digitalio.Direction.INPUT
key3.pull = digitalio.Pull.UP

key4 = digitalio.DigitalInOut(board.D4)   # Settings
key4.direction = digitalio.Direction.INPUT
key4.pull = digitalio.Pull.UP

# ---- Rotary Encoder ----
encoder = rotaryio.IncrementalEncoder(board.D5, board.D6)
last_position = encoder.position

# ---- Helper functions ----
def open_firefox():
    keyboard.send(Keycode.GUI, Keycode.R)  # Win + R
    time.sleep(0.2)
    keyboard.write("firefox")
    keyboard.send(Keycode.ENTER)

def open_file_explorer():
    keyboard.send(Keycode.GUI, Keycode.E)

def open_task_manager():
    keyboard.send(Keycode.CONTROL, Keycode.SHIFT, Keycode.ESCAPE)

def open_settings():
    keyboard.send(Keycode.GUI, Keycode.I)

# ---- Main Loop ----
while True:

    # ---- Buttons ----
    if not key1.value:
        open_firefox()
        time.sleep(0.5)

    if not key2.value:
        open_file_explorer()
        time.sleep(0.5)

    if not key3.value:
        open_task_manager()
        time.sleep(0.5)

    if not key4.value:
        open_settings()
        time.sleep(0.5)

    # ---- Rotary Encoder (Volume) ----
    position = encoder.position
    movement = position - last_position

    if movement != 0:
        if movement > 0:
            consumer.send(ConsumerControlCode.VOLUME_INCREMENT)
        else:
            consumer.send(ConsumerControlCode.VOLUME_DECREMENT)

        last_position = position

    time.sleep(0.01)
