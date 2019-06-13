#!/usr/bin/env python3

import pygame, sys
import serial

from pygame.locals import *

def main():
    mousex,mousey = (0,0)
    mdata = []
    marker = [0,0,0,0]
    active_marker = 0
    
    scale = 100
    
    debug_s = 0
    
    # initialize serial interface
    ser = serial.Serial("/dev/ttyUSB0")#, 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
    
    pygame.init()
    
    FPS_SETTING = 30
    fpsClock = pygame.time.Clock()
    
    # Set window title and icon
    pygame.display.set_caption('PCosc V01')
    icon = pygame.image.load('gfx/icon.png')
    pygame.display.set_icon(icon)
        
    # Load font
    o_font = pygame.font.Font('CosmicSansNeueMono.ttf', 16)
    
    # Bildschirm anlegen
    SURF = pygame.display.set_mode((1024,600), 0, 32)
    
    while True:
        
        for event in pygame.event.get():
            if event.type == QUIT \
                or (event.type == KEYDOWN and \
                    event.key == K_ESCAPE):
                pygame.quit()
                #logfile.close()
                try:
                    ser.close()
                except:
                    pass#if con_state > 0: logfile.write('\n#No serial connection to close\n')
                sys.exit()
                
            elif event.type == KEYDOWN:
                if event.key == K_1:
                    marker[0] = mousey
                elif event.key == K_2:
                    marker[1] = mousey
                elif event.key == K_3:
                    marker[2] = mousey
                elif event.key == K_4:
                    marker[3] = mousey
                elif event.key == K_UP:
                    if scale < 800: scale *= 2
                elif event.key == K_DOWN:
                    if scale > 25: scale /= 2
            
            elif event.type == MOUSEMOTION:
                mousex, mousey = event.pos
        
        # read in and shift serial data
        debug_s +=1
        while ser.inWaiting() >= 2:
            l_readin_upper = ser.read(1)
            l_readin_lower = ser.read(1)
            if l_readin_upper > b'\xFB' :
                if l_readin_lower > b'\xFB':
                    print("0x0000 in Schritt", debug_s)
                    debug_s = 0
                    continue
                else:
                    l_readin_upper = l_readin_lower
                    l_readin_lower = ser.read(1)
                
            l_readin = l_readin_upper + l_readin_lower
            mdata.append(int.from_bytes(l_readin, byteorder='big'))
            #print(int.from_bytes(ser.read(4), byteorder='little'))
            #print(ser.read(7))
            if len(mdata) > 1000:
                mdata.pop(0)
        
        SURF.fill((32,64,32))
        drawtext(SURF, o_font, \
                 "Tasten 1, 2, 3, 4, Pfeil-Hoch, Pfeil-Runter" + \
                 (" "*12) + "Scale=" + str(int(scale)) + "%", \
                 20, 20, 0, (128,192,128))
        
        i = 0
        for e in mdata:
            le = int(e * (scale / 100))
            pygame.draw.line(SURF, (0,255,0), (i+12,588), (i+12,588-le))
            i+=1
        #pygame.draw.polygon(SURF, (0,255,0), mdata)
        
        i = 0
        for e in marker:
            #if i == active_marker:
            #    lcolor = (255,255,0)
            #else:
            #    lcolor = (255,255,128)
            lcolor = (255,255,0)
            pygame.draw.line(SURF, lcolor, (10, e), (1014,e))
            drawtext(SURF, o_font, str(588-e), 5, e -15, 0, (255,255,0))
            i+=1
        
        pygame.display.update()
        fpsClock.tick(FPS_SETTING)



"""
Functions
"""

def drawtext(SURF, o_font, s_text, xpos, ypos, center=0, color=(128,128,128)):
    temptextObj = o_font.render(s_text, True, color)
    xpos = xpos - center * (list(o_font.size(s_text))[0] / 2)
    SURF.blit(temptextObj,(xpos,ypos))
    

# Wenn das Programm direkt aufgerufen wird (also nicht als Import), dann wird
# ..die Variable __name__ automatisch mit dem Wert '__main__' gefuellt
# ..und in diesem Fall die Funktion main() aufgerufen!
if __name__ == '__main__': main()

# -EOF-