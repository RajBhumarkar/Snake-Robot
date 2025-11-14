import numpy as np
import matplotlib.pyplot as plt

# constants from arduino code
PHASE_SHIFT_PER_SEGMENT = 1.2
Pi = np.pi

# Yaw servos (horizontal)
yawMidAngle = 90.0
yawAmplitude = 30.0  

# Pitch servos (vertical)
pitchMidAngle = 90.0
pitchAmplitude = 10.0 

num_segments = 3


t = np.arange(0.0, 2.01, 0.01)

yaw_angles = []
pitch_angles = []

for segmentIndex in range(num_segments):
    # YAW SERVO (horizontal S-curve)
    yaw_phase = 2 * Pi * segmentIndex * PHASE_SHIFT_PER_SEGMENT
    yaw_wave = np.sin(2 * Pi * t + yaw_phase)
    yaw_angle = yawMidAngle + yawAmplitude * yaw_wave
    yaw_angles.append(yaw_angle)
    
    # PITCH SERVO (vertical motion)
    # The Pi/2 offset creates the 90-degree phase shift between yaw and pitch
    pitch_phase = 2 * Pi * segmentIndex * PHASE_SHIFT_PER_SEGMENT + Pi/2
    pitch_wave = np.sin(2 * Pi * t + pitch_phase)
    pitch_angle = pitchMidAngle + pitchAmplitude * pitch_wave
    pitch_angles.append(pitch_angle)

# --- Plotting ---
plt.figure(figsize=(12, 7))

yaw_colors = ['#a6cee3', '#1f78b4', '#003366'] 
pitch_colors = ['#fdbf6f', '#e31a1c', '#800000'] 

# Plot Yaw angles (solid lines)
for i in range(num_segments):
    plt.plot(t, yaw_angles[i], 
             label=f'Yaw - Segment {i}', 
             color=yaw_colors[i], 
             linestyle='-')

for i in range(num_segments):
    plt.plot(t, pitch_angles[i], 
             label=f'Pitch - Segment {i}', 
             color=pitch_colors[i], 
             linestyle='--')

plt.title('Yaw and Pitch Servo Motion (Few Segments, 2D View)', fontsize=14)
plt.xlabel('Time (t)', fontsize=12)
plt.ylabel('Servo Angle (degrees)', fontsize=12)

plt.ylim(55, 125)

plt.grid(True)
plt.legend()
plt.tight_layout()

plt.savefig('snake_robot_plot.png', dpi=300)

plt.show()

print("Plot saved as 'snake_robot_plot.png'")