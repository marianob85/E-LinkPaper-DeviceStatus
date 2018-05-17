from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import influxdb
import Page
import InfluxDBDef
import epd4in2


class Printer(Page.PageInterface):
    def __init__(self):
        Page.PageInterface.__init__(self)
        self.w = 0
        self.h = 0
        self.font15 = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf', 15)
        self.font14r = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf', 14)
        self.font12r = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf', 12)

    def pageCount(self):
        return 1

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

    def getPage(self, pageIndex ):
        image = Image.new('1', (self.w, self.h), 1)
        draw = ImageDraw.Draw(image)
        result, error = self.__getPrinterStatus__()
        if error:
            draw.text((0, 0), str(error), font=self.font15, fill=0)
            return image

        hosts = {}

        for i in range(len(result)):
            key = result.keys()[i]
            res = list(result.get_points())
            data = {'model': key[1]['model'],
                    'agent_host': key[1]['agent_host'],
                    'Black Toner': res[i]['Black Toner'],
                    'Imaging Unit': res[i]['Imaging Unit'],
                    }
            try:
                hosts[key[1]['host']].append(data)
            except KeyError:
                hosts[key[1]['host']] = [data]

        startLine = 2

        sortetKey = sorted(hosts)

        #for key, value in hosts.items():
        for key in sortetKey:
            value = hosts[key]
            w, h = draw.textsize(key, font=self.font15)
            x = 2
            draw.text((x, startLine), key, font=self.font15, fill=0)
            draw.line([(0, startLine+h+1), (epd4in2.EPD_WIDTH , startLine+h+1)], fill=0, width=1)
            startLine += h + 3
            for device in value:
                # Print model
                x = 4
                w, h = draw.textsize(device['model'], font=self.font14r)
                draw.text((x, startLine), device['model'], font=self.font14r, fill=0)

                w, _ = draw.textsize(device['agent_host'], font=self.font12r)
                draw.text(( epd4in2.EPD_WIDTH - w - 2 , startLine), device['agent_host'], font=self.font12r, fill=0)

                startLine += h + 3

                # BlackT0ner:
                blackToner = 'BlackToner:{:3}%'.format(device['Black Toner'])
                w, h = draw.textsize(blackToner, font=self.font14r)
                draw.text((100, startLine), blackToner, font=self.font14r, fill=0)

                # UmageUnit:
                if device['Imaging Unit']:
                    imageUnit = 'ImagingUnit:{:3}%'.format(device['Imaging Unit'])
                    w, h = draw.textsize(imageUnit, font=self.font14r)
                    draw.text((250, startLine), imageUnit, font=self.font14r, fill=0)

                startLine += h + 10

        return image

    def getHeaderName(self, pageIndex ):
        return 'Printers status'
