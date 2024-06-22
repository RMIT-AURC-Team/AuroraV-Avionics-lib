import pathlib
import ctypes
import math
import pandas as pd
import matplotlib.pyplot as plt


class ArmMatrixInstanceF32(ctypes.Structure):
    _fields_ = [
        ('numRows', ctypes.c_uint16),
        ('numCols', ctypes.c_uint16),
        ('pData', ctypes.POINTER(ctypes.c_float)),
    ]


class KalmanFilter(ctypes.Structure):
    pass


KalmanFilter._fields_ = [
    ('A', ArmMatrixInstanceF32),
    ('H', ArmMatrixInstanceF32),
    ('Q', ArmMatrixInstanceF32),
    ('R', ArmMatrixInstanceF32),
    ('P', ArmMatrixInstanceF32),
    ('K', ArmMatrixInstanceF32),
    ('x', ArmMatrixInstanceF32),
    ('z', ArmMatrixInstanceF32),
    ('update', ctypes.CFUNCTYPE(None, ctypes.POINTER(KalmanFilter))),
]

filepath = pathlib.Path(__file__).parent
subroot = filepath.parent.resolve()

# Load the shared library
lib = ctypes.CDLL(f'{subroot}/bin/libkalmanfilter.so')

# Function signatures
lib.KalmanFilter_init.argtypes = [ctypes.POINTER(KalmanFilter)]
lib.KalmanFilter_init.restype = None

lib.KalmanFilter_update.argtypes = [ctypes.POINTER(KalmanFilter)]
lib.KalmanFilter_update.restype = None


if __name__ == "__main__":
    # ================================================
    # Read in CSV data
    # ================================================
    data_h = pd.read_csv(f"{subroot}/test/data_highres.csv")
    data_l = pd.read_csv(f"{subroot}/test/data_lowres.csv")
    t = data_l['Flight_Time_(s)']
    data_count = len(t)

    dt = 0.020

    accel = [
        [d for d in data_h['Accel_X']][61:-30:10],
        [d for d in data_h['Accel_Y']][61:-30:10],
        [d for d in data_h['Accel_Z']][61:-30:10]
    ]
    baro = [d for d in data_l["Baro_Altitude_AGL_(feet)"]]
    vel = data_l["Velocity_Up"]
    cosines = [math.cos(x*(math.pi/180)) for x in data_l["Tilt_Angle_(deg)"]]

    # ================================================
    # Initialise filter parameters
    # ================================================
    # Create KalmanFilter structure
    kf = KalmanFilter()

    # Initialize the Kalman filter with default values
    lib.KalmanFilter_init(ctypes.byref(kf))

    # Set filter parameters
    data = [1.0, dt, 0.5 * (dt**2),
            0.0, 1.0, dt,
            0.0, 0.0, 1.0]
    kf.A = ArmMatrixInstanceF32(
        3, 3, (ctypes.c_float * len(data))(*data))

    data = [1.0, 0.0, 0.0,
            0.0, 0.0, 1.0]
    kf.H = ArmMatrixInstanceF32(
        2, 3, (ctypes.c_float * len(data))(*data))

    data = [99.52, 0.0, 0.0,
            0.0, 1.42, 0.0,
            0.0, 0.0, 6.27]
    kf.Q = ArmMatrixInstanceF32(
        3, 3, (ctypes.c_float * len(data))(*data))

    data = [97.92, 0.0,
            0.0, 0.61]
    kf.R = ArmMatrixInstanceF32(
        2, 2, (ctypes.c_float * len(data))(*data))

    data = [1, 0.0, 0.0,
            0.0, 0.1, 0.0,
            0.0, 0.0, 100.0]
    kf.P = ArmMatrixInstanceF32(
        3, 3, (ctypes.c_float * len(data))(*data))

    data = [0.0, 0.0, 0.0]
    kf.x = ArmMatrixInstanceF32(
        3, 1, (ctypes.c_float * len(data))(*data))

    data = [0.0, 0.0]
    z = ArmMatrixInstanceF32(
        2, 1, (ctypes.c_float * len(data))(*data))

    # ================================================
    # Run filter
    # ================================================
    x_est = []
    for i in range(data_count):
        z.pData[0] = ctypes.c_float(baro[i])
        z.pData[1] = ctypes.c_float(
            3.28 * (cosines[i] * 9.81 * (accel[0][i])-9.81))
        lib.KalmanFilter_update(ctypes.byref(kf), ctypes.byref(z))
        x_est.append(kf.x.pData[1])

    plt.plot(t, [x for x in x_est])
    plt.plot(t, vel)
    plt.xlabel("Time (s)")
    plt.ylabel("Velocity (ft/s)")
    plt.legend(["Avionics", "Blue Raven"])
    plt.legend(["Calculated estimate", "BR ground truth"])
    plt.title("Global vertical velocity")
    plt.show()
