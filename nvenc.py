#!/usr/bin/env python

import ffmpeg
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
                   pix_fmt='bgr24',
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
    encoder = NVENCEncoder(1280, 720, "output.h264")
    frame = np.zeros((720, 1280, 3), dtype=np.uint8)
    encoder.encode_frame(frame)
    encoder.close()
