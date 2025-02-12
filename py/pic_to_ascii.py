import cv2
import os

def image_to_ascii(input_path, output_path):
    # 读取图片
    image = cv2.imread(input_path)
    if image is None:
        print("Error: Could not read image file.")
        return
    
    # 将图片转换为灰度图像
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # 调整图像大小以适应ASCII字符画输出宽度
    new_width = 80  # 输出的ASCII字符画宽度
    new_height = int(new_width * (gray_image.shape[0] / gray_image.shape[1]))
    resized_gray_image = cv2.resize(gray_image, (new_width, new_height))
    
    # ASCII字符集合，用于灰度到字符的映射
    ascii_chars_ = [' ', '.', '`', '^', ',', ':', ';', 'I', 'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?', ']', 
                   '[', '}', '{', '1', ')', '(', '|', '\\', '/', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z', 
                   'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o', 
                   '*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$']
    ascii_chars = [' ', '.', ':', '-', '=', '+', '*', '#', '%', '@']
    char_step = len(ascii_chars) / 256
    
    # 将灰度值映射为ASCII字符
    ascii_image = []
    for row in resized_gray_image:
        ascii_row = []
        for pixel in row:
            char_index = int(pixel * char_step)
            ascii_row.append(ascii_chars[char_index])
        ascii_image.append(''.join(ascii_row))
    
    # 保存为TXT文件
    with open(output_path, 'w') as f:
        for line in ascii_image:
            f.write(line + "\n")
    
    print(f"ASCII image saved to: {output_path}")

if __name__ == "__main__":
    input_image = "py\\pic\\cline.jpg"  # 输入图片文件名
    output_txt = "image_ascii.txt"   # 输出txt文件名

    image_to_ascii(input_image, output_txt)

    print("\nDone!")