import sys
import os
import time
import argparse
import json
from viz_nodes.display_manager import DisplayManager


def run_visualization(args):
    vehicle_rig = json.load(open(args.rig))
    vehicle_rig = vehicle_rig['rig']
    sensor_list = vehicle_rig['sensors']
    
    display_manager = DisplayManager(sensor_list, (10, 10), (1280, 720))
    
    while True:
        pass

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--rig', type=str, default='./rigs/carla-vehicle-default.json')
    parser.add_argument('--res', metavar='WIDTHxHEIGHT', default='1280x720', help='window resolution (default: 1280x720)')
    
    args = parser.parse_args()
    
    try:
        run_visualization(args)
    except KeyboardInterrupt:
        print('\nCancelled by user. Bye!')
    except RuntimeError as e:
        print(e)

if __name__ == '__main__':

    main()
