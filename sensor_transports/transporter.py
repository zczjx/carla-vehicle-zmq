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
        self._zmq_ipc_context = zmq.Context()
        self._zmq_ipc_socket = self._zmq_ipc_context.socket(zmq.SUB)
        self._zmq_ipc_socket.connect(self._zmq_ipc)
        self._zmq_ipc_socket.setsockopt_string(zmq.SUBSCRIBE, '')

        self._zmq_transport = sensor_context['zmq_transport']
        self._zmq_transport_context = zmq.Context()
        self._zmq_transport_socket = self._zmq_transport_context.socket(zmq.PUB)
        self._zmq_transport_socket.bind(self._zmq_transport)

    def _recv_ipc_data(self):
        return self._zmq_ipc_socket.recv()

    def _send_transport_data(self, data):
        self._zmq_transport_socket.send(data, copy=False)

    @abstractmethod
    def run_loop(self):
        pass

    def __del__(self):
        self._zmq_ipc_socket.close()
        self._zmq_ipc_context.term()
        self._zmq_transport_socket.close()
        self._zmq_transport_context.term()