
#http://anh.cs.luc.edu/handsonPythonTutorial/graphics.html
from graphics import *
import numpy as np
import time
from random import *

#https://github.com/LazoCoder/Braille-Translator
import printer, alphaToBraille, brailleToAlpha

#https://github.com/markomanninen/pybrl
# 64 symbols
brailles = [' ','⠮','⠐','⠼','⠫','⠩','⠯','⠄','⠷','⠾','⠡','⠬','⠠','⠤','⠨','⠌','⠴','⠂','⠆','⠒','⠲','⠢',
            '⠖','⠶','⠦','⠔','⠱','⠰','⠣','⠿','⠜','⠹','⠈','⠁','⠃','⠉','⠙','⠑','⠋','⠛','⠓','⠊','⠚','⠅',
            '⠇','⠍','⠝','⠕','⠏','⠟','⠗','⠎','⠞','⠥','⠧','⠺','⠭','⠽','⠵','⠪','⠳','⠻','⠘','⠸']

#https://github.com/markomanninen/pybrl
# corresponding bitwise matrix for Braille symbols
matrixcodes = [
    [[0, 0], [0, 0], [0, 0]],[[0, 1], [1, 0], [1, 1]],[[0, 0], [0, 1], [0, 0]],[[0, 1], [0, 1], [1, 1]],
    [[1, 1], [1, 0], [0, 1]],[[1, 1], [0, 0], [0, 1]],[[1, 1], [1, 0], [1, 1]],[[0, 0], [0, 0], [1, 0]],
    [[1, 0], [1, 1], [1, 1]],[[0, 1], [1, 1], [1, 1]],[[1, 0], [0, 0], [0, 1]],[[0, 1], [0, 0], [1, 1]],
    [[0, 0], [0, 0], [0, 1]],[[0, 0], [0, 0], [1, 1]],[[0, 1], [0, 0], [0, 1]],[[0, 1], [0, 0], [1, 0]],
    [[0, 0], [0, 1], [1, 1]],[[0, 0], [1, 0], [0, 0]],[[0, 0], [1, 0], [1, 0]],[[0, 0], [1, 1], [0, 0]],
    [[0, 0], [1, 1], [0, 1]],[[0, 0], [1, 0], [0, 1]],[[0, 0], [1, 1], [1, 0]],[[0, 0], [1, 1], [1, 1]],
    [[0, 0], [1, 0], [1, 1]],[[0, 0], [0, 1], [1, 0]],[[1, 0], [0, 1], [0, 1]],[[0, 0], [0, 1], [0, 1]],
    [[1, 0], [1, 0], [0, 1]],[[1, 1], [1, 1], [1, 1]],[[0, 1], [0, 1], [1, 0]],[[1, 1], [0, 1], [0, 1]],
    [[0, 1], [0, 0], [0, 0]],[[1, 0], [0, 0], [0, 0]],[[1, 0], [1, 0], [0, 0]],[[1, 1], [0, 0], [0, 0]],
    [[1, 1], [0, 1], [0, 0]],[[1, 0], [0, 1], [0, 0]],[[1, 1], [1, 0], [0, 0]],[[1, 1], [1, 1], [0, 0]],
    [[1, 0], [1, 1], [0, 0]],[[0, 1], [1, 0], [0, 0]],[[0, 1], [1, 1], [0, 0]],[[1, 0], [0, 0], [1, 0]],
    [[1, 0], [1, 0], [1, 0]],[[1, 1], [0, 0], [1, 0]],[[1, 1], [0, 1], [1, 0]],[[1, 0], [0, 1], [1, 0]],
    [[1, 1], [1, 0], [1, 0]],[[1, 1], [1, 1], [1, 0]],[[1, 0], [1, 1], [1, 0]],[[0, 1], [1, 0], [1, 0]],
    [[0, 1], [1, 1], [1, 0]],[[1, 0], [0, 0], [1, 1]],[[1, 0], [1, 0], [1, 1]],[[0, 1], [1, 1], [0, 1]],
    [[1, 1], [0, 0], [1, 1]],[[1, 1], [0, 1], [1, 1]],[[1, 0], [0, 1], [1, 1]],[[0, 1], [1, 0], [0, 1]],
    [[1, 0], [1, 1], [0, 1]],[[1, 1], [1, 1], [0, 1]],[[0, 1], [0, 1], [0, 0]],[[0, 1], [0, 1], [0, 1]]
]

