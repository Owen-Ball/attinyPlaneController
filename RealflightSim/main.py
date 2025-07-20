from RealFlightBridge.realflightbridge import RealFlightBridge
from RealFlightBridge.utils import *
import constants
import time
import controller


def runBridge(IP="127.0.0.1"):
    bridge = RealFlightBridge(IP)
    if not bridge.connect():
        raise("Unable to connect to RealFlight, please check connection!") 
    bridge.start()
    bridge.set_controls([0., 0., 0.0, 0., 0.5])

    running = True
    prev_time = time.time()

    while running:
        try:

            curr_time = time.time()
            loop_time = curr_time - prev_time
            prev_time = curr_time       
  
            controllerInputs = bridge.inputControls
            orientation = (bridge.roll, bridge.pitch, bridge.yaw)
            angular_velocity = bridge.angular_velocity
 
            """
            roll_target = controllerInputs[0]*60
            error = orientation[1] - roll_target 
            action = float_constrain(error * -1/30, -1, 1)
            controllerInputs[0] = action

            pitch_target = controllerInputs[1]*-  60
            error = orientation[0] - pitch_target 
            action = float_constrain(error * 1/30, -1, 1)
            controllerInputs[1] = action
            """
            controller.runRC(controllerInputs)
            controller.runFSM()
            controlValues = controller.runController(orientation, angular_velocity, loop_time)

            bridge.set_controls(controlValues)  
 
            bridge.update()
         
        except KeyboardInterrupt:
            print("Exit")
            running = False
            bridge.exit()

if __name__ == "__main__":
    runBridge()