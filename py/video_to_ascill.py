import cv2
import os
import json
import numpy as np

ASCII_CHARS = [' ', '.', ':', '-', '=', '+', '*', '#', '%', '@']

def clear_folder(folder):
    if os.path.exists(folder):
        for f in os.listdir(folder):
            os.remove(os.path.join(folder, f))
    else:
        os.makedirs(folder)

def convert_frame(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    normalized = gray / 255.0
    indices = (normalized * (len(ASCII_CHARS)-1)).astype(np.uint8)
    return '\n'.join(''.join(ASCII_CHARS[i] for i in row) for row in indices)

def process_video(video_path, output_dir):
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        raise Exception("无法打开视频文件")
    
    # 获取视频信息
    fps = cap.get(cv2.CAP_PROP_FPS)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    
    # 创建输出目录
    clear_folder(output_dir)
    
    # 处理每一帧
    for i in range(total_frames):
        ret, frame = cap.read()
        if not ret:
            break
            
        ascii_art = convert_frame(frame)
        with open(os.path.join(output_dir, f"frame_{i:04d}.txt"), "w") as f:
            f.write(ascii_art)
    
    # 保存配置文件
    config = {
        "name": os.path.basename(video_path),
        "width": width,
        "height": height,
        "fps": fps,
        "total_frames": total_frames
    }
    with open(os.path.join(output_dir, "config.json"), "w") as f:
        json.dump(config, f, indent=2)
    
    cap.release()
    print(f"转换完成！共处理 {total_frames} 帧")

if __name__ == "__main__":
    try:
        process_video("media/badapple.mp4", "ascii_frames")
    except Exception as e:
        print(f"错误发生: {str(e)}")