os.system('clear')

l1 = 10
l2 = 30
l3 = 60

chars = 10
rows = 3
cols = 2

scanDelay = 0.1

braille2matrix = {}
for i in range(len(brailles)):
    braille2matrix[brailles[i]] = matrixcodes[i]

textEng = "1 2 3 Hello How are you" 
textbraille = alphaToBraille.translate(textEng)

textBraille = "          "
#print(len(textEng))
#print(len(textbraille))
textBraille = textbraille[:min(len(textbraille), chars)]
textEng = brailleToAlpha.translate(textBraille)
print(textBraille)
print(textEng)

widthWin = 700
heightWin = 140
win = GraphWin("eBraille Display", widthWin, heightWin)
win.setBackground("white")
textMessage = Text(Point(widthWin/2, l1), textEng)
textMessage.setTextColor("black")
textMessage.setFace("courier")
textMessage.draw(win)
messageLED = textBraille
textMessage = Text(Point(widthWin/2, l2), messageLED)
textMessage.setSize(20)
textMessage.draw(win)
radiusDot = 10

p1 = Point(0,l3-10)
p2 = Point(widthWin, heightWin)

colorON = "black"
colorOFF = "grey"

displayBrailleBitMap = []
for i in textBraille:
    displayBrailleBitMap.append(braille2matrix[i])

def clearDisplayArea():
    r = Rectangle(p1, p2)
    r.setOutline("black")
    r.setFill("white")
    r.draw(win)

def showOnGraphics(data):
    #clearDisplayArea()
    #scan3x20(data)
    clearDisplayArea()
    scan6x10(data)

def scan3x20_a(data):
    for k in range(chars):
        for j in range(cols): 
            for i in range(rows):
                p = Point(20+70*k+30*j, l3+30*i)
                c = Circle(p, radiusDot)
                if (data[k][2*i+j] == 1):
                    colorDot = colorON
                else:
                    colorDot = colorOFF
                c.setOutline(colorDot)
                c.setFill(colorDot)
                c.draw(win)
            #time.sleep(scanDelay)
            clearDisplayArea()

def scan3x20(data):
    for k in range(chars):
        for j in range(cols):
            p = []
            c = []
            for i in range(rows):
                p.append(Point(20+70*k+30*j, l3+30*i))
                c.append(Circle(p[i], radiusDot))
                if (data[k][2*i+j] == 1):
                    colorDot = colorON
                else:
                    colorDot = colorOFF
                c[i].setOutline(colorDot)
                c[i].setFill(colorDot)
                
            for i in range(rows):
                c[i].draw(win)
            #time.sleep(scanDelay)
            clearDisplayArea()

def scan6x10(data):
    for k in range(chars):
        for j in range(cols): 
            p = []
            c = []
            for i in range(rows):
                p.append(Point(20+70*k+30*j, l3+30*i))
                c.append(Circle(p[i], radiusDot))
                if (data[k][2*i+j] == 1):
                    colorDot = colorON
                else:
                    colorDot = colorOFF
                c[i].setOutline(colorDot)
                c[i].setFill(colorDot)
                
            for i in range(rows):
                c[i].draw(win)
        #time.sleep(scanDelay)
        clearDisplayArea()


def showOnConsole(data):
    arr = [[[0 for i in range(rows)] for j in range(chars)] for k in range(cols)] 
    for k in range(chars):
        for j in range(rows):
            for i in range(cols):
                arr[i][k][j] = data[k][j*i]
    
    for k in range(chars):
        for j in range(rows):
            for i in range(cols): 
                os.system('clear')          
                for l in range(cols):
                    print(arr[l])

def bitMapBraille2BinaryBraille(brailleMatrix):
    chars = 10
    dots = 6
    rows = 3
    cols = 2
    data = [[0 for i in range(dots)] for j in range(chars)]
    for i in range(len(data)):
        for j in range(rows):
            for k in range(cols):
                data[i][2*j+k] = brailleMatrix[i][j][k]
    return data

binaryBraille = bitMapBraille2BinaryBraille(displayBrailleBitMap)
#showOnConsole(binaryBraille)
for i in range(100):
    showOnGraphics(binaryBraille)
    win.update()

