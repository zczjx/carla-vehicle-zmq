#!/usr/bin/env python

import sys
import os
import time
import argparse
import json
import carla
from vehicle import CarlaVehicle

def run_client(args, client):
    try:
        world = client.get_world()
        original_settings = world.get_settings()
        if args.sync:
            original_settings = world.get_settings()
            settings = world.get_settings()
            if not settings.synchronous_mode:
                settings.synchronous_mode = True
                settings.fixed_delta_seconds = 0.05
            world.apply_settings(settings)

        vehicle_rig = json.load(open(args.rig))
        vehicle = CarlaVehicle(carla_world=world, rig=vehicle_rig)
        vehicle.run_loop(sync=args.sync)
    finally:
        world.apply_settings(original_settings)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--rig', type=str, default='./rigs/carla-vehicle-default.json')
    parser.add_argument('--host', type=str, default='127.0.0.1')
    parser.add_argument('--port', type=int, default=2000)
    parser.add_argument('--sync', action='store_true', help='Synchronous mode execution')
    parser.add_argument('--res', metavar='WIDTHxHEIGHT', default='1280x720', help='window resolution (default: 1280x720)')
    
    args = parser.parse_args()
    
    client = carla.Client(args.host, args.port)
    client.set_timeout(5.0)
    try:
        run_client(args, client)
    except KeyboardInterrupt:
        print('\nCancelled by user. Bye!')
    except RuntimeError as e:
        print(e)

if __name__ == '__main__':

    main()

