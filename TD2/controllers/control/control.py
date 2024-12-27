# Copyright 1996-2023 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Description: Default controller of the e-puck robot in Python

from controller import Robot

# Constants
DISTANCE_SENSORS_NUMBER = 8
GROUND_SENSORS_NUMBER = 3
LEDS_NUMBER = 10
MAX_SPEED = 6.28

# Device names
distance_sensors_names = ["ps0", "ps1", "ps2", "ps3", "ps4", "ps5", "ps6", "ps7"]
ground_sensors_names = ["gs0", "gs1", "gs2"]
leds_names = ["led0", "led1", "led2", "led3", "led4", "led5", "led6", "led7", "led8", "led9"]

# Braitenberg weights and offsets
weights = [[-1.3, -1.0], [-1.3, -1.0], [-0.5, 0.5], [0.0, 0.0],
           [0.0, 0.0], [0.05, -0.5], [-0.75, 0], [-0.75, 0]]
offsets = [0.5 * MAX_SPEED, 0.5 * MAX_SPEED]


class EPuckController:
    def __init__(self):
        self.robot = Robot()
        self.time_step = int(self.robot.getBasicTimeStep())
        
        # Initialize sensors, motors, and LEDs
        self.distance_sensors = [self.robot.getDevice(name) for name in distance_sensors_names]
        for sensor in self.distance_sensors:
            sensor.enable(self.time_step)
        
        self.ground_sensors = [self.robot.getDevice(name) for name in ground_sensors_names]
        for sensor in self.ground_sensors:
            sensor.enable(self.time_step)
        
        self.leds = [self.robot.getDevice(name) for name in leds_names]
        
        self.left_motor = self.robot.getDevice('left wheel motor')
        self.right_motor = self.robot.getDevice('right wheel motor')
        self.left_motor.setPosition(float('inf'))
        self.right_motor.setPosition(float('inf'))
        self.left_motor.setVelocity(0.0)
        self.right_motor.setVelocity(0.0)
    
    def reset_actuator_values(self):
        self.speeds = [0.0, 0.0]
        self.leds_values = [False] * LEDS_NUMBER
    
    def get_sensor_input(self):
        self.distance_sensors_values = [sensor.getValue() / 4096 for sensor in self.distance_sensors]
        self.ground_sensors_values = [sensor.getValue() for sensor in self.ground_sensors]
    
    def cliff_detected(self):
        for value in self.ground_sensors_values:
            if value < 500.0:
                return True
        return False
    
    def blink_leds(self):
        counter = int(self.robot.getTime() * 10)
        self.leds_values[counter % LEDS_NUMBER] = True
    
    def set_actuators(self):
        for i in range(LEDS_NUMBER):
            self.leds[i].set(self.leds_values[i])
        self.left_motor.setVelocity(self.speeds[0])
        self.right_motor.setVelocity(self.speeds[1])
    
    def run_braitenberg(self):
        for i in range(2):
            self.speeds[i] = offsets[i]
            for j in range(DISTANCE_SENSORS_NUMBER):
                self.speeds[i] += self.distance_sensors_values[j] * weights[j][i] * MAX_SPEED
            self.speeds[i] = min(MAX_SPEED, max(-MAX_SPEED, self.speeds[i]))
    
    def go_backwards(self):
        self.left_motor.setVelocity(-MAX_SPEED)
        self.right_motor.setVelocity(-MAX_SPEED)
        self.robot.step(self.time_step * 10)  # 0.2 seconds
    
    def turn_left(self):
        self.left_motor.setVelocity(-MAX_SPEED)
        self.right_motor.setVelocity(MAX_SPEED)
        self.robot.step(self.time_step * 10)  # 0.2 seconds
    
    def run(self):
        while self.robot.step(self.time_step) != -1:
            self.reset_actuator_values()
            self.get_sensor_input()
            self.blink_leds()
            if self.cliff_detected():
                self.go_backwards()
                self.turn_left()
            else:
                self.run_braitenberg()
            self.set_actuators()


if __name__ == "__main__":
    controller = EPuckController()
    controller.run()
