import sys
import time
import zmq
from viz_nodes.viz_node import VizNode

class LocationVizNode(VizNode):
    def __init__(self, sensor_context={}, display_manager=None):
        super().__init__(sensor_context, display_manager)
        self._sensor_context = sensor_context
        self._display_manager = display_manager

    def run_loop(self):
        pass

    def render(self):
        pass