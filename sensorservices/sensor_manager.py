import sys
import time
import carla

class SensorManager:
    def __init__(self, carla_world, sensor_list):
        self.carla_world = carla_world
        self.sensor_list = sensor_list
        self.sensors = []
        self.sensors_list = []
        self.sensors_dict = {}
        self.sensors_dict_list = []
        self.sensors_dict_list_list = []
        self.sensors_dict_list_list_list = []
        self.sensors_dict_list_list_list = []
    
    def spawn_sensors(self):
        for sensor in self.sensor_list:
            sensor_status = sensor['status']
            sensor_type = sensor['type']
            sensor_id = sensor['id']
            sensor_spawn_point = sensor['spawn_point']
    