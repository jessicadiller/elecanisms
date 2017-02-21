
import usb.core
import time

class jtest:

    def __init__(self):
        self.TOGGLE_LED1 = 1
        self.TOGGLE_LED2 = 2
        self.TOGGLE_LED3 = 3
        self.SET_DUTY_F = 4
        self.SET_DUTY_R = 5
        self.GET_DUTY_F = 6
        self.GET_DUTY_R = 7
        self.GET_ANGLE = 8
        self.GET_ANGLE_SPEED = 9
        self.GET_CURRENT = 10
        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003)
        if self.dev is None:
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()

# AS5048A Register Map
        self.ENC_NOP = 0x0000
        self.ENC_CLEAR_ERROR_FLAG = 0x0001
        self.ENC_PROGRAMMING_CTRL = 0x0003
        self.ENC_OTP_ZERO_POS_HI = 0x0016
        self.ENC_OTP_ZERO_POS_LO = 0x0017
        self.ENC_DIAG_AND_AUTO_GAIN_CTRL = 0x3FFD
        self.ENC_MAGNITUDE = 0x3FFE
        self.ENC_ANGLE_AFTER_ZERO_POS_ADDER = 0x3FFF

    def close(self):
        self.dev = None

    def toggle_led1(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED1)
        except usb.core.USBError:
            print "Could not send TOGGLE_LED1 vendor request."

    def toggle_led2(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED2)
        except usb.core.USBError:
            print "Could not send TOGGLE_LED1 vendor request."

    def toggle_led3(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED3)
        except usb.core.USBError:
            print "Could not send TOGGLE_LED1 vendor request."

    def set_duty_r(self, duty):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_DUTY_R, int(duty))
        except usb.core.USBError:
            print "Could not send SET_DUTY vendor request."

    def set_duty_f(self, duty):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_DUTY_F, int(duty))
        except usb.core.USBError:
            print "Could not send SET_DUTY vendor request."

    def get_duty_f(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_DUTY_F, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send GET_DUTY vendor request."
        else:
            return int(ret[0]) + int(ret[1]) *256

    def get_duty_r(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_DUTY_R, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send GET_DUTY vendor request."
        else:
            return int(ret[0]) + int(ret[1]) *256

    def get_angle(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_ANGLE, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send GET_ANGLE vendor request."
        else:
            return ret

    def get_angle_speed(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_ANGLE_SPEED, 0, 0, 4)
        except usb.core.USBError:
            print "Could not send GET_ANGLE_SPEED vendor request."
        else:
            return ret

    def get_current(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_CURRENT, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send GET_CURRENT vendor request."
        else:
            return ret

