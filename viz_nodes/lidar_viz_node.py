import sys
import time
import zmq
from viz_nodes.viz_node import VizNode

class LidarVizNode(VizNode):
    def __init__(self, sensor_context={}, display_manager=None):
        super().__init__(sensor_context, display_manager)
        self._sensor_context = sensor_context

    def run_loop(self):
        pass

    def render(self):
        pass