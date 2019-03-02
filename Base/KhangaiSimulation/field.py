import math
import matplotlib.pyplot as plt
import numpy as np

from simulate import gInitial_Pos

# Algorithm on Wikipedia : https://en.wikipedia.org/wiki/Spline_(mathematics)
def cubic_spline(p):
        a = [None]*3
        b = [None]*3
        d = [None]*3
        u = [None]*3
        h = [None]*2

        for i in range(3):
                a[i] = p[i][1]

        for i in range(2):
                if p[i+1][0] == p[i][0]:
                        h[i] = 0.00001
                else :
                        h[i] = p[i+1][0] - p[i][0]

        alpha = 3.0*(a[2] - a[1])/h[1] - 3.0*(a[1] - a[0])/h[0]

        c = [None]*3
        l = [None]*3
        z = [None]*3

        l[0] = 1
        u[0] = z[0] = 0

        l[1] = 2*(p[2][0] - p[0][0]) - h[0]*u[0]
        u[1] = h[1]/l[1]
        z[1] = (alpha - h[0]*z[0])/l[1]

        l[2] = 1
        z[2] = c[2] = 0

        for j in range(1,-1,-1):
                c[j] = z[j] - u[j]*c[j+1]
                b[j] = (a[j+1] - a[j])/h[j] - h[j]*(c[j+1] + 2*c[j])/3.0
                d[j] = (c[j+1] - c[j])/(3*h[j])
        
        return [a[0], b[0], c[0], d[0]]

gLast_State = gInitial_Pos

def cubic_theta(p, v, h, dt):
        xp = p[0][0]
        yp = p[0][1]
        x2 = p[2][0]
        x1 = p[1][0]
        coeff = cubic_spline(p)
        a = coeff[0]
        b = coeff[1]
        c = coeff[2]
        d = coeff[3]

        P = lambda x : (a + b*(x-xp) + c*(x-xp)**2 + d*(x-xp)**3)
        
        x_lst = [x/1000.0 for x in range(int(xp*1000), int((x2)*1000))]
        x = np.array(x_lst)
        y = P(x)
        # plt.plot(x,y)
        # plt.show()

        r = v*dt
        del_x = 0
        del_y = 0
        while del_x**2 + del_y**2 < r**2 :
                del_x += h
                del_y = P(xp + del_x) - yp
                # print(del_x, del_y)

        # input()

        return math.atan2(del_y, del_x)

def quad_fit(p):
        x = np.array([p[0][0], p[1][0], p[2][0]])
        y = np.array([p[0][1], p[1][1], p[2][1]])
        
        p = np.polyfit(x, y, 2)
        
        return p

def quad_theta(p, v, h, dt):
        q = quad_fit(p)
        z = np.poly1d(q)

        xp = p[0][0]
        yp = p[0][1]
        x2 = p[2][0]
        x1 = p[1][0]
        
        
        x_lst = [x/1000.0 for x in range(int(xp*1000), int((x2)*1000))]
        x = np.array(x_lst)
        y = z(x)
        # plt.plot(x,y)

        r = v*dt
        del_x = 0
        del_y = 0
        while del_x**2 + del_y**2 < r**2 :
                del_x += h
                del_y = z(xp + del_x) - yp

        return math.atan2(del_y, del_x)


def line_fit(p):

        sx = 0
        sx2 = 0
        sy = 0
        sxy = 0

        n = len(p);

        for i in range(n) :
                x = p[i][0];
                y = p[i][1];

                sx += x;
                sy += y;
                sx2 += (x*x);
                sxy += (x*y);

        a = (n * sxy - sx*sy) / (n*sx2 - sx*sx);
        b = (sy - a*sx) / n;

        return [a, b];

def hyper_fit(p):

        tol = 0.0001;
        q = [];

        for i in range(len(p)):
                if (abs(p[i][0])) < tol :
                        p[i][0] = 2*tol;
                q.append([1/p[i][0], p[i][1]]);

        return line_fit(q);                

        pass


def hyper_theta(p, v, h, dt):
        q = hyper_fit(p)
        a = q[0]
        b = q[1]

        z = lambda x : ((a/x) + b);

        xp = p[0][0]
        yp = p[0][1]
        x2 = p[2][0]
        x1 = p[1][0]
        
        
        x_lst = [x/1000.0 for x in range(int(xp*1000), int((x2)*1000))]
        x = np.array(x_lst)
        y = z(x)
        plt.plot(x,y)
        # plt.show()

        r = v*dt
        del_x = 0
        del_y = 0
        while del_x**2 + del_y**2 < r**2 :
                del_x += h
                del_y = z(xp + del_x) - yp

        return math.atan2(del_y, del_x)

