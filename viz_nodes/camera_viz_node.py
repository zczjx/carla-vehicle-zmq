import sys
import time
import zmq
from viz_nodes.viz_node import VizNode
import threading
import numpy as np
import msgpack
try:
    import pygame
except ImportError:
    raise RuntimeError('cannot import pygame, make sure pygame package is installed')

class CameraVizNode(VizNode):
    def __init__(self, sensor_context={}, display_manager=None):
        super().__init__(sensor_context, display_manager)
        self.surface = None
        self.metadata = None
        self.main_thread = threading.Thread(target=self.run_loop)
        self.main_thread.start()

    def run_loop(self):
        while True:
            payload_frame = self._recv_data()
            if len(payload_frame) < 128:
                # metadata frame
                self.update_metadata(packed_meta=payload_frame)
            elif self.metadata is not None:
                self.draw_image(height=self.metadata['height'], width=self.metadata['width'], image=payload_frame)

    def draw_image(self, height, width, image=memoryview(b'abcefg')):
        array = np.frombuffer(image, dtype=np.dtype('uint8'))
        array = np.reshape(array, (height, width, 4))
        array = array[:, :, :3]
        array = array[:, :, ::-1]
        if self._display_manager.render_enabled():
            self.surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))

    def render(self):
        if self.surface is not None:
            offset = self._display_manager.get_display_offset(self._display_pos)
            self._display_manager.display.blit(self.surface, offset)

    def update_metadata(self, packed_meta):
        meta_data = msgpack.unpackb(packed_meta)
        self.metadata = {}
        self.metadata['height'] = meta_data['height']
        self.metadata['width'] = meta_data['width']
        self.metadata['name'] = meta_data['name']

    def __del__(self):
        self.main_thread.join()
        super().__del__()
