import sys
import time
import zmq
import threading
import numpy as np
from sensor_transports.transporter import Transporter

class ImuTransporter(Transporter):
    def __init__(self, sensor_context={}):
        super().__init__(sensor_context)
        self.main_thread = threading.Thread(target=self.run_loop)
        self.main_thread.start()

    def run_loop(self):
        while True:
            ipc_data = self._recv_ipc_data()
            self._send_transport_data(ipc_data)

    def __del__(self):
        self.main_thread.join()
        super().__del__()
