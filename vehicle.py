#!/usr/bin/env python

import sys
import time
import carla
import random
from sensorservices.sensor_manager import SensorManager

class CarlaVehicle:
    def __init__(self, carla_world=None, rig={}):
        self.carla_world = carla_world
        self.rig = rig['rig']
        self.sensor_manager = SensorManager(carla_world=self.carla_world, sensor_list=self.rig['sensors'])
        self.actor_list = []
        self.spawn_vehicle()

    def __del__(self):
        for actor in self.actor_list:
            actor.destroy()
    
    def spawn_vehicle(self):
        vehicle_info = self.rig['vehicle']
        bp = self.carla_world.get_blueprint_library().filter(vehicle_info['bp_id'])[0]
        spawn_point = random.choice(self.carla_world.get_map().get_spawn_points())
        self.vehicle = self.carla_world.spawn_actor(bp, spawn_point)
        self.actor_list.append(self.vehicle)
        self.vehicle.set_autopilot(True)
        self.sensor_manager.spawn_sensors(attach_to=self.vehicle)

    def run_loop(self, sync=False):
        while True:
            # Carla Tick
            if sync:
                self.carla_world.tick()
            else:
                self.carla_world.wait_for_tick()
