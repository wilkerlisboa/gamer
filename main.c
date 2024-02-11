#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

SDL_Window* window;
SDL_Renderer* renderer;

int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}; // 0 representa quadrado vazio, 1 para 'X', 2 para 'O'
int currentPlayer = 1; // Inicia com o jogador 'X'
int winner = 0; // 0 para nenhum vencedor, 1 para 'X', 2 para 'O', 3 para empate

void drawBoard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Linhas horizontais
    for (int i = 1; i < 3; ++i) {
        SDL_RenderDrawLine(renderer, 0, i * 200, 600, i * 200);
    }

    // Linhas verticais
    for (int i = 1; i < 3; ++i) {
        SDL_RenderDrawLine(renderer, i * 200, 0, i * 200, 600);
    }

    // Desenha X e O no tabuleiro
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 1) {
                // Desenha 'X'
                SDL_RenderDrawLine(renderer, j * 200, i * 200, (j + 1) * 200, (i + 1) * 200);
                SDL_RenderDrawLine(renderer, (j + 1) * 200, i * 200, j * 200, (i + 1) * 200);
            } else if (board[i][j] == 2) {
                // Desenha 'O' (círculo aproximado)
                int centerX = j * 200 + 100;
                int centerY = i * 200 + 100;
                int radius = 50;

                for (int angle = 0; angle <= 360; angle += 5) {
                    double radianAngle = angle * (M_PI / 180.0);
                    int x = centerX + (int)(radius * cos(radianAngle));
                    int y = centerY + (int)(radius * sin(radianAngle));

                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int checkWinner() {
    // Verifica linhas e colunas
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][0] == board[i][2]) {
            return board[i][0]; // Linha i completa
        }

        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[0][i] == board[2][i]) {
            return board[0][i]; // Coluna i completa
        }
    }

    // Verifica diagonais
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        return board[0][0]; // Diagonal principal
    }

    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[0][2] == board[2][0]) {
        return board[0][2]; // Diagonal secundária
    }

    // Verifica empate
    int isBoardFull = 1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 0) {
                isBoardFull = 0;
                break;
            }
        }
        if (!isBoardFull) {
            break;
        }
    }

    if (isBoardFull) {
        return 3; // Empate
    }

    return 0; // Nenhum vencedor ainda
}

void showMessage(const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fim de jogo", message, NULL);
    printf("%s\n", message); // Exibe a mensagem no terminal
}

void handleMouseClick(int x, int y) {
    if (winner != 0) {
        // O jogo já terminou
        return;
    }

    int row = y / 200;
    int col = x / 200;

    if (board[row][col] == 0) {
        // Marca o quadrado se estiver vazio
        board[row][col] = currentPlayer;

        winner = checkWinner();
        if (winner != 0) {
            // Exibe o vencedor ou empate
            if (winner == 3) {
                showMessage("Empate!");
            } else {
                char message[20];
                sprintf(message, "Jogador %c venceu!", (winner == 1) ? 'X' : 'O');
                showMessage(message);
            }

            SDL_Delay(2000); // Aguarda 2 segundos antes de fechar a janela
        } else {
            currentPlayer = (currentPlayer == 1) ? 2 : 1; // Alterna o jogador
        }

        drawBoard();
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Jogo da Velha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleMouseClick(x, y);
            }
        }

        drawBoard();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
