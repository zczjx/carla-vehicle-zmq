import sys
import time
import zmq
from viz_nodes.viz_node import VizNode

class RadarVizNode(VizNode):
    def __init__(self, sensor_context={}, attach_to=None):
        super().__init__(sensor_context, attach_to)
        self._sensor_context = sensor_context
        self._attach_to = attach_to