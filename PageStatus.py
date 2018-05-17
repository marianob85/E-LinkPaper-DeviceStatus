import epd4in2
import pyping
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import Page


class Device(Page.PageInterface):
    def __init__(self):
        Page.PageInterface.__init__(self)
        self.w = 0
        self.h = 0
        self.font14r = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf', 14)

        self.devices = [
            ('192.168.1.1', 'M,MT-Gateway'),
            ('192.168.1.2', 'M:MT-WifiAP'),
            ('192.168.1.3', 'M:MT-LTE'),
            ('192.168.1.101', 'M:HTPC'),
            ('192.168.1.110', 'M:Server-Debian'),
            ('192.168.1.152', 'M:STB-DM800'),
            ('192.168.10.1', 'O:MT-Gateway'),
            ('192.168.10.2', 'O:MT-AP1'),
            ('192.168.10.3', 'O:MT-AP2'),
            ('192.168.10.4', 'O:MT-AP3'),
            ('192.168.10.160', 'O:STB-Zgemma'),
            ('192.168.10.172', 'O:MotionEyeOS'),
            ('192.168.10.174', 'O:NanoPiNeo2'),
            ('192.168.13.1', 'O:Huawei-HiLink'),
            ('192.168.20.1', 'P:MT-Gateway-P'),
        ]

    def pageCount(self):
        return 2

    def getPage(self, pageIndex):
        image = Image.new('1', (self.w, self.h), 1)
        draw = ImageDraw.Draw(image)

        deviceStatus = {}
        for device in self.devices:
            response = pyping.ping(device[0], 500, 2)
            if response.ret_code == 0:
                deviceStatus[device] = True
            else:
                deviceStatus[device] = False

        draw.line([(200, 0), (200, 300)], fill=0, width=1)
        # Create 2 columns
        _, maxH = draw.textsize('192.168.000.000', font=self.font14r)
        startLine = 2
        for device in self.devices:
            value = deviceStatus[device]

            draw.text((2, startLine), device[pageIndex], font=self.font14r, fill=0)

            if value:
                text = 'online'
                w, h = draw.textsize(text, font=self.font14r)
                draw.text(((epd4in2.EPD_WIDTH / 2) - w - 10, startLine), text, font=self.font14r, fill=0)
            else:
                text = 'offline'
                w, h = draw.textsize(text, font=self.font14r)
                draw.rectangle(
                    ((epd4in2.EPD_WIDTH / 2) - w - 11, startLine - 2, (epd4in2.EPD_WIDTH / 2) - 9, startLine + h + 2),
                    fill=0)
                draw.text(((epd4in2.EPD_WIDTH / 2) - w - 10, startLine), text, font=self.font14r, fill=255)
            startLine += maxH + 5

        return image

    def getHeaderName(self, pageIndex):
        return 'Device status'
