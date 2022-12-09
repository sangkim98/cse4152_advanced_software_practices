from cmath import sqrt
import matplotlib
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import find_peaks
from scipy.interpolate import splprep, splev
from scipy.optimize import minimize
import time


class LaneDetection:
    '''
    Lane detection module using edge detection and b-spline fitting

    args: 
        cut_size (cut_size=65) cut the image at the front of the car
        spline_smoothness (default=10)
        gradient_threshold (default=14)
        distance_maxima_gradient (default=3)

    '''

    def __init__(self, cut_size=65, spline_smoothness=10, gradient_threshold=80, distance_maxima_gradient=8):
        self.car_position = np.array([48,0])
        self.spline_smoothness = spline_smoothness
        self.cut_size = cut_size
        self.gradient_threshold = gradient_threshold
        self.distance_maxima_gradient = distance_maxima_gradient
        self.lane_boundary1_old = 0
        self.lane_boundary2_old = 0


    def cut_gray(self, state_image_full):
        '''
        ##### TODO #####
        This function should cut the image at the front end of the car (e.g. pixel row 65) 
        and translate to gray scale

        input:
            state_image_full 96x96x3

        output:
            gray_state_image 65x96x1
        '''
        
        state_image_cropped = state_image_full[:self.cut_size,:,:]

        r, g, b = state_image_cropped[:,:,[0]], state_image_cropped[:,:,[1]], state_image_cropped[:,:,[2]]

        gray_state_image = 0.2990 * r + 0.5870 * g + 0.1140 * b
        
        return gray_state_image[::-1]


    def edge_detection(self, gray_image):
        '''
        ##### TODO #####
        In order to find edges in the gray state image, 
        this function should derive the absolute gradients of the gray state image.
        Derive the absolute gradients using numpy for each pixel. 
        To ignore small gradients, set all gradients below a threshold (self.gradient_threshold) to zero. 

        input:
            gray_state_image 65x96x1

        output:
            gradient_sum 65x96x1

        '''

        kX = np.array([[-1,0,1],
                       [-2,0,2],
                       [-1,0,1]])

        kY = np.array([[1, 2, 1],
                       [0, 0, 0],
                       [-1,-2,-1]])

        kX = kX.reshape((3,3,1))
        kY = kY.reshape((3,3,1))

        gradient_sum = np.full_like(gray_image, 0)

        padded_gray_image = np.pad(gray_image, pad_width=((1,1),(1,1),(0,0)), mode='edge')

        for y_idx in range(len(gray_image[0])):
            for x_idx in range(len(gray_image)):
                gX = np.sum(np.multiply(kX, padded_gray_image[x_idx:x_idx+3, y_idx:y_idx+3, [0]]))
                gY = np.sum(np.multiply(kY, padded_gray_image[x_idx:x_idx+3, y_idx:y_idx+3, [0]]))
                abs_gradient = np.sqrt((gX)**2 + (gY)**2)

                gradient_sum[x_idx, y_idx, 0] = 0 if (abs_gradient < self.gradient_threshold) else abs_gradient
        
        return gradient_sum


    def find_maxima_gradient_rowwise(self, gradient_sum):
        '''
        ##### TODO #####
        This function should output arguments of local maxima for each row of the gradient image.
        You can use scipy.signal.find_peaks to detect maxima. 
        Hint: Use distance argument for a better robustness.

        input:
            gradient_sum 65x96x1

        output:
            maxima (np.array) shape : (Number_maxima, 2)

        '''

        argmaxima = []

        for row_idx, row in enumerate(gradient_sum):
            peaks, _ = find_peaks(row[:,0], distance=self.distance_maxima_gradient)
            for peak_col_idx in peaks:
                argmaxima.append([peak_col_idx, row_idx])

        argmaxima = np.array(argmaxima)

        return argmaxima


    def find_first_lane_point(self, gradient_sum):
        '''
        Find the first lane_boundaries points above the car.
        Special cases like just detecting one lane_boundary or more than two are considered. 
        Even though there is space for improvement ;) 

        input:
            gradient_sum 65x96x1

        output: 
            lane_boundary1_startpoint
            lane_boundary2_startpoint
            lanes_found  true if lane_boundaries were found
        '''
        
        # Variable if lanes were found or not
        lanes_found = False
        row = 0

        # loop through the rows
        while not lanes_found:
            
            # Find peaks with min distance of at least 3 pixel 
            argmaxima = find_peaks(gradient_sum[row, :, 0], distance=3)[0]

            # if one lane_boundary is found
            if argmaxima.shape[0] == 1:
                lane_boundary1_startpoint = np.array([[argmaxima[0],  row]])

                if argmaxima[0] < 48:
                    lane_boundary2_startpoint = np.array([[0,  row]])
                else: 
                    lane_boundary2_startpoint = np.array([[96,  row]])

                lanes_found = True
            
            # if 2 lane_boundaries are found
            elif argmaxima.shape[0] == 2:
                lane_boundary1_startpoint = np.array([[argmaxima[0],  row]])
                lane_boundary2_startpoint = np.array([[argmaxima[1],  row]])
                lanes_found = True

            # if more than 2 lane_boundaries are found
            elif argmaxima.shape[0] > 2:
                # if more than two maxima then take the two lanes next to the car, regarding least square
                A = np.argsort((argmaxima - self.car_position[0])**2)
                lane_boundary1_startpoint = np.array([[argmaxima[A[0]],  0]])
                lane_boundary2_startpoint = np.array([[argmaxima[A[1]],  0]])
                lanes_found = True

            row += 1
            
            # if no lane_boundaries are found
            if row == self.cut_size:
                lane_boundary1_startpoint = np.array([[0,  0]])
                lane_boundary2_startpoint = np.array([[0,  0]])
                break

        return lane_boundary1_startpoint, lane_boundary2_startpoint, lanes_found


    def lane_detection(self, state_image_full):
        '''
        ##### TODO #####
        This function should perform the road detection 

        args:
            state_image_full [96, 96, 3]

        out:
            lane_boundary1 spline
            lane_boundary2 spline
        '''

        # to gray
        gray_state = self.cut_gray(state_image_full)

        # edge detection via gradient sum and thresholding
        gradient_sum = self.edge_detection(gray_state)
        maxima = self.find_maxima_gradient_rowwise(gradient_sum)

        # first lane_boundary points
        lane_boundary1_points, lane_boundary2_points, lane_found = self.find_first_lane_point(gradient_sum)
        
        # if no lane was found,use lane_boundaries of the preceding step
        if lane_found:
            
            ##### TODO #####
            #  in every iteration: 
            # 1- find maximum/edge with the lowest distance to the last lane boundary point 
            # 2- append maximum to lane_boundary1_points or lane_boundary2_points
            # 3- delete maximum from maxima
            # 4- stop loop if there is no maximum left 
            #    or if the distance to the next one is too big (>=100)

            # lane_boundary 1
            
            # lane_boundary 2

            ################

            lane_boundary1 = 0
            lane_boundary2 = 0

            distance = 0

            while (maxima.size != 0) and (distance < 10):                
                
                distance_to_lane1_point = np.sum(np.abs(maxima-lane_boundary1_points[-1,:]), axis=1)
                distance_to_lane2_point = np.sum(np.abs(maxima-lane_boundary2_points[-1,:]), axis=1)

                distance_to_lane1_point_idx = np.argmin(distance_to_lane1_point)
                distance_to_lane2_point_idx = np.argmin(distance_to_lane2_point)

                distance, lane_idx_to_append = (distance_to_lane1_point[distance_to_lane1_point_idx], 1) \
                                            if distance_to_lane1_point[distance_to_lane1_point_idx] < distance_to_lane2_point[distance_to_lane2_point_idx] \
                                            else (distance_to_lane2_point[distance_to_lane2_point_idx], 2)

                if lane_idx_to_append == 1:
                    lane_boundary1_points = np.vstack((lane_boundary1_points, maxima[distance_to_lane1_point_idx]))
                    maxima = np.delete(maxima, distance_to_lane1_point_idx, axis=0)
                else:
                    lane_boundary2_points = np.vstack((lane_boundary2_points, maxima[distance_to_lane2_point_idx]))
                    maxima = np.delete(maxima, distance_to_lane2_point_idx, axis=0)
           
            ##### TODO #####
            # spline fitting using scipy.interpolate.splprep 
            # and the arguments self.spline_smoothness
            # 
            # if there are more lane_boundary points points than spline parameters 
            # else use perceding spline
            if lane_boundary1_points.shape[0] > 4 and lane_boundary2_points.shape[0] > 4:

                # Pay attention: the first lane_boundary point might occur twice
                # lane_boundary 1

                lane_boundary1, _ = splprep([lane_boundary1_points[1:,0], lane_boundary1_points[1:,1]], s=self.spline_smoothness, k=2)
                
                # lane_boundary 2
                
                lane_boundary2, _ = splprep([lane_boundary2_points[1:,0], lane_boundary2_points[1:,1]], s=self.spline_smoothness, k=2)

            else:
                lane_boundary1 = self.lane_boundary1_old
                lane_boundary2 = self.lane_boundary2_old
            ################

        else:
            lane_boundary1 = self.lane_boundary1_old
            lane_boundary2 = self.lane_boundary2_old

        self.lane_boundary1_old = lane_boundary1
        self.lane_boundary2_old = lane_boundary2

        # output the spline
        return lane_boundary1, lane_boundary2

    def plot_state_lane(self, state_image_full, steps, fig, waypoints=[]):
        '''
        Plot lanes and way points
        '''
        # evaluate spline for 6 different spline parameters.
        t = np.linspace(0, 1, 6)
        lane_boundary1_points_points = np.array(splev(t, self.lane_boundary1_old))
        lane_boundary2_points_points = np.array(splev(t, self.lane_boundary2_old))
        
        plt.gcf().clear()
        plt.imshow(state_image_full[::-1])
        plt.plot(lane_boundary1_points_points[0], lane_boundary1_points_points[1]+96-self.cut_size, linewidth=5, color='orange')
        plt.plot(lane_boundary2_points_points[0], lane_boundary2_points_points[1]+96-self.cut_size, linewidth=5, color='orange')
        if len(waypoints):
            plt.scatter(waypoints[0], waypoints[1]+96-self.cut_size, color='white')

        plt.axis('off')
        plt.xlim((-0.5,95.5))
        plt.ylim((-0.5,95.5))
        plt.gca().axes.get_xaxis().set_visible(False)
        plt.gca().axes.get_yaxis().set_visible(False)
        fig.canvas.flush_events()