import time
import platform

class Blink:
    ON = 1
    OFF = 0
    LED_status = OFF

    def __init__(self, ON_func, OFF_func, cycle_time, limit) :
        assert ON_func is not None, "ON_func is None"
        assert OFF_func is not None, "OFF_func is None"
        self.ON_func = ON_func
        self.OFF_func = OFF_func
        self.count = 0
        self.limit = limit
        self.cycle_time = cycle_time

    def task(self):
        if self.LED_status == self.ON:
            self.OFF_func()
            self.LED_status = self.OFF
        else:
            self.ON_func()
            self.LED_status = self.ON
    
    def loop(self):
        while True:
            if self.limit == self.count:
                return
            self.task()
            time.sleep(self.cycle_time)
            self.count += 1

def main():
    if "x86_64" == platform.machine():  # HOST環境だとこっちが動く
        def LED_ON():
            print(time.time(),": LED ON")
        def LED_OFF():
            print(time.time(),": LED OFF")
        blink = Blink(LED_ON, LED_OFF, 0.5, 10)
        blink.loop()
    else:  # ラズベリーパイだとこっちが動く
        import pigpio
        PIN = 17
        pigpio.pi().set_mode(PIN, pigpio.OUTPUT)
        def LED_ON():
            pigpio.pi().write(PIN,1)
        def LED_OFF():
            pigpio.pi().write(PIN,0)
        blink = Blink(LED_ON, LED_OFF, 0.5, 10)
        blink.loop()

if __name__ == '__main__':
    main()
