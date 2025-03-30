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
        print(sensor_name)
        pass
    
    def set_sensor_bp_attribute(self):
        pass
    
    def __del__(self):
        if self.sensor is not None:
            self.sensor.destroy()