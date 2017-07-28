#导入相关函数库
import pygame, sys, os
#OS经常和文件、目录打交道，
#这是就离不了os模块。os模
#块包含普遍的操作系统功能，
#与具体的平台无关。#sys系
#统相关的信息模块
from pygame.locals import *
from collections import deque

#将所在位置转化成成箱子
def to_box(level, index):
    if level[index] == '-' or level[index] == '@':
        level[index] = '$'
    else:
        level[index] = '*'
       
#将所在位置转化成人物
def to_man(level, i):
    if level[i] == '-' or level[i] == '$':
        level[i]='@'
    else:
        level[i]='+'
        
#将所在位置转化成地板
def to_floor(level, i):
    if level[i] == '@' or level[i] == '$':
        level[i]='-'
    else:
        level[i]='.'

def to_offset(d, width):
    d4 = [-1, -width, 1, width]
    m4 = ['l','u','r','d']
    return d4[m4.index(d.lower())]#python 的index方法是在字符串里查找子串第一次出现的位置，



	
class Pushbox:
    #初始化Pushbox中的成员
    def __init__(self):
        #用字符列表初始化迷宫
        self.level = list('---#&&&#####----------##.---###----------##$---###---------&&&----###&&------##--$-$---#&&#----&&&-&-&&-$-&&####&&&---#-##-##&##--..##-$--$----------..&&&&&&-###-#@##---.#----#-----#&#&&&###----##&&&&&#-------')
        #迷宫地图的横向字符数
        self.w = 19
        #迷宫地图的纵向字符数
        self.h = 11
        #确定游戏开始时的人物位置，level[self.man]
        self.man = 163
        self.hint = list(self.level) 
        self.sbox = 0
        #目的地数目
        self.destinations=6
        #抵达目的地的箱子数目   
        self.todes=0    
   #移动
    def move(self, d):
        self._move(d)
       
    #控制人物移动
    def _move(self, d):
        self.sbox = 0
        h = to_offset(d, self.w)
        #画图像素尺寸高度有关
        h2 = 2 * h
        if self.level[self.man + h] == '-' or self.level[self.man + h] == '.':#人的位置前面是地板或者目的地
        # 进行移动
            to_man(self.level, self.man+h)#人前面的位置变成人
            to_floor(self.level, self.man)#人的位置变成地板
            self.man += h
        elif self.level[self.man + h] == '*' or self.level[self.man + h] == '$':#人的前面
            if self.level[self.man + h2] == '-' or self.level[self.man + h2] == '.':#人前面的物体的前面
                #箱子离开目的地
                if self.level[self.man + h]=='*':
                   if self.level[self.man+h2]=='.'or self.level[self.man+h2]=='-':
                        self.todes=self.todes-1#抵达目的地的箱子数目减少
                        print(self.todes)
                to_box(self.level, self.man + h2)#人前面的物体的前面变成箱子              
                to_man(self.level, self.man + h)#人的前面变化成人
                to_floor(self.level, self.man)#人所在位置变成地板
                #箱子抵达目的地
                if self.level[self.man+h2]=='*':
                    self.todes=self.todes+1#抵达目的地的箱子数目增加
                    print(self.todes)
                self.man += h#人的位置增加
    #判断游戏是否胜利
    def juge(self):
        
        if self.todes==self.destinations:
            return True



     
    #人物像向上移动的画图函数
    def drawup(self, screen, skin):
        w = skin.get_width() / 4    
        for i in range(0,self.w):
            for j in range(0,self.h):
                if self.level[j*self.w + i] == '#':
                    screen.blit(skin, (i*w, j*w), (0,2*w,w,w))
                elif self.level[j*self.w + i] == '&':
                    screen.blit(skin, (i*w, j*w), (0,3*w,w,w))
                elif self.level[j*self.w + i] == '-':
                    screen.blit(skin, (i*w, j*w), (0,0,w,w))
                elif self.level[j*self.w + i] == '@':
                    screen.blit(skin, (i*w, j*w), (3*w,3*w,w,w))
                elif self.level[j*self.w + i] == '$':
                    screen.blit(skin, (i*w, j*w), (2*w,0,w,w))
                elif self.level[j*self.w + i] == '.':
                    screen.blit(skin, (i*w, j*w), (0,w,w,w))
                elif self.level[j*self.w + i] == '+':
                    screen.blit(skin, (i*w, j*w), (w,w,w,w))
                elif self.level[j*self.w + i] == '*':
                    screen.blit(skin, (i*w, j*w), (3*w,w,w,w))
                if self.sbox != 0 and self.hint[j*self.w+i]=='1':
                    screen.blit(skin,  (i*w+offset, j*w+offset), (3*w, 3*w, 4, 4))
     #人物像向下移动的画图函数
    def drawdown(self, screen, skin):
        w = skin.get_width() / 4       
        for i in range(0,self.w):
            for j in range(0,self.h):
                if self.level[j*self.w + i] == '#':
                    screen.blit(skin, (i*w, j*w), (0,2*w,w,w))
                elif self.level[j*self.w + i] == '&':
                    screen.blit(skin, (i*w, j*w), (0,3*w,w,w))
                elif self.level[j*self.w + i] == '-':
                    screen.blit(skin, (i*w, j*w), (0,0,w,w))
                elif self.level[j*self.w + i] == '@':
                    screen.blit(skin, (i*w, j*w), (w,0,w,w))
                elif self.level[j*self.w + i] == '$':
                    screen.blit(skin, (i*w, j*w), (2*w,0,w,w))
                elif self.level[j*self.w + i] == '.':
                    screen.blit(skin, (i*w, j*w), (0,w,w,w))
                elif self.level[j*self.w + i] == '+':
                    screen.blit(skin, (i*w, j*w), (w,w,w,w))
                elif self.level[j*self.w + i] == '*':
                    screen.blit(skin, (i*w, j*w), (3*w,w,w,w))
                if self.sbox != 0 and self.hint[j*self.w+i]=='1':
                    screen.blit(skin,  (i*w+offset, j*w+offset), (3*w, 3*w, 4, 4))
     #人物像向左移动的画图函数
    def drawleft(self, screen, skin):
        w = skin.get_width() / 4      
        for i in range(0,self.w):
            for j in range(0,self.h):
                if self.level[j*self.w + i] == '#':
                    screen.blit(skin, (i*w, j*w), (0,2*w,w,w))
                elif self.level[j*self.w + i] == '&':
                    screen.blit(skin, (i*w, j*w), (0,3*w,w,w))
                elif self.level[j*self.w + i] == '-':
                    screen.blit(skin, (i*w, j*w), (0,0,w,w))
                elif self.level[j*self.w + i] == '@':
                    screen.blit(skin, (i*w, j*w), (w,3*w,w,w))
                elif self.level[j*self.w + i] == '$':
                    screen.blit(skin, (i*w, j*w), (2*w,0,w,w))
                elif self.level[j*self.w + i] == '.':
                    screen.blit(skin, (i*w, j*w), (0,w,w,w))
                elif self.level[j*self.w + i] == '+':
                    screen.blit(skin, (i*w, j*w), (w,w,w,w))
                elif self.level[j*self.w + i] == '*':
                    screen.blit(skin, (i*w, j*w), (3*w,w,w,w))
                if self.sbox != 0 and self.hint[j*self.w+i]=='1':
                    screen.blit(skin,  (i*w+offset, j*w+offset), (3*w, 3*w, 4, 4))
     #人物像向右移动的画图函数
    def drawright(self, screen, skin):
        w = skin.get_width() / 4      
        for i in range(0,self.w):
            for j in range(0,self.h):
                if self.level[j*self.w + i] == '#':
                    screen.blit(skin, (i*w, j*w), (0,2*w,w,w))
                elif self.level[j*self.w + i] == '&':
                    screen.blit(skin, (i*w, j*w), (0,3*w,w,w))
                elif self.level[j*self.w + i] == '-':
                    screen.blit(skin, (i*w, j*w), (0,0,w,w))
                elif self.level[j*self.w + i] == '@':
                    screen.blit(skin, (i*w, j*w), (2*w,3*w,w,w))
                elif self.level[j*self.w + i] == '$':
                    screen.blit(skin, (i*w, j*w), (2*w,0,w,w))
                elif self.level[j*self.w + i] == '.':
                    screen.blit(skin, (i*w, j*w), (0,w,w,w))
                elif self.level[j*self.w + i] == '+':
                    screen.blit(skin, (i*w, j*w), (w,w,w,w))
                elif self.level[j*self.w + i] == '*':
                    screen.blit(skin, (i*w, j*w), (3*w,w,w,w))
                if self.sbox != 0 and self.hint[j*self.w+i]=='1':
                    screen.blit(skin,  (i*w+offset, j*w+offset), (3*w, 3*w, 4, 4))
    
