import InfluxDBDef
import Page
import influxdb
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont


class Mariano(Page.PageInterface):
    def __init__(self):
        Page.PageInterface.__init__(self)
        self.w = 0
        self.h = 0
        self.font14 = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf', 15)
        self.font14r = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf', 14)
        self.font12r = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf', 12)

    def __getPrinterStatus__(self):
        try:
            client = influxdb.InfluxDBClient(InfluxDBDef.host, InfluxDBDef.port, InfluxDBDef.user, InfluxDBDef.password,
                                             InfluxDBDef.database)
            result = client.query(
                'SELECT last("BlackTonerLevel")/last("BlackTonerMaxCapacity")*100 AS "Black Toner", last("ImagingUnitLevel")/last("ImagingUnitMaxCapacity")*100 AS "Imaging Unit" FROM "Printer" WHERE time > now() - 90d GROUP BY "model", "id", "host", "agent_host"')
            client.close()
            return result, None
        except influxdb.exceptions.InfluxDBClientError as error:
            return None, error

    def pageCount(self):
        return 1

    def getPage(self, pageIndex ):
        image = Image.new('1', (self.w, self.h), 1)
        draw = ImageDraw.Draw(image)
        result, error = self.__getPrinterStatus__()
        if error:
            draw.text((0, 0), str(error), font=self.font14, fill=0)
            return image

        # UpTime
        # Drive: C,D,E
        # Temp: CPU, MB, GPU
        # Mem free: GB/%

        return image

    def getHeaderName(self, pageIndex ):
        return 'Mariano status'