class Field:
        
        def __init__(self, xc, yc, id, constraint, next):
                self.xc = xc
                self.yc = yc
                self.id = id
                self.constraint = constraint
                self.next = next

        def nextFieldReached(self, state):
                return (self.next.constraint(state))

        def angleOfAttack(self, state, vel, dt):

                if self.id == 'H' or self.id == 'I' or self.id == 'J' or self.id == 'K' or self.id == 'S1' or self.id == 'S2':
                        dx = self.next.xc - state[0]
                        dy = self.next.yc - state[1]
                        # print(dx, dy)
                        return math.atan2(dy, dx)
                else:
                        p0 = [state[0], state[1]]
                        p1 = [self.next.xc, self.next.yc]
                        p2 = [self.next.next.xc, self.next.next.yc]

                        p = [p0, p1, p2]
                        dx = p1[0] - p0[0]
                        dy = p1[1] - p0[1]

                        theta = 0
                        zd_tol = 0.1
                        zt_tol = 0.001

                        p01 = [(x-y) for x,y in zip(p1,p0)];
                        p12 = [(x-y) for x,y in zip(p2,p1)];

                        p01_dot_p12 = sum(p*q for p,q in zip(p01, p12))

                        mag = lambda x: math.sqrt(sum(p*p for p in x))

                        alpha = math.acos(p01_dot_p12 / (mag(p01) * mag(p12)))

                        if (abs(dx) < zd_tol) or (abs(dy) < zd_tol) :
                                theta = math.atan2(dy, dx)
                        elif (abs(alpha) < zt_tol) :
                                theta = math.atan2(dy, dx)
                        else :
                                theta = quad_theta(p, vel, 0.001, dt)
                                # print("Here")

                        return theta


def is_Bounded(rect, state):
        # Since the axis of the rectangle are aligned with the axes, we can
        # easily calculate if a point is inside the rectange

        ux = rect[0]
        lx = rect[1]
        uy = rect[2]
        ly = rect[3]

        # Correct the bounds for checking whether the robot has reached within
        # next state's zone
        if (ux > lx) :
                mx = ux
                mnx = lx
        else :
                mnx = ux
                mx = lx
        
        if (uy > ly) :
                my = uy
                mny = ly
        else :
                mny = uy
                my = ly

        x = state[0]
        y = state[1]

        # Check if x is bounded
        if (x > mx or x < mnx) :
                return False

        # Check if y is bounded
        if (y > my or y < mny) :
                return False

        return True


def fieldA_cons(state):
        rect = [0, 0.1, 0, 0.1]
        
        return is_Bounded(rect, state)


def fieldB_cons(state):
        rect = [1.4, 1.8, 1.85, 1.65]
        
        return is_Bounded(rect, state)


def fieldC_cons(state):
        rect = [2.70, 2.5, 1.55, 1.75]
        
        return is_Bounded(rect, state)


def fieldD_cons(state):
        rect = [3.1, 2.9, 0.5, 0.9]
        
        return is_Bounded(rect, state)


def fieldE_cons(state):
        rect = [3.8, 4.2, 0.5, 0.8]
        
        return is_Bounded(rect, state)


def fieldF_cons(state):
        rect = [4.3, 4.8, 1.65, 1.9]
        
        return is_Bounded(rect, state)


def fieldG_cons(state):
        rect = [5.8, 5.3, 1.9, 1.6]

        return is_Bounded(rect, state)


def fieldH_cons(state):
        rect = [6.0, 6.5, 1.1, 1.35]

        return is_Bounded(rect, state)


def fieldI_cons(state):
        rect = [8.5, 9.0, 1.125, 1.35]

        return is_Bounded(rect, state)


def fieldJ_cons(state):
        rect = [8.5, 9.0, 4.7, 5.3]

        return is_Bounded(rect, state)


def fieldK_cons(state):
        rect = [8.5, 9.0, 3.7, 4.2]

        return is_Bounded(rect, state)


def fieldL_cons(state):
        rect = [4.7, 5.3, 3.7, 4.2]

        return is_Bounded(rect, state)
