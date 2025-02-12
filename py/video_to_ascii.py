import cv2
import os

def video_to_ascii(input_path, output_dir, frame_save_interval=100):
    # 创建输出目录
    os.makedirs(output_dir, exist_ok=True)
    
    # 初始化视频捕获
    cap = cv2.VideoCapture(input_path)
    
    if not cap.isOpened():
        print("Error: Could not open video file.")
        return
    
    # 获取视频属性
    frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS)
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    
    # 生成配置文件
    config_file = os.path.join(output_dir, "config.ini")
    with open(config_file, 'w') as f:
        f.write(f"[Video Information]\n")
        f.write(f"resolution={frame_width}x{frame_height}\n")
        f.write(f"fps={fps:.2f}\n")
    
    # ASCII字符集合，用于灰度到字符的映射
    ascii_chars = [' ', '.', '`', '^', ',', ':', ';', 'I', 'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?', ']', 
                   '[', '}', '{', '1', ')', '(', '|', '\\', '/', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z', 
                   'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o', 
                   '*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$']
    char_step = len(ascii_chars) / 256
    
    # 逐帧处理视频
    frame_count = 0
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        
        # 将帧转换为灰度图像
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 调整帧的宽度以适应ASCII字符画输出宽度
        new_width = 80  # 输出的ASCII字符画宽度
        new_height = int(new_width * (frame_height / frame_width))
        resized_gray_frame = cv2.resize(gray_frame, (new_width, new_height))
        
        # 将灰度值映射为ASCII字符
        ascii_frame = []
        for row in resized_gray_frame:
            ascii_row = []
            for pixel in row:
                char_index = int(pixel * char_step)
                ascii_row.append(ascii_chars[char_index])
            ascii_frame.append(''.join(ascii_row))
        
        # 保存为TXT文件
        output_path = os.path.join(output_dir, f"frame_{frame_count:04d}.txt")
        with open(output_path, 'w') as f:
            for line in ascii_frame:
                f.write(line + "\n")
        
        # 每处理frame_save_interval帧打印一条进度信息
        if frame_count % frame_save_interval == 0:
            print(f"Processed frame {frame_count} / {total_frames}")
        
        frame_count += 1
    
    cap.release()
    print(f"\nProcessing complete. ASCII frames saved in: {output_dir}")

if __name__ == "__main__":
    input_video = "py\\media\\badapple.mp4"  # 输入视频文件名
    output_dir = "ascii_frames"  # 输出目录

    # 如果不指定参数，使用默认值
    video_to_ascii(input_video, output_dir)

    print("\nDone!")