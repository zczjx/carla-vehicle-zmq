#!/usr/bin/env python

import glob
import os
import sys
import random
import time
import numpy as np

from viz_nodes.camera_viz_node import CameraVizNode
from viz_nodes.lidar_viz_node import LidarVizNode
from viz_nodes.location_viz_node import LocationVizNode
from viz_nodes.radar_viz_node import RadarVizNode

try:
    import pygame
    from pygame.locals import K_ESCAPE
    from pygame.locals import K_q
except ImportError:
    raise RuntimeError('cannot import pygame, make sure pygame package is installed')


class DisplayManager:
    def __init__(self, sensor_list, grid_size, window_size):
        pygame.init()
        pygame.font.init()
        self.display = pygame.display.set_mode(window_size, pygame.HWSURFACE | pygame.DOUBLEBUF)
        self.grid_size = grid_size
        self.window_size = window_size
        self.viz_node_list = self.launch_sensor_viz_node(sensor_list)

    def launch_sensor_viz_node(self, sensor_list):
        viz_node_list = []
        for sensor_context in sensor_list:
            if sensor_context['status'] == 'disabled':
                continue
            if sensor_context['type'] == 'camera':
                viz_node_list.append(CameraVizNode(sensor_context, self))
            elif sensor_context['type'] == 'lidar':
                viz_node_list.append(LidarVizNode(sensor_context, self))
            elif sensor_context['type'] == 'location':
                viz_node_list.append(LocationVizNode(sensor_context, self))
            elif sensor_context['type'] == 'radar':
                viz_node_list.append(RadarVizNode(sensor_context, self))
        return viz_node_list

    def get_window_size(self):
        return [int(self.window_size[0]), int(self.window_size[1])]

    def get_display_size(self):
        return [int(self.window_size[0]/self.grid_size[1]), int(self.window_size[1]/self.grid_size[0])]

    def get_display_offset(self, gridPos):
        dis_size = self.get_display_size()
        return [int(gridPos[1] * dis_size[0]), int(gridPos[0] * dis_size[1])]


    def get_viz_node_list(self):
        return self.viz_node_list

    def render(self, font, clock):
        if not self.render_enabled():
            return

        for node in self.viz_node_list:
            node.render()

        self.display.blit(
            font.render('% 5d FPS ' % clock.get_fps(), True, (255, 255, 255)),
            (8, 10))
        pygame.display.flip()

    def render_enabled(self):
        return self.display != None

    def get_exit_event(self):
        call_exit = False
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                call_exit = True
            elif event.type == pygame.KEYDOWN:
                if event.key == K_ESCAPE or event.key == K_q:
                    call_exit = True
        return call_exit