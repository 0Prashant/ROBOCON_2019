import math
from vector import Vector

class Robot:
        
        def __init__(self, state, field):
                self.state = state
                self.field = field
                self.is_first_reading = True
                self.first_heading = 0


        def getField(self):
                return self.field


        def run(self, state, vel, dt):

                if self.is_first_reading :
                        self.is_first_reading = False
                        self.first_heading = state[2]
                        vx = 0
                        vy = 0
                        rw = 0

                else :
                        #! Need to implement Cubic Spline instead of linear spline
                        # This is for linear spline
                        del_theta = state[2] - self.first_heading
                        aoA = self.field.angleOfAttack(state, vel, dt)
                        # print(aoA*57.3, state[0], state[1])

                        vx = vel*math.cos(aoA)
                        vy = vel*math.sin(aoA)
                        rw = del_theta * 1000.0 / dt

                        pass

                if self.field.nextFieldReached(state):
                        self.field = self.field.next

                return Vector(vx, vy)


