import os

def get_terminal_size(max_width=80, max_height=24):
    """
    获取当前终端的行数和列数，如果超出范围或不可用，则返回最大值。
    
    :param max_width: 列数最大值
    :param max_height: 行数最大值
    :return: 终端的列数和行数
    """
    try:
        # 尝试获取终端大小
        size = os.get_terminal_size()
        columns = size.columns
        lines = size.lines
        
        # 检查是否超出范围
        if columns <= 0:
            columns = max_width
        if lines <= 0:
            lines = max_height
        
        return columns, lines
    except OSError:
        # 如果无法获取终端大小（例如，终端不可用），使用最大值
        return max_width, max_height

# 示例用法
columns, lines = get_terminal_size()
print(f"Terminal width: {columns} columns")
print(f"Terminal height: {lines} lines")