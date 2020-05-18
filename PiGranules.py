#!/usr/bin/env python3

import argparse
import random
import time

import signal
import RPi.GPIO as GPIO
import os
import subprocess
from PIL import Image, ImageDraw , ImageFont
from ST7789 import ST7789


from pythonosc import udp_client


class PyGranule:
    def __init__(self):
        self.parser = argparse.ArgumentParser()
        self.parser.add_argument("--ip", default="127.0.0.1",help="The ip of the OSC server")
        self.parser.add_argument("--port", type=int, default=2112,help="The port the OSC server is listening on")
        self.args = self.parser.parse_args()
        self.client = udp_client.SimpleUDPClient(self.args.ip, self.args.port)
        self.hostfound = False
        
        SPI_SPEED_MHZ = 80

        self.image = Image.new("RGB", (240, 240), (0, 0, 0))
        self.draw = ImageDraw.Draw(self.image)
        # font = ImageFont.load("arial.pil")
  

        self.st7789 = ST7789(
            rotation=90,  # Needed to display the right way up on Pirate Audio
            port=0,       # SPI port
            cs=1,         # SPI port Chip-select channel
            dc=9,         # BCM pin used for data/command
            backlight=13,
            spi_speed_hz=SPI_SPEED_MHZ * 1000 * 1000
        )

        self.st7789.display(self.image)


        # The buttons on Pirate Audio are connected to pins 5, 6, 16 and 24
        # Boards prior to 23 January 2020 used 5, 6, 16 and 20 
        # try changing 24 to 20 if your Y button doesn't work.
        self.BUTTONS = [5, 6, 16, 24]

        # These correspond to buttons A, B, X and Y respectively
        self.LABELS = ['Test Sound', 'B', 'X', 'Y']

        # Set up RPi.GPIO with the "BCM" numbering scheme
        GPIO.setmode(GPIO.BCM)

        # Buttons connect to ground when pressed, so we should set them up
        # with a "PULL UP", which weakly pulls the input signal to 3.3V.
        GPIO.setup(self.BUTTONS, GPIO.IN, pull_up_down=GPIO.PUD_UP)

        self.process = subprocess.Popen(['./PiGranuleEngine', ' --files TestSounds'], 
                           stdout=subprocess.PIPE,
                           universal_newlines=True)

    def update(self):
        self.st7789.display(self.image)
    def playtest(self):
        self.client.send_message("/playtest",0)
    def search(self):
        self.client.send_message("/searchforhost",0)
    def run(self):
        output = self.process.stdout.readline()
        t = output.strip()
        self.image = Image.new("RGB", (240, 240), (0, 0, 0))
        self.draw = ImageDraw.Draw(self.image)
        # font = ImageFont.load("arial.pil")
        print("engine" +t)
        self.draw.text((10, 230),t)
        self.draw.text((100,100),"hello")
        self.update()
        # Do something else
        self.return_code = self.process.poll()
        if self.return_code is not None:
            print('RETURN CODE', self.return_code)
            # Process has finished, read rest of the output 
        for output in self.process.stdout.readlines():
            t = output.strip()
            print(t)
            self.draw.text((10, 230),t)
            self.update()
            return
        

engine = PyGranule()

# "handle_button" will be called every time a button is pressed
# It receives one argument: the associated input pin.
def handle_button(pin):
    if pin == 5 :
        engine.playtest()
    if pin == 16:
        engine.search()
    

# Loop through out buttons and attach the "handle_button" function to each
# We're watching the "FALLING" edge (transition from 3.3V to Ground) and
# picking a generous bouncetime of 100ms to smooth out button presses.
for pin in engine.BUTTONS:
    GPIO.add_event_detect(pin, GPIO.FALLING, handle_button, bouncetime=100)






while True:
    engine.run()
    