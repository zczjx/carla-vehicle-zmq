#!/usr/bin/env python

import ffmpeg
import sys
import time
import zmq
import numpy as np

class NVENCEncoder:
    def __init__(self, width, height, output_file, fps=30):
        self.width = width
        self.height = height
        self.fps = fps

        # 创建 FFmpeg 进程
        self.process = (
            ffmpeg
            .input('pipe:',
                   format='rawvideo',
                   pix_fmt='rgb24',
                   s=f'{width}x{height}',
                   r=fps)
            .output(output_file,
                   vcodec='h264_nvenc',  # 使用 NVENC
                   preset='p4',          # 性能预设
                   rc='cbr',            # 固定码率
                   b='5M',              # 码率
                   gpu='0')            # GPU ID
            .overwrite_output()
            .run_async(pipe_stdin=True)
        )

    def encode_frame(self, frame):
        try:
            # 确保输入是 numpy 数组
            if not isinstance(frame, np.ndarray):
                frame = np.array(frame)

            # 写入帧
            self.process.stdin.write(frame.tobytes())

        except Exception as e:
            print(f"编码错误: {e}")

    def close(self):
        if self.process:
            self.process.stdin.close()
            self.process.wait()

if __name__ == "__main__":
    zmq_context = zmq.Context()
    zmq_socket = zmq_context.socket(zmq.SUB)
    zmq_socket.connect("ipc:///tmp/view_camera.raw")
    zmq_socket.setsockopt_string(zmq.SUBSCRIBE, '')
    frame_cnt = 0
    encoder = NVENCEncoder(1280, 720, "output.h264")

    while frame_cnt < 3000:
        payload_frame = zmq_socket.recv()
        if len(payload_frame) < 128:
            continue
        frame = np.frombuffer(payload_frame, dtype=np.uint8)
        frame = np.reshape(frame, (720, 1280, 4))
        frame = frame[:, :, :3]
        frame = frame[:, :, ::-1]
        encoder.encode_frame(frame)
        frame_cnt += 1

    encoder.close()
