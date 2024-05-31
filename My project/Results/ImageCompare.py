import sys

import numpy as np
import cv2

def MSE(original, ours):
    error = np.sum((original.astype("float") - ours.astype("float")) ** 2)
    error /= original.shape[0] * original.shape[1]
    return error

original_path = sys.argv[1]
original = cv2.imread(original_path, cv2.IMREAD_UNCHANGED)
ours_path = sys.argv[2]
ours = cv2.imread(ours_path, cv2.IMREAD_UNCHANGED)

f = open(ours_path[:ours_path.rfind('.')] + '.txt', 'w')
print(f'Mean squared error: {MSE(original, ours):.6f}', file=f)
