from random import randint
from time import time

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Rectangle

from field import *
from robot import *
from vector import Vector

noise = np.random.normal(0, 0.02, 100)

def calPos(x, vx, ax, t):
        index = randint(0, 99)
        return (x + vx*t + 0.5*ax*t*t) + noise[index]

gInitial_Pos = [0.5, 0.5, 0]


def main():
        
        # plt.show()

        plt.xlim(0, 10)
        plt.ylim(0, 6)
        currentAxis = plt.gca()
        currentAxis.set_aspect('equal', adjustable='box')

        # Plotting Poles
        plt.scatter(2.000, 1.250, s=10, color='red')
        plt.scatter(3.500, 1.250, s=10, color='red')
        plt.scatter(5.000, 1.250, s=10, color='red')
        # Plotting Bridge (Lower Side)
        plt.scatter(6.500, 0.725, s=10, color='red')
        plt.scatter(7.000, 0.725, s=10, color='red')
        plt.scatter(7.500, 0.725, s=10, color='red')
        plt.scatter(8.000, 0.725, s=10, color='red')
        # Plotting Bridge (Upper Side)
        plt.scatter(6.500, 1.725, s=10, color='red')
        plt.scatter(7.000, 1.725, s=10, color='red')
        plt.scatter(7.500, 1.725, s=10, color='red')
        plt.scatter(8.000, 1.725, s=10, color='red')


        fieldStop2 = Field(5.000, 4.000, 'S2', fieldL_cons, 0)
        fieldStop = Field(5.000, 4.000, 'S1', fieldL_cons, fieldStop2)
        fieldL = Field(5.000, 4.000, 'L', fieldL_cons, fieldStop)
        fieldK = Field(8.700, 4.000, 'K', fieldK_cons, fieldL)
        fieldJ = Field(8.700, 5.000, 'J', fieldJ_cons, fieldK)
        fieldI = Field(8.700, 1.250, 'I', fieldI_cons, fieldJ)
        fieldH = Field(6.300, 1.225, 'H', fieldH_cons, fieldI)
        fieldG = Field(5.500, 1.750, 'G', fieldG_cons, fieldH)
        fieldF = Field(4.800, 1.800, 'F', fieldF_cons, fieldG)
        fieldE = Field(4.000, 0.700, 'E', fieldE_cons, fieldF)
        fieldD = Field(3.000, 0.800, 'D', fieldD_cons, fieldE)
        fieldC = Field(2.600, 1.650, 'C', fieldC_cons, fieldD)
        fieldB = Field(1.500, 1.750, 'B', fieldB_cons, fieldC)
        fieldA = Field(0.500, 0.500, 'A', fieldA_cons, fieldB)

        # Plotting the set-points
        plt.scatter(fieldA.xc, fieldA.yc, s=5, color='green')
        plt.scatter(fieldB.xc, fieldB.yc, s=5, color='green')
        plt.scatter(fieldC.xc, fieldC.yc, s=5, color='green')
        plt.scatter(fieldD.xc, fieldD.yc, s=5, color='green')
        plt.scatter(fieldE.xc, fieldE.yc, s=5, color='green')
        plt.scatter(fieldF.xc, fieldF.yc, s=5, color='green')
        plt.scatter(fieldG.xc, fieldG.yc, s=5, color='green')
        plt.scatter(fieldH.xc, fieldH.yc, s=5, color='green')
        plt.scatter(fieldI.xc, fieldI.yc, s=5, color='green')
        plt.scatter(fieldJ.xc, fieldJ.yc, s=5, color='green')
        plt.scatter(fieldK.xc, fieldK.yc, s=5, color='green')
        plt.scatter(fieldL.xc, fieldL.yc, s=5, color='green')
        plt.scatter(fieldStop.xc, fieldStop.yc, s=5, color='green')
        plt.scatter(fieldStop2.xc, fieldStop2.yc, s=5, color='green')

        # for i in range(1000):
        robo_pos = gInitial_Pos
        robo_dim = [0.6, 0.6]

        khangai = Robot(robo_pos, fieldA)

        vel = Vector(0,0)
        last_vel = Vector(0,0)
        accel = Vector(0,0)

        robo_speed = 0.8
        accel_factor = 1

        xPos = []
        yPos = []
        mass = 10

        # i = 0
        # file_name = "data_set/dataSet" + str(i) + ".csv"
        # f = open(file_name, "w")

        # f.writelines("time, x, y\n")

        i = 0
        t = 0.00
        print_rect = 0
        w = robo_dim[0]
        h = robo_dim[1]

        x = robo_pos[0]
        y = robo_pos[1]
        here = True
        hereJ = True
        currentAxis.add_patch(Rectangle((x - w/2.0, y - h/2.0), w, h,alpha=1, fill=None))

        then = time()
        while khangai.getField() != fieldStop :

                if time() - then > 0.01 :
                        dt = time() - then
                        then = time()

                        vel += accel*dt
                        x = calPos(x, vel.x, accel.x, dt)
                        y = calPos(y, vel.y, accel.y, dt)
                        robo_pos[0] = x
                        robo_pos[1] = y
                        t += dt
                        # print(str(x) + ' ' + str(y))

                        if (khangai.getField().id == 'I' and here) :
                                here = False
                                print('Bridge Crossing Time : ' + str(t))
                                robo_speed = 1.6
                                

                        if (khangai.getField().id == 'J' and hereJ) :
                                hereJ = False
                                print('Gerege Passing Time : ' + str(t))
                                robo_speed = 0.8

                        vel = khangai.run(robo_pos, robo_speed, dt)

                        accel = accel_factor*(vel - last_vel)/dt
                        last_vel = vel

                        xPos.append(x)
                        yPos.append(y)

                        print_rect += 1
                        if not (print_rect % 10):
                                currentAxis.add_patch(Rectangle((x - w/2.0, y - h/2.0), w, h,alpha=1, fill=None))

                        plt.scatter(x, y, s=1, color='black')
                        plt.pause(0.04)
        
        # f.close()
        # print("Time Taken : " + str(t))
        # plt.plot(xPos, yPos, label='Simple Movement')
        # for i in range(len(xPos)):
        #         currentAxis.add_patch(Rectangle((xPos[i] - w/2.0, yPos[i] - h/2.0), w, h,alpha=1, fill=None))
        currentAxis.add_patch(Rectangle((x - w/2.0, y - h/2.0), w, h,alpha=1, fill=None))
        plt.show()
        print(t)


if __name__=="__main__":
   main()
