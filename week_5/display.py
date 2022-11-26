def display(input_txt_path, output_txt_path):
    import matplotlib.pyplot as plt
    
    '''
    input1 : input_txt_path = path of input_example.txt
    input2 : output_txt_path = path of output_example.txt
    return : save convex_hull image
    '''
    
    with open(input_txt_path, "r") as f:
        input_lines = f.readlines()
    with open(output_txt_path, "r") as f:
        output_lines = f.readlines()
        
    whole_points = input_lines
    area = round(float(output_lines[0]), 1)
    hull_points = output_lines[1:]

    x_list = [int(x.split(" ")[0]) for x in whole_points]
    y_list = [int(x.split(" ")[1]) for x in whole_points]
    plt.plot(x_list, y_list, marker='.', linestyle='None')

    hx_list = [int(x.split(" ")[0]) for x in hull_points]
    hy_list = [int(x.split(" ")[1]) for x in hull_points]

    plt.plot(hx_list, hy_list, marker='*', linestyle='None', markersize=10)

    title = plt.title(f'Area : {area}')
    plt.setp(title, color='r')
    plt.savefig(output_txt_path[:-3]+"png", bbox_inches='tight')


if __name__ == "__main__":
    input_txt_path = "./input3.txt"
    output_txt_path = "./output3.txt"
    display(input_txt_path, output_txt_path)

