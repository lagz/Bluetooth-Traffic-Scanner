from http.server import BaseHTTPRequestHandler, HTTPServer
import time
from numpy import math

hostName = "192.168.1.115"
serverPort = 8008

def getdevices():
    pos = -1
    x = 0
    with open("localdevices0.txt", "r") as f1:
        lines = []
        for line in f1:
            lines.insert(x, line)
            x = x + 1
        devnum = lines[pos]
        while(len(devnum) == 18):
            pos = pos - 1
            devnum = lines[pos]
    devicesfound = "<html><h1 style = font-size: 30px>" + devnum + "</h1>"
    return devicesfound, devnum

def getmacs():
    x = 0
    i = 0
    with open("localdevices0.txt", "r") as f2:
        lines = []
        macadds = []
        for line in f2:
            lines.insert(x, line)
            x = x + 1
        for n in lines:
            if len(n) >= 17:
                macadds.insert(i, "<p>" + n + "</p>")
            i = i + 1
    return macadds

def estimatepop(devices):
    estlow = int(devices) * 0.667
    pop = "<h5 style =\"color:white;font-size:20px;\">" + str(math.ceil(estlow)) + " - " + str(devices) + "</h5>"
    return pop

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("<font color=\"white\">", "utf-8"))
        self.wfile.write(bytes("<head><title>Foot Traffic Analyzer</title></head>","utf-8"))
        self.wfile.write(bytes("<h1 style=\"color:white;font-size:40px;\">Kettering University Foot Traffic Analyzer</h1>", "utf-8"))
        self.wfile.write(bytes("<body>", "utf-8"))
        self.wfile.write(bytes("<p>Created by: Maxwell LaGassa, Zachary Herman, and Jack Madsen</p>", "utf-8"))
        self.wfile.write(bytes("<HR WIDTH=\"100%\" COLOR=\"WHITE\" SIZE=\"1\">", "utf-8"))  
        self.wfile.write(bytes("<h2 style=\"color:white;font-size:20px;\">Number of Devices:</h2>", "utf-8"))
        devs1, devices = getdevices()
        self.wfile.write(bytes(devs1, "utf-8"))
        self.wfile.write(bytes("<h3 style=\"color:white;font-size:20px;\">Most Recent 5 Device Addresses:</h3>", "utf-8"))
        macs = getmacs()
        self.wfile.write(bytes(macs[-1], "utf-8"))
        self.wfile.write(bytes(macs[-2], "utf-8"))      
        self.wfile.write(bytes(macs[-3], "utf-8"))
        self.wfile.write(bytes(macs[-4], "utf-8"))
        self.wfile.write(bytes(macs[-5], "utf-8"))
        self.wfile.write(bytes("<h4 style=\"color:white;font-size:20px;\">Estimated Population:</h4>", "utf-8"))
        self.wfile.write(bytes(estimatepop(devices),"utf-8"))
        self.wfile.write(bytes("<body style=\"background-color:black;\">", "utf-8"))
        self.wfile.write(bytes("</body></html>", "utf-8"))

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")

