# cse20
# pa5 Chess
# test_chess.py evaluates chess.py

import re
import os
import io
import sys

if __name__ == '__main__':
    
    command = 'python3' # may need to change to py or python!!!
    score = 0
    module = 'chess.py'
    total = 40
    classes = ['Board', 'Rook', 'Bishop', 'Queen', 'King', 'Knight']
    method = ['draw', 'get_name', 'moves']

#1 check the module
    try:
        print(f'\nTesting the presence of {module} ...')
        import chess
        print(f'\t{module} is present +1/1 points')
        score += 1
    except:
        print(f'\t{module} is not present +0/{total} points')
    else:

#2 check class Board
        try:
            print(f'\nTesting class {classes[0]} ...')
            board = chess.Board()
            print(f'\t{classes[0]} is implemented +1/1 points')
            score += 1
        except:
            print(f'\t{classes[0]} is not implemented +0/1 points')

#3 check class Board method draw
        try:
            stdout = sys.stdout
            sys.stdout = io.StringIO()  
            board.draw()
            output = sys.stdout.getvalue().split('\n')
            sys.stdout = stdout  
            
            out = '''   a   b   c   d   e   f   g   h
 +---+---+---+---+---+---+---+---+
8|   |   |   |   |   |   |   |   |8
 +---+---+---+---+---+---+---+---+
7|   |   |   |   |   |   |   |   |7
 +---+---+---+---+---+---+---+---+
6|   |   |   |   |   |   |   |   |6
 +---+---+---+---+---+---+---+---+
5|   |   |   |   |   |   |   |   |5
 +---+---+---+---+---+---+---+---+
4|   |   |   |   |   |   |   |   |4
 +---+---+---+---+---+---+---+---+
3|   |   |   |   |   |   |   |   |3
 +---+---+---+---+---+---+---+---+
2|   |   |   |   |   |   |   |   |2
 +---+---+---+---+---+---+---+---+
1|   |   |   |   |   |   |   |   |1
 +---+---+---+---+---+---+---+---+
   a   b   c   d   e   f   g   h
'''.split('\n')
            for i in range(len(out)):
                assert output[i].strip() == out[i].strip()
            print(f'\t{method[0]} is implemented correctly +2/2 points')
            score += 2
        except:
            sys.stdout = stdout 
            print(f'\t{method[0]} is not implemented correctly +0/2 points')
            
#4 check class Rook
        try:
            print(f'\nTesting class {classes[1]} ...')
            rook = chess.Rook(board, 'd5')
            assert rook.get_name() == 'R'
            print(f'\t{method[1]} is implemented correctly +1/1 points')
            score += 1
        except:
            print(f'\t{method[1]} is not implemented correctly +0/1 points')

#5 check class Rook method moves
        try:
            rook.moves(board)
            assert board.board == {'a1': ' ', 'a2': ' ', 'a3': ' ', 'a4': ' ',
                                   'a5': 'x', 'a6': ' ', 'a7': ' ', 'a8': ' ',
                                   'b1': ' ', 'b2': ' ', 'b3': ' ', 'b4': ' ',
                                   'b5': 'x', 'b6': ' ', 'b7': ' ', 'b8': ' ',
                                   'c1': ' ', 'c2': ' ', 'c3': ' ', 'c4': ' ',
                                   'c5': 'x', 'c6': ' ', 'c7': ' ', 'c8': ' ',
                                   'd1': 'x', 'd2': 'x', 'd3': 'x', 'd4': 'x',
                                   'd5': 'R', 'd6': 'x', 'd7': 'x', 'd8': 'x',
                                   'e1': ' ', 'e2': ' ', 'e3': ' ', 'e4': ' ',
                                   'e5': 'x', 'e6': ' ', 'e7': ' ', 'e8': ' ',
                                   'f1': ' ', 'f2': ' ', 'f3': ' ', 'f4': ' ',
                                   'f5': 'x', 'f6': ' ', 'f7': ' ', 'f8': ' ',
                                   'g1': ' ', 'g2': ' ', 'g3': ' ', 'g4': ' ',
                                   'g5': 'x', 'g6': ' ', 'g7': ' ', 'g8': ' ',
                                   'h1': ' ', 'h2': ' ', 'h3': ' ', 'h4': ' ',
                                   'h5': 'x', 'h6': ' ', 'h7': ' ', 'h8': ' '}
            print(f'\t{method[2]} is implemented correctly +2/2 points')
            score += 2
        except:
            print(f'\t{method[2]} is not implemented correctly +0/2 points')
        
