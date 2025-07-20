from constants import *
import PID
from RealFlightBridge.utils import *

curr_mode = SYSTEM_MODE.MANUAL_MODE
raw_controls = []

elevatorPID = PID.rcPID(ELEV_RATE_KP, ELEV_RATE_KI, ELEV_RATE_KD)
aileronPID = PID.rcPID(AIL_RATE_KP, AIL_RATE_KI, AIL_RATE_KD)

accum = 0
accum2 = 0

def runRC(controllerInputs):
    global raw_controls
    raw_controls = controllerInputs
    
def getElevator(mode):
    if (mode == SYSTEM_MODE.MANUAL_MODE): scaler = ELEV_MANUAL
    elif (mode == SYSTEM_MODE.RATE_MODE): scaler = ELEV_RATE
    else: scaler = ELEV_ANGLE
    return raw_controls[1] * scaler

def getAileron(mode):
    if (mode == SYSTEM_MODE.MANUAL_MODE): scaler = AIL_MANUAL
    elif (mode == SYSTEM_MODE.RATE_MODE): scaler = AIL_RATE
    else: scaler = AIL_ANGLE
    return raw_controls[0] * scaler


def getGear():
    return raw_controls[6] > 0

def getAux():
    return raw_controls[5] > 0


def runFSM():
    global curr_mode
    prev_mode = curr_mode
    if (getAux()):
        switchmode = SWITCH_MODE.RATE_MANUAL
    else:
        switchmode = SWITCH_MODE.ANGLE_MANUAL

    
    if (getGear()):
        if (switchmode == SWITCH_MODE.RATE_MANUAL):
            curr_mode = SYSTEM_MODE.RATE_MODE
        else:
            curr_mode = SYSTEM_MODE.ANGLE_MODE
    else:
        curr_mode = SYSTEM_MODE.MANUAL_MODE

    if curr_mode != prev_mode:
        print("MODE CHANGED TO", curr_mode)
        if (curr_mode == SYSTEM_MODE.RATE_MODE):
            elevatorPID.resetConstants(ELEV_RATE_KP, ELEV_RATE_KI, ELEV_RATE_KD)
            aileronPID.resetConstants(AIL_RATE_KP, AIL_RATE_KI, AIL_RATE_KD)
        else:
            print("IMPLEMENT")
            elevatorPID.resetConstants(ELEV_ANGLE_KP, ELEV_ANGLE_KI, ELEV_ANGLE_KD)
            aileronPID.resetConstants(AIL_ANGLE_KP, AIL_ANGLE_KI, AIL_ANGLE_KD)
        elevatorPID.resetIntegral()
        aileronPID.resetIntegral()


def runController(orientation, angular_velocity, t):
    global prev_ail, accum, accum2
    elevator_output = 0
    aileron_output = 0
    match curr_mode:
        case SYSTEM_MODE.MANUAL_MODE:

            elevator_output = getElevator(curr_mode)
            aileron_output = getAileron(curr_mode)
        case SYSTEM_MODE.RATE_MODE:
            PID.rcPID.updatePIDLoopTime(t)
  
            pitch_rate_error = angular_velocity[1] - getElevator(curr_mode)
            roll_rate_error = angular_velocity[0] - getAileron(curr_mode)
            elevator_output = -elevatorPID.updatePID(pitch_rate_error, 0)
            aileron_output = aileronPID.updatePID(roll_rate_error, 0)
            elevatorPID.cumm_error *= .999
            print(elevatorPID.cumm_error)
        
        case SYSTEM_MODE.ANGLE_MODE:
            elevator_output = -elevatorPID.updatePID(orientation[1], getElevator(curr_mode))
            #print(angular_velocity[1])
            aileron_output = aileronPID.updatePID(orientation[0], getAileron(curr_mode))
            
    
    out = ""
    for i in raw_controls:
        out += str(round(i, 3)) + "\t" + "\t"
    #print(out)

    return [aileron_output, elevator_output] + raw_controls[2:]