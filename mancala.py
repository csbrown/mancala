class board:

    def __init__(self):
        self.board = [[4,4,4,4,4,4,0],[4,4,4,4,4,4,0]]

    def move(self, side, place):
        handfull = self.board[side][place]
        self.board[side][place] = 0
        playerside = side
        place += 1
        while handfull:
            if place < 6 or (playerside == side and place == 6): 
                self.board[side][place] += 1
                handfull -= 1
                place += 1
            else:
                side = (side + 1)%2
                place = 0

        if place < 7 and self.board[side][place - 1] == 1 and playerside == side:
            self.board[playerside][6] += self.board[side][place - 1] + self.board[(side + 1)%2][6 - place]
            self.board[side][place - 1] = 0
            self.board[(side + 1)%2][6-place] = 0

        if place == 7 and side == playerside:
            return playerside
        else:
            return (playerside + 1)%2

    def check_winner(self):
        empty1 = 0
        empty2 = 0
        gameover = False
        for i in range(len(self.board[0])-1):
            empty1+=self.board[0][i]
            empty2+=self.board[1][i]

        #check for game_over
        if not empty1:
            self.board[1][6] += empty2
            gameover = True
        if not empty2:
            self.board[0][6] += empty1
            gameover = True

        if gameover:
            if self.board[0][6] < self.board[1][6]:
                return 2
            else:
                return 1

        return 0
            

    def printboard(self):
        side1 = '  '
        side2 = '  '
        ends = str(self.board[1][6])
        for i in range(len(self.board[0])-1):
            side1 += str(self.board[0][i]) + " "
            side2 += str(self.board[1][5-i]) + " "
            ends += "  "
        ends += " " + str(self.board[0][6])
        print side1
        print ends
        print side2

def main():
    game = board()
    side = 0
    winner = 0
    while not winner:
        game.printboard()
        place = int(raw_input("player " + str(side + 1) + " to move... which space to move? ")) - 1
        side = game.move(side, place)
        winner = game.check_winner()
    print "player " + str(winner) + " wins!"
        

if __name__ == '__main__':
    main()
