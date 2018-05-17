import sys

sys.path.append('4.2inch_e-paper')
import epd4in2
import DS18B20
from time import sleep
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import glob
import threading
import signal
import PageMikrotik
import PagePrinter
import PageMariano
import PageStatus


# Add influxDB temp update


class PageManager():
    def __init__(self):
        self.epd = epd4in2.EPD()
        self.epd.init()
        self.pages = []
        self.headerHeight = 20
        self.headerFont = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf', 14)
        self.currentPage = (0, 0)
        self.timer = None
        base_dir = '/sys/bus/w1/devices/'
        self.tempSensor = None
        try:
            self.tempSensor = DS18B20.DS18B20(glob.glob(base_dir + '28*')[0])
        except:
            None

    def close(self):
        if self.timer:
            self.timer.cancel()
        for page in self.pages:
            page[0].close()

    def add(self, page, timeout):
        self.pages.append((page, timeout))
        page.init(epd4in2.EPD_WIDTH, epd4in2.EPD_HEIGHT - self.headerHeight)

    def __getCurrentPage__(self):
        return self.pages[self.currentPage[0]]

    def __drawPage__(self):
        image = Image.new('1', (epd4in2.EPD_WIDTH, epd4in2.EPD_HEIGHT), 1)  # 1: clear the frame
        image.paste(self.__getCurrentPage__()[0].getPage(self.currentPage[1]), (0, self.headerHeight))
        self.__printHeader__(image)
        self.epd.display_frame(self.epd.get_frame_buffer(image))

    def __pagesCount__(self):
        count = 0
        for page in self.pages:
            count += page[0].pageCount()
        return count

    def __printHeader__(self, image):
        draw = ImageDraw.Draw(image)
        if self.tempSensor:
            temp, _ = self.tempSensor.read_temp()
            page = '{:03.1f}\xb0C    Page {}/{}'.format(temp, self.__currentPageNo__() + 1, self.__pagesCount__())
        else:
            page = 'Page {}/{}'.format(self.__currentPageNo__() + 1, self.__pagesCount__())
        w, h = draw.textsize(page, font=self.headerFont)
        y = (self.headerHeight - 0 - h) / 2 + 0

        draw.rectangle((0, 0, 400, self.headerHeight), fill=0)
        headerText = self.__getCurrentPage__()[0].getHeaderName(self.currentPage[1])
        draw.text((2, y), headerText, font=self.headerFont, fill=255)
        draw.text((epd4in2.EPD_WIDTH - w - 2, y), page, font=self.headerFont, fill=255)

    def __setTimer__(self, enable):
        if enable:
            timeOut = self.pages[self.currentPage[0]][1]
            self.timer = threading.Timer(timeOut, self.__onPageTimer__)
            self.timer.start()
        else:
            self.timer.cancel()
            self.timer = None

    def __onPageTimer__(self):
        self.__setNextPage()
        self.__drawPage__()
        self.__setTimer__(True)

    def __setNextPage(self):
        currentPageCount = self.__getCurrentPage__()[0].pageCount()
        if self.currentPage[1] == currentPageCount - 1:
            self.currentPage = ((self.currentPage[0] + 1) % len(self.pages), 0)
        else:
            self.currentPage = (self.currentPage[0], self.currentPage[1] + 1)

    def __currentPageNo__(self):
        pageNo = 0
        for i in range(len(self.pages)):
            for j in range(self.pages[i][0].pageCount()):
                if self.currentPage == (i, j):
                    return pageNo
                pageNo += 1

        return pageNo

    def setPage(self, index):
        self.currentPage = index
        self.__drawPage__()

    def autoChange(self, enable):
        self.__setTimer__(enable)


class ServiceExit(Exception):
    """
    Custom exception which is used to trigger the clean exit
    of all running threads and the main program.
    """
    pass


def service_shutdown(signum, frame):
    print('Caught signal %d' % signum)
    raise ServiceExit


if __name__ == '__main__':
    # Register the signal handlers
    signal.signal(signal.SIGTERM, service_shutdown)
    signal.signal(signal.SIGINT, service_shutdown)

    pageManager = PageManager()

    pageTimeOut = 30
    pageManager.add(PageStatus.Device(),60)
    #pageManager.add(PageMariano.Mariano(),pageTimeOut )
    #pageManager.add(PageMikrotik.Mikrotik(), pageTimeOut)
    #pageManager.add(PagePrinter.Printer(), pageTimeOut)
    pageManager.setPage((0,0))
    pageManager.autoChange(True)

    try:
        while True:
            sleep(1)
    except ServiceExit:
        pageManager.close()
        exit()