#6 check class Bishop
        try:
            board.empty()
            print(f'\nTesting class {classes[2]} ...')
            bishop = chess.Bishop(board, 'e5')
            assert bishop.get_name() == 'B'
            print(f'\t{method[1]} is implemented correctly +1/1 points')
            score += 1
        except:
            print(f'\t{method[1]} is not implemented correctly +0/1 points')

#7 check class Bishop method moves
        try:
            bishop.moves(board)
            assert board.board == {'a1': 'x', 'a2': ' ', 'a3': ' ', 'a4': ' ',
                                   'a5': ' ', 'a6': ' ', 'a7': ' ', 'a8': ' ',
                                   'b1': ' ', 'b2': 'x', 'b3': ' ', 'b4': ' ',
                                   'b5': ' ', 'b6': ' ', 'b7': ' ', 'b8': 'x',
                                   'c1': ' ', 'c2': ' ', 'c3': 'x', 'c4': ' ',
                                   'c5': ' ', 'c6': ' ', 'c7': 'x', 'c8': ' ',
                                   'd1': ' ', 'd2': ' ', 'd3': ' ', 'd4': 'x',
                                   'd5': ' ', 'd6': 'x', 'd7': ' ', 'd8': ' ',
                                   'e1': ' ', 'e2': ' ', 'e3': ' ', 'e4': ' ',
                                   'e5': 'B', 'e6': ' ', 'e7': ' ', 'e8': ' ',
                                   'f1': ' ', 'f2': ' ', 'f3': ' ', 'f4': 'x',
                                   'f5': ' ', 'f6': 'x', 'f7': ' ', 'f8': ' ',
                                   'g1': ' ', 'g2': ' ', 'g3': 'x', 'g4': ' ',
                                   'g5': ' ', 'g6': ' ', 'g7': 'x', 'g8': ' ',
                                   'h1': ' ', 'h2': 'x', 'h3': ' ', 'h4': ' ',
                                   'h5': ' ', 'h6': ' ', 'h7': ' ', 'h8': 'x'}
            print(f'\t{method[2]} is implemented correctly +2/2 points')
            score += 2
        except:
            print(f'\t{method[2]} is not implemented correctly +0/2 points')

#8 check class Queen
        try:
            board.empty()
            print(f'\nTesting class {classes[3]} ...')
            queen = chess.Queen(board, 'e4')
            assert queen.get_name() == 'Q'
            print(f'\t{method[1]} is implemented correctly +1/1 points')
            score += 1
        except:
            print(f'\t{method[1]} is not implemented correctly +0/1 points')

#9 check class Queen method moves
        try:
            queen.moves(board)
            assert board.board == {'a1': ' ', 'a2': ' ', 'a3': ' ', 'a4': 'x',
                                   'a5': ' ', 'a6': ' ', 'a7': ' ', 'a8': 'x',
                                   'b1': 'x', 'b2': ' ', 'b3': ' ', 'b4': 'x',
                                   'b5': ' ', 'b6': ' ', 'b7': 'x', 'b8': ' ',
                                   'c1': ' ', 'c2': 'x', 'c3': ' ', 'c4': 'x',
                                   'c5': ' ', 'c6': 'x', 'c7': ' ', 'c8': ' ',
                                   'd1': ' ', 'd2': ' ', 'd3': 'x', 'd4': 'x',
                                   'd5': 'x', 'd6': ' ', 'd7': ' ', 'd8': ' ',
                                   'e1': 'x', 'e2': 'x', 'e3': 'x', 'e4': 'Q',
                                   'e5': 'x', 'e6': 'x', 'e7': 'x', 'e8': 'x',
                                   'f1': ' ', 'f2': ' ', 'f3': 'x', 'f4': 'x',
                                   'f5': 'x', 'f6': ' ', 'f7': ' ', 'f8': ' ',
                                   'g1': ' ', 'g2': 'x', 'g3': ' ', 'g4': 'x',
                                   'g5': ' ', 'g6': 'x', 'g7': ' ', 'g8': ' ',
                                   'h1': 'x', 'h2': ' ', 'h3': ' ', 'h4': 'x',
                                   'h5': ' ', 'h6': ' ', 'h7': 'x', 'h8': ' '}
            print(f'\t{method[2]} is implemented correctly +2/2 points')
            score += 2
        except:
            print(f'\t{method[2]} is not implemented correctly +0/2 points')
        
