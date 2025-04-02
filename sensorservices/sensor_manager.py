import sys
import time
import carla
from sensorservices.camera_service import CameraService
from sensorservices.gnss_service import GnssService
from sensorservices.imu_service import ImuService
from sensorservices.lidar_service import LidarService
from sensorservices.radar_service import RadarService

class SensorManager:
    def __init__(self, carla_world, sensor_list):
        self.carla_world = carla_world
        self.sensor_list = sensor_list
        self.active_sensor_services = []
    
    def spawn_sensors(self, attach_to=None):
        for sensor_context in self.sensor_list:
            if sensor_context['status'] == 'disabled':
                continue
            sensor_service = None
            if sensor_context['type'] == 'camera':
                sensor_service = CameraService(self.carla_world, sensor_context, attach_to)
            elif sensor_context['type'] == 'gnss':
                sensor_service = GnssService(self.carla_world, sensor_context, attach_to)
            elif sensor_context['type'] == 'imu':
                sensor_service = ImuService(self.carla_world, sensor_context, attach_to)
            elif sensor_context['type'] == 'lidar':
                sensor_service = LidarService(self.carla_world, sensor_context, attach_to)
            elif sensor_context['type'] == 'radar':
                sensor_service = RadarService(self.carla_world, sensor_context, attach_to)
            if sensor_service is not None:
                self.active_sensor_services.append(sensor_service)
    
    def run_sensors(self):
        for sensor_service in self.active_sensor_services:
            sensor_service.run()

    