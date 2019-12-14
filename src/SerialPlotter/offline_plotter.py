#!/usr/bin/env python3

VERSION = "V0.2b"

import pygame, sys

from pygame.locals import *

def main():
    ERR_MSG = "Fun is not implemented yet!"
    mousex,mousey = (0,0)
    mouseDown = False
    mouseUp = False
    
    scale_x = 1
    scale_y = 2
    offset_x = 0
    offset_y = 588
    
    full_data = []
    input_file = open("data.txt", "r")
    for line in input_file:
        full_data.append(int(line))
    input_file.close()
    
    #max_marker = len(full_data) / 10
    #marker = new_marker()
    
    pygame.init()
    
    FPS_SETTING = 30
    fpsClock = pygame.time.Clock()
    
    # Set window title and icon
    pygame.display.set_caption("Plotter " + VERSION)
    #icon = pygame.image.load('gfx/icon.png')
    #pygame.display.set_icon(icon)
        
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
                sys.exit()
                
            elif event.type == KEYDOWN:
                if event.key == K_1:
                    scale_x = 1
                    scale_y = 1
                    offset_x = 0
                    offset_y = 588
                elif event.key == K_UP:
                    scale_y += 1
                elif event.key == K_DOWN:
                    scale_y -= 1
                elif event.key == K_LEFT:
                    if scale_x > 1:
                        scale_x += 1
                    else:
                        scale_x += 0.05
                elif event.key == K_RIGHT:
                    if scale_x > 1:
                        scale_x -= 1
                    elif scale_x > 0.05:
                        scale_x -= 0.05
                elif event.key == K_p:
                    for i in range(1):#try:
                        output_file = open("print.txt", "w")
                        for line in scope_data:
                            output_file.write(str(line)+"\n")
                        input_file.close()
                    #except:
                    #    ERR_MSG = "Can not write scope to file print.txt!"
                    
            
            elif event.type == MOUSEMOTION:
                mousex, mousey = event.pos
            
            if event.type == MOUSEBUTTONDOWN:
                mouseDown_pos = (mousex, mousey)
                mouseDown = True
                mouseUp = False
            elif event.type == MOUSEBUTTONUP:
                mouseDown = False
                mouseUp = True
        
        if mouseDown:
            offset_x -= int((mouseDown_pos[0] - mousex) / 4)
            offset_y += int((mouseDown_pos[1] - mousey) / 4)
        
        SURF.fill((32,32,16))
        drawtext(SURF, o_font, \
                 ERR_MSG + \
                 (" "*12) + "Scale = " + str(int(scale_x*100)) + "% " \
                 + str(int(scale_y*100)) + "%" + \
                (" "*12) + "Offset = (" + str(offset_x) + ", " \
                 + str(offset_y) +")", \
                 20, 20, 0, (128,192,128))
        drawtext(SURF, o_font, "Use arrow keys to scale, P to output scope to file " +\
                 "and mouse button down to scroll through scope", \
                 20, 40, 0, (128,192,128))
        
        #i = 0
        #for e in full_data:
        #    le = int(e * scale_y)
        #    pygame.draw.line(SURF, (0,255,0), (i+12,588), (i+12,588-le))
        #    i+=1
        
        if offset_x < 1: offset_x = 1
        if offset_x > len(full_data)-1024: offset_x = len(full_data)-1024
        #for i in range(offset_x, offset_x + int(1024 / scale_x)):
        #    pygame.draw.line(SURF, (0,92,0), \
        #                     ((i-1) * scale_x , offset_y - full_data[i-1] ), \
        #                     ((i  ) * scale_x , offset_y - full_data[i  ] ) )
        #    #drawtext(SURF, o_font, str(full_data[i]), 20+i*20, 200, 0, (128,192,128))
        
        l_max_val = 1
        l_min_val = 1023
        
        l_offset_x_upper_limit = offset_x + int(1024 / scale_x)
        if l_offset_x_upper_limit > len(full_data): l_offset_x_upper_limit = len(full_data)
        scope_data = full_data[offset_x : l_offset_x_upper_limit]
        i = 1
        l_prev_e = scope_data[0]
        for l_curr_e in scope_data:
            pygame.draw.line(SURF, (0,128,0), \
                             ((i-1) * scale_x , offset_y - l_prev_e * scale_y ), \
                             ((i  ) * scale_x , offset_y - l_curr_e * scale_y ) )
            if l_max_val < l_curr_e: l_max_val = l_curr_e
            if l_min_val > l_curr_e: l_min_val = l_curr_e
            l_prev_e = l_curr_e
            i+=1
        
        #if (l_max_val - l_min_val) * scale_y < 300:
        #    scale_y += 0.1
            #if offset_y < (288 - l_min_val) * scale_y: offset_y += 10
        #if (l_max_val - l_min_val) * scale_y > 310:
        #    scale_y -= 0.1
            #if offset_y > (290 - l_min_val) * scale_y: offset_y -= 10
        #if (offset_y - l_min_val * scale_y) > 590 - l_min_val: offset_y -= 10
        #if (offset_y - l_max_val * scale_y) <  50 + l_max_val: offset_y += 10
        #if scale_y * 200 > l_max_val: scale_y -= 0.1
        #if scale_y * 220 < l_max_val: scale_y += 0.1
        drawtext(SURF, o_font, "Max="+ str(l_max_val) + " Min=" + str(l_min_val), 20, 200, 0, (128,192,128))
             
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