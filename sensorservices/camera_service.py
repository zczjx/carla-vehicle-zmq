import sys
import time
import carla
import msgpack
from sensorservices.sensor_service import SensorService

class CameraService(SensorService):
    def __init__(self, carla_world, sensor_context={}, attach_to=None):
        super().__init__(carla_world, sensor_context, attach_to)

        self.frame_count = 0
        self.last_time = time.time()
        self.fps = 0
        self.fps_print_interval = 1.0  # 每秒打印一次 FPS
        self.set_sensor_bp_attribute()
        self.sensor = self._spawn_sensor()
        self.sensor.listen(lambda data: self.sensor_data_callback(data, self._sensor_name))

    def set_sensor_bp_attribute(self):
        self._sensor_bp.set_attribute('image_size_x', str(self._sensor_context['image_size_x']))
        self._sensor_bp.set_attribute('image_size_y', str(self._sensor_context['image_size_y']))

    def sensor_data_callback(self, data, sensor_name=''):
        meta_data = {'name': str(sensor_name), 'type': self._type}
        meta_data['fov'] = int(data.fov)
        meta_data['height'] = int(data.height)
        meta_data['width'] = int(data.width)
        packed_meta_data = msgpack.packb(meta_data)
        self._zmq_socket.send(packed_meta_data, copy=False)
        self._zmq_socket.send(data.raw_data, copy=False)
        # 增加帧计数
        self.frame_count += 1

        # 计算经过的时间
        current_time = time.time()
        elapsed_time = current_time - self.last_time

        # 每隔一秒计算并打印 FPS
        if elapsed_time >= self.fps_print_interval:
            self.fps = self.frame_count / elapsed_time
            print(f"{sensor_name} FPS: {self.fps:.2f}")

            # 重置计数器
            self.frame_count = 0
            self.last_time = current_time

    def __del__(self):
        super().__del__()
        if self.sensor is not None:
            self.sensor.destroy()