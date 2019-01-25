import cv2
import numpy as np

def drawing():
	img = np.zeros((512,512,3),np.uint8)
	k=ord('g')

	while True:
		if k==ord('g'):
			cv2.circle(img, (255,255), 80, (0,255,0),-1)
		elif k==ord('r'):
			cv2.circle(img, (255,255),80,(0,0,255),-1)
		elif k==ord('b'):
			cv2.circle(img, (255,255), 80, (255,0,0),-1)

		cv2.imshow('circle',img)

		k=cv2.waitKey(0) & 0xff

		if k == 27:
			break

	cv2.destroyAllWindows()

drawing()
