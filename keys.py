import pygame
import sys
#pip install pygame

# Inicializar o Pygame
pygame.init()

# Definir as dimensões da janela
window_width, window_height = 800, 600
window = pygame.display.set_mode((window_width, window_height))
pygame.display.set_caption('keys up down left rigth')

# Definir a cor
black = (0, 0, 0)
blue = (0, 0, 255)

# Definir as dimensões iniciais do retângulo
rect_width, rect_height = 100, 100
rect_x, rect_y = (window_width - rect_width) // 2, (window_height - rect_height) // 2

# Velocidade de movimento
move_speed = 10

# Loop principal do jogo
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Obter o estado das teclas
    keys = pygame.key.get_pressed()
    
    # Redimensionar o retângulo
    if keys[pygame.K_UP]:
        rect_width += 10
        rect_height += 10
    if keys[pygame.K_DOWN]:
        rect_width = max(10, rect_width - 10)
        rect_height = max(10, rect_height - 10)
    
    # Mover o retângulo
    if keys[pygame.K_LEFT]:
        rect_x = max(-rect_width, rect_x - move_speed)
    if keys[pygame.K_RIGHT]:
        rect_x = min(window_width, rect_x + move_speed)

    # Limpar a tela
    window.fill(black)
    
    # Desenhar o retângulo
    pygame.draw.rect(window, blue, (rect_x, rect_y, rect_width, rect_height))
    
    # Atualizar a tela
    pygame.display.flip()

    # Controlar a taxa de atualização
    pygame.time.Clock().tick(30)

# Encerrar o Pygame
pygame.quit()
sys.exit()

