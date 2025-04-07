#!/usr/bin/env python

import sys
import os
import time
import argparse
import json
from sensor_transports.transport_manager import TransportManager


def run_transport_server(args):
    vehicle_rig = json.load(open(args.rig))
    vehicle_rig = vehicle_rig['rig']
    sensor_list = vehicle_rig['sensors']

    transport_manager = TransportManager(sensor_list)

    while True:
        time.sleep(0.03)
        transport_manager.run_loop()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--rig', type=str, default='./rigs/carla-vehicle-default.json')

    args = parser.parse_args()

    try:
        run_transport_server(args)
    except KeyboardInterrupt:
        print('\nCancelled by user. Bye!')
    except RuntimeError as e:
        print(e)

if __name__ == '__main__':

    main()