def main():

    # 初始化
    pygame.init()
    pygame.mixer.init()
    screen = pygame.display.set_mode((1068,620))
    # 加载图片内容
    skinfilename = os.path.join('skin.png')
    try:
        skin = pygame.image.load(skinfilename)
    except pygame.error, msg:
        print 'cannot load skin'
        raise SystemExit, msg
    #我们把可能发生错误的语句放在try模块里，
    #用except来处理异常。except可以处理一个
    #专门的异常，也可以处理一组圆括号中的异
    #常， 如果except后没有指定异常，则默认
    #处理所有的异常。每一个try，都必须至少有
    #一个except#raise引发一个异常
    skin = skin.convert()
    background=pygame.image.load("2.jpg")
    gameover=pygame.image.load("3.jpg")
   
     #加载音乐
    pygame.mixer.music.load('1.mp3')
    laugh=pygame.mixer.Sound("2.wav")
    pygame.mixer.music.play(-1, 9)
    pygame.mixer.music.set_volume(0.25)
    laugh.set_volume(0.35)
    screen.fill(skin.get_at((0,0)))
    pygame.display.set_caption('Minions love bananas.py')

    # 创建一个Pushbox的对象
    skb = Pushbox()
    skb.drawdown(screen,skin)

    #创建游戏时钟
    clock = pygame.time.Clock()
    pygame.key.set_repeat(200,50)

    #消息循环处理
    while True:
        clock.tick(60)
        for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit() 
                    sys.exit()
                elif event.type == KEYDOWN:
                    if event.key == K_UP:
                        skb.move('u')
                        skb.drawup(screen,skin)
                    elif event.key == K_DOWN:
                        skb.move('d')
                        skb.drawdown(screen,skin)
                    elif event.key == K_LEFT:
                        skb.move('l')
                        skb.drawleft(screen,skin)
                    elif event.key == K_RIGHT:
                        skb.move('r')
                        skb.drawright(screen,skin)
                    # 空格键直接结束游戏
                    elif event.key ==K_SPACE:
                        screen = pygame.display.set_mode((700,460))
                        screen.blit( gameover, (0,0))
                        pygame.mixer.music.set_volume(0)
                        laugh.play() 
        if skb.juge():
         screen = pygame.display.set_mode((1000,500))
         screen.blit(background, (0,0))
         pygame.mixer.music.set_volume(0)
         laugh.play() 
                                              
        pygame.display.update()
   
if __name__ == '__main__':
    main()
