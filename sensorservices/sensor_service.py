import sys
import time
import carla
import zmq
from abc import ABC, abstractmethod

class SensorService(ABC):
    def __init__(self, carla_world, sensor_context={}, attach_to=None):
        self._carla_world = carla_world
        self._sensor_context = sensor_context
        self._attach_to = attach_to
        self._type = sensor_context['type']
        self._sensor_name = sensor_context['name']
        self._sensor_bp_id = sensor_context['bp_id']
        self._sensor_bp = self._carla_world.get_blueprint_library().find(self._sensor_bp_id)
        self._spawn_point = sensor_context['spawn_point']
        self._transform = carla.Transform(carla.Location(x=self._spawn_point['x'], y=self._spawn_point['y'], z=self._spawn_point['z']),
                                          carla.Rotation(roll=self._spawn_point['roll'], pitch=self._spawn_point['pitch'], yaw=self._spawn_point['yaw']))
        self._zmq_ipc = sensor_context['zmq_ipc']
        self._zmq_context = zmq.Context()
        self._zmq_socket = self._zmq_context.socket(zmq.PUB)
        self._zmq_socket.bind(self._zmq_ipc)

    @abstractmethod
    def sensor_data_callback(self, data, sensor_name=''):
        pass

    @abstractmethod
    def set_sensor_bp_attribute(self):
        pass

    def _spawn_sensor(self):
        return self._carla_world.spawn_actor(self._sensor_bp, self._transform, attach_to=self._attach_to)

    def __del__(self):
        if self._zmq_socket is not None:
            self._zmq_socket.close()
            self._zmq_context.term()