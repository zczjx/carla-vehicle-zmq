import sys
import time
import zmq
import threading
import numpy as np

from sensor_transports.transporter import Transporter

class CameraTransporter(Transporter):
    def __init__(self, sensor_context={}):
        super().__init__(sensor_context)

    def run_loop(self):
        pass

    def render(self):
        pass

    def __del__(self):
        super().__del__()