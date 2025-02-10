import cv2
import numpy as np
import shutil
import time
import sys
import os
import pygame

# 字符集，按亮度从暗到亮排列
ASCII_CHARS = [' ', '.', ':', '-', '=', '+', '*', '#', '%', '@']

def convert_frame_to_ascii(frame, cols, rows):
    """将视频帧转换为ASCII字符画"""
    # 转换为灰度图像
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # 获取原始尺寸和宽高比
    height, width = gray.shape
    aspect_ratio = width / height
    
    # 计算新尺寸（保持宽高比）
    new_width = cols
    new_height = int(new_width / aspect_ratio)
    new_width = new_width*2
    
    # 确保不超过终端行数限制
    if new_height > rows:
        new_height = rows
        new_width = int(new_height * aspect_ratio)
        new_width = new_width*2
    
    # 调整图像大小
    resized = cv2.resize(gray, (new_width, new_height), interpolation=cv2.INTER_AREA)
    
    # 标准化到0-1范围并映射到字符索引
    normalized = resized / 255.0
    indices = (normalized * (len(ASCII_CHARS)-1)).astype(np.uint8)
    
    # 生成ASCII艺术
    ascii_matrix = np.take(ASCII_CHARS, indices)
    return '\n'.join([''.join(row) for row in ascii_matrix])

def play_background_music(file_path):
    """
    在后台播放音乐
    """
    # 初始化 pygame
    pygame.mixer.init()
    
    try:
        # 加载并播放音乐
        pygame.mixer.music.load(file_path)
        pygame.mixer.music.play()
    except pygame.error as e:
        print(f"Error playing music: {e}")

def main():
    # 输入视频路径
    video_path = 'badapple.mp4'

    # 音频路径
    audio_path = 'badapple.mp3'
    
    # 打开视频文件
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print("无法打开视频文件")
        return
    
    # 创建输出目录
    output_dir = "ascii_frames"
    os.makedirs(output_dir, exist_ok=True)
    
    # 播放背景音乐
    play_background_music(audio_path)

    try:
        frame_count = 0
        prev_size = (0, 0)
        
        while True:
            # 获取当前终端尺寸
            cols, rows = shutil.get_terminal_size()
            
            # 读取视频帧
            ret, frame = cap.read()
            if not ret:
                break  # 视频结束
            
            # 转换帧为ASCII
            ascii_art = convert_frame_to_ascii(frame, cols, rows)
            
            # 清屏并显示新帧
            sys.stdout.write("\033[H\033[J")  # 清屏
            sys.stdout.write(ascii_art)
            sys.stdout.flush()
            
            # 保存到文件（每帧单独保存）
            if cols != prev_size[0] or rows != prev_size[1]:
                frame_count = 0  # 分辨率变化时重置计数
                prev_size = (cols, rows)
            
            with open(f"{output_dir}/frame_{frame_count:04d}.txt", "w") as f:
                f.write(ascii_art)
            
            # 控制播放速度
            fps = cap.get(cv2.CAP_PROP_FPS)
            delay = max(1/(fps+1), 0.01)  # 防止除零错误
            time.sleep(delay)
            
            frame_count += 1

    except KeyboardInterrupt:
        print("\n播放已终止")
    finally:
        cap.release()
        print(f"所有帧已保存到 {output_dir} 目录")

if __name__ == "__main__":
    main()