import time

class DS18B20():
    def __init__(self, tempSensor):
        self.tempSensorDevice = tempSensor

    def read_rom(self):
        with open(self.tempSensorDevice +'/name', 'r') as f:
            return f.readline().rstrip()
        return ""

    def read_temp_raw(self):
        with open(self.tempSensorDevice + '/w1_slave', 'r') as f:
            return f.readlines()
        return ""

    def read_temp(self):
        lines = self.read_temp_raw()
        # Analyze if the last 3 characters are 'YES'.
        while lines[0].strip()[-3:] != 'YES':
            time.sleep(0.2)
            lines = self.read_temp_raw()
        # Find the index of 't=' in a string.
        equals_pos = lines[1].find('t=')
        if equals_pos != -1:
            # Read the temperature .
            temp_string = lines[1][equals_pos + 2:]
            temp_c = float(temp_string) / 1000.0
            temp_f = temp_c * 9.0 / 5.0 + 32.0
            return temp_c, temp_f
