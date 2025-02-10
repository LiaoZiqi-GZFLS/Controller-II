import cv2
import numpy as np
import shutil
import time
import sys
import os
import pygame

ASCII_CHARS = [' ', '.', ':', '-', '=', '+', '*', '#', '%', '@']

def convert_frame_to_ascii(frame, cols, rows):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    height, width = gray.shape
    aspect_ratio = width / height
    new_width = cols
    new_height = int(new_width / aspect_ratio)
    
    if new_height > rows:
        new_height = rows
        new_width = int(new_height * aspect_ratio)
    
    resized = cv2.resize(gray, (new_width * 2, new_height), interpolation=cv2.INTER_AREA)
    normalized = resized / 255.0
    indices = (normalized * (len(ASCII_CHARS)-1)).astype(np.uint8)
    ascii_matrix = np.take(ASCII_CHARS, indices)
    return '\n'.join([''.join(row) for row in ascii_matrix])

def play_background_music(file_path):
    pygame.mixer.init()
    try:
        pygame.mixer.music.load(file_path)
        pygame.mixer.music.play()
    except pygame.error as e:
        print(f"Error playing music: {e}")
        return False
    return True

def main():
    video_path = 'badapple.mp4'
    audio_path = 'badapple.mp3'
    
    if not os.path.exists(video_path) or not os.path.exists(audio_path):
        print("视频或音频文件不存在！")
        return
    
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print("无法打开视频文件")
        return
    
    output_dir = "ascii_frames"
    os.makedirs(output_dir, exist_ok=True)
    
    if not play_background_music(audio_path):
        print("无法播放音频！")
        return
    
    try:
        frame_count = 0
        prev_size = (0, 0)
        start_time = time.perf_counter()
        
        while True:
            cols, rows = shutil.get_terminal_size()
            
            # 根据音频播放时间调整视频帧的位置
            music_time = pygame.mixer.music.get_pos() / 1000  # 转换为秒
            video_time = (time.perf_counter() - start_time)
            time_diff = video_time - music_time
            
            # 调整视频播放位置以同步音频
            cap.set(cv2.CAP_PROP_POS_MSEC, music_time * 1000)
            
            ret, frame = cap.read()
            if not ret:
                break
            
            ascii_art = convert_frame_to_ascii(frame, cols, rows)
            
            sys.stdout.write("\033[H\033[J")
            sys.stdout.write(ascii_art)
            sys.stdout.flush()
            
            # 保存到文件
            with open(f"{output_dir}/frame_{frame_count:04d}.txt", "w") as f:
                f.write(ascii_art)
            
            # 计算下一帧的延迟，基于音频播放时间
            next_music_time = pygame.mixer.music.get_pos() / 1000 + 0.016  # 假设希望大约每 16ms 一帧
            sleep_time = next_music_time - time.perf_counter() + start_time
            if sleep_time > 0:
                time.sleep(sleep_time)
            
            frame_count += 1
    
    except KeyboardInterrupt:
        print("\n播放已终止")
    finally:
        cap.release()
        print(f"所有帧已保存到 {output_dir} 目录")

if __name__ == "__main__":
    main()