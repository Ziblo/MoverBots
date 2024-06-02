from inputs import get_gamepad
import serial
import struct
import threading

class EncodedGamepad:
    def __init__(self):
        self.decode_joy_dict = {
            'ABS_X':  JoyAxis('Lx', 1),
            'ABS_Y':  JoyAxis('Ly', 2),
            'ABS_RX': JoyAxis('Rx', 3),
            'ABS_RY': JoyAxis('Ry', 4),
        }
        self.decode_btns_dict = {
            'BTN_NORTH':  Btn('X', 5),
            'BTN_SOUTH':  Btn('B', 6),
            'BTN_EAST':   Btn('A', 7),
            'BTN_WEST':   Btn('Y', 8),
            'BTN_SELECT': Btn('+', 9),
            'BTN_START':  Btn('-', 10),
        }
    
    def do_event(self, event):
        if event.code in self.decode_joy_dict:
            # joystick update
            axis = self.decode_joy_dict[event.code]
            if axis.new_value(event.state): #if it has a new value
                val = axis.stable_value
                # print(axis, val)
                packed_data = pack_joy_data(axis, val)
                send_to_serial(packed_data)
        if event.code in self.decode_btns_dict:
            btn = self.decode_btns_dict[event.code]
            # button update
            # print(self.decode_btns_dict[event.code], event.state)
            packed_data = pack_btn_data(btn, bool(event.state))
            send_to_serial(packed_data)

class JoyAxis:
    def __init__(self, name : str, index : int):
        self.value : int = 0
        self.stable_value : int = 0
        self.index = index
        #hard coded values
        self.hysterisis_size : int = 1000
        self.deadzone : int = 3000
        self.maximum : int = 32767
        self.name : str = name
    
    def hysterisis_filter(self) -> bool:
        if abs(self.value - self.stable_value) > self.hysterisis_size:
            self.stable_value = self.value
            return True
        return False
        
    def deadzone_filter(self) -> bool:
        val = abs(self.value)
        if val < self.deadzone:
            self.stable_value = 0
        elif val > self.maximum:
            self.stable_value = int(val/(self.value) * self.maximum)
        else:
            return False
        return True
    
    def new_value(self, new_value) -> bool:
        stable = self.stable_value
        self.value = int(new_value)
        if not self.deadzone_filter():
            self.hysterisis_filter()
        if stable != self.stable_value:
            return True
        return False

    def __str__(self) -> str:
        return self.name

class Btn:
    def __init__(self, name : str, index : int):
        self.name = name
        self.index = index
    
    def __str__(self) -> str:
        return self.name

def pack_btn_data(btn, val: bool) -> bytearray:
    # Pack button data into a bytearray.
    # Format: [0xAF, btn_index, val, 0x0A]
    return bytearray([0xAF, btn.index, int(val), 0x0A])

def pack_joy_data(axis, val: int) -> bytearray:
    # Pack joystick data into a bytearray.
    # Format: [0xAF, axis_index, packed_val, 0x0A]
    # Ensure val fits within the range of a signed 16-bit integer
    val = max(-32768, min(32767, val))
    # Pack val as a signed 16-bit integer ('<h' for little-endian, signed 16-bit integer)
    packed_val = struct.pack('<h', val)
    # Combine 0xAF, axis_index, packed_val, and 0x0A into a single byte array
    return bytearray([0xAF, axis.index]) + packed_val + bytearray([0x0A])

def send_to_serial(data: bytearray):
    # Send data to the serial port and print the hex representation for debugging.
    ser.write(data)
    print('Sent data:', ''.join(format(x, '02x') for x in data))


def gamepad_event_handler():
    while True:
        events = get_gamepad()
        for event in events:
            if event.ev_type != 'Sync':
                gp.do_event(event)

if __name__ == '__main__':
    ser = serial.Serial("COM5", 115200)
    gp = EncodedGamepad()
    
    # Start a separate thread to handle gamepad events
    gamepad_thread = threading.Thread(target=gamepad_event_handler, daemon=True)
    gamepad_thread.start()
    
    # Main loop for reading from serial port
    while True:
        while ser.in_waiting > 0:
            line = ser.readline().decode('ascii').strip()
            if line:  # Check if line is not empty
                print(line)