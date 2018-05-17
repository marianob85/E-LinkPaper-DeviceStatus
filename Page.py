class PageInterface():
    def __init__(self):
        self.w = 0
        self.h = 0

    def init(self, w, h):
        self.w = w
        self.h = h

    def pageCount(self):
        pass

    def getPage(self, pageIndex):
        pass

    def getHeaderName(self, pageIndex ):
        pass

    def close(self):
        pass
