import sys
import time
import carla
from sensorservices.sensor_service import SensorService

class ImuService(SensorService):
    def __init__(self, carla_world, sensor_context={}, attach_to=None):
        super().__init__(carla_world, sensor_context, attach_to)
        self._sensor.listen(lambda data: self.sensor_data_callback(data, self._sensor_name))
    
    def sensor_data_callback(self, data, sensor_name=''):
        print(sensor_name)