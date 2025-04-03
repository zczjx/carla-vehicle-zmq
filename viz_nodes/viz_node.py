import sys
import time
import zmq
from abc import ABC, abstractmethod

class VizNode(ABC):
    def __init__(self, sensor_context={}, attach_to=None):
        self._sensor_context = sensor_context
        self._attach_to = attach_to
        self._type = sensor_context['type']
        self._sensor_name = sensor_context['name']
        self._sensor_bp_id = sensor_context['bp_id']