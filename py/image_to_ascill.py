import cv2
import numpy as np
import argparse

ASCII_CHARS = [' ', '.', ':', '-', '=', '+', '*', '#', '%', '@']

def convert_image(image_path, output_path, max_width=None):
    # 读取图片并转换为灰度图
    img = cv2.imread(image_path)
    if img is None:
        raise ValueError("无法读取图片文件")
    
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # 自动调整宽度（保持宽高比）
    height, width = gray.shape
    if max_width and width > max_width:
        new_width = max_width
        new_height = int(height * (new_width / width))
        gray = cv2.resize(gray, (new_width, new_height), interpolation=cv2.INTER_AREA)
    
    # 转换灰度值为ASCII字符
    normalized = gray / 255.0
    indices = (normalized * (len(ASCII_CHARS)-1)).astype(np.uint8)
    ascii_art = '\n'.join([''.join(ASCII_CHARS[i] for i in row) for row in indices])
    
    # 保存结果
    with open(output_path, 'w') as f:
        f.write(ascii_art)
    print(f"转换成功！保存至 {output_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='图片转字符画工具')
    parser.add_argument('input', help='输入图片路径')
    parser.add_argument('-o', '--output', default='output.txt', help='输出文件路径')
    parser.add_argument('-w', '--width', type=int, help='最大输出宽度（自动保持宽高比）')
    args = parser.parse_args()
    
    convert_image(args.input, args.output, args.width)