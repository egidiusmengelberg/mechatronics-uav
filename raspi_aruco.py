import sys
import time

import numpy as np
import cv2
import serial

from picamera2 import Picamera2, Preview
from scipy.spatial.transform import Rotation as R

ARUCO_DICT = {
    "DICT_4X4_50": cv2.aruco.DICT_4X4_50,
    "DICT_4X4_100": cv2.aruco.DICT_4X4_100,
    "DICT_4X4_250": cv2.aruco.DICT_4X4_250,
    "DICT_4X4_1000": cv2.aruco.DICT_4X4_1000,
    "DICT_5X5_50": cv2.aruco.DICT_5X5_50,
    "DICT_5X5_100": cv2.aruco.DICT_5X5_100,
    "DICT_5X5_250": cv2.aruco.DICT_5X5_250,
    "DICT_5X5_1000": cv2.aruco.DICT_5X5_1000,
    "DICT_6X6_50": cv2.aruco.DICT_6X6_50,
    "DICT_6X6_100": cv2.aruco.DICT_6X6_100,
    "DICT_6X6_250": cv2.aruco.DICT_6X6_250,
    "DICT_6X6_1000": cv2.aruco.DICT_6X6_1000,
    "DICT_7X7_50": cv2.aruco.DICT_7X7_50,
    "DICT_7X7_100": cv2.aruco.DICT_7X7_100,
    "DICT_7X7_250": cv2.aruco.DICT_7X7_250,
    "DICT_7X7_1000": cv2.aruco.DICT_7X7_1000,
    "DICT_ARUCO_ORIGINAL": cv2.aruco.DICT_ARUCO_ORIGINAL,
    "DICT_APRILTAG_16h5": cv2.aruco.DICT_APRILTAG_16h5,
    "DICT_APRILTAG_25h9": cv2.aruco.DICT_APRILTAG_25h9,
    "DICT_APRILTAG_36h10": cv2.aruco.DICT_APRILTAG_36h10,
    "DICT_APRILTAG_36h11": cv2.aruco.DICT_APRILTAG_36h11
}

def handleSerialRequest():
    if ser.in_waiting > 0:
        request = ser.readline().decode().strip()
        if request == 'where am I?':
            response = f"{X}_{Y}_{Z}_{A}_{B}_{C}"
            ser.write((response + '\n').encode())
        elif request == 'knock knock!':
            ser.write('whos there?\n'.encode())

def pose_estimation(frame, aruco_dict_type, matrix_coefficients, distortion_coefficients):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    dictionary = cv2.aruco.getPredefinedDictionary(aruco_dict_type)
    parameters = cv2.aruco.DetectorParameters()
    detector = cv2.aruco.ArucoDetector(dictionary, parameters)
    corners, ids, rejected_img_points = detector.detectMarkers(gray)
    
    if ids is not None:
        for i in range(len(ids)):
            rvec, tvec = estimate_pose(corners[i], matrix_coefficients, distortion_coefficients)
            return rvec, tvec
    return None, None
    
def estimate_pose(corner, matrix_coefficients, distortion_coefficients):
    obj_points = np.array([
        [-0.5, 0.5, 0],
        [0.5, 0.5, 0],
        [0.5, -0.5, 0],
        [-0.5, -0.5, 0]
    ])
    img_points = np.array(corner[0], dtype=np.float64)
    _, rvec, tvec = cv2.solvePnP(obj_points, img_points, matrix_coefficients, distortion_coefficients)
    return rvec, tvec
    
def print_pose(rvec, tvec):
    rodrigues_vector = np.array(rvec).flatten()
    pitch, yaw, roll = rodrigues_to_euler(rodrigues_vector)
    print(f"Pitch: {pitch:.2f}, Yaw: {yaw:.2f}, Roll: {roll:.2f}")
    print(f"X: {tvec[0][0]:.2f}, Y: {tvec[1][0]:.2f}, Z: {tvec[2][0]:.2f}\n")


def rodrigues_to_euler(rodrigues_vector):
    rotation = R.from_rotvec(rodrigues_vector)
    euler_angles = rotation.as_euler('zyx', degrees=False)
    pitch, yaw, roll = euler_angles[1], euler_angles[0], euler_angles[2]
    return pitch, yaw, roll
    
def handle_serial_request(ser, picam2, aruco_dict_type, matrix_coefficients, distortion_coefficients):
    if ser.in_waiting > 0:
        request = ser.readline().decode().strip()
        print(f'recieved - {request}')
        if request == 'where am I?':
            frame = picam2.capture_array()
            rvec, tvec = pose_estimation(frame, aruco_dict_type, matrix_coefficients, distortion_coefficients)
            if rvec is not None and tvec is not None:
                rodrigues_vector = np.array(rvec).flatten()
                pitch, yaw, roll = rodrigues_to_euler(rodrigues_vector)
                X, Y, Z = tvec[0][0], tvec[1][0], tvec[2][0]
                response = f"{X:.2f}_{Y:.2f}_{Z:.2f}_{pitch:.2f}_{yaw:.2f}_{roll:.2f}"
                ser.write((response + '\n').encode())
                print(f'send - {response}')
            else:
                ser.write("No markers detected\n".encode())
                print(f'send - No markers detected')
        elif request == 'knock knock!':
            ser.write('whos there?\n'.encode())

if __name__ == "__main__":
    ser = serial.Serial('/dev/serial0', 115200, timeout=1)
    aruco_type = "DICT_7X7_50"
    arucoDict = ARUCO_DICT[aruco_type]

    intrinsic_camera = np.array([[2.53857481e+03, 0.00000000e+00, 1.67006102e+03],
                                 [0.00000000e+00, 2.54019864e+03, 1.28944277e+03],
                                 [0.00000000e+00, 0.00000000e+00, 1.00000000e+00]])

    distortion = np.array([0.19518353, -0.32629074, 0.00423279, 0.00227138, 0.03963533])

    picam2 = Picamera2()
    config = picam2.create_preview_configuration()
    picam2.configure(config)
    picam2.start()

    time.sleep(2)

    try:
        while True:
            handle_serial_request(ser, picam2, arucoDict, intrinsic_camera, distortion)
            time.sleep(0.1)  # Add a small delay to prevent high CPU usage
    except KeyboardInterrupt:
        print("Interrupted by user")

    picam2.stop()

