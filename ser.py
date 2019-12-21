#!/usr/bin/env python

import serial
import tkinter as tk

PORT = '/dev/ttyUSB0'
BAUD = 9600

WINW = 320
WINH = 200


def main():

    root = tk.Tk()
    app = Window(root)
    root.wm_title('MYSerial console')
    root.geometry('{}x{}'.format(WINW, WINH))
    root.mainloop()

    # with serial.Serial(PORT, BAUD) as ser:
    #     send_string = input('Send: ').encode('utf-8')
    #     ser.write()


class Window(tk.Frame):
    def __init__(self, master=None):
        tk.Frame.__init__(self, master)
        self.master = master

        self.pack(fill=tk.BOTH, expand=1)

        sendButton = tk.Button(self, text='Send', command=self.clickSendButton)
        sendButton.place(anchor='ne', x=WINW, y=0)

    def clickSendButton(self):
        print('Send')


if __name__ == '__main__':
    main()
