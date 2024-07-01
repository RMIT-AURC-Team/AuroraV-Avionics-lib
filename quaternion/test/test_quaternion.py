import pathlib
import ctypes
import pandas as pd
import matplotlib.pyplot as plt


class Quaternion(ctypes.Structure):
    pass


Quaternion._fields_ = [
    ('w', ctypes.c_float),
    ('x', ctypes.c_float),
    ('y', ctypes.c_float),
    ('z', ctypes.c_float),
    ('init', ctypes.CFUNCTYPE(None, ctypes.POINTER(Quaternion))),
    ('normalise', ctypes.CFUNCTYPE(None, ctypes.POINTER(Quaternion))),
    ('fromEuler', ctypes.CFUNCTYPE(None, ctypes.POINTER(Quaternion),
     ctypes.c_float, ctypes.c_float, ctypes.c_float)),
]

filepath = pathlib.Path(__file__).parent
subroot = filepath.parent.resolve()

# Load the shared library
lib = ctypes.CDLL(f'{subroot}/bin/libquaternion.so')

# Function signatures
lib.Quaternion_mul.argtypes = [
    ctypes.POINTER(Quaternion), ctypes.POINTER(Quaternion)]
lib.Quaternion_mul.restype = Quaternion

lib.Quaternion_init.argtypes = [ctypes.POINTER(Quaternion)]
lib.Quaternion_init.restype = None

lib.Quaternion_normalise.argtypes = [ctypes.POINTER(Quaternion)]
lib.Quaternion_normalise.restype = None

lib.Quaternion_fromEuler.argtypes = [ctypes.POINTER(
    Quaternion), ctypes.c_float, ctypes.c_float, ctypes.c_float]
lib.Quaternion_fromEuler.restype = None


def calculate_quat(gyro, dt):
    q = Quaternion()
    lib.Quaternion_init(ctypes.byref(q))
    quats = [q]

    for i in range(data_count):
        gyro_x = gyro[0][i]
        gyro_y = gyro[1][i]
        gyro_z = gyro[2][i]

        q_dot = Quaternion()
        lib.Quaternion_init(ctypes.byref(q_dot))

        q_dot.fromEuler(
            ctypes.byref(q_dot),
            dt * (gyro_x),
            dt * (gyro_y),
            dt * (gyro_z)
        )
        q_w = lib.Quaternion_mul(ctypes.byref(quats[i]), ctypes.byref(q_dot))
        q_w.normalise(ctypes.byref(q_w))

        quats.append(q_w)

    quats = [(q.x, q.y, q.z, q.w) for q in quats]
    return quats


if __name__ == "__main__":

    ###########################################################################
    #                            INITILISATION                                #
    ###########################################################################

    # Extract CSV data
    data = pd.read_csv(f"{subroot}/test/data_highres.csv")
    t = data['Flight_Time_(s)']
    data_count = len(t)

    gyro_data = [
        [-z for z in data['Gyro_Z']],
        [y for y in data['Gyro_Y']],
        [x for x in data['Gyro_X']]
    ]

    ###########################################################################
    #                              EXECUTION                                  #
    ###########################################################################

    freq = 500
    dt = 1/freq

    quats = calculate_quat(gyro_data, dt)

    plt.plot(t, quats[1:])
    plt.xlabel("Time (s)")
    plt.legend(["x", "y", "z", "w"], loc="upper left")
    plt.show()
