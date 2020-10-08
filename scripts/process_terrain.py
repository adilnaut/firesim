import gdal
import numpy as np

classes = { "Forest" : ["Forest"],
            "Woodland": ["Woodland"],
            "Grassland": ["Grassland", "Herbaceous",  "Scrub"] ,
             "Soil": ["Steppe"],
             "Water": ["Wetlands","Wetland" ]
             }


filepath = r"/Users/adilettuleuov/Downloads/lf19715421_US_140EVT"

# Open the file:
raster = gdal.Open(filepath)

x_size = raster.RasterXSize
y_size = raster.RasterYSize

# rasterArray = raster.ReadAsArray()
band = raster.GetRasterBand(1)
# Get nodata value from the GDAL band object
nodata = band.GetNoDataValue()

rasterArray = raster.ReadAsArray()

#Create a masked array for making calculations without nodata values
rasterArray = np.ma.masked_equal(rasterArray, nodata)
# type(rasterArray)

i = 0
j = 0
c = 0
A = []
while(c < 10000):
    print("{} {} {}", str(i) ,str(j), str(c))
    a = rasterArray[i + j*100]
    A.append(a)
    if c % 100 == 1:
        i += 1
    if j == 99:
        j = 0;
    j += 1
    c += 1

f = open("../data/terrain.txt", "w")
result = ""
print(A)
for a in A:
    element = str(a)
    result += str(a)
f.write(result)
# Check type of the variable 'raster'
# type(raster)
