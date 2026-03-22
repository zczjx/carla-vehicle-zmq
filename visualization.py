#!/usr/bin/env python

import sys
import os
import time
import argparse
import json
from viz_nodes.display_manager import DisplayManager

try:
    import pygame
except ImportError:
    raise RuntimeError('cannot import pygame, make sure pygame package is installed')

def get_font():
    fonts = [x for x in pygame.font.get_fonts()]
    default_font = 'ubuntumono'
    font = default_font if default_font in fonts else fonts[0]
    font = pygame.font.match_font(font)
    return pygame.font.Font(font, 14)

def _resolve_grid_size(vehicle_rig, grid_arg):
    """grid_size is [rows, cols], matching display_position [row, col]."""
    if grid_arg:
        parts = grid_arg.lower().split('x', 1)
        if len(parts) == 2:
            return [int(parts[0]), int(parts[1])]
    props = vehicle_rig.get('properties') or {}
    gs = props.get('grid_size')
    if isinstance(gs, (list, tuple)) and len(gs) == 2:
        return [int(gs[0]), int(gs[1])]
    return [2, 3]


def run_visualization(args):
    vehicle_rig = json.load(open(args.rig))
    vehicle_rig = vehicle_rig['rig']
    sensor_list = vehicle_rig['sensors']
    grid_size = _resolve_grid_size(vehicle_rig, args.grid)

    display_manager = DisplayManager(sensor_list, grid_size=grid_size, window_size=[args.width, args.height])

    font = get_font()
    clock = pygame.time.Clock()
    while True:
        time.sleep(0.03)
        clock.tick(0)
        display_manager.render(font, clock)
        call_exit = display_manager.get_exit_event()

        if call_exit:
            break

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--rig', type=str, default='./rigs/carla-vehicle-default.json')
    parser.add_argument('--res', metavar='WIDTHxHEIGHT', default='1920x1080', help='window resolution (default: 1920x1080)')
    parser.add_argument('--grid', metavar='ROWSxCOLS', default=None,
                        help='tile grid, e.g. 2x2 for four cameras (overrides rig properties.grid_size)')

    args = parser.parse_args()
    args.width, args.height = [int(x) for x in args.res.split('x')]

    try:
        run_visualization(args)
    except KeyboardInterrupt:
        print('\nCancelled by user. Bye!')
    except RuntimeError as e:
        print(e)

if __name__ == '__main__':

    main()
