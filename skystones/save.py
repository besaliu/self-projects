import pygame
import sys

pygame.init()
screen_info = pygame.display.Info()
WIDTH, HEIGHT = screen_info.current_w, screen_info.current_h - 60
CELL_SIZE = min(WIDTH // 3, HEIGHT // 3)
FPS = 60

WHITE = (255, 255, 255)
BEIGE = (210, 180, 140)
BLACK = (0, 0, 0)
selected = False
digit = 0
yourscore = 0
cpuscore = 0
snake_images = {
    'snake1.jpg': {'name': 'snake1.jpg', 'left': 1, 'right': 0, 'top': 2, 'bottom': 0},
    'snake2.jpg': {'name': 'snake2.jpg', 'left': 2, 'right': 2, 'top': 0, 'bottom': 2},
    'snake3.jpg': {'name': 'snake3.jpg', 'left': 0, 'right': 3, 'top': 2, 'bottom': 3},
    'snake4.jpg': {'name': 'snake4.jpg', 'left': 3, 'right': 2, 'top': 3, 'bottom': 0},
    'snake5.jpg': {'name': 'snake5.jpg', 'left': 0, 'right': 4, 'top': 1, 'bottom': 2},
}

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("PyStones")
font = pygame.font.Font(None, 36)

board = [[[] for _ in range(3)] for _ in range(3)]


def can_capture(snake_image, stone, direction):
    if direction == 'top':
        return stone['top'] > snake_images[snake_image]['bottom']
    if direction == 'bottom':
        return stone['bottom'] > snake_images[snake_image]['top']
    if direction == 'left':
        return stone['left'] < snake_images[snake_image]['right']
    if direction == 'right':
        return stone['right'] > snake_images[snake_image]['left']


def printboard(board):
    for row in board:
        print(row)


def draw(board):
    for row in range(3):
        for col in range(3):
            pygame.draw.rect(screen, BEIGE, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE), 10)
            img_data = board[row][col]
            if img_data:
                img_path, owner = img_data
                img = pygame.image.load(img_path)
                img = pygame.transform.scale(img, (CELL_SIZE - 20, CELL_SIZE - 20))
                screen.blit(img, (col * CELL_SIZE + 10, row * CELL_SIZE + 10))


def hasStone(board, row, col, direction):
    if direction == 'left':
        if col > 0 and board[row][col - 1]:
            return board[row][col - 1]
    elif direction == 'right':
        if col < len(board[0]) - 1 and board[row][col + 1]:
            return board[row][col + 1]
    elif direction == 'top':
        if row > 0 and board[row - 1][col]:
            return board[row - 1][col]
    elif direction == 'bottom':
        if row < len(board) - 1 and board[row + 1][col]:
            return board[row + 1][col]
    return None


def get_clicked(pos):
    col = pos[0] // CELL_SIZE
    row = pos[1] // CELL_SIZE
    return row, col


p1 = True
clock = pygame.time.Clock()
turntext = font.render("Your turn!", True, BEIGE)

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

        if event.type == pygame.KEYDOWN:
            if pygame.K_0 <= event.key <= pygame.K_9:
                digit = int(pygame.key.name(event.key))
                selected = True

        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1 and selected:
                mouse_pos = pygame.mouse.get_pos()
                clicked_row, clicked_col = get_clicked(mouse_pos)

                # Check if the clicked position is within the valid range
                if 0 <= clicked_row < 3 and 0 <= clicked_col < 3:
                    if not board[clicked_row][clicked_col]:
                        snake_image = f'snake{digit}.jpg'
                        board[clicked_row][clicked_col] = [snake_image, 'you' if p1 else 'cpu']

                        top = hasStone(board, clicked_row, clicked_col, 'top')
                        bottom = hasStone(board, clicked_row, clicked_col, 'bottom')
                        left = hasStone(board, clicked_row, clicked_col, 'left')
                        right = hasStone(board, clicked_row, clicked_col, 'right')

                        if top:
                            if can_capture(snake_image, snake_images[top[0]], 'top') and board[clicked_row][clicked_col][1] != top[1]:
                                print("You captured the stone on top of you")
                                if p1:
                                    yourscore += 1
                                    cpuscore -= 1
                                    top[1] = 'you'
                                else:
                                    cpuscore += 1
                                    yourscore =- 1
                                    top[1] = 'cpu'

                        elif bottom:
                            if can_capture(snake_image, snake_images[bottom[0]], 'bottom') and board[clicked_row][clicked_col][1] != bottom[1]:
                                print("You captured the stone below of you")
                                if p1:
                                    yourscore += 1
                                    cpuscore -= 1
                                    bottom[1] = 'you'
                                else:
                                    cpuscore += 1
                                    yourscore -= 1
                                    bottom[1] = 'cpu'

                        if left:
                            if can_capture(snake_image, snake_images[left[0]], 'left') and board[clicked_row][clicked_col][1] != left[1]:
                                print("You captured the stone on your left")
                                if p1:
                                    yourscore += 1
                                    cpuscore -= 1
                                    left[1] = 'you'
                                else:
                                    cpuscore += 1
                                    yourscore -= 1
                                    left[1] = 'cpu'

                        elif right:
                            if can_capture(snake_image, snake_images[right[0]], 'right') and board[clicked_row][clicked_col][1] != right[1]:
                                print("You captured the stone on your right")
                                if p1:
                                    yourscore += 1
                                    cpuscore -= 1
                                    right[1] = 'you'
                                else:
                                    cpuscore += 1
                                    yourscore -= 1
                                    right[1] = 'cpu'

                        selected = False
                        if p1:
                            yourscore += 1
                            p1 = not p1
                        else:
                            cpuscore += 1
                            p1 = not p1
                        if yourscore < 0:
                            yourscore = 0
                        if cpuscore < 0:
                            cpuscore = 0

                    else:
                        print("Cell already occupied.")
                else:
                    print("Clicked outside the valid range of the board.")

    screen.fill(BLACK)
    if p1:
        screen.blit(turntext, (WIDTH - 200, HEIGHT - 500))

    text_surface1 = font.render("Your Score: {}".format(yourscore), True, WHITE)
    text_rect1 = text_surface1.get_rect()
    text_rect1.center = (WIDTH - 200, HEIGHT - 300)
    screen.blit(text_surface1, text_rect1)

    text_surface2 = font.render("Opponent Score: {}".format(cpuscore), True, WHITE)
    text_rect2 = text_surface2.get_rect()
    text_rect2.center = (WIDTH - 200, HEIGHT - 200)
    screen.blit(text_surface2, text_rect2)

    draw(board)
    pygame.display.flip()
    clock.tick(FPS)