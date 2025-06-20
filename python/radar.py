import numpy as np
import matplotlib.pyplot as plt
import serial
import time
from matplotlib.animation import FuncAnimation
import math

PORT = 'COM4'  
BAUD_RATE = 9600


fig = plt.figure(figsize=(10, 8))
ax = plt.subplot(111, polar=True)
ax.set_title('Arduino Radar Görüntüleyici', fontsize=16)
ax.set_theta_zero_location('N')  # 0 derece yukarıda olsun
ax.set_theta_direction(-1)  # Saat yönünün tersi
ax.set_rlabel_position(0)  # r-etiketlerini merkeze hizala
ax.grid(True)

# max mesafe (cm)
max_distance = 400
ax.set_ylim(0, max_distance)

angles = []
distances = []
line, = ax.plot([], [], 'r-', linewidth=1)
scatter = ax.scatter([], [], c='g', s=50)

# geçmiş verileri depolamak için
history_angles = []
history_distances = []
history_scatter = ax.scatter([], [], c='darkgreen', s=10, alpha=0.5)

try:
    # seri port bağlantısını başlat
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    print(f"{PORT} portu üzerinden Arduino'ya bağlandı!")
    time.sleep(2)  
except Exception as e:
    print(f"Bağlantı hatası: {e}")
    print("Lütfen doğru port numarasını kullanın.")
    exit()

def init():
    """Animasyon başlatma fonksiyonu"""
    line.set_data([], [])
    scatter.set_offsets(np.c_[[], []])
    history_scatter.set_offsets(np.c_[[], []])
    return line, scatter, history_scatter

def animate(i):
    """Her kare için güncelleme fonksiyonu"""
    global angles, distances, history_angles, history_distances
    
    try:
        # arduino'dan veri oku
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            if ' ' in data:
                distance_str, angle_str = data.split(' ')
                try:
                    distance = int(distance_str)
                    angle = int(angle_str)
                    
                    # mesafeyi sınırla
                    if distance > max_distance or distance < 0:
                        distance = max_distance
                    
                    # açıyı radyana çevir (matlotlib için)
                    angle_rad = math.radians(angle)
                    
                    # açı ve mesafeyi kaydet
                    angles = [angle_rad]
                    distances = [distance]
                    
                    # geçmiş verilere ekle
                    history_angles.append(angle_rad)
                    history_distances.append(distance)
                    
                    # son 100 veriyi tut
                    if len(history_angles) > 100:
                        history_angles = history_angles[-100:]
                        history_distances = history_distances[-100:]
                    
                    print(f"Mesafe: {distance} cm, Açı: {angle}°")
                except ValueError:
                    print(f"Geçersiz veri: {data}")
    except Exception as e:
        print(f"Veri okuma hatası: {e}")
    
    # Grafik verilerini güncelle
    line.set_data(angles, distances)
    scatter.set_offsets(np.c_[angles, distances])
    history_scatter.set_offsets(np.c_[history_angles, history_distances])
    
    return line, scatter, history_scatter


ani = FuncAnimation(fig, animate, init_func=init, interval=50, blit=True, cache_frame_data=False)

plt.show()
ser.close()