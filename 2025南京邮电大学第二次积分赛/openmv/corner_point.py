# corner_point - By: Administrator - Wed Apr 2 2025

import sensor
import time
from pyb import UART
from pid import PID
import display
import pyb

uart = UART(3,9600)
corner = 120
Output = 0
count = 0
error = 0
mode = 10
status = 0
thresholds = (0, 80)

flag = 0


Rec_flag = (155,50,5,10)
Rec_bzd = (95,10,10,10)
Rec_swd = (60,25,10,10)

Rec_more = (60,25,10,10)
Rec_left = (15,60,5,5)
Rec_ssl = (85,55,5,10)

Rec_mid = (70,50,10,10)
Rec1_mid = (70,60,10,15)
Rec2_mid = (70,75,10,5)
Rec3_mid = (70,80,10,5)
Rec4_mid = (70,85,10,15)

pid = PID(p=0.3, i=0, d=0)
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)

clock = time.clock()
lcd = display.SPIDisplay()

while True:
    clock.tick()
    img = sensor.snapshot().binary([thresholds])
    rec1 = img.get_statistics(roi = Rec1_mid)
    rec2 = img.get_statistics(roi = Rec2_mid)
    rec3 = img.get_statistics(roi = Rec3_mid)
    rec4 = img.get_statistics(roi = Rec4_mid)
    recm = img.get_statistics(roi = Rec_mid)

    recf = img.get_statistics(roi = Rec_flag)
    recb = img.get_statistics(roi = Rec_bzd)
    recs = img.get_statistics(roi = Rec_swd)

    recl = img.get_statistics(roi = Rec_left)
    recmo = img.get_statistics(roi = Rec_more)
    recss = img.get_statistics(roi = Rec_ssl)

    img.draw_rectangle(Rec1_mid)
    img.draw_rectangle(Rec2_mid)
    img.draw_rectangle(Rec3_mid)
    img.draw_rectangle(Rec4_mid)
    img.draw_rectangle(Rec_mid)

    img.draw_rectangle(Rec_flag)
    img.draw_rectangle(Rec_bzd)
    img.draw_rectangle(Rec_swd)

    img.draw_rectangle(Rec_left)
    img.draw_rectangle(Rec_more)
    img.draw_rectangle(Rec_ssl)
    lcd.write(img)

    if mode == 10 :
        if (recm.mean() <= 20 and rec2.mean()+rec3.mean() >= 50):
            mode = 1

    # 任务1：直行巡线
    if mode == 1 :
        error = rec1.mean() - rec4.mean()
        Output = pid.get_pid(error,1)
        if(recm.mean() >= 20):
            Output = 0
        uart.write("@D%f\r\n"%Output)
        print(Output)
        if(count < 4):
            if(recf.mean() >= 180):
                count += 1
                pyb.delay(200)
                if(count == 4):
                    pyb.delay(300)
                    uart.write("@t1\r\n")
                    pyb.delay(4000)
        if(count == 4 and recb.mean() >=25):
            uart.write("@t2\r\n")
            mode = 2

    # 倒车入库直线过程
    elif mode == 2 :
        if(count == 4 and recs.mean() >= 20):
            uart.write("@t3\r\n")
            mode = 3
    # 出库
    elif mode == 3 :
        if(status == 0):
            if(recb.mean() >= 15):
                uart.write("@t2\r\n")
                status = 1
        if(status == 1):
            if(rec2.mean()+rec3.mean() >= 100):
                mode = 4
                count = 0
                uart.write("@t9\r\n")

    # 任务2
    elif mode == 4 :
        if(count < 4):
            error = rec1.mean() - rec4.mean()
            if(recm.mean() >= 20):
                error = 0
            Output = pid.get_pid(error,1)
            uart.write("@D%f\r\n"%Output)
            print(Output)
            if(recl.mean() >= 100):
                count += 1
                if(count == 4):
                    uart.write("@t4\r\n")   #向右打方向盘倒车
                    pyb.delay(2000)
                pyb.delay(150)
        if(count == 4):
            if(recmo.mean() >= 50):
                mode = 5
    elif mode == 5 :
        uart.write("@t5\r\n")           #倒车直行
        if(recss.mean() > 55):
            uart.write("@t6\r\n")       #向左打方向盘倒车
            flag = 1
            pyb.delay(1600)
        if(flag == 1):
            uart.write("@t7\r\n")       #停车
            mode = 0








