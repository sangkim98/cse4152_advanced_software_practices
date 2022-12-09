import time

import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import splev, splprep
from scipy.optimize import minimize
from scipy.signal import find_peaks


class LateralController:
    '''
    Lateral control using the Stanley controller

    functions:
        stanley 

    init:
        gain_constant (default=5)
        damping_constant (default=0.5)
    '''

    def __init__(self, gain_constant=4, damping_constant=0.7):

        self.gain_constant = gain_constant
        self.damping_constant = damping_constant
        self.previous_steering_angle = 0
        self.car_dir = np.array([0,1])

    def stanley(self, waypoints, speed):
        '''
        ##### TODO #####
        one step of the stanley controller with damping
        args:
            waypoints (np.array) [2, num_waypoints]
            speed (float)
        '''

        # derive orientation error as the angle of the first path segment to the car orientation

        # derive stanley control law
        # derive cross track error as distance between desired waypoint at spline parameter equal zero and the car position
        # prevent division by zero by adding as small epsilon 
        # derive damping
        
        # clip to the maximum stering angle (0.4) and rescale the steering action space

        """
        crosstrack error : waypoints의 맨 처음값 - (48,0)
        heading error : waypoints의 첫 번째 인덱스 값과 그 다음 인덱스에 해당되는 값 사용
        """

        epsilon = 1E-10

        target_dir = np.array([waypoints[0][1]-waypoints[0][0], waypoints[1][1]-waypoints[1][0]])

        heading_error = np.arcsin((np.cross(target_dir, self.car_dir)) / (np.linalg.norm(target_dir) * np.linalg.norm(self.car_dir)))
        crosstrack_error = np.linalg.norm(np.array([waypoints[0][0],waypoints[1][0]]) - np.array([47.5,0]))

        if (waypoints[0][0] - 47.5) < 0:
            crosstrack_error = -crosstrack_error

        second_term = np.arctan(self.gain_constant * crosstrack_error / (epsilon + speed))

        steering = np.clip(heading_error+second_term, -0.4, 0.4)

        damped_steering = steering - self.damping_constant*(steering - self.previous_steering_angle)

        self.previous_steering_angle = damped_steering

        return damped_steering