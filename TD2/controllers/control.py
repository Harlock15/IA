import numpy as np
import skfuzzy as fuzz
from skfuzzy import control as ctrl
from controller import Robot

# Create a robot instance and get the timestep
robot = Robot()
timestep = int(robot.getBasicTimeStep())

# Distance sensors initialization
ps = []
sensor_names = ['ps1', 'ps2', 'ps5', 'ps6']
for name in sensor_names:
    sensor = robot.getDevice(name)
    if sensor:
        sensor.enable(timestep)
    ps.append(sensor)

# Motors initialization
left_motor = robot.getDevice('left wheel motor')
right_motor = robot.getDevice('right wheel motor')
left_motor.setPosition(float('inf'))
right_motor.setPosition(float('inf'))
left_motor.setVelocity(0.0)
right_motor.setVelocity(0.0)

# Define fuzzy variables
right_dist = ctrl.Antecedent(np.arange(0, 1.01, 0.01), 'right_dist')
left_dist = ctrl.Antecedent(np.arange(0, 1.01, 0.01), 'left_dist')
speed = ctrl.Consequent(np.arange(0, 1.01, 0.01), 'speed')
orientation = ctrl.Consequent(np.arange(-1, 1.01, 0.01), 'orientation')

# Define fuzzy membership functions for inputs (right_dist, left_dist)
right_dist['close'] = fuzz.trimf(right_dist.universe, [0.0, 0.0, 0.5])
right_dist['far'] = fuzz.trimf(right_dist.universe, [0.2, 1.0, 1.0])

left_dist['close'] = fuzz.trimf(left_dist.universe, [0.0, 0.0, 0.5])
left_dist['far'] = fuzz.trimf(left_dist.universe, [0.2, 1.0, 1.0])

# Define fuzzy membership functions for output (speed)
speed['slow'] = fuzz.trimf(speed.universe, [0.0, 0.0, 0.5])
speed['fast'] = fuzz.trimf(speed.universe, [0.5, 1.0, 1.0])

# Define fuzzy membership functions for output (orientation)
orientation['left'] = fuzz.trimf(orientation.universe, [-1.0, -1.0, 0])
orientation['straight'] = fuzz.trimf(orientation.universe, [-0.1, 0, 0.1])
orientation['right'] = fuzz.trimf(orientation.universe, [0, 1.0, 1.0])

# Define fuzzy rules
rule1 = ctrl.Rule(right_dist['close'] & left_dist['far'], orientation['left'])
rule2 = ctrl.Rule(left_dist['close'] & right_dist['far'], orientation['right'])
rule3 = ctrl.Rule(right_dist['close'] & left_dist['close'], orientation['straight'])

rule4 = ctrl.Rule(right_dist['far'] & left_dist['far'], speed['fast'])
rule5 = ctrl.Rule(right_dist['close'] | left_dist['close'], speed['slow'])

# Control system creation
orientation_ctrl = ctrl.ControlSystem([rule1, rule2, rule3])
orientation_sim = ctrl.ControlSystemSimulation(orientation_ctrl)

speed_ctrl = ctrl.ControlSystem([rule4, rule5])
speed_sim = ctrl.ControlSystemSimulation(speed_ctrl)

# Function to normalize sensor values to [0, 1]
def normalize(sensor_value):
    return sensor_value / 4096.0

# Main loop
while robot.step(timestep) != -1:
    # Read and normalize sensor values
    right_sensor = max(normalize(ps[2].getValue()), normalize(ps[3].getValue()))
    left_sensor = max(normalize(ps[0].getValue()), normalize(ps[1].getValue()))

    # Set the fuzzy inputs
    orientation_sim.input['right_dist'] = right_sensor
    orientation_sim.input['left_dist'] = left_sensor
    speed_sim.input['right_dist'] = right_sensor
    speed_sim.input['left_dist'] = left_sensor

    # Compute fuzzy outputs
    orientation_sim.compute()
    speed_sim.compute()

    # Get the output from the fuzzy system
    orientation_value = orientation_sim.output['orientation']
    speed_value = speed_sim.output['speed']

    # Set motor speeds based on fuzzy control output
    max_speed = 6.28  # Maximum motor speed in Webots
    if orientation_value < 0:  # Turn left
        left_speed = max_speed * speed_value * (1 + orientation_value)
        right_speed = max_speed * speed_value
    elif orientation_value > 0:  # Turn right
        left_speed = max_speed * speed_value
        right_speed = max_speed * speed_value * (1 - orientation_value)
    else:  # Go straight
        left_speed = max_speed * speed_value
        right_speed = max_speed * speed_value

    # Apply motor speeds
    left_motor.setVelocity(left_speed)
    right_motor.setVelocity(right_speed)
