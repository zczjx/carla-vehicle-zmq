import sys
import time
import carla
from sensorservices.sensor_service import SensorService

class LidarService(SensorService):
    def __init__(self, carla_world, sensor_context={}, attach_to=None):
        super().__init__(carla_world, sensor_context, attach_to)
        self.set_sensor_bp_attribute()
        self.sensor = self._spawn_sensor()
        self.sensor.listen(lambda data: self.sensor_data_callback(data, self._sensor_name))

    def sensor_data_callback(self, data, sensor_name=''):
        self._zmq_socket.send(data.raw_data, copy=False)
        pass

    def set_sensor_bp_attribute(self):
        self._sensor_bp.set_attribute('range', '100')
        # self._sensor_bp.set_attribute('dropoff_general_rate', self._sensor_bp.get_attribute('dropoff_general_rate').recommended_values[0])
        # self._sensor_bp.set_attribute('dropoff_intensity_limit', self._sensor_bp.get_attribute('dropoff_intensity_limit').recommended_values[0])
        # self._sensor_bp.set_attribute('dropoff_zero_intensity', self._sensor_bp.get_attribute('dropoff_zero_intensity').recommended_values[0])
        for key in self._sensor_context['attribute']:
            self._sensor_bp.set_attribute(key, self._sensor_context['attribute'][key])

    def __del__(self):
        super().__del__()
        if self.sensor is not None:
            self.sensor.destroy()