import serial
import matplotlib.pyplot as plt
import time, sys
port = '/dev/ttyUSB0'
if len(sys.argv) > 1:
    port = sys.argv[1]
ser = serial.Serial(port, 115200, timeout=1)
weights = []
start = time.time()
try:
    while time.time() - start < 60:
        line = ser.readline().decode(errors='ignore').strip()
        if not line: continue
        print(line)
        if 'w=' in line:
            try:
                w = int(line.split('w=')[-1])
                weights.append((time.time()-start,w))
            except:
                pass
finally:
    ser.close()
if weights:
    ts = [t for t,w in weights]
    ws = [w for t,w in weights]
    plt.plot(ts, ws)
    plt.xlabel('s')
    plt.ylabel('digipot value (0..255)')
    plt.title('STDP weight evolution')
    plt.show()
