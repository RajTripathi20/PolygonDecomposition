import turtle
import random
import math

for num_points in range(3, 102):

    # Set up the turtle
    turtle.speed('fastest')
    turtle.penup()
    turtle.goto(-200, 0)
    turtle.pendown()

    # Generate random points
    #num_points = random.randint(4, 10)

    points = []
    for i in range(num_points):
        x = random.randint(-150, 150)
        y = random.randint(-100, 100)
        points.append((x, y))

    # Add current turtle position to the points list
    points.append(turtle.pos())

    # Find the centroid of the polygon
    cx = sum(p[0] for p in points) / (num_points + 1)
    cy = sum(p[1] for p in points) / (num_points + 1)

    # Find the angle between each pair of adjacent points
    angles = []
    for i in range(num_points + 1):
        p1 = points[i]
        p2 = points[(i+1) % (num_points + 1)]
        angle = math.atan2(p2[1] - p1[1], p2[0] - p1[0])
        angles.append(angle)

    # Sort the points by angle
    points_sorted = sorted(points, key=lambda p: (180 + math.degrees(math.atan2(p[1]-cy, p[0]-cx))) % 360)

    # Check if the points are in clockwise order and reverse if needed
    area = 0.0
    for i in range(num_points + 1):
        p1 = points_sorted[i]
        p2 = points_sorted[(i+1) % (num_points + 1)]
        area += p1[0]*p2[1] - p2[0]*p1[1]
    if area > 0:
        points_sorted.reverse()

    # # Draw the polygon
    # turtle.fillcolor('gray')
    # turtle.begin_fill()
    # turtle.goto(points_sorted[0])
    # for i in range(num_points + 1):
    #     turtle.goto(points_sorted[i])
    #     turtle.write(f"{points_sorted[i]}", align="left")
    # turtle.end_fill()

    # Reverse the list of vertices to get them in clockwise order
    #vertices = list(reversed(vertices))
    print(points_sorted)
    with open("Inputs/inputRandom"+str(num_points)+".txt",'w') as f:
        f.write(str(len(points_sorted)) + '\n')
        letter="id"
        id = 1
        for each in points_sorted:
             f.write(letter+str(id)+' '+'{:.2f}'.format(round(each[0], 2))+' '+'{:.2f}\n'.format(round(each[0], 2)))
             id += 1


turtle.done()