#10 check class King
        try:
            board.empty()
            print(f'\nTesting class {classes[4]} ...')
            king = chess.King(board, 'e5')
            assert king.get_name() == 'K'
            print(f'\t{method[1]} is implemented correctly +1/1 points')
            score += 1
        except:
            print(f'\t{method[1]} is not implemented correctly +0/1 points')

#11 check class King method moves
        try:
            king.moves(board)
            assert board.board == {'a1': ' ', 'a2': ' ', 'a3': ' ', 'a4': ' ',
                                   'a5': ' ', 'a6': ' ', 'a7': ' ', 'a8': ' ',
                                   'b1': ' ', 'b2': ' ', 'b3': ' ', 'b4': ' ',
                                   'b5': ' ', 'b6': ' ', 'b7': ' ', 'b8': ' ',
                                   'c1': ' ', 'c2': ' ', 'c3': ' ', 'c4': ' ',
                                   'c5': ' ', 'c6': ' ', 'c7': ' ', 'c8': ' ',
                                   'd1': ' ', 'd2': ' ', 'd3': ' ', 'd4': 'x',
                                   'd5': 'x', 'd6': 'x', 'd7': ' ', 'd8': ' ',
                                   'e1': ' ', 'e2': ' ', 'e3': ' ', 'e4': 'x',
                                   'e5': 'K', 'e6': 'x', 'e7': ' ', 'e8': ' ',
                                   'f1': ' ', 'f2': ' ', 'f3': ' ', 'f4': 'x',
                                   'f5': 'x', 'f6': 'x', 'f7': ' ', 'f8': ' ',
                                   'g1': ' ', 'g2': ' ', 'g3': ' ', 'g4': ' ',
                                   'g5': ' ', 'g6': ' ', 'g7': ' ', 'g8': ' ',
                                   'h1': ' ', 'h2': ' ', 'h3': ' ', 'h4': ' ',
                                   'h5': ' ', 'h6': ' ', 'h7': ' ', 'h8': ' '}
            print(f'\t{method[2]} is implemented correctly +2/2 points')
            score += 2
        except:
            print(f'\t{method[2]} is not implemented correctly +0/2 points')

#12 check class Knight
        try:
            board.empty()
            print(f'\nTesting class {classes[5]} ...')
            knight = chess.Knight(board, 'f5')
            assert knight.get_name() == 'N'
            print(f'\t{method[1]} is implemented correctly +1/1 points')
            score += 1
        except:
            print(f'\t{method[1]} is not implemented correctly +0/1 points')

#13 check class Knight method moves
        try:
            knight.moves(board)
            assert board.board == {'a1': ' ', 'a2': ' ', 'a3': ' ', 'a4': ' ',
                                   'a5': ' ', 'a6': ' ', 'a7': ' ', 'a8': ' ',
                                   'b1': ' ', 'b2': ' ', 'b3': ' ', 'b4': ' ',
                                   'b5': ' ', 'b6': ' ', 'b7': ' ', 'b8': ' ',
                                   'c1': ' ', 'c2': ' ', 'c3': ' ', 'c4': ' ',
                                   'c5': ' ', 'c6': ' ', 'c7': ' ', 'c8': ' ',
                                   'd1': ' ', 'd2': ' ', 'd3': ' ', 'd4': 'x',
                                   'd5': ' ', 'd6': 'x', 'd7': ' ', 'd8': ' ',
                                   'e1': ' ', 'e2': ' ', 'e3': 'x', 'e4': ' ',
                                   'e5': ' ', 'e6': ' ', 'e7': 'x', 'e8': ' ',
                                   'f1': ' ', 'f2': ' ', 'f3': ' ', 'f4': ' ',
                                   'f5': 'N', 'f6': ' ', 'f7': ' ', 'f8': ' ',
                                   'g1': ' ', 'g2': ' ', 'g3': 'x', 'g4': ' ',
                                   'g5': ' ', 'g6': ' ', 'g7': 'x', 'g8': ' ',
                                   'h1': ' ', 'h2': ' ', 'h3': ' ', 'h4': 'x',
                                   'h5': ' ', 'h6': 'x', 'h7': ' ', 'h8': ' '}
            print(f'\t{method[2]} is implemented correctly +2/2 points')
            score += 2
        except:
            print(f'\t{method[2]} is not implemented correctly +0/2 points')

