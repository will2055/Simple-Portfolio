###########################################################
#                   Image Object detection                #
#                                                         #
#   Programmed by: Brandon Williams (2/22/16)             #
#   Class: CS 301, Spring 2016                            #
#   Instructor: Dean Zeller                               #
#                                                         #
#   Description:                                          #
#   This file contains the following functions:           #
#      ~ getPixel                                         #
#      ~ groupPix                                         #
#      ~ makeTwoArr                                       #
#      ~ readCoor                                         #
#      ~ drawBorder                                       #
###########################################################
from graphics_copy_WilliamsBrandon import GraphicsImage, GraphicsWindow;
from tkinter import *;

###########################################################
#                       Declare Images                    #
#   All used images are declared here for use.            #
###########################################################
img1=GraphicsImage('Monochromatic_proof of method.png');
img2=GraphicsImage('Dichromatic_proof of method.png');
img3=GraphicsImage('try_num9.png');
img4=GraphicsImage('try_num5.png');

###########################################################
#                     Declare Functions                   #
# Declared functions used in the testing file are below.  #
#                                                         #
# Most functions use functions from the 'graphics_copy_Wil#
#liamsBrandon' file                                       #
###########################################################

###########################################################
#                       getPixel                          #
#  This function uses the classes and function of the file#
# 'graphics_copy_WilliamsBrandon' to read the values to   #
#return a list.                                           #
###########################################################
def getPixel(image, i, j):
    red=image.getRed(i,j);
    green=image.getGreen(i,j);
    blue=image.getBlue(i,j);
    pixel=[red,green,blue];
    return pixel;
###########################################################
#                       groupPix                          #
#  takes in the image 1-D array, image height and width.  #
#This function then runs through the 2-D array and finds  #
#'like' pixels. The pixel that are different are          #
#saved in a different array than the other pixels. The    #
#purpose is to differentiate between the background color #
#and the object color. Those arrays are made of the       #
#locations of the different (or like) pixels. Only the    #
#object locations are used later.                         #
###########################################################
def groupPix(array, height, width):
    background=[];
    obj=[];
    pixPos=[];
    firstPix=array[0][0];
    background.append(firstPix);
    for row in range(height):
        for col in range(width):
            try:
                if(array[row][col+1]==array[row][col]):
                    if(array[row][col+1]==background[0]):
                        backPix=array[row][col+1];
                        background.append(backPix);
                    elif(array[row][col+1]==obj[0]):
                        objPix=array[row][col+1];
                        obj.append(objPix);
                        x=col+1;
                        y=row;
                        pixPos.append((x,y));
                if(array[row][col+1]!=array[row][col]):
                    if(array[row][col+1]!=background[0]):
                        objPix=array[row][col+1];
                        obj.append(objPix);
                        x=col+1;
                        y=row;
                        pixPos.append((x,y));

            except IndexError:
                break;
    return pixPos;

###########################################################
#                       makeTwoArr                        #
# Takes in a 1-D array and converts it into a 2-D array.  #
###########################################################
def makeTwoArr(array, height, width):
    newArray=[[0 for x in range(width)] for x in range(height)];
    location=0;
    for row in range(height):
        for col in range(width):
            newArray[row][col]=array[location];
            location+=1;

    return newArray;

###########################################################
#                       readCoord                         #
#  takes a whole array and returns the first and last     #
#array values.                                            #
###########################################################    
def readCoord(array):
    firstCoord=array[0];
    endArr=len(array)-1;
    secondCoord=array[endArr];
    coords=[firstCoord,secondCoord]
    return coords;

###########################################################
#                       drawBorder                        #
#  Uses the array from readCoord to draw a rectangle at   #
#the coordinates in the array values.                     #
###########################################################
def drawBorder(array, img):
    win = GraphicsWindow();
    canvas = win.canvas();
    canvas.drawImage(img);
    coord1=array[0];
    coord2=array[1];
    canvas.drawRect(coord1,coord2);

    
####################################Test Area#####################################

    #image 1#
def image1():
    print("*******************Image 1*************************");  
    picArray=[];
    width=img1.width();
    height=img1.height();
    for i in range (width):
        for j in range (height):
            pix=getPixel(img1,i,j);
            picArray.append(pix);
    twoDi=makeTwoArr(picArray, width, height);
    obj=groupPix(twoDi,height,width);
    coords=readCoord(obj);
    print(coords);
    drawBorder(coords,img1);

    #image 2#
def image2():
    print("*******************Image 2*************************");
    picArray=[];
    width=img2.width();
    height=img2.height();
    for i in range (height):
        for j in range (width):
            pix=getPixel(img2,i,j);
            picArray.append(pix);
    twoDi2=makeTwoArr(picArray, width, height);
    obj=groupPix(twoDi2,height,width);
    coords=readCoord(obj);
    print(coords);
    drawBorder(coords,img2);

    #image 3#
def image3():
    print("*******************Image 3*************************");
    picArray=[];
    width=img3.width();
    height=img3.height();
    print(width,height);
    for i in range (height):
        for j in range (width):
            try:
                pix=getPixel(img3,i,j);
                picArray.append(pix);
            except TclError:
                break;
    twoDi3=makeTwoArr(picArray, width, height);
    obj=groupPix(twoDi3,height,width);
    coords=readCoord(obj);
    print(coords);
    drawBorder(coords,img3);

    #image 4#
def image4():
    print("*******************Image 3*************************");
    picArray=[];
    width=img4.width();
    height=img4.height();
    print(width,height);
    for i in range (height):
        for j in range (width):
            try:
                pix=getPixel(img4,i,j);
                picArray.append(pix);
            except TclError:
                break;
    twoDi3=makeTwoArr(picArray, width, height);
    obj=groupPix(twoDi3,height,width);
    coords=readCoord(obj);
    print(coords);
    drawBorder(coords,img4);

image1();
image2();
image3();
image4();
