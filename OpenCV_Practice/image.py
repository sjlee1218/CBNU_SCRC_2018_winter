import cv2
import numpy as np

def showImage(path):
    img=cv2.imread(path,cv2.IMREAD_COLOR)
    cv2.imshow('road', img)
    
    cv2.waitKey(0)
    cv2.destroyAllWindows()

showImage('/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/OpenCV_Practice/center_line.jpg')
