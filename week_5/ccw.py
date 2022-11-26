import numpy as np
import math
import matplotlib.pyplot as plt

def cross_product(v1, v2):
    return v1[0] * v2[1] - v1[1] * v2[0]

def area_of_polygon(points):
    sum = 0

    for point_idx in range(len(points)):
        v1 = points[point_idx]
        v2 = points[(point_idx+1)%len(points)]

        sum += cross_product(v1, v2)

    return sum / 2

def find_convex_hull_points(points):
    points_stack = []

    points_stack.append(points[0])
    points_stack.append(points[1])

    point_idx = 2

    while(point_idx != len(points)):
        previous = (points_stack[-1] - points_stack[-2])
        current = (points[point_idx] - points_stack[-1])

        if(cross_product(previous, current) <= 0):
            points_stack.pop()
        else:
            points_stack.append(points[point_idx])
            point_idx += 1

    return np.array(points_stack)

def inORout_of_polygon(points, points_to_compare):
    
    check_result = list()

    for point_to_check in points_to_compare:
        
        state = True

        for point_idx in range(len(points)):
            v1 = points[(point_idx+1) % len(points)] - points[point_idx]
            v2 = point_to_check - points[point_idx]

            if(cross_product(v1, v2) <= 0):
                state = False
                break

        if(state == False):
            check_result.append("out")
        else:
            check_result.append("in")

    return check_result

def main():
    
    text_input = "input"
    text_output = "output"
    text_txt = ".txt"
    text_point_in_polygon = "point_in_polygon_"

    polygon_input_fileName = text_point_in_polygon+text_input+text_txt
    f_polygon_input = open(polygon_input_fileName)

    points_to_check = list()

    for line in f_polygon_input:
        x, y = line.split()
        points_to_check.append([int(x),int(y)])

    points_to_check = np.array(points_to_check)

    f_polygon_input.close()

    for input_idx in range(1,4):
        points_list = list()

        input_fileName = text_input+str(input_idx)+text_txt
        output_fileName = text_output+str(input_idx)+text_txt
        polygon_output_fileName = text_point_in_polygon+text_output+str(input_idx)+text_txt
        
        f_input = open(input_fileName)

        print(f"File Index : {input_idx}")

        for line in f_input:
            x, y = line.split()
            points_list.append([int(x),int(y)])

        points_list = sorted(points_list, key=lambda x : x[0])
        points_list = sorted(points_list, key=lambda x : x[1])

        starting_point = points_list[0]
        sorted_points = points_list[1:]

        sorted_points = sorted(sorted_points, key=lambda point : (math.atan2(point[1]-starting_point[1], point[0]-starting_point[0]),((point[1]-starting_point[1]**2) + (point[0]-starting_point[0])**2)))

        points_list = [starting_point] + sorted_points

        points_list = np.array(points_list)

        convex_hull_points = find_convex_hull_points(points_list)
        area = area_of_polygon(convex_hull_points)
        in_or_out = inORout_of_polygon(convex_hull_points, points_to_check)

        f_output = open(output_fileName, "w")
        f_polygon_output = open(polygon_output_fileName, "w")

        f_output.write(str(area)+"\n")
        for coord in convex_hull_points:
            f_output.write(f"{coord[0]} {coord[1]}"+"\n")

        for result in in_or_out:
            f_polygon_output.write(result+"\n")

        f_input.close()   
        f_polygon_output.close()
        f_output.close()
    
main()