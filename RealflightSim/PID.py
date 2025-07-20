class rcPID(object):
    loop_time = 0
    inv_loop_time = 0

    def __init__(self, KP, KI, KD):
        self.KP = KP
        self.KI = KI
        self.KD = KD
        self.cumm_error = 0
        self.prev_error = 0

    def resetConstants(self, KP, KI, KD):
        self.KP = KP
        self.KI = KI
        self.KD = KD

    def updatePID(self, sensor_val, target):
        error = target - sensor_val
        self.cumm_error += rcPID.loop_time*error
        rate_error = (error - self.prev_error)*rcPID.inv_loop_time
        self.prev_error = error
        return self.KP*error + self.KI*self.cumm_error + self.KD*rate_error
    
    def resetIntegral(self):
        self.cumm_error = 0

    @staticmethod
    def updatePIDLoopTime(t):
        rcPID.loop_time = t
        if t==0: t = 1
        rcPID.inv_loop_time = 1/t