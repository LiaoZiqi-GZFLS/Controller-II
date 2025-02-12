def display_ascii_image(file_path):
    # 读取TXT文件内容
    with open(file_path, 'r') as file:
        ascii_content = file.readlines()

    # 去除行尾的换行符
    ascii_content = [line.strip() + '\n' for line in ascii_content]

    # 确保每一行的长度一致（可选）
    max_length = max(len(line) for line in ascii_content)
    ascii_content = [line.ljust(max_length).rstrip('\n') + '\n' for line in ascii_content]

    # 打印ASCII字符画
    print("-" * max_length)  # 分隔线
    for line in ascii_content:
        print(line.rstrip('\n'))  # 使用rstrip去掉行尾的换行符，避免多打印一个空行
    print("-" * max_length)  # 分隔线

if __name__ == "__main__":
    # 指定要显示的ASCII字符画TXT文件路径
    #ascii_file = "ascii_frames/frame_1000.txt"
    ascii_file = "image_ascii.txt"
    display_ascii_image(ascii_file)