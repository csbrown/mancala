#include<stdio.h>
#include<string.h>

#define LENGTH 3

struct Vertex;

typedef struct Vertex Vertex;

struct Vertex {int sides[2][LENGTH]; int ends[2]; Vertex *children; Vertex *parent;};

int has_more(struct Vertex *node, int side) {
    int i;
    int more = 0;
    for (i=0;i<LENGTH;i++)
        more += (*node).sides[side][i];
    return more;
} 

/* gets a winner when a node is end of game */
int get_winner(struct Vertex *node) {
    /*load all pieces into ends*/
    int i;
    int j;
    for (j=0;j<2;j++)
        for (i=0;i<LENGTH;i++) {
            (*node).ends[j] += (*node).sides[j][i];
            (*node).sides[j][i] = 0;
        }

        

    if ((*node).ends[0] < (*node).ends[1])
        return 1;
    else if ((*node).ends[1] < (*node).ends[0])
        return 0;
    else
        return -1; 
}

/* determines if a player is a definite winner at intermediate stage.  The current player just needs ONE child to be a definite win.  The opposing player needs ALL children to be definite wins */
int winning_player(int children_wins[LENGTH], int side) {
    int opposingside = (side + 1)%2;
    int i;
    /*if player can force a win, he forces a win*/
    for (i = 0; i < LENGTH; i++)
        if (children_wins[i] == side)
            return side;
    /*if player cannot win, but can force a tie, he forces a tie*/
    for (i = 0; i < LENGTH; i++)
        if (children_wins[i] == -1)
            return -1;
    /*if player cannot force a win or force a tie, then the opposing player must win*/
    return opposingside;
}
    
/*returns whose move it is now*/
int move(Vertex *node, int side, int place) {

    int playerside = side;
    int opposingside = (side + 1)%2;
    int handfull = (*node).sides[side][place];
    (*node).sides[side][place] = 0;
    place += 1;

    while (handfull) {
        if ((place+1)%(LENGTH+1) ) {
            (*node).sides[side][place] += 1;
            place += 1;
            handfull -= 1;
        } else if (playerside == side) {
            (*node).ends[side] += 1;
            place = 0;
            side = opposingside;
            handfull -= 1;
        } else {
            place = 0;
            side = playerside;
        }
    }

    /*if ending in an empty spot, take yours and all across from it*/
    if (playerside == side && place != 0)
        if ((*node).sides[side][place-1] == 1) {
            (*node).ends[side] += (*node).sides[side][place-1] + (*node).sides[opposingside][LENGTH - place];
            (*node).sides[side][place-1] = 0;
            (*node).sides[opposingside][LENGTH - place] = 0;
        }

    /*if ending in own end, go again*/
    if (side == opposingside && place == 0)
        return playerside; 
    else
        return opposingside;
}

int is_full(Vertex *node, int side, int place) {
    if ((*node).sides[side][place])
        return 1;
    else
        return 0;
}

Vertex init_child(Vertex *node, Vertex *children) {
    int i;
    int j;
    Vertex child = (*node);
    for (j = 0; j < 2; j++) {
        for (i = 0; i < LENGTH; i++)
            child.sides[j][i] = (*node).sides[j][i];
        child.ends[j] = (*node).ends[j];
    }
    child.children = children;
    child.parent = node;
    return child;
}

void print_board(Vertex *node) {
    int i;
    printf("\n\n  ");
    for (i = 0; i<LENGTH; i++)
        printf("%d ", (*node).sides[0][LENGTH -1 -i]);
    printf("\n%d ", (*node).ends[0]);
    for (i=0; i<LENGTH;i++)
        printf("  ");
    printf("%d\n  ", (*node).ends[1]);
    for (i = 0; i<LENGTH; i++)
        printf("%d ", (*node).sides[1][i]);
    printf("\n");
    return;
}


/* This returns whether this child is a definite win and for which side. -1 = no definite win, 0 = side 0 win, 1 = side 1 win */
int depth_first(Vertex *node, int side) {
    int i;
    int opposingside = (side + 1)%2;

    int child_wins[LENGTH];
    for (i=0;i<LENGTH;i++)
        child_wins[i] = -2;

    int winner = -2;
    int nextplayer;
    /*end of game*/
    if (!has_more(node, opposingside) || !has_more(node, side)) {
        winner = get_winner(node);
        printf("\n============================");
        printf("\n%d's turn, %d wins! \n", side, winner);
        print_board(node);
        printf("============================\n");
        return winner;
    }

    /*recurse into all possible moves*/
    for (i = 0; i < LENGTH; i++) {
        if (is_full(node, side, i)) {
            /*initializes a child to be just like parent, then moves the child*/  
            Vertex children[LENGTH];
            *((*node).children + i) = init_child(node, children);
            nextplayer = move((*node).children + i, side, i);
            child_wins[i] = depth_first((*node).children + i, nextplayer);
        }
    }

    /*figure out who can win with a perfect strategy at this point*/
    winner = winning_player(child_wins, side);
    printf("\n---------------------------------------");
    printf("\n%d's turn, player %d is winning!\n", side, winner);
    for (i=0;i<LENGTH;i++)
        printf("%d ", child_wins[i]);
    print_board(node);
    printf("---------------------------------------\n");
    return winner; 
}
                
int main(int argc, char **argv) {

    printf("what?");
    int winner;
    Vertex children[LENGTH];
    Vertex head = {.sides = {{4,4,4}, {4,4,4}}, .ends = {0}, .children = children, .parent = NULL};
    winner = depth_first(&head, 1);
    printf("\nside %d has a winning strategy!", winner);
    return 0;
}
    
    
    
    
    

