import sys
import time
import zmq
import threading
import numpy as np
from sensor_transports.video_rtp_streamer import VideoRTPStreamer
from sensor_transports.transporter import Transporter

class CameraTransporter(Transporter):
    def __init__(self, sensor_context={}):
        super().__init__(sensor_context)
        self.xres = sensor_context['image_size_x']
        self.yres = sensor_context['image_size_y']
        self.video_rtp_streamer = VideoRTPStreamer(self.xres, self.yres)
        self.main_thread = threading.Thread(target=self.run_loop)
        self.main_thread.start()

    def run_loop(self):
        while True:
            ipc_data = self._recv_ipc_data()
            filtered_data = self.filter_ipc_data(ipc_data)
            if filtered_data is not None:
                encoded_data = self.video_rtp_streamer.encode_frame(filtered_data)
                rtp_data = self.video_rtp_streamer.pack_rtp(encoded_data)
                print('camera_transporter send data')
                self._send_transport_data(rtp_data)
                print('camera_transporter send data done')

    def filter_ipc_data(self, ipc_data):
        if len(ipc_data) < 128:
            return None
        frame = np.frombuffer(ipc_data, dtype=np.uint8)
        frame = np.reshape(frame, (self.yres, self.xres, 4))
        frame = frame[:, :, :3]
        frame = frame[:, :, ::-1]
        return frame

    def __del__(self):
        self.main_thread.join()
        self.video_rtp_streamer.close()
        super().__del__()