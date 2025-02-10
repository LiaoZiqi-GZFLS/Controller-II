import pygame

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
    play_background_music('badapple.mp3')

if __name__ == '__main__':
    main()