#
# check the program control flow
#

#14 check the main errors
        try:
            print('\nTesting main with input ex1 ...')
            os.system(f'{command} {module} < ex1 > output 2> errors_ex1')
            f = open('errors_ex1', 'r')
            errors = f.read()
            f.close()
            assert errors.strip() == ''
            print(f'\t{module} runs without errors +4/4 points')
            score += 4
        except:
            print(f'\t{module} runs with errors +0/4 points')
            
#15 check the main output
        try:
            f = open('output', 'r')
            output = f.readlines()
            f.close()
            f = open('ex1.out')
            standard = f.readlines()
            f.close()
            i = 0
            for line in standard:
                if line.strip() == '':
                    continue
                if output[i].strip() == '':
                    i += 1
                assert line.strip() == output[i].strip()
                i += 1
            print(f'\toutput is correct +4/4 points')
            score += 4
        except:
            print(f'\toutput is incorrect +0/4 points')
            
#16 check the main errors
        try:
            print('\nTesting main with input ex2 ...')
            os.system(f'{command} {module} < ex2 > output 2> errors_ex2')
            f = open('errors_ex2', 'r')
            errors = f.read()
            f.close()
            assert errors.strip() == ''
            print(f'\t{module} runs without errors +4/4 points')
            score += 4
        except:
            print(f'\t{module} runs with errors +0/4 points')
            
#17 check the main output
        try:
            f = open('output', 'r')
            output = f.readlines()
            f.close()
            f = open('ex2.out')
            standard = f.readlines()
            f.close()
            i = 0
            for line in standard:
                if line.strip() == '':
                    continue
                if output[i].strip() == '':
                    i += 1
                assert line.strip() == output[i].strip()
                i += 1
            print(f'\toutput is correct +4/4 points')
            score += 4
        except:
            print(f'\toutput is incorrect +0/4 points')
            
#18 check specifications
        try:
            print('\nTesting specifications ...')
            f = open(f'{module}', 'r')
            code = f.read()
            f.close()

            if re.findall(r'# *author\b', code) != [] and \
               re.findall(r'# *date\b', code) != []:
                print(f'\tcomment block is used +1/1 points')
                score += 1
            else:
                print(f'\tcomment block is not used +0/1 points')
                
            if re.findall(r'\belse\b|\belif\b', code) != []:
                print(f'\telse or elif is used +1/1 points')
                score += 1
            else:
                print(f'\tneither else or elif is used +0/1 points')

            if re.findall(r'\bwhile\b', code) != []:
                print(f'\twhile is used +1/1 points')
                score += 1
            else:
                print(f'\twhile is not used +0/1 points')
                
            if re.findall(r'\bupper\b|\blower\b|\bsplit\b|\bjoin\b|\bisalpha\b', code) != []:
                print(f'\tstring method is used +1/1 points')
                score += 1
            else:
                print(f'\tstring method is not used +0/1 points')
                
            if re.findall(r'\bdict\b|\{\w*\}', code) != []:
                print(f'\tdictionary is used +1/1 points')
                score += 1
            else:
                print(f'\tdictionary is not used +0/1 points')
        except:
            pass
        
# output results
    print(f'\n{module}: {score} points out of {total}')
    with open('tmp', 'w') as f:
        f.write(str(score))        
