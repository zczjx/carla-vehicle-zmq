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
            if sensor_context['type'] == 'camera':
                self.active_sensor_services.append(CameraService(self.carla_world, sensor_context, attach_to))
            elif sensor_context['type'] == 'gnss':
                self.active_sensor_services.append(GnssService(self.carla_world, sensor_context, attach_to))
            elif sensor_context['type'] == 'imu':
                self.active_sensor_services.append(ImuService(self.carla_world, sensor_context, attach_to))
            elif sensor_context['type'] == 'lidar':
                self.active_sensor_services.append(LidarService(self.carla_world, sensor_context, attach_to))
            elif sensor_context['type'] == 'radar':
                self.active_sensor_services.append(RadarService(self.carla_world, sensor_context, attach_to))

    def get_active_sensor_services(self):
        return self.active_sensor_services

    def run_sensors(self):
        for sensor_service in self.active_sensor_services:
            sensor_service.run()
