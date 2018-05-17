from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import influxdb
import datetime
import Page
import InfluxDBDef

class Mikrotik(Page.PageInterface):
    def __init__(self):
        Page.PageInterface.__init__(self)
        self.w = 0
        self.h = 0
        self.font14 = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf', 14)
        self.font14r = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf', 14)
        #self.font16 = ImageFont.truetype('/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf', 16)

    def pageCount(self):
        return 1

    def __getMikrotikStatus__(self):
        try:
            client = influxdb.InfluxDBClient(InfluxDBDef.host, InfluxDBDef.port, InfluxDBDef.user, InfluxDBDef.password, InfluxDBDef.database)
            result = client.query(
                'SELECT last("sysUpTime") / 100 AS "UpTime", last("mtxrHlTemperature") / 10 AS "Temperature", last("mtxrHlVoltage") / 10 AS "Voltage", last("mtxrLicVersion") AS "Firmware" FROM "MikroTik"  WHERE time > now() - 1d GROUP BY "sysName", "sysDescr", "agent_host" ')
            client.close()
            return result, None
        except influxdb.exceptions.InfluxDBClientError as error:
            return None, error

    def getPage(self, pageIndex ):
        image = Image.new('1', (self.w, self.h), 1)
        draw = ImageDraw.Draw(image)
        result, error = self.__getMikrotikStatus__()
        if error:
            draw.text((0, 0), str(error), font=self.font14, fill=0)
            return image

        tags = {'sysName':(0,0), 'agent_host':(0,0) }
        measurementsOrder = ['Firmware','Voltage','Temperature','UpTime']
        measurements = {'Firmware':(0,0),'Voltage':(0,0),'Temperature':(0,0),'UpTime':(0,0)}
        formats = {'Firmware':('Fv:{}',lambda x : x ),'Voltage':('{}V', lambda x : x ),'Temperature':("{}\xb0C",lambda x : x),'UpTime':('{}',lambda x : datetime.timedelta(seconds=round(x)))}

        for i in range(len(result)):
            key = result.keys()[i]
            res = list(result.get_points())

            w, h = draw.textsize(key[1]['sysName'], font=self.font14)
            tags['sysName'] = (max(tags['sysName'][0], w), max(tags['sysName'][1], h) )

            w, h = draw.textsize(key[1]['agent_host'], font=self.font14r)
            tags['agent_host'] = ( max( tags['agent_host'][0], w ), max(tags['agent_host'][1], h ))

            for measurement in measurementsOrder:
                value = measurements[measurement]
                textFormat = formats[measurement]
                val = textFormat[0].format(textFormat[1](res[i][measurement]))
                w, h = draw.textsize(val, font=self.font14r)
                measurements[measurement] = (max(value[0], w), max(value[1], h))

        startLine = 2
        for i in range(len(result)):
            key = result.keys()[i]
            res = list(result.get_points())
            x = 2

            draw.text((x, startLine), key[1]['sysName'], font=self.font14, fill=0)
            x += tags['sysName'][0] + 20
            draw.text(( self.w - tags['agent_host'][0] - 1, startLine + 1), key[1]['agent_host'], font=self.font14r, fill=0)
            startLine += tags['sysName'][1]
            startLine += 1
            x = 4

            for measurement in measurementsOrder:
                value = measurements[measurement]
                textFormat = formats[measurement]
                if res[i][measurement]:
                    val = textFormat[0].format( textFormat[1](res[i][measurement]))
                    draw.text((x, startLine), val, font=self.font14r, fill=0)
                x += measurements[measurement][0] + 20

            startLine += measurements['Firmware'][1] + 2
            startLine += 2
        return image

    def getHeaderName(self, pageIndex ):
        return 'Mikrotik RouterOS'
