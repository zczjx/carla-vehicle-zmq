#!/usr/bin/env python

import glob
import os
import sys
import random
import time
import numpy as np

from sensor_transports.camera_transporter import CameraTransporter
from sensor_transports.gnss_transporter import GnssTransporter
from sensor_transports.imu_transporter import ImuTransporter
from sensor_transports.lidar_transporter import LidarTransporter
from sensor_transports.radar_transporter import RadarTransporter

class TransportManager:
    def __init__(self, sensor_list):
        self.transporter_list = self.launch_sensor_transporter(sensor_list)
    def run_loop(self):
        pass

    def launch_sensor_transporter(self, sensor_list):
        transporter_list = []
        for sensor in sensor_list:
            if sensor['status'] == 'disabled':
                continue
            if sensor['type'] == 'camera':
                transporter_list.append(CameraTransporter(sensor))
            elif sensor['type'] == 'gnss':
                transporter_list.append(GnssTransporter(sensor))
            elif sensor['type'] == 'imu':
                transporter_list.append(ImuTransporter(sensor))
            elif sensor['type'] == 'lidar':
                transporter_list.append(LidarTransporter(sensor))
            elif sensor['type'] == 'radar':
                transporter_list.append(RadarTransporter(sensor))
        return transporter_list

