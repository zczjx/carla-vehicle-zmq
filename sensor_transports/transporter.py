import sys
import time
import zmq
import numpy as np
import msgpack
import time
from abc import ABC, abstractmethod

class Transporter(ABC):
    def __init__(self, sensor_context={}):
        self._sensor_context = sensor_context
        self._type = sensor_context['type']
        self._sensor_name = sensor_context['name']
        self._zmq_ipc = sensor_context['zmq_ipc']
        self._zmq_context = zmq.Context()
        self._zmq_socket = self._zmq_context.socket(zmq.SUB)
        self._zmq_socket.connect(self._zmq_ipc)
        self._zmq_socket.setsockopt_string(zmq.SUBSCRIBE, '')

    def _recv_data(self):
        return self._zmq_socket.recv()

    @abstractmethod
    def run_loop(self):
        pass

    @abstractmethod
    def render(self):
        pass

    def __del__(self):
        self._zmq_socket.close()
        self._zmq_context.term()