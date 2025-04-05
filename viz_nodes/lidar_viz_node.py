import sys
import time
import zmq
from viz_nodes.viz_node import VizNode
import threading
import numpy as np
try:
    import pygame
except ImportError:
    raise RuntimeError('cannot import pygame, make sure pygame package is installed')

class LidarVizNode(VizNode):
    def __init__(self, sensor_context={}, display_manager=None):
        super().__init__(sensor_context, display_manager)
        self.surface = None
        self.name = sensor_context['name']
        self.image_size_x = sensor_context['image_size_x']
        self.image_size_y = sensor_context['image_size_y']
        self.lidar_range = 2.0 * float(sensor_context['attribute']['range'])
        self.main_thread = threading.Thread(target=self.run_loop)
        self.main_thread.start()

    def run_loop(self):
        while True:
            payload_frame = self._recv_data()
            self.draw_image(height=self.image_size_y, width=self.image_size_x, image=payload_frame)

    def draw_image(self, height, width, image=memoryview(b'abcefg')):
        disp_size = (self.image_size_x, self.image_size_y)
        points = np.frombuffer(image, dtype=np.dtype('f4'))
        if self.name == 'lidar_cast':
            points = np.reshape(points, (int(points.shape[0] / 4), 4))
        elif self.name == 'lidar_cast_semantic':
            points = np.reshape(points, (int(points.shape[0] / 6), 6))
        else:
            raise ValueError('Invalid lidar name')
        lidar_data = np.array(points[:, :2])
        lidar_data *= min(disp_size) / self.lidar_range
        lidar_data += (0.5 * disp_size[0], 0.5 * disp_size[1])
        lidar_data = np.fabs(lidar_data)  # pylint: disable=E1111
        lidar_data = lidar_data.astype(np.int32)
        lidar_data = np.reshape(lidar_data, (-1, 2))
        lidar_img_size = (disp_size[0], disp_size[1], 3)
        lidar_img = np.zeros((lidar_img_size), dtype=np.uint8)
        lidar_img[tuple(lidar_data.T)] = (255, 255, 255)

        if self._display_manager.render_enabled():
            self.surface = pygame.surfarray.make_surface(lidar_img)

    def render(self):
        if self.surface is not None:
            offset = self._display_manager.get_display_offset(self._display_pos)
            self._display_manager.display.blit(self.surface, offset)

    def __del__(self):
        self.main_thread.join()
        super().__del__()