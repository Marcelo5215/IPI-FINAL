#include "game_tree.h"

char gridChar(int i) {
    //Caracteres a ser desenhados na tela
    switch(i) {
        case -1:
            return 'X';
        case 0:
            return ' ';
        case 1:
            return 'O';
    }

    return 'H';
}

//Desenha na tela
void draw(int b[9]) {
    printf(" %c | %c | %c\n",gridChar(b[0]),gridChar(b[1]),gridChar(b[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(b[3]),gridChar(b[4]),gridChar(b[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(b[6]),gridChar(b[7]),gridChar(b[8]));
}

int win(const int board[9]) {
    //determines if a player has won, returns 0 otherwise.
    //Combinacoes de vitoria possiveis:
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
    //Se o jogador ou o computador estiver jogado em algumas dessas 3 casas
        if(board[wins[i][0]] != 0 &&
           board[wins[i][0]] == board[wins[i][1]] &&
           board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    //Retorna 0 se ninguem tiver ganhado ainda
    return 0;
}

int minimax(int board[9], int player) {
    //How is the position like for player (their turn) on board?
    int winner = win(board);
    if(winner != 0) return winner*player;

    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    int i;
    for(i = 0; i < 9; ++i) {//For all moves,
        if(board[i] == 0) {//If legal,
            board[i] = player;//Try the move
            //Chama a pontuacao para o jogador recursivamente como uma arvore
            int thisScore = -minimax(board, player*-1);

            if(thisScore > score) {
                score = thisScore;
                move = i;
            }//Pick the one that's worst for the opponent
            board[i] = 0;//Reset board after try
        }
    }
    if(move == -1){
        return 0;
    }
    return score;
}

void computerMove(int board[9]) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
}

void playerMove(int board[9]) {
    //Armazena o movimento do jogador em uma casa igual a 0 (nao jogada)
    int move = 0;
    do {
        printf("\nInput move ([0..8]): ");
        scanf("%d", &move);
        printf("\n");
    } while (move >= 9 || (move < 0 && board[move] == 0));
    board[move] = -1;
}

// int banana() {
//     //Tabuleiro, inicialmente com 0s
//     int board[9] = {0,0,0,0,0,0,0,0,0};
//     //computer squares are 1, player squares are -1.
//     printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
//     int player=0;
//     scanf("%d",&player);
//     printf("\n");
//     unsigned turn;
//     //Enquanto nao tiverem 9 jogadas e ninguem tiver ganhado
//     for(turn = 0; turn < 9 && win(board) == 0; ++turn) {
//         if((turn+player) % 2 == 0)
//             computerMove(board);
//         else {
//             //Desenha o tabuleiro na tela e espera a jogada do jogador
//             draw(board);
//             playerMove(board);
//         }
//     }
//     switch(win(board)) {
//         //Se retornar 0 apos o laco terminar = empate
//         case 0:
//             printf("A draw. How droll.\n");
//             break;
//         //Se retornar 1 eh pq o computador tem 3 casas seguidas
//         case 1:
//             draw(board);
//             printf("You lose.\n");
//             break;
//         //Se retornar -1 = 3 casas seguidas pelo jogador
//         case -1:
//             printf("You win. Inconceivable!\n");
//             break;
//     }
// }
