# assignment: programming assignment 5
# author: Morgan Stein
# date: 6 December 2023
# file: chess.py is a program that takes user input of a piece and coordinate on a chess board and shows the possible moves that piece can make
# input: the user inputs a letter corresponding to a chess piece (K-king, Q-Queen, N-Knight, R- Rook, B-Bishop) and a cordinate on the board (a1-h8)
# output: the program prints a picture of the chess board with the letter representing the chess piece and 'x's to show the places on the board where the piece can move
class Board:
    def __init__(self):
        self.board = {}
        self.empty()
    def empty(self):
        for col in 'abcdefgh':
            for row in '12345678':
                self.board[col+row] = ' '
    def set(self, pos, piece):
        if pos in self.board.keys():
            self.board[pos] = piece
    def get_keys(self):
        return self.board.keys()
    def draw(self):
        print("   a   b   c   d   e   f   g   h\n +---+---+---+---+---+---+---+---+")
        count = 8
        for i in range (count):
            row=count
            print(f"{count}|",end="")
            for col in 'abcdefgh':
                piece = self.board[col+str(row)]
                if piece == ' ':
                    print("   |", end="")
                else:
                    print(f" {piece} |", end="")
            print("\n +---+---+---+---+---+---+---+---+")
            count-=1
        print("   a   b   c   d   e   f   g   h")
        pass
class Chess_Piece:
    def __init__(self, board, pos, color='white'):
        self.position = self.get_index(pos)
        self.color = color
        board.set(pos, self.get_name())
    def get_index(self, pos):
        return ('abcdefgh'.index(pos[0].lower()), '12345678'.index(pos[1]))
    def get_name(self):
        return self.name(self)
    def moves(self, board):
        pass
class Rook (Chess_Piece):
    def get_name(self):
        return 'R'
    def __init__(self,board,pos,color='white'):
        super().__init__(board,pos,color)
        self.pos = self.get_index(pos)
    def moves(self, board):
        for key in board.get_keys():
            row, col = self.get_index(key)
            if row == self.pos[0] and col == self.pos[-1]:
                continue
            elif row == self.pos[0]:
                board.set(key,'x')
            elif col == self.pos[-1]:
                board.set(key,'x')
class Bishop(Chess_Piece):
    def get_name(self):
        return 'B'
    def __init__(self,board,pos,color='white'):
        super().__init__(board,pos,color)
        self.pos = self.get_index(pos)
    def moves(self, board):
        for key in board.get_keys():
            row, col = self.get_index(key)
            if row == self.pos[0] and col == self.pos[-1]:
                continue
            elif abs(row - self.pos[0]) == abs(col - self.pos[1]):
                board.set(key, 'x')
            
        
class King(Chess_Piece):
    def get_name(self):
        return 'K'
    def __init__(self,board,pos,color='white'):
        super().__init__(board,pos,color)
        self.pos = self.get_index(pos)
    def moves(self,board):
        for key in board.get_keys():
            row, col = self.get_index(key)
            if row == self.pos[0] and col == self.pos[-1]:
                continue
        row,col = self.position
        king_moves = [(row+1,col),(row-1,col),(row,col+1),(row,col-1),(row+1,col+1),(row-1,col-1),(row+1,col-1),(row-1,col+1)]
        for move in king_moves:
            if 0 <= move[0] < 8 and 0 <= move[1] < 8:
                key = f"{'abcdefgh'[move[0]]}{'12345678'[move[1]]}"
                board.set(key, 'x')
        
            
class Knight(Chess_Piece):
    def get_name(self):
        return 'N'
    def __init__(self,board,pos,color='white'):
        super().__init__(board,pos,color)
        self.pos = self.get_index(pos)
    def moves(self,board):
        for key in board.get_keys():
            row, col = self.get_index(key)
            if row == self.pos[0] and col == self.pos[-1]:
                board.set(key,Knight.get_name(Knight))
        row,col =self.position
        knight_moves = [(row+2,col+1),(row+2,col-1),(row-2,col+1),(row-2,col-1),(row+1,col+2),(row+1,col-2),(row-1,col+2),(row-1,col-2)]
        for move in knight_moves:
            if 0 <= move[0] < 8 and 0 <= move[1] < 8:
                key = f"{'abcdefgh'[move[0]]}{'12345678'[move[1]]}"
                board.set(key, 'x')
class Queen(Chess_Piece):
    def get_name(self):
        return 'Q'
    def __init__(self,board,pos,color='white'):
        super().__init__(board,pos,color)
        self.pos = self.get_index(pos)
    def moves(self,board):
        for key in board.get_keys():
            row, col = self.get_index(key)
            if row == self.pos[0] and col == self.pos[-1]:
                continue
            elif abs(row - self.pos[0]) == abs(col - self.pos[1]):
                board.set(key, 'x')
            elif row == self.pos[0]:
                board.set(key,'x')
            elif col == self.pos[-1]:
                board.set(key,'x')
            
        
        
if __name__ == '__main__':
    print('Welcome to the Chess Game!')
    board = Board()
    board.draw()
    while True:
        user_input = str(input('Enter a chess piece and its position or type X to exit:\n'))
        answer = str(user_input.upper())
        if answer == 'X':
            print("Goodbye!")
            break
        elif len(answer) != 3:
            continue
        elif answer[0] in 'KQNBR' and answer[1].lower() in 'abcdefgh' and answer[2] in '12345678':
            if answer[0] == 'R':
                rook_name = Rook.get_name(Rook)
                board.set(answer[1:],rook_name)
                rook = Rook(board,answer[1:])
                rook.moves(board)
            elif answer[0]=='B':
                bishop_name = Bishop.get_name(Bishop)
                board.set(answer[1:],bishop_name)
                bishop = Bishop(board,answer[1:])
                bishop.moves(board)
            elif answer[0]=='K':
                king_name = King.get_name(King)
                board.set(answer[1:],king_name)
                king = King(board,answer[1:])
                king.moves(board)
            elif answer[0]=='Q':
                queen_name = Queen.get_name(Queen)
                board.set(answer[1:],queen_name)
                queen = Queen(board,answer[1:])
                queen.moves(board)
            elif answer[0]=='N':
                knight_name = Knight.get_name(Knight)
                board.set(answer[1:],knight_name)
                knight = Knight(board,answer[1:])
                knight.moves(board)
                
            else:
                continue
        else:
            continue
        board.draw()
        board.empty()
 
